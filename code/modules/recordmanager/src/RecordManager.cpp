#include "RecordManager.h"
#include "netinet/in.h"
#include "cstring"
#include "cassert"
#include "Logger.h"

RecordManager::RecordManager() {
    bufPageManager = new BufPageManager(new FileManager);
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
    while (8 + (recNumPerPage+7)/8*8 + recSize*recNumPerPage > PAGE_SIZE) recNumPerPage--;
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

    return 0;
}

int RecordManager::closeFile() {
    assert(fileId != -1);
    bufPageManager->fileManager->closeFile(fileId);
    fileId = -1;
    return 0;
}

void RecordManager::_readFileHeaderPage(FileHeaderPageParameterType type) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, 0, index);
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_SIZE) {
        _readIntFromBuffer(buf, REC_SIZE_OFFSET, recSize);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_NUM_PER_PAGE) {
        _readIntFromBuffer(buf, REC_NUM_PER_PAGE_OFFSET, recNumPerPage);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_NUM_TOT) {
        _readIntFromBuffer(buf, REC_NUM_TOT_OFFSET, recNumTot);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::PAGE_NUM) {
        _readIntFromBuffer(buf, PAGE_NUM_OFFSET, pageNum);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::USABLE_PAGE_HEADER) {
        _readIntFromBuffer(buf, USABLE_PAGE_HEADER_OFFSET, usablePageHeader);
    }
}

void RecordManager::_writeFileHeaderPage(FileHeaderPageParameterType type) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, 0, index);
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_SIZE) {
        _writeIntToBuffer(buf, REC_SIZE_OFFSET, recSize);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_NUM_PER_PAGE) {
        _writeIntToBuffer(buf, REC_NUM_PER_PAGE_OFFSET, recNumPerPage);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::REC_NUM_TOT) {
        _writeIntToBuffer(buf, REC_NUM_TOT_OFFSET, recNumTot);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::PAGE_NUM) {
        _writeIntToBuffer(buf, PAGE_NUM_OFFSET, pageNum);
    }
    if (type == FileHeaderPageParameterType::ALL || type == FileHeaderPageParameterType::USABLE_PAGE_HEADER) {
        _writeIntToBuffer(buf, USABLE_PAGE_HEADER_OFFSET, usablePageHeader);
    }
    bufPageManager->markDirty(index);
}

void RecordManager::_readDataPagePointer(int pageId, bool isPrev, int &pointer) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, 0, index);
    if (isPrev) {
        _readIntFromBuffer(buf, PREV_USABLE_PAGE_OFFSET, pointer);
    } else {
        _readIntFromBuffer(buf, NEXT_USABLE_PAGE_OFFSET, pointer);
    }
}

void RecordManager::_writeDataPagePointer(int pageId, bool isPrev, int pointer) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, 0, index);
    if (isPrev) {
        _writeIntToBuffer(buf, PREV_USABLE_PAGE_OFFSET, pointer);
    } else {
        _writeIntToBuffer(buf, NEXT_USABLE_PAGE_OFFSET, pointer);
    }
    bufPageManager->markDirty(index);
}

void RecordManager::_readIntFromBuffer(BufType buf, int offset, int &value) {
    char *tmpBuf = (char*)buf;
    memcpy(&value, tmpBuf+offset, 4);
    value = ntohl(value);
}

void RecordManager::_writeIntToBuffer(BufType buf, int offset, int value) {
    value = htonl(value);
    char *tmpBuf = (char*)buf;
    memcpy(tmpBuf+offset, &value, 4);
}