#include "cassert"
#include "cstring"
#include "BPlusTree.h"
#include "Logger.h"
#include "RID.h"

using std::to_string;
using std::string;
using std::map;

void BNode::update() {
    if (!isLeaf()) {
        for (int i = 0; i < chnum; ++i) {
            assert(ch[i]->chnum != 0);
            keys[i] = ch[i]->keys[ch[i]->chnum-1];
        }
    }
}

int BNode::keyToChild(int k) {
    int ret;
    for (ret = 0; ret < chnum; ++ret) {
        if (k <= keys[ret]) break;
    }
    return ret;
}

int BNode::getChildInd(BNode *p) {
    for (int i = 0; i < chnum; ++i) if (ch[i] == p) return i;
    assert(0);
    return -1;
}

bool BNode::isLeaf() {
    return chnum == 0 || ch[0] == nullptr;
}

void BNode::insertChild(int c, BNode *p, int k) {
    assert(chnum <= HIGH+1 && c <= chnum);
    for (int i = chnum-1; i >= c; --i) {
        ch[i+1] = ch[i];
        keys[i+1] = keys[i];
    }
    ch[c] = p;
    keys[c] = k;
    ++chnum;
}

void BNode::deleteChild(int c) {
    assert(c >= 0 && c < chnum);
    for (int i = c; i < chnum - 1; ++i) {
        ch[i] = ch[i+1];
        keys[i] = keys[i+1];
    }
    --chnum;
}

BPlusTree::BPlusTree()  {
    ndnum = 0;
    root = newNode();
}

BPlusTree::~BPlusTree() {
    // TODO: deleteTree(root);
}

BNode* BPlusTree::newNode() {
    BNode *ret = new BNode();
    ++ndnum;
    return ret;
}

BNode* BPlusTree::findNode(int k) {
    BNode *s = root;
    while (!s->isLeaf()) {
        int c = s->keyToChild(k);
        s = s->ch[std::min(c, s->chnum-1)];
    }
    return s;
}

void BPlusTree::handleSplit(BNode *p) {
    if (p->chnum == BNode::HIGH + 1) {
        int lnum = (BNode::HIGH+1) / 2;
        // 将 p 分裂为两部分，前 lnum 为一部分
        BNode *rnode = newNode(), *fa = p->fa;
        for (int i = lnum; i < p->chnum; ++i) {
            rnode->insertChild(i-lnum, p->ch[i], p->keys[i]);
        }
        p->chnum = lnum;
        // 更新连边和信息
        p->update(), rnode->update();
        if (fa) {
            int c = fa->getChildInd(p) + 1;
            fa->insertChild(c, rnode, 0);
        } else {
            // 处理为根的情况
            assert(p == root);
            fa = newNode();
            fa->insertChild(0, p, 0);
            fa->insertChild(1, rnode, 0);
            root = fa;
        }
        p->fa = rnode->fa = fa;
        fa->update();
        handleSplit(fa);
    }
}

void BPlusTree::handleMerge(BNode *p) {
    if (p != root) {
        if (p->chnum == BNode::LOW - 1) {

        }
    }
}

void BPlusTree::insertKey(int k) {
    BNode* p = findNode(k);
    int c = p->keyToChild(k);
    assert(p->keys[c] != k);  // 保证没有重复键
    p->insertChild(c, nullptr, k);
    handleSplit(p);
}

void BPlusTree::deleteKey(int k) {
    BNode* p = findNode(k);
    int c = p->keyToChild(k);
    assert(p->keys[c] == k);  // 保证被删除的键存在
    p->deleteChild(c);
    handleMerge(p);
}

void BPlusTree::printTreeUpdateInd(BNode *s, std::map<BNode*, int>& mp, int &lastInd) {
    if (mp.find(s) == mp.end()) mp[s] = ++lastInd;
}

void BPlusTree::printTree(BNode *s, map<BNode*, int> &mp, int &lastInd) {
    printTreeUpdateInd(s, mp, lastInd);
    string str = to_string(mp[s]) + ": " + to_string(mp[s->fa]) + " " + to_string(s->chnum) + " ";
    for (int i = 0; i < s->chnum; ++i) {
        printTreeUpdateInd(s->ch[i], mp, lastInd);
        str = str + to_string(mp[s->ch[i]]) + "|" + to_string(s->keys[i]) + " ";
    }
    Logger::logger.debug("%s", str.c_str());
    for (int i = 0; i < s->chnum; ++i) {
        if (s->ch[i] != nullptr)
            printTree(s->ch[i], mp, lastInd);
    }
}

void BPlusTree::printTree() {
    map<BNode*, int> mp;
    mp[nullptr] = 0;
    Logger::logger.info("Start printTree.");
    Logger::logger.debug("%d", ndnum);
    int lastInd = 0;
    printTree(root, mp, lastInd);
    Logger::logger.info("End printTree.");
}