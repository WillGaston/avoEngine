#include "node.hpp"

using namespace std;

BtreeNode(bool isLeaf) : this->isLeaf(isLeaf), numKeys(0) {
    keys.reserve(MAX_KEYS);
    if (!isLeaf) childNodes.reserve(ORDER);
}

BtreeNode::void addKey(int newKey) {
    int index = 0;
    while (index < numKeys && this->keys[index] < newKey) index++;
    keys.insert(keys.begin() + index, newKey);
    numKeys++;
}


BtreeNode::void addChildNode(unique_ptr<BtreeNode> newNode) {
    childNodes.push_back(newNode);
}

BtreeNode::int findKeyIndex(int64_t key) {
    int i = 0;
    while (i < numKeys && key > keys[i]) i++;
    return i;
}

BtreeNode::bool containsKey(int64_t key) {
    int index = findKeyIndex(key);
    return index < numKeys && keys[index] == key;
}

BtreeNode::bool isFull() return numKeys == MAX_KEYS;
BtreeNode::bool isMinimal() return numKeys == MIN_KEYS;
BtreeNode::bool isLeaf() return this->isLeaf;
BtreeNode::int getNumKeys() return numKeys;

BtreeNode::int64_t getKey(int index) return keys[index];
BtreeNode::BtreeNode* getChild(int index) return childNodes[index].get();

BtreeNode::unique_ptr<BtreeNode> split() {
    int midIndex = MAX_KEYS / 2;
    int64_t medianKey = keys[midIndex];
}

BtreeNode::void insertNotFull(int64_t key);

BtreeNode::insertKeyAt(int index, int64_t key) {
    keys.insert(keys.begin() + index, key);
    numKeys++;
}

BtreeNode::insertChildAt(int index, unique_ptr<BtreeNode> child) {
    childNodes.insert(childNodes.begin() + index, move(child));
}

BtreeNode::void removeKey(int index) {
    keys.erase(keys.begin() + index);
    numKeys--;
}

BtreeNode::void removeChild(int index) {
    childNodes.erase(childNodes.begin() + index);
}

BtreeNode::int64_t getPredecessor(int index) {
    BtreeNode *curr = childNodes[index].get();

    while(!curr->isLeaf()) curr = curr->childNodes[curr->getNumKeys].get();

    return curr->keys[curr->getNumKeys - 1];
}

BtreeNode::int64_t getSuccessor(int index) {
    BtreeNode *curr = childNodes[index + 1].get();

    while (!curr-isLeaf()) curr = curr->childNodes[0].get();

    return curr->keys[0];
}

BtreeNode::void merge(int index);
BtreeNode::void borrowFromPrev(int index);
BtreeNode::void borrowFromNext(int index);
BtreeNode::void fill(int index);