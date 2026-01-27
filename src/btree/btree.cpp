#include "btree.hpp"
#include <iostream>

using namespace std;

Btree::Btree() : root(nullptr), height(0), count(0) {}

BtreeNode* Btree::getRoot() const { return root.get(); }

int64_t Btree::getHeight() const { return this->height; }

int64_t Btree::getCount() const { return this->count; }

bool Btree::isEmpty() const { return root == nullptr; }

bool Btree::find(int64_t key) {
    if (!root) return false;
    return findRecurse(root.get(), key);
}

bool Btree::findRecurse(BtreeNode* root, int64_t key) {
    int index = root->findKeyIndex(key);
    if (index < root->getNumKeys() && key == root->getKey(index)) return true;
    if (root->isLeaf()) return false;
    return findRecurse(root->getChild(index), key);
}

void Btree::insert(int64_t key) {
    if (!root) {
        root = make_unique<BtreeNode>(true);
        root->addKey(key);
        this->height = 1;
        this->count = 1;
        return; 
    }

    if (root->isFull()) {
        auto newRoot = make_unique<BtreeNode>(false);
        newRoot->addChildNode(move(root));
        splitChild(newRoot.get(), 0);
        root = move(newRoot);
        height++;
    }

    insertRecurse(this->root.get(), key);
    this->count++;
}

void Btree::insertRecurse(BtreeNode* node, int64_t key) {
    if (node->isLeaf()) {
        node->addKey(key);
        return;
    }

    int index = node->findKeyIndex(key);
    BtreeNode *child = node->getChild(index);

    if (child->isFull()) {
        splitChild(node, index);

        if (key > node->getKey(index)) index++;
    }

    insertRecurse(node->getChild(index), key);
}

void Btree::splitChild(BtreeNode* node, int index) {
    BtreeNode *fullChild = node->getChild(index);
    auto [rightChild, medianKey] = fullChild->split();
    node->insertKeyAt(index, medianKey);
    node->insertChildAt(index + 1, move(rightChild));
}
 
void Btree::remove(int64_t key) {
    if (!root) return;

    removeRecurse(root.get(), key);

    if (root->getNumKeys() == 0) {
    if (!root->isLeaf()) {
        auto oldRoot = move(root);
        height--;
    } else {
        root.reset();
        height = 0;
    }
}

    this->count--;
}

void Btree::removeRecurse(BtreeNode* node, int64_t key) {
    int index = node->findKeyIndex(key);

    if (index < node->getNumKeys() && node->getKey(index) == key) {
        if (node->isLeaf()) {
            node->removeKey(index);
        } else {
            BtreeNode *leftChild = node->getChild(index);
            BtreeNode *rightChild = node->getChild(index + 1);

            if (!leftChild->isMinimal()) {
                int64_t predKey = node->getPredecessor(index);
                node->removeKey(index);
                node->insertKeyAt(index, predKey);
                removeRecurse(leftChild, predKey);
            } else if (!rightChild->isMinimal()) {
                int64_t succKey = node->getSuccessor(index);
                node->removeKey(index);
                node->insertKeyAt(index, succKey);
                removeRecurse(rightChild, succKey);
            } else {
                node->merge(index);
                removeRecurse(leftChild, key);
            }
        }
    } else {
        if (node->isLeaf()) return;

        bool isInLastChild = index == node->getNumKeys();
        BtreeNode *child = node->getChild(index);   
        
        if (child->isMinimal()) {
            node->fill(index);
            if (isInLastChild && index > node->getNumKeys()) index--;
            if (index < node->getNumKeys() && node->getKey(index) == key) {
                removeRecurse(node, key);
                return;
            } 
        }

        removeRecurse(node->getChild(index), key);
    }
}

void Btree::traverse() const {
    traverseRecurse(root.get());
    cout << endl;
}

void Btree::traverseRecurse(const BtreeNode* node) const {
    if (node == nullptr) return;

    int numKeys = node->getNumKeys();
    
    // ith traversal is before ith key (less than) 
    for (int i = 0; i < numKeys; i++) {
        if (!node->isLeaf()) traverseRecurse(node->getChild(i));
        cout << node->getKey(i) << " ";
    }

    if (!node->isLeaf()) traverseRecurse(node->getChild(numKeys));
}
