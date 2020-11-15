#ifndef _INDEX_MANAGER_H_
#define _INDEX_MANAGER_H_

#include "cstring"
#include "BufPageManager.h"
#include "AttrType.h"
#include "IndexHandler.h"

class IndexManager {
  public:
    const char* INDEX_DIR = "./";
    BufPageManager *commonBufPageManager;

    IndexManager();

    /*
     * 创建索引文件，对应文件为 <filename>_<indexNo>.ind
     * @dataName: 对应的数据文件名，不带后缀
     * @indexNo: 该数据文件名的索引编号
     * @attrTypeList: 注意其中的 void *data 均为 nullptr，主要用于给定属性类型和长度
     */
    void createIndex(const char *dataName, int indexNo, AttrList attrTypeList);

    /*
     * 删除索引文件，对应文件为 <filename>_<indexNo>.ind
     * @dataName: 对应的数据文件名，不带后缀
     * @indexNo: 该数据文件名的索引编号
     */
    void destroyIndex(const char *dataName, int indexNo);

    /*
     * 打开索引文件，对应文件为 <filename>_<indexNo>.ind
     * @dataName: 对应的数据文件名，不带后缀
     * @indexNo: 该数据文件名的索引编号
     * @handler: 返回的 IndexHandler，用于处理该索引文件
     */
    void openIndex(const char *dataName, int indexNo, IndexHandler &handler); 

    /*
     * 返回对应索引文件的路径
     * @dataName: 对应的数据文件名，不带后缀
     * @indexNo: 该数据文件名的索引编号
     */
    string getIndexPath(const char *dataName, int indexNo);
};

#endif