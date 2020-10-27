#include "AttrType.h"
#include "cstring"
#include "BufferReader.h"

bool Comp::comp(void *attr1, void *attr2, AttrType attrType, int attrLen, CompOp compOp) {
    // 负值、零、正值分别表示小于、等于、大于
    if (compOp == CompOp::NO_OP) return true;
    int res;
    switch (attrType)
    {
        case AttrType::INT: {
            int value1, value2;
            BufferReader::readInt(attr1, 0, value1);
            BufferReader::readInt(attr2, 0, value2);
            res = value1 - value2;
            break;
        }
        case AttrType::FLOAT: {
            float value1, value2;
            BufferReader::readFloat(attr1, 0, value1);
            BufferReader::readFloat(attr2, 0, value2);
            res = value1 - value2;
            break;
        }
        case AttrType::STRING: {
            res = 0;
            for (int i = 0; i < attrLen; ++i) {
                if (((char*)attr1)[i] < ((char*)attr2)[i]) {
                    res = -1;
                    break;
                } else if (((char*)attr1)[i] > ((char*)attr2)[i]) {
                    res = 1;
                    break;
                }
            }
            break;
        }
    }

    switch (compOp)
    {
    case CompOp::EQ_OP:
        return res == 0;
    case CompOp::LT_OP:
        return res < 0;
    case CompOp::GT_OP:
        return res > 0;
    case CompOp::LE_OP:
        return res <= 0;
    case CompOp::GE_OP:
        return res >= 0;
    case CompOp::NE_OP:
        return res != 0;  
    }
}