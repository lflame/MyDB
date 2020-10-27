#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_

#include "BufPageManager.h"
#include "FileManager.h"
#include "AttrType.h"

#define FILE_HEADER_PAGE_ID 0

// 文件首页参数
#define REC_SIZE_OFFSET 0
#define REC_NUM_PER_PAGE_OFFSET 4
#define REC_NUM_TOT_OFFSET 8
#define PAGE_NUM_OFFSET 12
#define USABLE_PAGE_HEADER_OFFSET 16

// 数据页参数
#define PREV_USABLE_PAGE_OFFSET 0
#define NEXT_USABLE_PAGE_OFFSET 4
#define USABLE_SLOT_NUM_OFFSET 8
#define USABLE_BITMAP_OFFSET 12
#define SLOT_OFFSET (USABLE_BITMAP_OFFSET+dataPageMapBitsNum/8)

class BufPageManager;

struct RID {
    // pageId 为记录所在页数， slotId 为记录所在槽数（从 0 开始计数）
    int pageId, slotId;

    bool operator==(const RID &b) {
        return pageId == b.pageId && slotId == b.slotId;
    }

    RID(int inPageId = -1, int inSlotId = -1) {
        pageId = inPageId;
        slotId = inSlotId;
    }
};

struct Record {
    // data 为数据指针
    char *data = nullptr;

    Record(int recSize) {
        data = new char[recSize];
    }

    ~Record() {
        if (data != nullptr)
            delete[] data;
    }
};

enum class FileHeaderPageParameterType {
    ALL,
    REC_SIZE,
    REC_NUM_PER_PAGE,
    REC_NUM_TOT,
    PAGE_NUM,
    USABLE_PAGE_HEADER
};

class RecordManager {
  public:
    BufPageManager *bufPageManager;
    int recSize, recNumPerPage, recNumTot, pageNum, usablePageHeader;

    // 数据页首空闲映射位数，不存储在文件中
    int dataPageMapBitsNum;

    RecordManager();

    ~RecordManager();

    /*
     * 创建数据库文件，并且设定头部初始值
     * **需要保证调用时还未打开文件**
     */
    void createFile(const char *name, int inRecSize);

    void deleteFile(const char *name);

    /*
     * @参数 name：要打开的文件名
     * @参数 recSize：每个记录占用大小(字节)
     * 成功打开文件则返回 0
     * TODO: 错误处理
     */
    int openFile(const char *name);

    // 成功关闭文件则返回 0
    int closeFile();

    // 在结尾加入一个新页
    void appendNewPage();

    /*
     * 获取一个空闲记录槽
     * @参数 rid：返回空闲的记录位置
     */
    void getUsableRID(RID &rid);

    /*
     * 标记占用一个记录槽
     */
    void takeUpRID(RID rid);

    /*
     * 标记释放一个记录槽
     */
    void freeRID(RID rid);

    /*
     * 返回某个记录槽的 bitmap 对应位
     */
    bool isUsedRID(RID rid);


    /*
     * 插入一个记录
     * @参数 recordData：要插入的记录数据
     * @参数 rid：返回存放记录的位置
     */ 
    void insertRecord(char *recordData, RID &rid);

    /*
     * 获取一个记录
     * @参数 recordData：将返回的记录存储到此地址
     * @参数 rid：存放记录的位置
     */
    void getRecord(char *recordData, RID rid);

    /*
     * 获取下一个满足某条件的记录
     * @参数 recordData：将返回的记录存储到此地址
     * @参数 suc：成功时返回 ture
     * @参数 rid：从该 rid 开始寻找
     * @参数 attrType：属性类型
     * @参数 attrOffset：属性起始地址相对于记录地址的偏移量（字节）
     * @参数 attrLen：属性的长度，仅在 attrType 为 STRING 时有效
     * @参数 compOp：同 value 比较的方式
     * @参数 value：在 compOp 不为 NO_OP 时有效
     */
    void getNextRecord(char *recordData,
                       bool &suc,
                       RID rid,
                       AttrType attrType,
                       int attrOffset,
                       int attrLen,
                       CompOp compOp,
                       void *value);

    /*
     * 更新一个记录
     * @参数 recordData：要更新的记录数据
     * @参数 rid：存放记录的位置
     */
    void updateRecord(char *recordData, RID rid);

    /*
     * 删除一个记录
     * @参数 rid：存放记录的位置
     */
    void deleteRecord(RID rid);

  private:
    int fileId = -1;  // 保证只打开一个文件

    int powerToIndex[305];  // 从2^k到k的映射，其中 k 在 [0, 7] 范围内

    // 利用相应成员变量读取/写入文件头页的参数，不带 markDirty
    void _readFileHeaderPage(FileHeaderPageParameterType type);
    void _writeFileHeaderPage(FileHeaderPageParameterType type);

    /*
     * 读取/写入数据页的两个指针，不带 markDirty
     * @参数 pageId：要操作的当前数据页
     * @参数 isPrev：为 True 时操作 prevUsablePage，False 时操作 nextUsablePage
     * @参数 pointer：读取/写入的参数值
     */
    void _readDataPagePointer(int pageId, bool isPrev, int &pointer);
    void _writeDataPagePointer(int pageId, bool isPrev, int pointer);

    // 以下为较底层的读写页的函数

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