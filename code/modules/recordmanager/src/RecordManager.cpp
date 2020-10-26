#include "RecordManager.h"
#include "bitset"
#include "netinet/in.h"
#include "cstring"
#include "cassert"
#include "Logger.h"
#define lowbit(x) ((x)&(-x))

RecordManager::RecordManager() {
    bufPageManager = new BufPageManager(new FileManager);

    memset(powerToIndex, -1, sizeof(powerToIndex));
    for (int i = 0; i <= 7; ++i)
        powerToIndex[1<<i] = i;
}

RecordManager::~RecordManager() {
    delete bufPageManager;
}

void RecordManager::createFile(const char *name, int inRecSize) {
    assert(fileId == -1);
    bufPageManager->fileManager->createFile(name);
    bufPageManager->fileManager->openFile(name, fileId);
    
    // 设定初始参数
    // NOTE: 若数据页头部存储参数类型改变，此处计算 recNumPerPage 时也需要相应改变
    recSize = inRecSize;
    recNumPerPage = PAGE_SIZE/recSize;
    while (12 + (recNumPerPage+7)/8*8 + recSize*recNumPerPage > PAGE_SIZE) recNumPerPage--;
    recNumTot = 0;
    pageNum = 1;
    usablePageHeader = -1;

    _writeFileHeaderPage(FileHeaderPageParameterType::ALL);
    bufPageManager->fileManager->closeFile(fileId);
    fileId = -1;
}

void RecordManager::deleteFile(const char *name) {
    remove(name);
}

int RecordManager::openFile(const char *name) {
    assert(fileId == -1);
    bufPageManager->fileManager->openFile(name, fileId);
    // 读入参数
    _readFileHeaderPage(FileHeaderPageParameterType::ALL);
    dataPageMapBitsNum = (recNumPerPage+7)/8*8;

    return 0;
}

int RecordManager::closeFile() {
    assert(fileId != -1);
    bufPageManager->fileManager->closeFile(fileId);
    fileId = -1;
    return 0;
}

void RecordManager::appendNewPage() {
    pageNum++;

    // 初始化 usableSlotNum
    _writeIntToPage(pageNum-1, USABLE_SLOT_NUM_OFFSET, recNumPerPage);

    // 更新空闲页链表
    int oldPageHeader = usablePageHeader;
    usablePageHeader = pageNum-1;
    _writeDataPagePointer(oldPageHeader, true, usablePageHeader);
    _writeDataPagePointer(usablePageHeader, false, oldPageHeader);
    _writeDataPagePointer(usablePageHeader, true, 0);
    _writeFileHeaderPage(FileHeaderPageParameterType::USABLE_PAGE_HEADER);
    
    // 初始化 bitmap
    char tmp = 0xff;
    for (int i = 0; i < dataPageMapBitsNum/8; ++i) {
        _writeDataToPage(pageNum-1, USABLE_BITMAP_OFFSET+i, &tmp, 1);
    }
}

void RecordManager::getUsableRID(RID &rid) {
    assert(fileId != -1);
    if (usablePageHeader == -1) {
        // 开辟一个新页
        appendNewPage();
    }
    rid.pageId = usablePageHeader;
    char tmp = 0;
    for (int i = 0; i < dataPageMapBitsNum/8; ++i) {
        _readDataFromPage(pageNum-1, USABLE_BITMAP_OFFSET+i, &tmp, 1);
        int bit = (unsigned)lowbit(tmp);
        if (bit != 0) {
            int index = powerToIndex[bit];
            assert(index != -1);
            rid.slotId = i*8 + index;
            break;
        }
    }
}

void RecordManager::takeUpRID(RID rid) {
    assert(fileId != -1);
    int pageId = rid.pageId, slotId = rid.slotId;

    // 更改占用位
    char tmp;
    _readDataFromPage(pageId, USABLE_BITMAP_OFFSET + slotId/8, &tmp, 1);
    int slotOffset = slotId - slotId/8*8;
    assert(tmp & (1<<slotOffset));
    tmp ^= (1<<slotOffset);
    _writeDataToPage(pageId, USABLE_BITMAP_OFFSET + slotId/8, &tmp, 1);

    // 更改 usableSlotNum
    int usableSlotNum;
    _readIntFromPage(pageId, USABLE_SLOT_NUM_OFFSET, usableSlotNum);
    usableSlotNum--;
    _writeIntToPage(pageId, USABLE_SLOT_NUM_OFFSET, usableSlotNum);

    // 更改链表
    if (usableSlotNum == 0) {
        int prevPage, nextPage;
        _readDataPagePointer(pageId, true, prevPage);
        _readDataPagePointer(pageId, false, nextPage);
        _writeDataPagePointer(prevPage, false, nextPage);
        _writeDataPagePointer(nextPage, true, prevPage);
    }
}

