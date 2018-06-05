#include <time.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <sys/time.h>

#include <unistd.h>
#include <fcntl.h>
#include "log.h"

using namespace std;
/*
<ResourceReq>
<Header>
<Version>2.0.0</Version>
<AppID>SJDHPOI</AppID>
<SvcCode>300008</SvcCode>
<UserID></UserID>
<ScheduleType>1</ScheduleType>
<CEID>cmccnavi20130506</CEID>
<RecType>1</RecType>
<ECCode></ECCode>
<LoginName>SJDHPOI</LoginName>
<Password>20121215WytR</Password>
<ReqNo>1ffe985132cc543733b6520c2ab008cf</ReqNo>
<RPID>cp_1</RPID>
<ResourceID>021001</ResourceID>
<TID>1234567890</TID>
<ParamList></ParamList>
<URLSuffix></URLSuffix>
</Header>
<Body><![CDATA[query_type:TQUERY
data_type:POI
geoobj:116.397944;39.908159;116.397944;39.908159
keywords:±ö¹Ý¾Æµê
page_num:10
page:1
city:110000
citySuggestion:true
qii:true
]]></Body>
</ResourceReq>
*/
string get_http_send_body()
{
	stringstream streamPara;
	streamPara << "query_type:TQUERY\n"
		<< "data_type:POI\n"
		<< "geoobj:116.397944;39.908159;116.397944;39.908159\n"
		<< "keywords:±ö¹Ý¾Æµê\n"
		<< "page_num:50\n"
		<< "page:1\n"
		<< "city:110000\n"
		<< "citySuggestion:true\n"
		<< "qii:true\n";
	string sPara = streamPara.str();
	//cout << "sPara:" << sPara << endl;

	stringstream sSearchXml;
	sSearchXml<<"<?xml version=\"1.0\" encoding=\"gbk\"?>\r\n"
		<<"<ResourceReq>\r\n"
		<<"<Header>\r\n"
		<<"<Version>"<<"2.0.0"<<"</Version>\r\n"
		<<"<AppID>"<<"SJDHPOI"<<"</AppID>\r\n"
		<<"<SvcCode>"<< "300008" <<"</SvcCode>\r\n"
		<<"<UserID>"<< "" <<"</UserID>\r\n"
		<<"<ScheduleType>1</ScheduleType>\r\n"
		<<"<CEID>"<< "cmccnavi20130506" <<"</CEID>\r\n"
		<<"<RecType>1</RecType>\r\n<ECCode></ECCode>\r\n"
		<<"<LoginName>"<< "SJDHPOI" <<"</LoginName>\r\n"
		<<"<Password>"<< "20121215WytR" <<"</Password>\r\n"
		<<"<ReqNo>"<< "1ffe985132cc543733b6520c2ab008cf" <<"</ReqNo>\r\n"
		<<"<RPID>cp_1</RPID>\r\n"
		<<"<ResourceID>"<< "021001" <<"</ResourceID>\r\n"
		<<"<TID>1234567890</TID>\r\n<ParamList></ParamList>\r\n"
		<<"<URLSuffix></URLSuffix>\r\n"
		<<"</Header>\r\n"
		<<"<Body><![CDATA["
		<<sPara
		<<"]]>"
		<<"</Body>\r\n"
		<<"</ResourceReq>";
	
	/*
	stringstream sSearchXml;
	sSearchXml<<"<?xml version=\"1.0\" encoding=\"gbk\"?>\r\n"
		<<"<ResourceReq>\r\n"
		<<"<Header>\r\n"
		<<"<Version>"<<RESOURCE_GATEWAY_SEARCH_VERSION<<"</Version>\r\n"
		<<"<AppID>"<<RESOURCE_GATEWAY_SEARCH_APPID<<"</AppID>\r\n"
		<<"<SvcCode>"<<RESOURCE_GATEWAY_SEARCH_SVCCODE<<"</SvcCode>\r\n"
		<<"<UserID>"<<searchPara.userPhone<<"</UserID>\r\n"
		<<"<ScheduleType>1</ScheduleType>\r\n"
		<<"<CEID>"<<RESOURCE_GATEWAY_SEARCH_CEID<<"</CEID>\r\n"
		<<"<RecType>1</RecType>\r\n<ECCode></ECCode>\r\n"
		<<"<LoginName>"<<RESOURCE_GATEWAY_SEARCH_LOGINNAME<<"</LoginName>\r\n"
		<<"<Password>"<<RESOURCE_GATEWAY_SEARCH_PASSWORD<<"</Password>\r\n"
		<<"<ReqNo>"<<sReqNo<<"</ReqNo>\r\n"
		<<"<RPID>cp_1</RPID>\r\n"
		<<"<ResourceID>"<<RESOURCE_GATEWAY_SEARCH_RESOURCEID<<"</ResourceID>\r\n"
		<<"<TID>1234567890</TID>\r\n<ParamList></ParamList>\r\n"
		<<"<URLSuffix></URLSuffix>\r\n"
		<<"</Header>\r\n"
		<<"<Body><![CDATA["
		<<sPara
		<<"]]>"
		<<"</Body>\r\n"
		<<"</ResourceReq>";
	*/

	//cout << "body:" << sSearchXml.str() << endl;
	return sSearchXml.str();
}

