/**
 * @file arg_config.h
 * Copyright (C) 

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 * @brief 参数配置管理
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-12
 */

#ifndef _BASE_ARG_CONFIG_H_
#define _BASE_ARG_CONFIG_H_
#include <string>
#include <map>
#include <stdint.h>

namespace mu
{

class ArgConfig
{
public:
    /**
     * @brief ArgConfig 
     *
     * @param argc 参数必须为奇数个由使用者控制
     * @param argv[]
     */
    ArgConfig(int argc,char* argv[]);
    ArgConfig(std::string _args);
    ~ArgConfig();

    /**
     * @brief Get 获取字符串value 
     *
     * @param _name key值
     * @param _defaultValue 默认值
     *
     * @return key对应的值
     */
    std::string Get(const std::string& _name, const std::string& _defaultValue);    
    /**
     * @brief GetInteger 获取整数值
     *
     * @param _name key值
     * @param _defaultValue 默认值
     *
     * @return  key对应的值
     */
    int32_t GetInteger(const std::string& _name,int32_t _defaultValue);
    /**
     * @brief GetReal 获取浮点数
     *
     * @param _name key值
     * @param _defaultValue 默认值
     *
     * @return key对应的值
     */
    double GetReal(const std::string& _name, double _defaultValue);
    /**
     * @brief GetBoolean 获取bool值
     *
     * @param _name key值
     * @param _defaultValue 默认值
     *
     * @return 
     */
    bool GetBoolean(const std::string& _name, bool _defaultValue);

    /**
     * @brief LoadFromFile 从配置文件加载配置
     *
     * @param _file 文件路径
     *
     * @return 0- 加载成功 其他- 加载失败
     */
    int LoadFromFile(const std::string& _file);
    /**
     * @brief IsEnableOption 是否存在参数
     *
     * @param _name 参数名字
     *
     * @return true- 存在 false- 不存在
     */
    bool IsEnableOption(const std::string& _name);
private:
    /**
     * @brief 所有的配置
     */
    std::map<std::string, std::string> m_configs;    
};

}; // namespace mu
#endif

