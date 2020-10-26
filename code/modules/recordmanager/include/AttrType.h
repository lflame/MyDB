#ifndef _ATTRTYPE_H_
#define _ATTRTYPE_H_

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

#endif