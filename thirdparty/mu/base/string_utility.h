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
#include <iostream>
#include <vector>

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
    static int Split(const std::string& _str,
        std::vector<std::string>& _ret_,
        const std::string _sep = ",");
    /**
     * @brief 判断字符串是否以_prefix开头
     *
     * @param _str 检查字符串
     * @param _prefix 头字符串
     *
     * @return true- 正确 false- 不正确
     */
    static bool StartWith(const std::string& _str, const std::string& _prefix);
    /**
     * @brief EndWith 判断是否以_suffix结尾
     *
     * @param _std 检查字符串
     * @param _suffix 尾字符串
     *
     * @return true- 正确 false- 不正确
     */
    static bool EndWith(const std::string& _str, const std::string& _suffix);
    /**
     * @brief Ltrim 去除左边空格
     *
     * @param _str_ 原始字符串
     *
     * @return 去除左空格的字符串
     */
    static std::string& Ltrim(std::string& _str_);
    /**
     * @brief Rtrim 去除右边空格
     *
     * @param _str_ 原始字符串
     *
     * @return 去除完右空格的字符串
     */
    static std::string& Rtrim(std::string& _str_);
    /**
     * @brief Trim 去除字符串左右两边空格
     *
     * @param _str_ 原始字符串
     *
     * @return  去除完左右空格的字符串
     */
    static std::string& Trim(std::string& _str_);
    /**
     * @brief Trim 去除字符串列表两边空格
     *
     * @param _strList_ 字符串列表
     */
    static void Trim(std::vector<std::string>& _strList_);
};

}; // namespace mu
#endif
