#ifndef _RECORDMANAGER_H_
#define _RECORDMANAGER_H_

#define REC_SIZE_OFFSET 0
#define REC_NUM_PER_PAGE_OFFSET 4
#define REC_NUM_TOT_OFFSET 8
#define PAGE_NUM_OFFSET 12
#define USABLE_PAGE_HEADER_OFFSET 16

class BufPageManager;

struct RID {
    // pageId 为记录所在页数， slotId 为记录所在槽数（从 0 开始计数）
    int pageId = -1, slotId = -1;

    bool operator==(const RID &b) {
        return pageId == b.pageId && slotId == b.slotId;
    }

    RID(int inPageId, int inSlotId) {
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

    /*
     * 插入一个记录
     */ 
    void insertRecord();

  private:
    int fileId = -1;  // 保证只打开一个文件

    // 利用相应成员变量读取/写入文件头页的参数
    void _readFileHeaderPage(FileHeaderPageParameterType type);
    void _writeFileHeaderPage(FileHeaderPageParameterType type);

    /*
     * 从 Buf 的 offset （字节）地址开始，写入/读取一个 int, 统一使用网络字节序
     * @参数 buf：缓存数组指针
     * @参数 offset：偏移地址(字节)
     * @参数 value：要读取/写入的值
     */ 
    void _readIntFromBuffer(BufType buf, int offset, int &value);
    void _writeIntToBuffer(BufType buf, int offset, int value);
};

#endif