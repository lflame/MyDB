#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_

#include "map"
#include "AttrType.h"

struct BPlusTreeNode;

typedef BPlusTreeNode BNode;
typedef AttrListWithRID BKey;

struct BPlusTreeNode {
    static const int HIGH = 5, LOW = HIGH/2;
    // 此处开数组使用 HIGH+1 是为了更方便处理上溢的情况，而稳定状态下至多使用 HIGH
    BPlusTreeNode *ch[HIGH+1], *fa;
    int chnum;
    BKey keys[HIGH+1];

    BPlusTreeNode(int attrNum) {
        chnum = 0;
        for (int i = 0; i <= HIGH; ++i) ch[i] = nullptr, keys[i].attrList.init(attrNum);
        fa = nullptr;
    }

    // 利用子节点信息更新当前节点信息
    void update();

    // 找到键 k 对应所在的子节点，返回子节点下标
    int keyToChild(BKey k);

    // 判断 p 为当前节点的哪个儿子，返回子节点下标
    int getChildInd(BNode *p);

    // 检查是否为叶子节点
    bool isLeaf();

    // 将节点 p 及键 k 分别插入到 ch[c] 和 keys[c] 上，p 可以为 nullptr
    void insertChild(int c, BNode *p, BKey k);

    // 删除 ch[c] 和 keys[c]
    void deleteChild(int c);
};

class BPlusTree {
public:
    // 根节点
    BNode *root;
    // 节点数
    int ndnum;
    // 只关注其中的类型和长度
    AttrList attrList;
    // 用于方便代码书写
    BKey defaultKey;

    BPlusTree(AttrList attrList);
    ~BPlusTree();
    // 回收整棵树
    void deleteTree(BNode *p);
    // 找到键 k 所在的叶子节点，若不存在该键则返回其应该在的叶子节点处
    BNode* findNode(BKey k);
    BNode* newNode();
    void deleteNode(BNode *p);
    // 插入键 k
    void insertKey(BKey k);
    // 删除键 k
    void deleteKey(BKey k);
    // 处理上溢的分裂
    void handleSplit(BNode *p);
    // 处理下溢的合并
    void handleMerge(BNode *p);
    // 调用 update 直到根
    void updateToRoot(BNode *p);
    /*
     * 打印整棵树，便于调试，注意只处理了单个数据且为 int 的情况
     * 格式为：
     *   节点数
     *   节点编号: 父亲编号 子节点数目 [子节点编号-键、...]
     *   ...
     */
    void printTree();
    // mp 存储指针到编号的映射，lastInd 存储目前遍历到的最大编号
    void printTree(BNode *s, std::map<BNode*, int>& mp, int &lastInd);
    // 检查 s 是否在 mp 中出现过，以决定是否为其赋上新的编号
    void printTreeUpdateInd(BNode *s, std::map<BNode*, int>& mp, int &lastInd);
};

#endif