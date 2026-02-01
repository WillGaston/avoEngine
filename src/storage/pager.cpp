#include "pager.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>

Pager::Pager(const string& filename): fileName(filename), numPages(0) {
    openFile();
}

Pager::~Pager() {
    flushall();
    if (file.is_open()) file.close();
}

Page *Pager::getPage(uint32_t pageNum) {
    if (pageCache.count(pageNum)) return pageCache[pageNum].get();

    auto newPage = make_unique<Page>(pageNum);

    if (pageNum < numPages) readPageFromDisk(pageNum, newPage.get());

    Page *pagePtr = newPage.get();
    pageCache[pageNum] = move(newPage);

    return pagePtr;
}

void Pager::readPageFromDisk(uint32_t pageNum, Page *page) {
    file.seekg(pageNum * pageSize);
    file.read(reinterpre_cast<char *>(page->getData()), PAGE_SIZE);

    if (file.fail() || !file.eof()) throw runtime_error("Failed to read page from disk");

    if (page->isDirty()) page->toggleDirty();
}

void Pager::writePageToDisk(uint32_t pageNum, Page *page) {
    if (!page) throw invalid_argument("page pointer is null");
    file.seekp(pageNum * pageSize);
    file.read(reinterpre_cast<char *>(page->getData()), PAGE_SIZE);

    if (file.fail() || !file.eof()) throw runtime_error("Failed to read page from disk");

    if (page->isDirty()) page->toggleDirty();
}

void Pager::flushAll() {
    for (auto &[pageNum, page] : pageCache) {
        if (page && page->isDirty()) writePageToDisk(pageNum, page.get());
    }

    if (file.is_open()) file.flush();
}

void Pager::close() {
    flushAll();
    if (file.is_open()) file.close();
    pageCache.clear();
}

uint32_t Pager::getNumPages() const { return numPages; }

uint32_t Pager::allocatePage() {
    uint32_t newPageNum = numPages;
    numPages++;
    return newPageNum;
}

void Pager::openFile() {
    file.open(fileName, ios::in | ios::out | ios::binary);

    if (!file.is_open()) {
        file.open(fileName, ios::out | ios::binary);
        file.close();
        file.open(fileName, ios::in | ios::out | ios::binary);
    }

    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    numPages = fileSize / PAGE_SIZE;
    file.seekg(0, ios::beg);
}