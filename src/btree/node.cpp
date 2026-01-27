#include "node.hpp"
#include <utility>

using namespace std;

BtreeNode::BtreeNode(bool leaf) : leaf(leaf), numKeys(0) {
    keys.reserve(MAX_KEYS);
    if (!leaf) childNodes.reserve(ORDER);
}

void BtreeNode::addKey(int newKey) {
    int index = 0;
    while (index < numKeys && this->keys[index] < newKey) index++;
    keys.insert(keys.begin() + index, newKey);
    numKeys++;
}


void BtreeNode::addChildNode(unique_ptr<BtreeNode> newNode) {
    childNodes.push_back(move(newNode));
}

int BtreeNode::findKeyIndex(int64_t key) {
    int i = 0;
    while (i < numKeys && key > keys[i]) i++;
    return i;
}

bool BtreeNode::containsKey(int64_t key) {
    int index = findKeyIndex(key);
    return index < numKeys && keys[index] == key;
}

bool BtreeNode::isFull() const { return numKeys == MAX_KEYS; }
bool BtreeNode::isMinimal() const { return numKeys == MIN_KEYS; }
bool BtreeNode::isLeaf() const { return this->leaf; }
int BtreeNode::getNumKeys() const { return numKeys; }

int64_t BtreeNode::getKey(int index) const { return keys[index]; }
BtreeNode* BtreeNode::getChild(int index) const { return childNodes[index].get(); }

pair<unique_ptr<BtreeNode>, int64_t> BtreeNode::split() {
    int midIndex = MAX_KEYS / 2;
    int64_t medianKey = keys[midIndex];

    auto rightNode = make_unique<BtreeNode>(leaf);

    for (int i = midIndex + 1; i < numKeys; i++) rightNode->keys.push_back(keys[i]);
    rightNode->numKeys = numKeys - midIndex - 1;

    if (!isLeaf()) {
        for (int i = midIndex + 1; i <= numKeys; i++) rightNode->childNodes.push_back(move(childNodes[i]));
        childNodes.resize(midIndex + 1);
    }

    keys.resize(midIndex);
    numKeys = midIndex;

    return {move(rightNode), medianKey};
}

void BtreeNode::insertKeyAt(int index, int64_t key) {
    keys.insert(keys.begin() + index, key);
    numKeys++;
}

void BtreeNode::insertChildAt(int index, unique_ptr<BtreeNode> child) {
    childNodes.insert(childNodes.begin() + index, move(child));
}

void BtreeNode::removeKey(int index) {
    keys.erase(keys.begin() + index);
    numKeys--;
}

void BtreeNode::removeChild(int index) {
    childNodes.erase(childNodes.begin() + index);
}

int64_t BtreeNode::getPredecessor(int index) {
    BtreeNode *curr = childNodes[index].get();

    while(!curr->isLeaf()) curr = curr->childNodes[curr->getNumKeys()].get();

    return curr->keys[curr->getNumKeys() - 1];
}

int64_t BtreeNode::getSuccessor(int index) {
    BtreeNode *curr = childNodes[index + 1].get();

    while (!curr->isLeaf()) curr = curr->childNodes[0].get();

    return curr->keys[0];
}

void BtreeNode::merge(int index) {
    BtreeNode *leftChild = childNodes[index].get();
    BtreeNode *rightChild = childNodes[index + 1].get();

    leftChild->keys.push_back(keys[index]);

    for (int i = 0; i < rightChild->numKeys; i++) leftChild->keys.push_back(rightChild->keys[i]);

    if (!leftChild->isLeaf()) {
        for (int i = 0; i <= rightChild->numKeys; i++) leftChild->childNodes.push_back(move(rightChild->childNodes[i]));
    }

    leftChild->numKeys += rightChild->numKeys + 1;

    keys.erase(keys.begin() + index);
    numKeys--;

    childNodes.erase(childNodes.begin() + index + 1);
}

void BtreeNode::borrowFromPrev(int index) {
    BtreeNode *child = childNodes[index].get();
    BtreeNode *sibling = childNodes[index - 1].get();

    child->keys.insert(child->keys.begin(), keys[index - 1]);
    child->numKeys++;

    keys[index - 1] = sibling->keys[sibling->numKeys - 1];
    sibling->keys.pop_back();
    sibling->numKeys--;

    if (!child->isLeaf()) {
        child->childNodes.insert(child->childNodes.begin(), move(sibling->childNodes[sibling->numKeys + 1]));
        sibling->childNodes.pop_back();
    }
}

void BtreeNode::borrowFromNext(int index) {
    BtreeNode *child = childNodes[index].get();
    BtreeNode *sibling = childNodes[index + 1].get();

    child->keys.push_back(keys[index]);
    child->numKeys++;

    keys[index] = sibling->keys[0];
    sibling->keys.erase(sibling->keys.begin());
    sibling->numKeys--;

    if (!child->isLeaf()) {
        child->childNodes.push_back(move(sibling->childNodes[0]));
        sibling->childNodes.erase(sibling->childNodes.begin());
    }
}

void BtreeNode::fill(int index) {
    if (index != 0 && childNodes[index - 1]->numKeys > MIN_KEYS) borrowFromPrev(index);
    else if (index != numKeys && childNodes[index + 1]->numKeys > MIN_KEYS) borrowFromNext(index);
    else if (index != numKeys) merge(index);
    else merge(index - 1);
}