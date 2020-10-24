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

void RecordManager::createFile(const char *name) {
    bufPageManager->fileManager->createFile(name);
}

void RecordManager::deleteFile(const char *name) {
    remove(name);
}

int RecordManager::openFile(const char *name, int inRecSize) {
    assert(_fileId == -1);
    return bufPageManager->fileManager->openFile(name, _fileId);
}

int RecordManager::closeFile() {
    assert(_fileId != -1);
    bufPageManager->fileManager->closeFile(_fileId);
    _fileId = -1;
    return 0;
}