void RecordManager::insertRecord(char *recordData, RID &rid) {
    assert(fileId != -1);
    getUsableRID(rid);
    _writeDataToPage(rid.pageId, SLOT_OFFSET + rid.slotId*recSize, recordData, recSize);
    takeUpRID(rid);
}

void RecordManager::getRecord(char *recordData, RID rid) {
    assert(fileId != -1);
    _readDataFromPage(rid.pageId, SLOT_OFFSET + rid.slotId*recSize, recordData, recSize);
}

void RecordManager::_readFileHeaderPage(FileHeaderPageParameterType type) {
    assert(fileId != -1);
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_SIZE) {
        _readIntFromPage(FILE_HEADER_PAGE_ID, REC_SIZE_OFFSET, recSize);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_NUM_PER_PAGE) {
        _readIntFromPage(FILE_HEADER_PAGE_ID, REC_NUM_PER_PAGE_OFFSET, recNumPerPage);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_NUM_TOT) {
        _readIntFromPage(FILE_HEADER_PAGE_ID, REC_NUM_TOT_OFFSET, recNumTot);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::PAGE_NUM) {
        _readIntFromPage(FILE_HEADER_PAGE_ID, PAGE_NUM_OFFSET, pageNum);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::USABLE_PAGE_HEADER) {
        _readIntFromPage(FILE_HEADER_PAGE_ID, USABLE_PAGE_HEADER_OFFSET, usablePageHeader);
    }
}

void RecordManager::_writeFileHeaderPage(FileHeaderPageParameterType type) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, 0, index);
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_SIZE) {
        _writeIntToPage(FILE_HEADER_PAGE_ID, REC_SIZE_OFFSET, recSize);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_NUM_PER_PAGE) {
        _writeIntToPage(FILE_HEADER_PAGE_ID, REC_NUM_PER_PAGE_OFFSET, recNumPerPage);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_NUM_TOT) {
        _writeIntToPage(FILE_HEADER_PAGE_ID, REC_NUM_TOT_OFFSET, recNumTot);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::PAGE_NUM) {
        _writeIntToPage(FILE_HEADER_PAGE_ID, PAGE_NUM_OFFSET, pageNum);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::USABLE_PAGE_HEADER) {
        _writeIntToPage(FILE_HEADER_PAGE_ID, USABLE_PAGE_HEADER_OFFSET, usablePageHeader);
    }
    bufPageManager->markDirty(index);
}

void RecordManager::_readDataPagePointer(int pageId, bool isPrev, int &pointer) {
    assert(fileId != -1);
    if (pageId == -1) return;
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, 0, index);
    if (pageId == 0) {
        // 特判文件首页的情况
        assert(isPrev == false);
        pointer = usablePageHeader;
    } else {
        if (isPrev) {
            _readIntFromPage(pageId, PREV_USABLE_PAGE_OFFSET, pointer);
        } else {
            _readIntFromPage(pageId, NEXT_USABLE_PAGE_OFFSET, pointer);
        }
    }
}

void RecordManager::_writeDataPagePointer(int pageId, bool isPrev, int pointer) {
    assert(fileId != -1);
    if (pageId == -1) return;
    if (pageId == 0) {
        // 特判文件首页的情况
        assert(isPrev == false);
        usablePageHeader = pointer;
        _writeFileHeaderPage(FileHeaderPageParameterType::USABLE_PAGE_HEADER);
    } else {
        if (isPrev) {
            _writeIntToPage(FILE_HEADER_PAGE_ID, PREV_USABLE_PAGE_OFFSET, pointer);
        } else {
            _writeIntToPage(FILE_HEADER_PAGE_ID, NEXT_USABLE_PAGE_OFFSET, pointer);
        }
    }
}

void RecordManager::_readIntFromPage(int pageId, int offset, int &value) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, pageId, index);
    char *tmpBuf = (char*)buf;
    memcpy(&value, tmpBuf+offset, 4);
    value = ntohl(value);
}

void RecordManager::_writeIntToPage(int pageId, int offset, int value) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, pageId, index);
    value = htonl(value);
    char *tmpBuf = (char*)buf;
    memcpy(tmpBuf+offset, &value, 4);
    bufPageManager->markDirty(index);
}

void RecordManager::_readDataFromPage(int pageId, int offset, char *data, int len) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, pageId, index);
    char *tmpBuf = (char*)buf;
    memcpy(data, tmpBuf+offset, len);
}

void RecordManager::_writeDataToPage(int pageId, int offset, char *data, int len) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, pageId, index);
    char *tmpBuf = (char*)buf;
    memcpy(tmpBuf+offset, data, len);
    bufPageManager->markDirty(index);
}