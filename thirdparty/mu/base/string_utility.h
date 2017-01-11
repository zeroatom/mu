/**
 * @file string_utility.h
 * @brief 字符串处理库
 * @author lee
 * @version 1.0.0
 * @date 2017-01-11
 */

#ifndef _BASE_STRING_UTILITY_H_
#define _BASE_STRING_UTILITY_H_

#include <string.h>
#include <vector>
using namespace std;
namespace mu
{

class StringUtility
{
public:
    /**
     * @brief 切割字符串
     *
     * @param _str 被切割字符串
     * @param _ret_ 存储切割完字符串容器
     * @param _sep 分割字符串
     *
     * @return 0- 成功 其他- 错误
     */
    static int split(const string& _str, vector<string>& _ret_, string _sep = ",");
};

}; // namespace mu
#endif
