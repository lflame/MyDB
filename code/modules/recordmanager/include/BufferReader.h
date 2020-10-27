#ifndef _BUFFER_READER_H_
#define _BUFFER_READER_H_

// 用于对 buffer 进行二进制读取
class BufferReader {
  public:
    // 从 buf+offset 读取长为 len 的数据到 data
    static void read(void *buf, int offset, void *data, int len);

    static void readInt(void *buf, int offset, int &value);

    static void readFloat(void *buf, int offset, float &value);
};

#endif