#include <vector>
#include <memory>

using namespace std;

#pragma once

class Page {
public:
    Page(uint32_t pageNum);

    void writeData(uint32_t offset, const void *data, uint32_t size);
    void readData(uint32_t offset, void *buffer, uint32_t size) const;

    const uint8_t getData() const;

    uint32_t getPageNum() const;
    void clear();
    bool isDirty();
    void toggleDirty();
private:
    uint32_t pageNum;
    bool dirty;
    vector<int8_t> data;
};