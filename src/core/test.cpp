#include "test.h"

void testLog()
{
    Logging("1");
    Logging(1) << 1;
    Logging(1.23f);
    Logging(1.2345);
    Logging('c');
    for(int i = 0; i < 1000; i++)
    {
        Logging("now:") << i;
    }
}
