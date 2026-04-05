#include <vector>
#include <memory>
#include <cstring>
#include <stdexcept>

using namespace std;

#pragma once

#define PAGE_SIZE 4096

class Page {
public:
    Page(uint32_t pageNum);

    void writeData(uint32_t offset, const void *data, uint32_t size);
    void readData(uint32_t offset, void *buffer, uint32_t size) const;

    const uint8_t* getData() const;
    uint8_t *getData();

    uint32_t getPageNum() const;
    void clear();
    bool isDirty();
    void toggleDirty();
private:
    uint32_t pageNum;
    bool dirty;
    vector<uint8_t> data;
};