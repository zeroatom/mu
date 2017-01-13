#include "arg_config.h"
#include "string_utility.h"

#include <stdlib.h>
#include <algorithm>
#include <fstream>

using namespace mu;

ArgConfig::ArgConfig(int argc,char* argv[])
{
    int maxKeyIndex = argc - 2;
    for(int i = 1; i <= maxKeyIndex ; i=i+2)
    {
        std::string key = argv[i];
        std::string value = argv[i+1];
        m_configs[key] = value;
    }
    
    if(IsEnableOption("-f"))
    {
        std::string configFile = Get("-f","default.config");
        LoadFromFile(configFile);
    }

}

ArgConfig::ArgConfig(std::string _args)
{
    std::vector<std::string> v;
    StringUtility::Split(_args,v, " ");

    int maxKeyIndex = v.size() - 2;
    for(int i = 0; i <= maxKeyIndex; i=i+2)
    {
        std::string key = v[i];
        std::string value = v[i+1];
        m_configs[key] = value;
    }

    if(IsEnableOption("-f"))
    {
        std::string configFile = Get("-f","default.config");
        LoadFromFile(configFile);
    }

}

ArgConfig::~ArgConfig()
{
    m_configs.clear();
}

std::string ArgConfig::Get(const std::string& _name, const std::string& _defaultValue)
{
    return m_configs.count(_name) ? m_configs[_name] : _defaultValue;
}

int32_t ArgConfig::GetInteger(const std::string& _name,int32_t _defaultValue)
{
    std::string valStr = Get(_name,"");
    const char* value = valStr.c_str();
    char* end;
    int32_t n = strtol(value,&end, 0);
    return end > value ? n : _defaultValue;
}

double ArgConfig::GetReal(const std::string& _name, double _defaultValue)
{
    std::string valStr = Get(_name,"");
    const char* value = valStr.c_str();
    char* end;
    double n = strtod(value,&end);
    return end > value ? n : _defaultValue;
}

bool ArgConfig::GetBoolean(const std::string& _name, bool _defaultValue)
{
    std::string valStr = Get(_name,"");
    std::transform(valStr.begin(), valStr.end(), valStr.begin(), ::tolower);
    if(valStr == "true" || valStr == "yes" || valStr == "on" || valStr == "1")
    {
        return true;
    }
    else if(valStr == "false" || valStr == "no" || valStr == "off" || valStr == "0")
    {
        return false;
    }
    else
    {
        return _defaultValue;
    }
}

int ArgConfig::LoadFromFile(const std::string& _file)
{
    std::ifstream inf(_file.c_str());
    std::string all;
    std::string tmp;

    while(false == inf.eof())
    {
        if(!getline(inf, tmp))
        {
            break;
        }
        if(false == tmp.empty() && '#' == tmp[0])
        {
            continue;
        }

        all += tmp + " ";
        tmp.clear();
    }

    std::vector<std::string> v;
    StringUtility::Split(all,v, " ");

    int maxKeyIndex = v.size() - 2;
    for(int i = 0; i <= maxKeyIndex; i=i+2)
    {
        std::string key = v[i];
        std::string value = v[i+1];
        m_configs[key] = value;
    }
    return 0;

}
bool ArgConfig::IsEnableOption(const std::string& _name)
{
    return m_configs.count(_name) ? true : false;
}
