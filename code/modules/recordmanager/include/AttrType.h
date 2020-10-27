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
    // 用于比较记录和 value，成功时返回 true
    static bool comp(char *recordData, AttrType attrType, int attrOffset, int attrLen);
};

#endif