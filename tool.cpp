#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdarg.h>

using namespace std;


#define LOG_NORMAL(fmt,args...) log_down(__FILE__, __LINE__,"NORMAL", fmt, ##args)
#define LOG_MAX_LEN 2048
void log_down(const char* fileName, const int line, const char* level, const char* fmt, ...)
{
	
	stringstream str;
	str << "[" << fileName << ":" << line << "]";

	char buf[LOG_MAX_LEN] = {0};
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, LOG_MAX_LEN, fmt, ap);
	va_end(ap);
	cout << str.str() << buf << endl;
}

#define LOG_INFO(fmt...) log_write(__FILE__, __LINE__, "INFO", fmt)
void log_write(const char* file, const int line, const char* level, const char* fmt...)
{

	char tmstr[32] = {0};
	char loginstr[512] = {0};
	char buf[1024] = {0};
	
	time_t tm = time(NULL);
	strftime(tmstr, 32, "(%Y%m%d %H:%M:%S)", localtime(&tm));
	snprintf(loginstr, 512, "[(%s:%d)%s]\t", file, line, tmstr);
	
	
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, 1024, fmt, ap);
	va_end(ap);
	cout << loginstr << buf << endl;
	
	
	string log;
	log.reserve(2048);
	log += loginstr;
	log += buf;
	cout << "log:" << log << endl;
	/*
	 string* logPtr = new string;
	 logPtr->reserve(2048);
	 *logPtr += loginstr;
	 *logPtr += buf;
	 */
	//这里可以充当生产者，生产日志，将log对象提交到无锁多线程安全队列中，消费者可以起一个线程从无锁多线程安全队列中取日志对象写到文件中去，这样就可以实现异步日志系统
	return;
}

int main(int argc, const char *argv[])
{
	string empty;
	cout << empty.size() << endl;
	cout << empty.capacity() << endl;
	empty.reserve(100);
	cout << empty.size() << endl;
	cout << empty.capacity() << endl;
	//sprintf((char*)empty.c_str(), "hello this is my test example");
	empty += "hello this is my test example";
	cout << empty << endl;
	cout << empty.size() << endl;
	cout << empty.capacity() << endl;

	string test = "my hello";
	LOG_NORMAL("this is test:%s, str.size:%d", test.c_str(), test.size());
	LOG_INFO("this is test:%s, str.size:%d", test.c_str(), test.size());
	return 0;
}
