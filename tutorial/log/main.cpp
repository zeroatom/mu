#include "thirdparty/mu/base/log.h"

using namespace mu;

int main(int argc,char **argv)
{
    string opt = "-log_level 4 -log_path ./logs -log_filename test -log_print_file false -log_print_screen true";
    Singleton<LogService>::Instance().Start(opt);

    LOGDEBUG("dfdfd %d",1);
    LOGTRACE("dfdfd %d",1);
    LOGINFO("dfdfd %d",1);
    LOGWARN("dfdfd %d",1);
    LOGERROR("dfdfd %d",1);
    LOGFATAL("dfdfd %d",1);
    return 0;
}
