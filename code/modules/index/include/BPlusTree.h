#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_

struct BPlusTreeNode {
    static const int HIGH = 5, LOW = HIGH/2;
    // 此处开数组使用 HIGH+1 是为了更方便处理上溢的情况，而稳定状态下至多使用 HIGH
    BPlusTreeNode *ch[HIGH+1];
    int keys[HIGH+1], chnum;

    BPlusTreeNode() {
        chnum = 0;
        for (int i = 0; i < HIGH; ++i) ch[i] = nullptr;
    }

    // 利用子节点信息更新当前节点信息
    void update();

    // 找到键 k 对应所在的子节点，返回子节点下标
    int keyToChild(int k);

    // 将节点 p 及键 k 分别插入到 ch[c] 和 keys[c] 上，p 可以为 nullptr
    void insertChild(int c, BPlusTreeNode *p, int k);
};

typedef BPlusTreeNode BNode;

class BPlusTree {
public:
    BNode *root;

    BPlusTree();
    ~BPlusTree();
    // 找到键 k 所在的叶子节点，若不存在该键则返回其应该在的叶子节点处
    BNode* findNode(int k);
    BNode* newNode();
    // 插入键 k 作为新节点
    void insertNode(int k);
    // 处理上溢的分裂
    void handleSplit(BNode *p);
};

#endif