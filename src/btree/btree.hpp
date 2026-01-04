#include "node.hpp"
#include <cstdint>
#include <memory>

using namespace std;

class Btree {
public:
    Btree();
    void insert(int64_t key);
    bool find(int64_t key);
private:
    unique_ptr<BtreeNode> root; // root node of the tree
    int64_t count; //  num items
    int64_t height; // height from root to leaf node
}