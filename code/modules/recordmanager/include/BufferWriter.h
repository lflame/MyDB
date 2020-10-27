#ifndef _BUFFER_WRITER_H_
#define _BUFFER_WRITER_H_

// 用于对 buffer 进行二进制写入
class BufferWriter {
  public:
    // 从 data 向 buf+offset 写入长为 len 的数据
    static void write(void *buf, int offset, void *data, int len);

    static void writeInt(void *buf, int offset, int value);

    static void writeFloat(void *buf, int offset, float value);
};

#endif