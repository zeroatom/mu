#include "thirdparty/mu/base/string_utility.h"

#include <gtest/gtest.h>
#include <iostream>    


using namespace mu;
using namespace std;

TEST(StringUtilityTest,HandleNoneZeroInput)
{
    string str = "1,2,3";
    vector<string> ret;
    StringUtility::Split(str,ret);
    EXPECT_EQ((unsigned int)3,ret.size());

    string src = "7road1234";
    string prifix = "7road";
    string sufix = "1234";

    EXPECT_EQ(true,StringUtility::StartWith(src,prifix));
    EXPECT_EQ(false,StringUtility::StartWith(src,sufix));
    EXPECT_EQ(true,StringUtility::EndWith(src,sufix));
    EXPECT_EQ(false,StringUtility::EndWith(src,prifix));

    string src2 = "  7road  ";
    cout << "原始字符串: >" << src2 << "<"<<endl;
    cout << "去除所有空格: >"<< StringUtility::Trim(src2) << "<" <<endl;

    vector<string> srcV;
    srcV.push_back("  21 ");
    srcV.push_back(" 12  ");
    StringUtility::Trim(srcV);
    cout << "去除空格的vector" << endl;
    for(vector<string>::iterator it = srcV.begin(); it != srcV.end(); ++it)
    {
        cout << *(it) << endl;
    }
    cout << "-----------------"<<endl;
}


