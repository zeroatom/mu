#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "time_utility.h"

using namespace mu;

/**
 * @brief 得到当前毫秒数
 *
 * @return 
 */
int64_t TimeUtility::GetCurrentMs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    int64_t timestamp = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return timestamp;
}

/**
 * @brief 得到字符串形式的时间格式: 1999-9-9 9:9:9
 *
 * @return 时间的字符串
 */
std::string TimeUtility::GetStringTime()
{
    time_t now = time(NULL);

    struct tm tm_now;
    struct tm* p_tm_now;

    p_tm_now = localtime_r(&now, &tm_now);

    char buff[256] = {0};
    snprintf(buff, sizeof(buff), "%04d-%02d-%02d% 02d:%02d:%02d",
        1900 + p_tm_now->tm_year,
        p_tm_now->tm_mon + 1,
        p_tm_now->tm_mday,
        p_tm_now->tm_hour,
        p_tm_now->tm_min,
        p_tm_now->tm_sec);

    return std::string(buff);
}

time_t TimeUtility::ConvertTime(const std::string &time)
{
    tm tm_;
    char buf[128] = { 0 };
    strncpy(buf, time.c_str(), sizeof(buf)-1);
    buf[sizeof(buf) - 1] = 0;
    strptime(buf, "%Y-%m-%d %H:%M:%S", &tm_);
    tm_.tm_isdst = -1;
    return mktime(&tm_);
}


time_t TimeUtility::GetTimeDiff(const std::string &_t1, const std::string&_t2)
{
    time_t time1 = ConvertTime(_t1);
    time_t time2 = ConvertTime(_t2);
    time_t time = time1 - time2;
    return time;
}

bool TimeUtility::IsSameDay(struct tm& t1, struct tm& t2)
{
    return t1.tm_mday == t2.tm_mday && t1.tm_mon == t2.tm_mon && t1.tm_year == t2.tm_year;
}
