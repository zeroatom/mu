/**
 * @file log.h
 * @brief 日志
 * @author lee
 * @version 1.0.0
 * @date 2017-01-11
 */

#ifndef _MU_BASE_LOG_H_
#define _MU_BASE_LOG_H_
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/syscall.h>

using namespace std;

#include "singleton.h"
#include "arg_config.h"

#define gettid() ::syscall(SYS_gettid)

namespace mu
{

enum LogLevel
{
    //致命错误 
    LOG_FATAL = 0,
    //一般错误
    LOG_ERROR = 1,
    //警告
    LOG_WARN = 2,
    //信息
    LOG_INFO = 3,
    //跟踪
    LOG_TRACE = 4,
    //调试
    LOG_DEBUG = 5,
    LOG_LEVEL_NUM
};

/**
* @brief 日志封装
*/
class Log
{
public:
    /**
     * @brief 
     *
     * @param _level 日志等级
     * @param _path 日志路径
     * @param _file 日志文件名称
     * @param _printFile 打印文件标识
     * @param printSceen 打印屏幕标识
     */
    Log(int _level, const string& _path, const string& _file,
            bool _printFile, bool _printSceen);
    ~Log();

    /**
     * @brief 是否日志级别开启
     *
     * @param _level 日志级别
     *
     * @return true-开启 false-未开启
     */
    bool IsLevelEnabled(int _level);
    /**
     * @brief ModLevel 修改日志等级
     *
     * @param _level 等级
     * @param _flag 标志
     */
    void ModLevel(int _level, bool _flag);
    /**
     * @brief ModPrintFile 修改打印文件标志
     *
     * @param _flag 标志
     */
    void ModPrintFile(bool _flag);
    /**
     * @brief ModPrintScreen 修改打印屏幕标志
     *
     * @param _flag 标志
     */
    void ModPrintScreen(bool _flag);

    /**
     * @brief 记录日志
     *
     * @param _level 日志级别
     * @param _file 文件名称
     * @param _line 行数 
     * @param _content 日志内容
     * @param _tid 线程id
     */
    void LogContent(int _level, const char* _file, unsigned int _line, const char* _func, const string& _content, long _tid);

protected:
    bool CheckAndCreateDir(struct tm& _tm_val);

protected:
    /**
     * @brief 开启的日志等级
     */
    int m_enableLevel;
    /**
     * @brief 开启屏幕打印
     */
    bool m_enableScreen;
    /**
     * @brief 开启文件
     */
    bool m_enableFile;

    /**
     * @brief 上次创建日志文件时间
     */
    struct tm m_lastCreateDirTm;


    /**
     * @brief 文件流
     */
    ofstream m_file;
    /**
     * @brief 文件路径
     */
    string m_path;
    /**
     * @brief 日志文件名
     */
    string m_filename;
};

//生成异步记录日志宏
#define LOG_IMPL_MACRO(func,LOG_LEVEL)                                       \
    inline void func(const char* _file, unsigned int _line, const char* _func, const char* _fmt, ...)                      \
    {                                                                           \
        if( NULL != m_log && m_log->IsLevelEnabled(LOG_LEVEL))                   \
        {                                                                       \
            char content[512];                                                  \ 
            va_list ap;                                                         \
            va_start(ap,_fmt);                                                  \
            vsnprintf(content,sizeof(content) - 1, _fmt, ap);                   \
            va_end(ap);                                                         \
            m_log->LogContent(LOG_LEVEL,_file, _line, _func,content,gettid());   \
        }                                                                       \
    }                                                                           \

/**
 * @brief 日志服务
 */
class LogService
{
public:
    LogService();
    ~LogService();
    /**
     * @brief start 开启日志服务
     *
     * @param _opt 开启参数
     *  -log_level 日志等级
     *  -log_path 日志路径
     *  -log_filename 日志名称
     *  -log_print_file 是否打印文件
     *  -log_print_screen 是否打印屏幕
     *
     * @return 0- 开启成功 其他- 失败
     */
    int Start(const string& _opt);
    /**
     * @brief Start 开启日志服务
     *
     * @param _arg 开启参数
     *
     * @return 0- 开启成功 其他- 失败
     */
    int Start(ArgConfig& _arg);
    /**
     * @brief stop 停止日志服务
     *
     * @return 0- 停止成功 其他- 失败
     */
    int Stop();


    LOG_IMPL_MACRO(AsyncLogDebug, LOG_DEBUG);
    LOG_IMPL_MACRO(AsyncLogTrace, LOG_TRACE);
    LOG_IMPL_MACRO(AsyncLogInfo, LOG_INFO);
    LOG_IMPL_MACRO(AsyncLogWarn, LOG_WARN);
    LOG_IMPL_MACRO(AsyncLogError, LOG_ERROR);
    LOG_IMPL_MACRO(AsyncLogFatal, LOG_FATAL);

    /**
     * @brief 修改日志等级开启标识
     *
     * @param _level 日志等级
     * @param _flag 开启标识
     */
    void ModLevel(int _level, bool _flag);
    /**
     * @brief 修改日志写文件标识
     *
     * @param _flag 开启标识
     */
    void ModPrintFile(bool _flag);
    /**
     * @brief 开启打印屏幕标识
     *
     * @param _flag 开启标识
     */
    void ModPrintScreen(bool _flag);

private:
    /**
     * @brief 日志处理
     */
    Log* m_log;
};
#define LOGDEBUG(fmt, ...)  Singleton<LogService>::Instance().AsyncLogDebug(__FILE__,__LINE__,__FUNCTION__,fmt, ## __VA_ARGS__)
#define LOGTRACE(fmt, ...)  Singleton<LogService>::Instance().AsyncLogTrace(__FILE__,__LINE__,__FUNCTION__,fmt, ## __VA_ARGS__)
#define LOGINFO(fmt, ...)   Singleton<LogService>::Instance().AsyncLogInfo(__FILE__,__LINE__,__FUNCTION__,fmt, ## __VA_ARGS__)
#define LOGWARN(fmt, ...)   Singleton<LogService>::Instance().AsyncLogWarn(__FILE__,__LINE__,__FUNCTION__,fmt, ## __VA_ARGS__)
#define LOGERROR(fmt, ...)  Singleton<LogService>::Instance().AsyncLogError(__FILE__,__LINE__,__FUNCTION__,fmt, ## __VA_ARGS__)
#define LOGFATAL(fmt, ...)  Singleton<LogService>::Instance().AsyncLogFatal(__FILE__,__LINE__,__FUNCTION__,fmt, ## __VA_ARGS__)

};

#endif
