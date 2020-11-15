#ifndef _ATTR_TYPE_H_
#define _ATTR_TYPE_H_

#include "cassert"
#include "RID.h"

#define MAX_STRING_LEN 255

enum class AttrType {
    INT,
    FLOAT,
    STRING
};

enum class CompOp {
    EQ_OP,
    LT_OP,
    GT_OP,
    LE_OP,
    GE_OP,
    NE_OP,
    NO_OP
};

class Comp {
  public:
    // **注意 attr 为 int 时需要为网络序**，满足 attr1 OP attr2 时返回 true
    static bool comp(void *attr1, void *attr2, AttrType attrType, int attrLen, CompOp compOp);

    static int sgn(const float &value);
};

// 目前该类在 RM 模块中未使用，而仅在 IX 模块中使用
// **注意类型为 int 时需要为网络序**
class Attr {
  public:
    void *data;
    AttrType attrType;
    int attrLen;

    Attr() { data = nullptr; }
    Attr(void *data, AttrType type, int len) {
        this->data = data;
        this->attrType = type;
        this->attrLen = len;
    }
    // **注意需要为网络序**
    Attr(int *value) {
        data = value;
        attrType = AttrType::INT;
        attrLen = 4;
    }
    Attr(float *value) {
        data = value;
        attrType = AttrType::FLOAT;
        attrLen = 4;
    }

    Attr& operator=(const Attr &other) {
        data = other.data;
        attrType = other.attrType;
        attrLen = other.attrLen;
    }

    bool operator==(const Attr &other) const {
        assert(other.attrLen == attrLen);
        return Comp::comp(data, other.data, attrType, attrLen, CompOp::EQ_OP);
    }
    bool operator<(const Attr &other) const {
        assert(other.attrLen == attrLen);
        return Comp::comp(data, other.data, attrType, attrLen, CompOp::LT_OP);
    }
    bool operator>(const Attr &other) const {
        assert(other.attrLen == attrLen);
        return Comp::comp(data, other.data, attrType, attrLen, CompOp::GT_OP);
    }
    bool operator<=(const Attr &other) const {
        assert(other.attrLen == attrLen);
        return Comp::comp(data, other.data, attrType, attrLen, CompOp::LE_OP);
    }
    bool operator>=(const Attr &other) const {
        assert(other.attrLen == attrLen);
        return Comp::comp(data, other.data, attrType, attrLen, CompOp::GE_OP);
    }
    bool operator!=(const Attr &other) const {
        assert(other.attrLen == attrLen);
        return Comp::comp(data, other.data, attrType, attrLen, CompOp::NE_OP);
    }
};

// 目前该类在 RM 模块中未使用，而仅在 IX 模块中使用
// 主要用于建立联合索引
class AttrList {
  public:
    int attrNum;
    Attr *attrs;

    AttrList() {
        this->attrNum = 0;
        attrs = nullptr;
    }

    void init(int attrNum) {
        if (attrs != nullptr) delete[] attrs;
        this->attrNum = attrNum;
        if (attrNum != 0) attrs = new Attr[attrNum];
    }

    AttrList(int attrNum) {
        init(attrNum);
    }

    ~AttrList() {
        delete[] attrs;
    }

    Attr& operator[](int ind) {
        return attrs[ind];
    }

    AttrList& operator=(const AttrList &other) {
        init(other.attrNum);
        for (int i = 0; i < attrNum; ++i)
            attrs[i] = other.attrs[i];
    }

    bool operator==(const AttrList &other) const {
        assert(other.attrNum == attrNum);
        for (int i = 0; i < attrNum; ++i) if (attrs[i] != other.attrs[i]) {
            return false;
        }
        return true;
    }
    bool operator<(const AttrList &other) const {
        assert(other.attrNum == attrNum);
        for (int i = 0; i < attrNum; ++i) {
            if (attrs[i] > other.attrs[i]) return false;
            else if (attrs[i] < other.attrs[i]) return true;
        }
        return false;
    }
    bool operator>(const AttrList &other) const {
        assert(other.attrNum == attrNum);
        for (int i = 0; i < attrNum; ++i) {
            if (attrs[i] > other.attrs[i]) return true;
            else if (attrs[i] < other.attrs[i]) return false;
        }
        return false;
    }
    bool operator<=(const AttrList &other) const {
        assert(other.attrNum == attrNum);
        for (int i = 0; i < attrNum; ++i) {
            if (attrs[i] > other.attrs[i]) return false;
            else if (attrs[i] < other.attrs[i]) return true;
        }
        return true;
    }
    bool operator>=(const AttrList &other) const {
        assert(other.attrNum == attrNum);
        for (int i = 0; i < attrNum; ++i) {
            if (attrs[i] > other.attrs[i]) return true;
            else if (attrs[i] < other.attrs[i]) return false;
        }
        return true;
    }
    bool operator!=(const AttrList &other) const {
        assert(other.attrNum == attrNum);
        for (int i = 0; i < attrNum; ++i) if (attrs[i] != other.attrs[i]) {
            return true;
        }
        return false;
    }
};

// 目前该类在 RM 模块中未使用，而仅在 IX 模块中使用
// 主要用于建立联合索引
class AttrListWithRID {
  public:
    AttrList attrList;
    RID rid;

    AttrListWithRID() {}

    AttrListWithRID(AttrList attrList, RID rid) {
        this->attrList = attrList;
        this->rid = rid;
    }

    bool operator==(const AttrListWithRID &other) const {
        return attrList == other.attrList && rid == other.rid;
    }
    bool operator<(const AttrListWithRID &other) const {
        return attrList < other.attrList || (attrList == other.attrList && rid < other.rid);
    }
    bool operator>(const AttrListWithRID &other) const {
        return attrList > other.attrList || (attrList == other.attrList && rid > other.rid);
    }
    bool operator<=(const AttrListWithRID &other) const {
        return attrList < other.attrList || (attrList == other.attrList && rid <= other.rid);
    }
    bool operator>=(const AttrListWithRID &other) const {
        return attrList > other.attrList || (attrList == other.attrList && rid >= other.rid);
    }
    bool operator!=(const AttrListWithRID &other) const {
        return !(*this==other);
    }
};

#endif