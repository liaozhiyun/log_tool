#include "log.h"
int main(int argc, const char *argv[])
{
	MyLog::Init("./log/");
	MyLog::Log("hello");
	return 0;
}
