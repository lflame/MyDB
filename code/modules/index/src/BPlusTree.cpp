#include "cassert"
#include "BPlusTree.h"
#include "RID.h"

void BNode::update() {
    for (int i = 0; i < chnum; ++i) {
        assert(ch[i]->chnum != 0);
        keys[i] = ch[i]->keys[ch[i]->chnum-1];
    }
}

int BNode::keyToChild(int k) {
    assert(chnum != 0);
    int ret;
    for (ret = 0; ret <= chnum; ++ret) {
        if (k <= keys[ret]) break;
    }
    return ret;
}

void BNode::insertChild(int c, BNode *p, int k) {
    assert(chnum <= HIGH+1);
    for (int i = chnum; i >= c; --i) {
        ch[i+1] = ch[i];
        keys[i+1] = keys[i];
    }
    ch[c] = p;
    keys[c] = k;
    ++chnum;
}

BPlusTree::BPlusTree()  {
    root = newNode();
}

BPlusTree::~BPlusTree() {
    // TODO: deleteTree(root);
}

BNode* BPlusTree::newNode() {
    BNode *ret = new BNode();
    return ret;
}

BNode* BPlusTree::findNode(int k) {
    BNode *s = root;
    while (s->chnum) {
        int c = s->keyToChild(k);
        s = s->ch[c];
    }
    return s;
}

void BPlusTree::handleSplit(BNode *p) {
    
}

void BPlusTree::insertNode(int k) {
    BNode* p = findNode(k);
    int c = p->keyToChild(k);
    assert(p->keys[c] != k);  // 保证没有重复键
    p->insertChild(c, nullptr, k);
    handleSplit(p);
}