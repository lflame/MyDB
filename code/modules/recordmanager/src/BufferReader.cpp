#include "BufferReader.h"
#include "cstring"
#include "netinet/in.h"

void BufferReader::read(void *buf, int offset, void *data, int len) {
    memcpy(data, (char*)buf + offset, len);
}

void BufferReader::readInt(void *buf, int offset, int &value) {
    read(buf, offset, &value, 4);
    value = ntohl(value);
}

void BufferReader::readFloat(void *buf, int offset, float &value) {
    read(buf, offset, &value, 4);
}