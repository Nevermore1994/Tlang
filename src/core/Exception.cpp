#include"Exception.h"

using namespace T;

constexpr int32_t kBufSize = 1024;
constexpr int32_t kErrorSize = 2048;

Exception::Exception(Compiler& m)
    :manager_(m)
{

}

void Exception::handleException(WorkStage stage, ErrorLevel level, const char* fmt, va_list ap)
{
    char buf[kBufSize];
	char errstr[kErrorSize];
	vsprintf_s(buf, kBufSize,fmt, ap);
	if (stage == STAGE_COMPILE)
	{
        auto fileName = manager_.getLex()->getReadFile().getFileName();
        auto lineNum = manager_.getLex()->getFileLineNum();
		if (level == LEVEL_WARNING)
		{
			sprintf_s(errstr, kErrorSize, "%s(NO. %lld line):compiler warning：%s\n:", fileName, lineNum, buf);
		}
		else
		{
			sprintf_s(errstr, kErrorSize, "%s(NO. %lld line):compile error ：%s\n:", fileName, lineNum, buf);
			//Cleanup();
			exit(-1);
		}
	}
	else
	{
		sprintf_s(errstr, kErrorSize, "link error:%s\n", buf);
		//Cleanup();
		exit(-1);
	}
}
//end namespace T;
