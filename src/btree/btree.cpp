#include "btree.hpp"
#include "node.hpp"
#include <iostream>

using namespace std;

Btree::Btree() : root(nullptr), height(0), count(0) {}

Btree::getHeight() return this.height;

Btree::getCount() return this.count;

Btree::isEmpty() 

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

}

Btree::insertRecurse(BtreeNode* node, int64_t key) {

}

Btree::splitChild(BtreeNode* node, int index) {

}
 
Btree::remove(int64_t key) {

}

Btree::removeRecurse(BtreeNode* node, int64_t key) {

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
