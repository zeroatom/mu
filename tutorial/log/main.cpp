#include "thirdparty/mu/base/log.h"

using namespace mu;

int main(int argc,char **argv)
{
    Singleton<LogService>::Instance().Start("-log");

    LOGDEBUG("dfdfd %d",1);
    LOGTRACE("dfdfd %d",1);
    LOGINFO("dfdfd %d",1);
    LOGWARN("dfdfd %d",1);
    LOGERROR("dfdfd %d",1);
    LOGFATAL("dfdfd %d",1);
    return 0;
}
