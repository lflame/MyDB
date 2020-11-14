#include "Tests.h"
#include "cstdio"
#include "cstring"
#include "cassert"
#include "fcntl.h"
#include "netinet/in.h"
#include "Logger.h"
#include "RecordManager.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "BPlusTree.h"

void Tests::testAll() {
    // testLogger();
    // testRM1();
    // testRM2();
    // testBPlusTree1();
    testBPlusTree2();
    Logger::logger.debug("测试通过");
}

void Tests::testLogger() {
    Logger::logger.info("Start testLogger.");

    Logger::logger.debug("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.info("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.warning("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.error("%s %d %.2f", "test", 3, 3.142);
    Logger::logger.fatal("%s %d %.2f", "test", 3, 3.142);

    Logger::logger.info("End testLogger.");
}

void Tests::testRM1() {
    Logger::logger.info("Start testRM1.");
    const char* filename = "test.db";

    // 测试创建文件后初始文件头
    RecordManager rm;
    if (access(filename, F_OK) != -1) {
        // 测试文件已存在，需要先删除
        rm.deleteFile(filename);
    }
    const int testRecSize = 10;
    rm.createFile(filename, testRecSize);
    rm.recSize=rm.recNumPerPage=rm.recNumTot=rm.pageNum=rm.usablePageHeader=0;
    rm.openFile(filename);
    Logger::logger.debug("recSize: %d, recNumPerPage: %d, recNumTot: %d, pageNum: %d, usablePageHeader: %d",
        rm.recSize, rm.recNumPerPage, rm.recNumTot, rm.pageNum, rm.usablePageHeader);
    Logger::logger.debug("dataPageMapBitsNum: %d", rm.dataPageMapBitsNum);
    assert(rm.recSize == testRecSize);
    assert(rm.recNumTot == 0);
    assert(rm.pageNum == 1);
    assert(rm.usablePageHeader == -1);
    assert(12 + (rm.recNumPerPage+7)/8*8 + rm.recSize*rm.recNumPerPage <= PAGE_SIZE);

    // 测试插入超过一页
    char record[testRecSize] = "123456789";
    RID rid;
    for (int i = 1; i <= rm.recNumPerPage + 2; ++i) {
        rm.insertRecord(record, rid);
        if (i >= rm.recNumPerPage - 2)
            Logger::logger.debug("rid.pageId: %d, rid.slotId: %d", rid.pageId, rid.slotId);
    }
    assert(rm.pageNum == 3);
    assert(rid == RID(2, 1));

    // 测试读取
    char tmpRecord[testRecSize];
    rm.getRecord(tmpRecord, RID(1, 1));
    Logger::logger.debug("record: %s", tmpRecord);
    assert(strcmp(record, tmpRecord)==0);

    // 测试删除
    char record2[testRecSize] = "987654321";
    rid = RID(1, 3);
    rm.deleteRecord(rid);
    rm.insertRecord(record2, rid);
    Logger::logger.debug("rid.pageId: %d, rid.slotId: %d", rid.pageId, rid.slotId);
    assert(rid == RID(1, 3));
    rm.getRecord(tmpRecord, rid);
    Logger::logger.debug("record: %s", tmpRecord);
    assert(strcmp(record2, tmpRecord)==0);
    rm.insertRecord(record2, rid);
    Logger::logger.debug("rid.pageId: %d, rid.slotId: %d", rid.pageId, rid.slotId);
    assert(rid == RID(2, 2));

    rm.closeFile();
    Logger::logger.info("End testRM1.");
}

void Tests::testRM2() {
    Logger::logger.info("Start testRM2.");
    const char* filename = "test.db";

    // 测试创建文件后初始文件头
    RecordManager rm;
    if (access(filename, F_OK) != -1) {
        // 测试文件已存在，需要先删除
        rm.deleteFile(filename);
    }
    // int + float + 5 bytes string
    const int testRecSize = 13;
    rm.createFile(filename, testRecSize);
    rm.openFile(filename);

    // record1: 5 1.32 ab
    // record2: -3 2.15 abc
    // record3: 5 2.15 ab
    // record4: -9 2.0 aba
    char record1[testRecSize], record2[testRecSize], record3[testRecSize], record4[testRecSize];
    BufferWriter::writeInt(record1, 0, 5);
    BufferWriter::writeFloat(record1, 4, 1.32);
    BufferWriter::write(record1, 8, (void*)"ab", 3);

    BufferWriter::writeInt(record2, 0, -3);
    BufferWriter::writeFloat(record2, 4, 2.15);
    BufferWriter::write(record2, 8, (void*)"abc", 4);

    BufferWriter::writeInt(record3, 0, 5);
    BufferWriter::writeFloat(record3, 4, 2.15);
    BufferWriter::write(record3, 8, (void*)"ab", 3);

    BufferWriter::writeInt(record4, 0, -9);
    BufferWriter::writeFloat(record4, 4, 2.0);
    BufferWriter::write(record4, 8, (void*)"aba", 4);

    RID rid;
    rm.insertRecord(record1, rid);
    rm.insertRecord(record2, rid);
    rm.insertRecord(record3, rid);
    rm.insertRecord(record4, rid);

    // 查找 int 值小于 0 的第一个 record
    char tmpRecord[testRecSize];
    bool suc;
    int tmpInt = htonl(0);
    float tmpFloat;
    char tmpString[5];
    rm.getNextRecord(tmpRecord, suc, RID(1, 0), AttrType::INT, 0, 4, CompOp::LT_OP, &tmpInt);
    assert(suc);
    BufferReader::readInt(tmpRecord, 0, tmpInt);
    BufferReader::readFloat(tmpRecord, 4, tmpFloat);
    BufferReader::read(tmpRecord, 8, tmpString, 5);
    Logger::logger.debug("record: %d %.3f %s", tmpInt, tmpFloat, tmpString);
    assert(tmpInt==-3 && Comp::sgn(tmpFloat-2.15)==0 && strcmp(tmpString, "abc")==0);

    // 删除 record2
    rm.deleteRecord(RID(1, 1));

    // 查找 float 值等于 2.15 的第一个 record
    tmpFloat = 2.15;
    rm.getNextRecord(tmpRecord, suc, RID(1, 0), AttrType::FLOAT, 4, 4, CompOp::EQ_OP, &tmpFloat);
    assert(suc);
    BufferReader::readInt(tmpRecord, 0, tmpInt);
    BufferReader::readFloat(tmpRecord, 4, tmpFloat);
    BufferReader::read(tmpRecord, 8, tmpString, 5);
    Logger::logger.debug("record: %d %.3f %s", tmpInt, tmpFloat, tmpString);
    assert(tmpInt==5 && Comp::sgn(tmpFloat-2.15)==0 && strcmp(tmpString, "ab")==0);

    // 查找 record2 之后 String 串大于 ab 的第一个 record
    strcpy(tmpString, "ab");
    rm.getNextRecord(tmpRecord, suc, RID(1, 1), AttrType::STRING, 8, 5, CompOp::GT_OP, tmpString);
    assert(suc);
    BufferReader::readInt(tmpRecord, 0, tmpInt);
    BufferReader::readFloat(tmpRecord, 4, tmpFloat);
    BufferReader::read(tmpRecord, 8, tmpString, 5);
    Logger::logger.debug("record: %d %.3f %s", tmpInt, tmpFloat, tmpString);
    assert(tmpInt==-9 && Comp::sgn(tmpFloat-2)==0 && strcmp(tmpString, "aba")==0);


    rm.closeFile();
    Logger::logger.info("End testRM2.");
}

void Tests::testBPlusTree1() {
    if (BPlusTreeNode::HIGH != 5) {
        Logger::logger.info("testBPlusTree1 needs HIGH to be 5. Skip.");
        return;
    }
    Logger::logger.info("Start testBPlusTree1.");

    BPlusTree tree;
    tree.insertKey(5);
    tree.insertKey(2);
    tree.insertKey(9);
    tree.insertKey(4);
    tree.insertKey(3);
    tree.insertKey(6);
    tree.deleteKey(6);
    tree.deleteKey(9);
    tree.printTree();
    assert(tree.ndnum == 3);
    assert(tree.root->keys[1] == 5);
    tree.deleteKey(5);
    tree.printTree();
    assert(tree.ndnum == 1);
    tree.deleteKey(3);
    tree.deleteKey(2);
    tree.deleteKey(4);
    assert(tree.ndnum == 1 && tree.root->chnum == 0);
    tree.printTree();

    Logger::logger.info("End testBPlusTree1.");
}

void Tests::testBPlusTree2() {
    if (BPlusTreeNode::HIGH != 5) {
        Logger::logger.info("testBPlusTree2 needs HIGH to be 5. Skip.");
        return;
    }

    Logger::logger.info("Start testBPlusTree2.");

    BPlusTree tree;
    tree.insertKey(5);
    tree.insertKey(2);
    tree.insertKey(9);
    tree.insertKey(4);
    tree.insertKey(3);
    tree.insertKey(6);
    tree.insertKey(7);
    tree.insertKey(8);
    tree.insertKey(-1);
    tree.insertKey(10);
    tree.insertKey(11);
    tree.insertKey(12);
    tree.insertKey(13);
    tree.insertKey(14);
    tree.insertKey(15);
    tree.insertKey(16);
    tree.insertKey(17);
    tree.insertKey(18);
    tree.insertKey(19);
    tree.insertKey(25);
    assert(tree.root->keys[1] == 25);
    assert(tree.ndnum == 9);
    assert(tree.root->ch[1]->ch[2]->keys[3] == 25);

    tree.deleteKey(12);
    tree.deleteKey(13);
    tree.printTree();
    assert(tree.root->ch[1]->ch[0]->keys[1] == 14);
    
    tree.deleteKey(15);
    tree.printTree();
    assert(tree.root->ch[1]->ch[1]->keys[1] == 17);
    assert(tree.root->ch[1]->keys[1] == 17);

    tree.deleteKey(14);
    tree.printTree();
    assert(tree.ndnum == 8);
    assert(tree.root->ch[1]->chnum == 2);
    assert(tree.root->ch[1]->ch[0]->keys[1] == 16);

    tree.deleteKey(25);
    tree.printTree();
    assert(tree.root->keys[1] == 19);

    tree.deleteKey(19);
    tree.printTree();
    assert(tree.root->ch[1]->ch[0]->chnum == 2);
    assert(tree.root->ch[0]->chnum == 3);

    tree.deleteKey(18);
    tree.printTree();
    assert(tree.root->keys[1] == 17);
    assert(tree.ndnum == 7);
    assert(tree.root->ch[0]->chnum == 2);

    Logger::logger.info("End testBPlusTree2.");
}