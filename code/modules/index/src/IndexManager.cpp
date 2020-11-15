#include "unistd.h"
#include "cassert"
#include "IndexManager.h"
#include "BufferReader.h"
#include "BufferWriter.h"

using std::string;
using std::to_string;

IndexManager::IndexManager() {
    commonBufPageManager = new BufPageManager(new FileManager);
}

void IndexManager::createIndex(const char *dataName, int indexNo, AttrList attrTypeList) {
    string indexPath = getIndexPath(dataName, indexNo);
    commonBufPageManager->fileManager->createFile(indexPath.c_str());
    // TODO: 初始化文件内容
    int fileId;
    commonBufPageManager->fileManager->openFile(indexPath.c_str(), fileId);
    
    commonBufPageManager->fileManager->closeFile(fileId);
}

void IndexManager::destroyIndex(const char *dataName, int indexNo) {
    string indexPath = getIndexPath(dataName, indexNo);
    assert(access(indexPath.c_str(), 0));
    remove(indexPath.c_str());
}

void IndexManager::openIndex(const char *dataName, int indexNo, IndexHandler &handler) {

}

string IndexManager::getIndexPath(const char *dataName, int indexNo) {
    return string(INDEX_DIR) + string(dataName) + "_" + to_string(indexNo) + ".ind";
}