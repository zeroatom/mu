#include "string_utility.h"

#include <algorithm>

namespace mu
{

int StringUtility::Split(const std::string& _str,
    std::vector<std::string>& _ret_,
    const std::string _sep)
{
    if (_str.empty())
    {
        return 0;
    }

    std::string tmp;
    std::string::size_type pos_begin = _str.find_first_not_of(_sep);
    std::string::size_type comma_pos = 0;

    while (pos_begin != std::string::npos)
    {
        comma_pos = _str.find(_sep, pos_begin);
        if (comma_pos != std::string::npos)
        {
            tmp = _str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + _sep.length();
        }
        else
        {
            tmp = _str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty())
        {
            _ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

bool StringUtility::StartWith(const std::string& _str, const std::string& _prefix)
{
    if (_prefix.length() > _str.length()) {
        return false;
    }

    if (memcmp(_str.c_str(), _prefix.c_str(), _prefix.length()) == 0) {
        return true;
    }

    return false;
}
bool StringUtility::EndWith(const std::string& _str, const std::string& _suffix)
{
    if(_suffix.length() > _str.length())
    {
        return false;
    }

    return (_str.substr(_str.length() - _suffix.length()) == _suffix);
}

std::string& StringUtility::Ltrim(std::string& _str_)
{
    std::string::iterator it = find_if(_str_.begin(), _str_.end(), std::not1(std::ptr_fun(::isspace)));
    _str_.erase(_str_.begin(), it);
    return _str_;
}
std::string& StringUtility::Rtrim(std::string& _str_)
{
    std::string::reverse_iterator it = find_if(_str_.rbegin(),
        _str_.rend(), std::not1(std::ptr_fun(::isspace)));

    _str_.erase(it.base(), _str_.end());
    return _str_;
}
std::string& StringUtility::Trim(std::string& _str_)
{
    return Rtrim(Ltrim(_str_));
}
void StringUtility::Trim(std::vector<std::string>& _strList_)
{
    std::vector<std::string>::iterator it;
    for(it = _strList_.begin(); it != _strList_.end(); ++it)
    {
        *it = Trim(*it);
    }
}

} // namespace mu
