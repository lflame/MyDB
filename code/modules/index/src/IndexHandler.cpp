#include "IndexHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

IndexHandler::IndexHandler(const char *dataName, int indexNo, string indexPath) {
    this->dataName = dataName;
    this->indexNo = indexNo;
    this->indexPath = indexPath;
    this->bufPageManager = new BufPageManager(new FileManager);
    this->bufPageManager->fileManager->openFile(indexPath.c_str(), this->fileId);
}

IndexHandler::~IndexHandler() {
    delete bufPageManager;
}

void IndexHandler::_readIntFromPage(int pageId, int offset, int &value) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, pageId, index);
    BufferReader::readInt((char*)buf, offset, value);
}

void IndexHandler::_writeIntToPage(int pageId, int offset, int value) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, pageId, index);
    BufferWriter::writeInt((char*)buf, offset, value);
    bufPageManager->markDirty(index);
}

void IndexHandler::_readDataFromPage(int pageId, int offset, char *data, int len) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, pageId, index);
    BufferReader::read((char*)buf, offset, data, len);
}

void IndexHandler::_writeDataToPage(int pageId, int offset, char *data, int len) {
    assert(fileId != -1);
    int index = -1;
    BufType buf = bufPageManager->getPage(fileId, pageId, index);
    BufferWriter::write((char*)buf, offset, data, len);
    bufPageManager->markDirty(index);
}