#include "cstdio"
#include "cstring"
#include "Logger.h"

int main()
{
    Logger::logger.warning("Test: %d %.2f", 3, 3.142);
    return 0;
}