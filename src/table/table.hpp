#include <vector>
#include <memory>
#include "btree.hpp"
#include "pager.hpp"

using namespace std;

#pragma once

struct Row {
    int64_t key;
    vector<uint8_t> data;
};

class Table {
public:
    Table(const string& fileName);
    ~Table();

    bool insert(int64_t key, const vector<uint8_t>& data);
    vector<uint8_t> find(int64_t key);
    bool remove(int64_t key);
    int64_t getRowCount() const;
    int64_t getHeight() const;
    void close();
private:
    unique_ptr<Btree> btree;
    unqiue_ptr<Pager> pager;

    vector<uint8_t> serializeData(const vector<uint8_t>& data);
    vector<uint8_t> deserializeData(const vector<uint8_t>& serializedData);

    int64_t writeDataToDisk(const vector<uint8_t>& data);
    vector<uint8_t> readDataFromDisk(int64_t offset);
};