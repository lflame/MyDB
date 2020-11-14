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

    // 较小数据
    void testBPlusTree1();
    // 较大数据
    void testBPlusTree2();
};

#endif