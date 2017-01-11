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

using namespace std;

#include "singleton.h"

namespace mu
{

enum LogLevel
{
    //致命错误 
    LOG_FATAL = 0,
    //一般错误
    LOG_ERROR,
    //警告
    LOG_WARN,
    //信息
    LOG_INFO,
    //跟踪
    LOG_TRACE,
    //调试
    LOG_DEBUG,
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
     * @param _allClass 所有开启日志的类
     * @param _path 日志路径
     * @param _file 日志文件名称
     * @param _printFile 打印文件标识
     * @param printSceen 打印屏幕标识
     */
    Log(int _level, const string& _allClass, const string& _path, const string& _file,
            bool _printFile, bool printSceen);

    /**
     * @brief 是否日志级别开启
     *
     * @param _level 日志级别
     *
     * @return true-开启 false-未开启
     */
    bool IsLevelEnabled(int _level);

    /**
     * @brief 记录日志
     *
     * @param _level 日志级别
     * @param _strClass 日志属于的类
     * @param _content 日志内容
     */
    void LogContent(int _level, const char* _strClass, const string& _content);
protected:
    /**
     * @brief 开启的日志等级
     */
    bool m_enableLevel;
    /**
     * @brief 开启屏幕打印
     */
    bool m_enableScreen;
    /**
     * @brief 开启文件
     */
    bool m_enableFile;
    typedef set<string> strSet;
    /**
     * @brief 开启的所有类
     */
    strSet* m_enableClassSet;

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

#define LOG_IMPL_NONE_ARG(func,LOG_LEVEL)                                       \
    inline void func(const char* _class, const char* _fmt)                      \
    {                                                                           \
        if(m_log->IsLevelEnabled(LOG_LEVEL))                                    \
        {                                                                       \
            const char* className = m_log->findClassName(_class);               \
            if(className)                                                       \
            {                                                                   \
                m_log->LogContent(LOG_LEVEL,className,string(_fmt),gettid());   \
            }                                                                   \
        }                                                                       \
    }                                                                           \

//生成异步记录日志宏
#define LOG_IMPL_MACRO(AsyncFunc, LOG_LEVEL)    \
    LOG_IMPL_NONE_ARG(AsyncFunc, LOG_LEVEL)     \
    LOG_IMPL_ARG1(AsyncFunc, LOG_LEVEL)         \
    LOG_IMPL_ARG2(AsyncFunc, LOG_LEVEL)         \
    LOG_IMPL_ARG3(AsyncFunc, LOG_LEVEL)         \
    LOG_IMPL_ARG4(AsyncFunc, LOG_LEVEL)         \
    LOG_IMPL_ARG5(AsyncFunc, LOG_LEVEL)         \
    LOG_IMPL_ARG6(AsyncFunc, LOG_LEVEL)         \

/**
 * @brief 日志服务
 */
class LogService
{
public:
    LogService();
    ~LogService();


    LOG_IMPL_MACRO(AsyncLogDebug, LOG_DEBUG);
    LOG_IMPL_MACRO(AsyncLogTrace, LOG_TRACE);
    LOG_IMPL_MACRO(AsyncLogInfo, LOG_INFO);
    LOG_IMPL_MACRO(AsyncLogwarn, LOG_WARN);
    LOG_IMPL_MACRO(AsyncLogError, LOG_ERROR);
    LOG_IMPL_MACRO(AsyncLogFatal, LOG_FATAL);

    /**
     * @brief 修改日志等级开启标识
     *
     * @param _level 日志等级
     * @param _flag 开启标识
     */
    void mod_level(int _level, bool _flag);
    /**
     * @brief 修改日志记录类开启标识
     *
     * @param _class 日志类
     * @param _flag 开启标识
     */
    void mod_class(int _class, bool _flag);
    /**
     * @brief 修改日志写文件标识
     *
     * @param _flag 开启标识
     */
    void mod_print_file(bool _flag);
    /**
     * @brief 开启打印屏幕标识
     *
     * @param _flag 开启标识
     */
    void mod_print_screen(bool _flag);

private:
    /**
     * @brief 日志处理
     */
    Log* m_log;
};
//异步打印日志 宏
#define LOGDEBUG(content)  Singleton<LogService>::Instance().AsyncLogDebug content
#define LOGTRACE(content)  Singleton<LogService>::Instance().AsyncLogTrace content
#define LOGINFO(content)   Singleton<LogService>::Instance().AsyncLogInfo  content
#define LOGWARN(content)   Singleton<LogService>::Instance().AsyncLogWarn  content
#define LOGERROR(content)  Singleton<LogService>::Instance().AsyncLogError content
#define LOGFATAL(content)  Singleton<LogService>::Instance().AsyncLogFatal content

};

#endif

