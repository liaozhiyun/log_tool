#ifndef _LOG_H__
#define _LOG_H__

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

using namespace std;

enum log_level{ERR,WARN,INFO,DEBUG,DATA,OTHER};

class log_error
{
	private:
		string m_err_info;
	public:
		log_error(string info)
		{
			m_err_info = info;
		}
		~log_error(){}
		void out_err()
		{
			cout << m_err_info << endl;
			return;
		}
};

//这个类不能命名为log, 否则在用log*时会报编译错误，原因未知
class my_log
{
	public:
		~my_log();
		int log_input(const char* file, const int line, const log_level lev, const char *fmt...);
		static my_log* instance()
		{
			if(m_instance == NULL)
			{
				m_instance = new my_log();
			}
			return m_instance;
		}

	private:
		class gc
		{
			public:
				~gc()
				{
					if(m_instance != NULL)
					{
						delete m_instance;
					}
				}
		};
		static gc m_gc;
		static my_log* m_instance;
		my_log();
	private:
		log_level m_log_level;
		string m_log_file_name;
		fstream m_log_file;
	private:
		int build_log_file_name();
		string m_ymd;
		void set_log_level();
};

#define LOGW(level, fmt...) my_log::instance()->log_input(__FILE__, __LINE__, level, fmt)
#define LOG_ERR(fmt...) my_log::instance()->log_input(__FILE__, __LINE__, ERR, fmt)
#define LOG_WARN(fmt...) my_log::instance()->log_input(__FILE__, __LINE__, WARN, fmt)
#define LOG_INFO(fmt...) my_log::instance()->log_input(__FILE__, __LINE__, INFO, fmt)
#define LOG_DEBUG(fmt...) my_log::instance()->log_input(__FILE__, __LINE__, DEBUG, fmt)
#define LOG_DATA(fmt...) my_log::instance()->log_input(__FILE__, __LINE__, DATA, fmt)
#define LOG_OTHER(fmt...) my_log::instance()->log_input(__FILE__, __LINE__, OTHER, fmt)
#endif

/*
	日记系统改造：
分配一把锁与条件变量，日记本身只往日记数据库（队列）提交日记数据，
日记记录线程往日记数据库拿日记数据，解析后写到对应的日记记录文件中。
这是生产者与消费者模型。这样可以高效的记录日记而对系统本身的性能影响较小
struct log_msg
{
	int id;//ERR, WARN, INFO, DEBUG, DATA, OTHER
	unsigned char* ptr;
	int len;
};
这个结构在提交时以指针的形式提交，这样就只有4(32bit) or 8(64bit)字节的拷贝，指针的高效使用
*/
