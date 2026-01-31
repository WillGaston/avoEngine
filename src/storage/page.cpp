#include "page.hpp"

using namespace std;

Page::Page(uint32_t newPageNum) : pageNum(newPageNum), dirty(false) {
    data.resize(PAGE_SIZE, 0);
}

void Page::writeData(uint32_t offset, const void *data, uint32_t size) {
    if (offset + size > PAGE_SIZE) throw out_of_range("Write exceeds page size");

    memcpy(this->data.data() + offset, data, size);
    this->dirty = true;
}

void Page::readData(uint32_t offset, void *buffer, uint32_t size) const {
    if (offset + size > PAGE_SIZE) throw out_of_range("Read exceeds page size");

    memcpy(buffer, this->data.data() + offset size);
}

const uint8_t Page::getData() const {
    return data.data();
}

void Page::clear() {
    fill(data.begin(), data.end(), 0);
    this->dirty = true;
}

uint32_t Page::getPageNum() const { return pageNum; }
bool Page::isDirty() { return dirty; }
void Page::toggleDirty() { dirty = !dirty; }