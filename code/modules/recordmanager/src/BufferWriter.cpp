#include "BufferWriter.h"
#include "cstring"
#include "netinet/in.h"

void BufferWriter::write(void *buf, int offset, void *data, int len) {
    memcpy((char*)buf + offset, data, len);
}

void BufferWriter::writeInt(void *buf, int offset, int value) {
    value = htonl(value);
    write(buf, offset, &value, 4);
}

void BufferWriter::writeFloat(void *buf, int offset, float value) {
    write(buf, offset, &value, 4);
}