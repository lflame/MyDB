#include "cstdio"
#include "cstring"
#include "Logger.h"
#include "RecordManager.h"

void testLogger() {
    Logger::logger.info("Start testLogger.");

    Logger::logger.debug("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.info("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.warning("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.error("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.fatal("%s %d %.2f", "test", 3, 3.142);

    Logger::logger.info("End testLogger.");
}

void testRM() {
    Logger::logger.info("Start testRM.");

    RecordManager rm;
    rm.createFile("test.db", 10);
    rm.recSize=rm.recNumPerPage=rm.recNumTot=rm.pageNum=rm.usablePageHeader=0;
    rm.openFile("test.db");
    Logger::logger.debug("recSize: %d, recNumPerPage: %d, recNumTot: %d, pageNum: %d, usablePageHeader: %d",
        rm.recSize, rm.recNumPerPage, rm.recNumTot, rm.pageNum, rm.usablePageHeader);

    Logger::logger.info("End testRM.");
}

int main()
{
    // testLogger();
    testRM();
    return 0;
}