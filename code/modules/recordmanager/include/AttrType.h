#ifndef _ATTR_TYPE_H_
#define _ATTR_TYPE_H_

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

#endif