#include "test.h"

void testLog()
{
    LOG_ERROR << "11" << "error";
    LOG_DEBUG << "11" << "debug";
    LOG_WARN << "11" << "warning";
    LOG_INFO << "11" << "info";
}
