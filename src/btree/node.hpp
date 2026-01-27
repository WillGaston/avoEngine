#include <vector>
#include <memory>
#include <memory>

using namespace std;

#pragma once

class BtreeNode {
public:
    BtreeNode(bool leaf);

    void addKey(int newItem); // add a data item to the current btree node
    void addChildNode(unique_ptr<BtreeNode> newNode); // add a btree node to children array

    int findKeyIndex(int64_t key); // find index of key location
    bool containsKey(int64_t key); // check if key exists

    bool isFull() const;
    bool isMinimal() const;
    bool isLeaf() const;
    int getNumKeys() const;

    int64_t getKey(int index) const;
    BtreeNode* getChild(int index) const;

    pair<unique_ptr<BtreeNode>, int64_t> split();

    void insertNotFull(int64_t key);
    void insertKeyAt(int index, int64_t key);
    void insertChildAt(int index, unique_ptr<BtreeNode> child);

    void removeKey(int index);
    void removeChild(int index);
    int64_t getPredecessor(int index);
    int64_t getSuccessor(int index);
    void merge(int index);
    void borrowFromPrev(int index);
    void borrowFromNext(int index);
    void fill(int index); // fixes node underflow
private:
    bool leaf;
    int64_t numKeys;
    vector<int> keys;
    vector<unique_ptr<BtreeNode>> childNodes;

    static const int ORDER = 10;
    static const int MAX_KEYS = ORDER - 1;
    static const int MIN_KEYS = (ORDER - 1) / 2;
};
