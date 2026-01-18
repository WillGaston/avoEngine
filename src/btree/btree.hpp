#include "node.hpp"
#include <cstdint>
#include <memory>

using namespace std;

class Btree {
public:
    Btree();
    void insert(int64_t key);
    bool find(int64_t key);     
    void remove(int64_t key);
    void traverse() const;
    int64_t getCount() const;
    int64_t getHeight() const;
    bool isEmpty() const;
private:
    unique_ptr<BtreeNode> root; // root node of the tree
    int64_t count; //  num items
    int64_t height; // height from root to leaf node

    bool findRecurse(BtreeNode* node, int64_t key);
    int insertRecurse(BtreeNode* node, int64_t key);
    void splitChild(BtreeNode* node, int index);
    void removeRecurse(BtreeNode* node, int64_t key);
    void traverseRecurse(const BtreeNode* node) const;
}