#include <vector>
#include <memory>

using namespace std;

class BtreeNode {
public:
    BtreeNode(bool isLeaf);

    void addDataItem(int newItem); // add a data item to the current btree node
    void addChildNode(unique_ptr<BtreeNode> newNode); // add a btree node to children array
private:
    bool isLeaf;
    int64_t numDataItems;
    vector<int> numDataItems;
    vector<unique_ptr<BtreeNode>> childNodes;
}