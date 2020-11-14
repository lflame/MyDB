#ifndef _INDEX_MANAGER_H_
#define _INDEX_MANAGER_H_

#include "BufPageManager.h"
#include "AttrType.h"

class IndexManager {
  public:
    BufPageManager *bufPageManager;

    /*
     * 创建索引，对应文件为 <filename>_<indexNo>.ind
     * @filename: 对应的数据文件名，不带后缀
     * @indexNo: 该数据文件名的索引编号
     * @attrTypeList: 注意其中的 void *data 均为 nullptr，主要用于给定属性类型和长度
     */
    void createIndex(const char *filename, int indexNo, AttrList attrTypeList);

    void destroyIndex(const char *filename, int indexNo);

    void openIndex(const char *filename, int indexNo);

    IndexManager();
};

#endif