#include "cstdio"
#include "cstring"
#include "cassert"
#include "Logger.h"
#include "RecordManager.h"
#include "fcntl.h"

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
    const char* filename = "test.db";

    // 测试创建文件后初始文件头
    RecordManager rm;
    if (access(filename, F_OK) != -1) {
        // 测试文件已存在，需要先删除
        rm.deleteFile(filename);
    }
    rm.createFile(filename, 10);
    rm.recSize=rm.recNumPerPage=rm.recNumTot=rm.pageNum=rm.usablePageHeader=0;
    rm.openFile(filename);
    Logger::logger.debug("recSize: %d, recNumPerPage: %d, recNumTot: %d, pageNum: %d, usablePageHeader: %d",
        rm.recSize, rm.recNumPerPage, rm.recNumTot, rm.pageNum, rm.usablePageHeader);
    assert(rm.recSize == 10);
    assert(rm.recNumTot == 0);
    assert(rm.pageNum == 1);
    assert(rm.usablePageHeader == -1);
    assert(8 + (rm.recNumPerPage+7)/8*8 + rm.recSize*rm.recNumPerPage <= PAGE_SIZE);

    Logger::logger.info("End testRM.");
}

int main()
{
    // testLogger();
    testRM();
    return 0;
}