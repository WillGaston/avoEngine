#include <vector>
#include <memory>

using namespace std;

class BtreeNode {
public:
    BtreeNode(bool isLeaf);

    void addDataItem(int newItem); // add a data item to the current btree node
    void addChildNode(unique_ptr<BtreeNode> newNode); // add a btree node to children array

    int findKeyIndex(int64_t key) const; // find index of key location
    bool containsKey(int64_t key) const; // check if key exists

    bool isFull() const;
    bool isMinimal() const;
    bool isLead() const;
    int getNumDataItems() const;

    int64_t getKey(int index) const;
    BtreeNode* getChild(int index) const;

    unique_ptr<BtreeNode> split(int& medianKey); // returns right half

    void insertNotFull(int64_t key);

    void removeKey(int index);
    void removeChild(int index);
    int64_t getPredecessor(int index) const;
    int64_t getSuccessor(int index) const;
    void merge(int index);
    void borrowFromPrev(int index);
    void borrowFromNext(int index);
    void fill(int index); // fixes node underflow
private:
    bool isLeaf;
    int64_t numDataItems;
    vector<int> dataItems;
    vector<unique_ptr<BtreeNode>> childNodes;

    static const int ORDER = 10;
    static const int MAX_KEYS = ORDER - 1;
    static const int MIN_KEYS = (ORDER - 1 ) / 2;
}