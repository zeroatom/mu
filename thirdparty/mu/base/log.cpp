#include <errno.h>
#include <sys/syscall.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "log.h"
#include "time_utility.h"


using namespace mu;

Log::Log(int _level, const string& _path, const string& _file,
            bool _printFile, bool _printSceen):
    m_enableLevel(0)
{
   for(int i = 0; i < _level && i < LOG_LEVEL_NUM; ++i) 
   {
       m_enableLevel |= (1 << i);
   }

   m_path = _path;
   m_filename = _file;
   m_enableFile = _printFile;
   m_enableScreen = _printSceen;
}

Log::~Log()
{
    m_file.close();
}

bool Log::IsLevelEnabled(int _level)
{
    return m_enableLevel & (1 << _level);
}

void Log::ModLevel(int _level, bool _flag)
{
    if(_flag)
    {
        m_enableLevel |= (1 << _level);
    }
    else 
    {
        m_enableLevel &= (1 << _level);
    }
}

void Log::ModPrintFile(bool _flag)
{
    m_enableFile = _flag;
}

void Log::ModPrintScreen(bool _flag)
{
    m_enableScreen = _flag;
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

void Log::LogContent(int _level, const char* _file, unsigned int _line, const char* _func, const string& _content, long _tid)
{
    struct timeval curtm;
    gettimeofday(&curtm, NULL);
    struct tm tm_val = *localtime(&(curtm.tv_sec));

    char logBuff[512];
    ::snprintf(logBuff, sizeof(logBuff) - 1,"%02d:%02d:%02d.%03ld %ld [%s] %s:%s:%d ",
        tm_val.tm_hour, tm_val.tm_min, tm_val.tm_sec, curtm.tv_usec/1000,
        _tid, g_log_level_desp[_level], _file, _func,_line);
    if(m_enableFile && CheckAndCreateDir(tm_val))
    {
        m_file << logBuff << _content << endl;
        m_file.flush();
    }

    if(m_enableScreen)
    {
        printf("%s%s%s%s\n", g_log_color_head[_level], logBuff, _content.c_str(), g_log_color_tail[_level]);
    }
}

bool Log::CheckAndCreateDir(struct tm& _tm_val)
{
    bool timeEqual = TimeUtility::IsSameDay(m_lastCreateDirTm, _tm_val);

    char file[1024];

    if(false == timeEqual)
    {
        int rc = ::access(m_path.c_str(), F_OK);
        if( 0 != rc)
        {
            rc = ::mkdir(m_path.c_str(), 0777);
            if(0 != rc)
            {
                printf("log mkdir<%s>failed error<%s>\n", m_path.c_str(), ::strerror(errno));
                return false;
            }
        }


        ::snprintf(file, sizeof(file), "%s/%d-%d-%d",
                   m_path.c_str(), _tm_val.tm_year + 1900, _tm_val.tm_mon + 1, _tm_val.tm_mday);
        rc = ::access(file, F_OK);
        if (0 != rc)
        {
            rc = ::mkdir(file, 0777);
            if (rc != 0)
            {
                printf("log mkdir file<%s>failed error<%s>\n", file, ::strerror(errno));
                return false;
            }
        }
    }

    if(false == timeEqual)
    {
        ::snprintf(file, sizeof(file), "%s/%d-%d-%d/%s.log",
            m_path.c_str(), _tm_val.tm_year + 1900, _tm_val.tm_mon + 1, _tm_val.tm_mday,
            m_filename.c_str());        
        m_file.close();
        m_file.open(file);
        m_lastCreateDirTm = _tm_val;
    }
    return true;
}

LogService::LogService():
    m_log(NULL)
{
}

LogService::~LogService()
{
    Stop();
}

int LogService::Start(const string& _opt)
{
    ArgConfig arg(_opt);
    return Start(arg);
}

int LogService::Start(ArgConfig& _arg)
{
    int level = _arg.GetInteger("-log_level", 5);
    string path = _arg.Get("-log_path","./log");
    string filename = _arg.Get("-log_filename", "log");
    bool printFile = _arg.GetBoolean("-log_print_file", true);
    bool printScreen = _arg.GetBoolean("-log_print_screen", false);

    m_log = new Log(level,path,filename,printFile,printScreen);
    return 0;
}

int LogService::Stop()
{
    if(NULL == m_log)
    {
        return 0;
    }

    delete m_log;
    m_log = NULL;
    return 0;
}

void LogService::ModLevel(int _level, bool _flag)
{
    if(NULL != m_log)
    {
        m_log->ModLevel(_level, _flag);
    }
}

void LogService::ModPrintFile(bool _flag)
{
    if(NULL != m_log)
    {
        m_log->ModPrintFile(_flag);
    }
}
void LogService::ModPrintScreen(bool _flag)
{
    if(NULL != m_log)
    {
        m_log->ModPrintScreen(_flag);
    }
}
