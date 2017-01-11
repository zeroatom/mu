/**
 * @file time_utility.h
 * @brief 时间相关功能
 * @author lee
 * @version 1.0.0
 * @date 2017-01-11
 */

#ifndef _BASE_TIME_UTILITY_H_
#define _BASE_TIME_UTILITY_H_
#include <stdint.h>
#include <string>

namespace mu
{

class TimeUtility
{
public:
    /**
     * @brief 得到当前毫秒数
     *
     * @return 
     */
    static int64_t GetCurrentMs();
    /**
     * @brief 得到字符串形式的时间格式: 1999-9-9 9:9:9
     *
     * @return 时间的字符串
     */
    static std::string GetStringTime();
    /**
     * @brief 字符串格式转为time_t
     *
     * @param time
     *
     * @return 时间
     */
    static time_t ConvertTime(const std::string &_time);
    /**
     * @brief 获取时间差
     *
     * @param _t1 时间1
     * @param _t2 时间2
     *
     * @return 时间1-时间2
     */
    static time_t GetTimeDiff(const std::string &_t1, const std::string&_t2);
    
};

}; // namespace mu

#endif
