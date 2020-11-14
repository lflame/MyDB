#ifndef _TESTS_H_
#define _TESTS_H_

class Tests {  
  public:
    void testAll();

    void testLogger();

    // 测试大量插入、删除
    void testRM1();

    // 测试属性比较
    void testRM2();

    // 测试 B+ 树的插入
    void testBPlusTree1();
    // 测试 B+ 树的删除
    void testBPlusTree2();
};

#endif