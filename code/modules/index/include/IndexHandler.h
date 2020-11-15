#ifndef _INDEX_HANDLER_H_
#define _INDEX_HANDLER_H_

#include "cstring"
#include "BufPageManager.h"
#include "BPlusTree.h"

// 首页参数
#define NDNUM_OFFSET 0
#define ATTR_NUM_OFFSET 4
#define ATTR_LIST_OFFSET 8

// 数据页参数
#define FA_PAGE_OFFSET 0
#define CHNUM_OFFSET 4
#define CHILDREN_INFO_OFFSET 8

using std::string;


class IndexHandler {
  public:
    string indexPath;
    const char *dataName;
    int indexNo;
    BufPageManager *bufPageManager;
    BPlusTree *tree;

    // 用于存储 FileManager 使用的文件 id

    int fileId;

    IndexHandler(const char *dataName, int indexNo, string indexPath);

    ~IndexHandler();
    
  private:

    /*
     * 读取/写入文件头信息，注意由于为 handler，读取头信息一定仅进行一次
     */
    void _readFileHeaderPage();
    void _writeFileHeaderPage();


    /*
     * 从 pageId 这一页读取/写入一个节点信息
     * @参数 pageId: 页数
     * @参数 p: 要读取/写入的节点信息
     */
    void _readNodeFromPage(int pageId, BNode &p);
    void _writeNodeToPage(int pageId, const BNode &p);

    // 以下是比较底层的读写

    /*
     * 从 pageId 这一页的 offset （字节）地址开始，读取/写入一个 int, 统一使用网络字节序
     * 带 markDirty
     * @参数 pageId：页数
     * @参数 offset：偏移地址(字节)
     * @参数 value：要读取/写入的值
     */ 
    void _readIntFromPage(int pageId, int offset, int &value);
    void _writeIntToPage(int pageId, int offset, int value);

    /*
     * 从 pageId 这一页的 offset （字节）地址开始，依次读取/写入 len 个 char
     * @参数 pageId：页数
     * @参数 offset：偏移地址(字节)
     * @参数 data：要读取/写入的序列首地址
     * @参数 len：要读取/写入的 char 数
     */
    void _readDataFromPage(int pageId, int offset, char *data, int len);
    void _writeDataToPage(int pageId, int offset, char *data, int len);
};

#endif