#include "btree.hpp"
#include "node.hpp"
#include <iostream>

using namespace std;

Btree::Btree() : root(nullptr), height(0), count(0) {}

Btree::getHeight() return this->height;

Btree::getCount() return this->count;

Btree::isEmpty() return root == nullptr;

Btree::find(int64_t key) {
    if (!root) return false;
    return findRecurse(root, key);
}

Btree::findRecurse(BtreeNode* root, int64_t key) {
    int index = 0;

    while (index < root->getNumDataItems() && key > root->dataItems[i]) i++;

    if (index < root->getNumDataItems() && key == root->dataItems[i]) return true;
    if (isLeaf) return false;

    return findRecurse(root->childNodes[i], key);
}

Btree::insert(int64_t key) {
    if (!root) {
        root = make_unique<BtreeNode>(true);
        root->addKey(key);
        this->height = 1;
        this->count = 1;
        return; 
    }

    if (root->isFull()) {
        auto newRoot = make_unique<BtreeNode>(false);
        newRoot.addChildNode(move(root));
        splitChild(newRoot.get(), 0);
        root = move(newRoot);
        height++;
    }

    insertRecurse(this->root.get(), key);
    this->count++;
}

Btree::insertRecurse(BtreeNode* node, int64_t key) {
    if (node->isLeaf()) {
        node->insertNotFull(key);
        return;
    }

    int index = node->findKeyIndex(key);
    BtreeNode *child = node->getChild(index);

    if (child.isFull()) {
        splitChild(node, index);

        if (key > node->getKey(index)) index++;
    }

    insertRecurse(node->getChild(index), key);
}

Btree::splitChild(BtreeNode* node, int index) {
    BtreeNode *fullChild = node->getChild(index);

    int64_t medianKey;
    auto rightChild = fullChild->split(medianKey);

    node->insertKeyAt(index, medianKey);
    node->insertChildAt(index + 1, move(rightChild));
}
 
Btree::remove(int64_t key) {
    if (!root) return;

    removeRecurse(root, key);

    if (root->getNumKeys() == 0) {
        if (!root->isLeaf()) {
            BtreeNode *oldRoot = root;
            root = root->getChild(0);
        } else {
            root.reset();
        }
    }

    this->count--;
}

Btree::removeRecurse(BtreeNode* node, int64_t key) {
    int index = node->findKeyIndex(key);

    if (index < node->getNumKeys() && node->getKey(index) == key) {
        if (node->isLeaf()) {
            node->removeKey(index);
        } else {
            BtreeNode *leftChild = node->getChild(index);
            BtreeNode *rightChild = node->getChild(index + 1);

            if (!leftChild->isMinimal()) {
                int64_t predKey = node->getPredecessor(index);
                node->replaceKey(index, predKey);
                removeRecurse(leftChild, predKey);
            } else (!rightChild->isMinimal()) {
                int64_t succKey = node->getSuccessor(index);
                node->replaceKey(index, succKey);
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

Btree::traverse() {
    traverseRecurse(this.root);
    cout << endl;
}

Btree::traverseRecurse(const BtreeNode* node) {
    if (node == nullptr) return;

    int numKeys = node->getNumKeys();
    
    // ith traversal is before ith key (less than) 
    for (int i = 0; i < numKeys; i++) {
        if (!node->isLeaf()) traverseRecurse(node->getChild(i));
        cout << node->getKey(i) << " ";
    }

    if (!node->isLeaf()) traverseRecurse(node->getChild(numKeys));
}