string get_http_send_content(string host, string resource, string body)
{
	stringstream post_content;
	post_content << "POST " << resource << " HTTP/1.1\r\n"
		<< "accept: */*\r\n"
		<< "accept-language: zh-cn\r\n"
		<< "connection: keep-alive\r\n"
		<< "content-length: " << body.length() << "\r\n"
		<< "content-type: text/xml; charset=GBK\r\n"
		<< "host: " << host << "\r\n"
		<< "user-agent: Mozilla/4.0 (compatible; MSIE 6.0;windows NT 5.1; SV1)\r\n"
		<< "\r\n"
		<< body;
	//cout << "post_content:" << post_content.str() << endl;
	return post_content.str();

}

bool ParseURL(const string & url, string & host, string & resource)
{  
	const char * pos = strstr(url.c_str(), "http://");  
	if(pos == NULL) pos = url.c_str();  
	else pos += strlen("http://");  
	if(strstr( pos, "/") == 0)  
		return false;  
	char pHost[100];  
	char pResource[200];  
	sscanf(pos, "%[^/]%s", pHost, pResource);  

	host = pHost;  
	resource = pResource;  
	return true;  
}


int SetNoBlock( int s )
{
	int opts;
	opts = fcntl( s, F_GETFL );
	if( opts < 0 )
	{
		return -1;
	}
	opts = opts | O_NONBLOCK;
	opts = opts | O_NDELAY;
	if( fcntl( s, F_SETFL, opts ) < 0 )
	{
		return -1;
	}
	return 0;
}


int test_val()
{
	string sUrl = "http://223.100.246.7:9090/SearchWebProject/PoiSearch";
	//string sUrl = "http://172.17.13.59:9090/SearchWebProject/PoiSearch";
	string host, resource;
	if(!ParseURL(sUrl, host, resource))
	{
		cout << "parse url fail" << endl;
		return -1;  
	}

	 string ip = host.substr(0, host.find(':', 0)); 
	 string port = host.substr(host.find(':', 0) + 1);
	
	
	 string body = get_http_send_body();
	 string post_content = get_http_send_content(host, resource, body);
	
	 
	struct hostent * hp= gethostbyname(ip.c_str());
	if(hp == NULL)
	{  
		cout << "get hostent fail" << endl;
		return -1;  
	}

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if(sock == -1 || sock == -2)
	{  
		cout << "create socket fail" << endl;
		return -1;
	}  
	
	/*
	int ret1 = SetNoBlock(sock);
	if(ret1 != 0)
	{
		cout << "set no block fail" << endl;
		return -1;
	}
	*/

	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(port.c_str()));
	memcpy(&sa.sin_addr, hp->h_addr, 4);

	if(0 != connect(sock, (struct sockaddr*)&sa, sizeof(sa)))
	{  
		close(sock);  
		cout << "connect fail" << endl;
		LOG_INFO("connect fail");
		return -1;  
	}  


	 LOG_INFO("post_content:%s", post_content.c_str());
	 send(sock, post_content.c_str(), post_content.size(), 0);
	

	int	m_nContentLength = 1024*10;  
	char *pageBuf = (char*)malloc(m_nContentLength);  
	memset(pageBuf, 0, m_nContentLength);  

	int bytesRead = 0;  
	int ret = 1;  
	int find_firsd_flag;
	char* first_ptr = NULL;
	char* second_ptr = NULL;
	while(ret > 0)
	{  
		ret = recv(sock, pageBuf + bytesRead, m_nContentLength - bytesRead, 0);  
		if(ret > 0)  
		{  
			bytesRead += ret;  
		}  
		if(m_nContentLength - bytesRead < 100)
		{  	
			m_nContentLength *=2;  
			pageBuf = (char*)realloc(pageBuf, m_nContentLength);//ÖØÐÂ·ÖÅäÄÚ´æ   
		}

		first_ptr = strstr(pageBuf, "\r\n\r\n");//chunkedÊý¾Ý
		if(first_ptr != NULL)
		{
			second_ptr = strstr(first_ptr + 4, "\r\n\r\n");
			if(second_ptr != NULL)
			{
				//cout << "receive end" << endl;
				break;
			}
		}

		//cout << "ret:" << ret << endl;
	}  

	pageBuf[bytesRead] = '\0';  

	cout << "pageBuf:\n" << pageBuf << endl;

	free(pageBuf);
	close(sock);  
	return 0; 
}


int main(int argc, const char *argv[])
{
	//LOG_INFO("begin");
	timeval tm;
	for(int i = 0; i < 1; i++)
	{
		gettimeofday(&tm, NULL);
		double m_dTime = tm.tv_sec*1000 + (double)tm.tv_usec / 1000;
		test_val();
		gettimeofday(&tm, NULL);
		double dTime = tm.tv_sec*1000 + (double)tm.tv_usec / 1000;
		int diff_time = (int)(dTime - m_dTime);
		LOG_INFO("time:%dms", diff_time);
		cout << "time:" << diff_time << "ms" << endl;
		//sleep(3);
	}
	return 0;
}
