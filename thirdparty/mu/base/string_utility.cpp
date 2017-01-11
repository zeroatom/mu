#include "string_utility.h"
using namespace mu;


int StringUtility::split(const string& _str, vector<string>& _ret_, string _sep )
{
    if (_str.empty())
    {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = _str.find_first_not_of(_sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos)
    {
        comma_pos = _str.find(_sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = _str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + _sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
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
