#ifndef _RECORDMANAGER_H_
#define _RECORDMANAGER_H_

class BufPageManager;

typedef int PageNum;
typedef int SlotNum;

struct RID {
    // pageNum 为记录所在页数， slotNum 为记录所在槽数（从 0 开始计数）
    PageNum pageNum = 0;
    SlotNum slotNum = 0;

    bool operator==(const RID &b) {
        return pageNum == b.pageNum && slotNum == b.slotNum;
    }

    RID(PageNum inPageNum, SlotNum inSlotNum) {
        pageNum = inPageNum;
        slotNum = inSlotNum;
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

class RecordManager {
  public:
    BufPageManager *bufPageManager;
    int recSize, recNumPerPage, recNumTot;

    RecordManager();

    ~RecordManager();

    void createFile(const char *name);

    void deleteFile(const char *name);

    /*
     * @参数 name：要打开的文件名
     * @参数 recSize：每个记录占用大小(字节)
     * 成功打开文件则返回 0
     * TODO: 错误处理
     */
    int openFile(const char *name, int inRecSize);

    // 成功关闭文件则返回 0
    int closeFile();

  private:
    int _fileId = -1;  // 保证只打开一个文件
};

#endif