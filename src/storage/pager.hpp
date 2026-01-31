#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include "page.hpp"

using namespace std;

class Pager {
public:
    Pager(const string& filename);
    ~Pager();

    Page *getPage(uint32_t pageNum);

    void writePageToDisk(uint32_t pageNum, Page *page);
    void flushAll();
    void close();
    uint32_t getNumPages() const;
    uint32_t allocatePage();
private:
    string fileName;
    fstream file;
    map<uint32_t, unique_ptr<Page>> pageCache;
    int32_t numPages;

    void readPageFromDisk(uint32_t pageNum, Page *page);
    void writePage(uint32_t pageNum, Page *page);
    void openFile();
};