#include <string.h>

#include "log.h"
#include "time_utility.h"

using namespace mu;

Log::Log(int _level, const string& _allClass, const string& _path, const string& _file,
            bool _printFile, bool printSceen):
    m_enableLevel(_level),
    m_enableClassSet(new strSet())
{
   for(int i = 0; i <= _level; ++i) 
   {
       m_enableLevel |= (1 << i);
   }
}

bool Log::IsLevelEnabled(int _level)
{
    return m_enableLevel & (1 << _level);
}

// 颜色输出
static const char* g_log_color_head[] =
{
    "\033[0;35m",
    "\033[0;31m",
    "\033[1;34m",
    "\033[1;32m",
    "",
    "\033[1;33m"
};
static const char* g_log_color_tail[] =
{
    "\033[0m",
    "\033[0m",
    "\033[0m",
    "\033[0m",
    "",
    "\033[0m"
};

static const char* g_log_level_desp[] =
{
    "FATAL",
    "ERROR",
    "WARN ",
    "INFO ",
    "TRACE",
    "DEBUG"
};

void Log::LogContent(int _level, const char* _strClass, const string& _content)
{
    string stringTime = TimeUtility::GetStringTime();
    char logBuff[512];
    ::snprintf(logBuff, sizeof(logBuff) - 1,"%s %s [%ld] [%s]", stringTime.c_str(), g_log_level_desp[_level], gettid(), _strClass);
    if(m_enableFile)
    {
        m_file << logBuff << _content << endl;
        m_file.flush();
    }

    if(m_enableScreen)
    {
        printf("%s%s%s%s\n", g_log_color_head[_level], logBuff, _content.c_str(), g_log_color_tail[_level]);
    }
}
