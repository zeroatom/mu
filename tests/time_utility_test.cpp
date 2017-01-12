#include "thirdparty/mu/base/time_utility.h"

#include <gtest/gtest.h>
#include <iostream>

using namespace std;

using namespace mu;

TEST(TimeUtilityTest,HandleNoneZeroInput)
{
    cout <<"获取当前时间: "<< TimeUtility::GetCurrentMs() << endl;

    //EXPECT_EQ(1,2);
}


