#include "log.h"

//定义
my_log* my_log::m_instance = NULL;
my_log::gc my_log::m_gc;

my_log::my_log()
{
	try
	{
		build_log_file_name();
		set_log_level();
		m_log_file.open(m_log_file_name.c_str(),ios_base::out|ios_base::app);
		if(!m_log_file)
		{
			throw log_error("can't open log file " + m_log_file_name);
		}
	}
	catch(log_error berr)
	{
		berr.out_err();
	}
}

my_log::~my_log()
{
	m_log_file.close();
}

int my_log::build_log_file_name()
{
	char tmstr[32] = {0};
	//only create one log file every day,the name should include date
	time_t tm=time(NULL);
	strftime(tmstr,32,"log_%Y_%m_%d.txt",localtime(&tm));

	char temp[32] = {0};
	strftime(temp, 32, "%Y%m%d", localtime(&tm));
	m_ymd = string(temp);

	string log_path = "./";
	m_log_file_name = log_path + string(tmstr);
	return 0;
}

//这个以后从配置中读取
void my_log::set_log_level()
{
	m_log_level = DEBUG;
	return;
}

int my_log::log_input(const char *file, const int line, const log_level lev, const char *fmt...)
{
	char logredstr[128] = {0};
	char loginstr[1024] = {0};
	char tmstr[32] = {0};
	va_list argp;

	//check lev ,if lev larg than LogLevel,don't write file
	if(lev > m_log_level)
	{
		return 0; 
	}

	time_t tm=time(NULL);
	strftime(tmstr,32,"%Y%m%d_%H:%M:%S",localtime(&tm));
	sprintf(logredstr,"(%s:%d)#%s#",file,line,tmstr);

	va_start(argp,fmt);
	vsprintf(loginstr,fmt,argp);

	string levlstr;
	switch(lev)
	{
	case ERR:
		levlstr=string("ERR");
		break;
	case WARN:
		levlstr=string("WARN");
		break;
	case INFO:
		levlstr=string("INFO");
		break;
	case DEBUG:
		levlstr=string("DEBUG");
		break;
	case DATA:
		levlstr=string("DATA");
		break;
	case OTHER:
		levlstr=string("OTHER");
		break;
	default:
		levlstr=string("WRONG_LEVEL");
		break;
	}


	char temp[32] = {0};
	strftime(temp, 32, "%Y%m%d", localtime(&tm));
	string current_ymd = string(temp);
	if(current_ymd != m_ymd)//第二天的日记
	{
		m_log_file.close();
		build_log_file_name();
		m_log_file.open(m_log_file_name.c_str(),ios_base::out|ios_base::app);
		try
		{
			if(!m_log_file)
			{
				throw log_error("can't open log file " + m_log_file_name);
			}
		}
		catch(log_error berr)
		{
			berr.out_err();
		}
	}
	m_log_file << logredstr << levlstr << "#" << loginstr << endl;

	return 0 ;
}

