#ifndef _INDEX_HANDLER_H_
#define _INDEX_HANDLER_H_

#include "cstring"
#include "BufPageManager.h"

using std::string;


class IndexHandler {
  public:
    string indexPath;
    const char *dataName;
    int indexNo;
    BufPageManager *bufPageManager;

    // 用于存储 FileManager 使用的文件 id

    int fileId;

    IndexHandler(const char *dataName, int indexNo, string indexPath);

    ~IndexHandler();
    
  private:
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