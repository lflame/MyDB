#include "cstdio"
#include "cstring"
#include "Logger.h"

void testLogger() {
    printf("testLogger start.\n");
    Logger::logger.debug("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.info("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.warning("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.error("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.fatal("%s %d %.2f", "test", 3, 3.142);
    printf("testLogger end.\n");
}

int main()
{
    testLogger();
    
    return 0;
}