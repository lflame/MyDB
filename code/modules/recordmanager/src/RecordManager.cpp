#include "netinet/in.h"
#include "cstring"
#include "cassert"
#include "RecordManager.h"
#include "BufPageManager.h"
#include "FileManager.h"

RecordManager::RecordManager() {
    bufPageManager = new BufPageManager(new FileManager);
}

RecordManager::~RecordManager() {
    delete bufPageManager;
}

void RecordManager::createFile(const char *name, int inRecSize) {
    bufPageManager->fileManager->createFile(name);
    // TODO
}

void RecordManager::deleteFile(const char *name) {
    remove(name);
}

int RecordManager::openFile(const char *name) {
    assert(fileId == -1);
    return bufPageManager->fileManager->openFile(name, fileId);
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
        
    }

}

void RecordManager::_writeFileHeaderPage(FileHeaderPageParameterType type) {
    assert(fileId != -1);
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