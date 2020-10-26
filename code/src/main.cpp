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
    const int testRecSize = 10;
    rm.createFile(filename, testRecSize);
    rm.recSize=rm.recNumPerPage=rm.recNumTot=rm.pageNum=rm.usablePageHeader=0;
    rm.openFile(filename);
    Logger::logger.debug("recSize: %d, recNumPerPage: %d, recNumTot: %d, pageNum: %d, usablePageHeader: %d",
        rm.recSize, rm.recNumPerPage, rm.recNumTot, rm.pageNum, rm.usablePageHeader);
    Logger::logger.debug("dataPageMapBitsNum: %d", rm.dataPageMapBitsNum);
    assert(rm.recSize == testRecSize);
    assert(rm.recNumTot == 0);
    assert(rm.pageNum == 1);
    assert(rm.usablePageHeader == -1);
    assert(12 + (rm.recNumPerPage+7)/8*8 + rm.recSize*rm.recNumPerPage <= PAGE_SIZE);

    // 测试插入超过一页
    char record[testRecSize] = "123456789";
    RID rid;
    for (int i = 1; i <= rm.recNumPerPage + 2; ++i) {
        rm.insertRecord(record, rid);
        if (i >= rm.recNumPerPage - 2)
            Logger::logger.debug("rid.pageId: %d, rid.slotId: %d", rid.pageId, rid.slotId);
    }
    assert(rm.pageNum == 3);
    assert(rid.pageId == 2);
    assert(rid.slotId == 1);

    // 测试读取
    char record2[testRecSize];
    rm.getRecord(record2, RID(1, 1));
    Logger::logger.debug("record: %s", record2);
    assert(strcmp(record, record2)==0);

    Logger::logger.info("End testRM.");
}

int main()
{
    // testLogger();
    testRM();
    Logger::logger.info("测试通过");
    return 0;
}