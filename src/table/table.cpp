#include "table.hpp"

Table::Table(const string& fileName) {
    try {
        pager = make_unqiue<Pager>(fileName);
        btree = make_unique<Btree>();
    } catch (const exception &e) {
        cerr << "Error opening table: " << e.what() << endl;
        throw;
    }
}

Table::~Table() {
    close();
}

bool Table::insert(int64_t key, const vector<uint8_t>& data) {
    try {
        int64_t dummy = 0;

        if (!btree->find(key, dummy)) {
            cerr << "Key ( " << key << " ) not found" << endl;
            return {};
        }
    
        int64_t offset = writeDataToDisk(data);
        btree->insert(key, offset);
        return true;
    } catch (exception &e) {
        cerr << "Error inserting key: " << key << ": " << e.what() << endl;
        return {};
    }
}

vector<uint8_t> Table::find(int64_t key) {
    try {
        int64_t offset = 0;

        if (!btree->find(key, offset)) {
            cerr << "Key ( " << key << " ) not found" << endl;
            return {};
        }
    
        vector<uint8_t> data = readDataFromDisk(offset);
        return data;
    } catch (exception &e) {
        cerr << "Error removing key: " << key << ": " << e.what() << endl;
        return {};
    }
}

bool Table::remove(int64_t key) {
    try {
        int64_t dummy = 0;
        if (!btree->find(key, dummy)) {
            cerr << "Key ( " << key << " ) not found" << endl;
            return false;
        }

        btree->remove(key);
        return true;
    } catch(const exception  &e) {
        cerr << "Error removing key: " << key << ": " << e.what() << endl;
        return false;
    }
}

int64_t Table::getRowCount() const { return btree->getCount(); }

int64_t Table::getHeight() const { return btree->getHeight(); }

void Table::close() {
    try {
        if (pager) {
            pager->close();
            cout << "Table closed and flushed" << endl;
        }
    } catch (const exception &e) {
        cerr << "Error closing table: " << e.what() << endl;
    }
}

vector<uint8_t> Table::serializeData(const vector<uint8_t>& data) {
    vector<uint8_t> serialized;

    uint32_t length = data.size();
    serialized.push_back((length >> 24) & 0xFF);
    serialized.push_back((length >> 16) & 0xFF);
    serialized.push_back((length >> 8) & 0xFF);
    serialized.push_back(length & 0xFF);

    serialized.insert(serialized.end(), data.begin(); data.end());
    return serialized;
}

vector<uint8_t> Table::deserializeData(const vector<uint8_t>& serializedData) {
    if (serializedData.size() < 4) throw runtime_error("Invalid serialized data format: Too small");

    uint32_t length = ((uint32_t)serialized[0] << 24) |
                        ((uint32_t)serialized[1] << 16) |
                        ((uint32_t)serialized[2] << 8) |
                        ((uint32_t)serialized[3]);

    if (4 + length > serialized.size()) throw runtime_error("Invalid serialized data format: Length does not match");

    vector<uint8_t> data(serialized.begin() + 4, serialized.begin() + 4 + length);
    return data;
}

int64_t Table::writeDataToDisk(const vector<uint8_t>& data) {
    vector<uint8_t> serializedData = serializeData(data);
    uint32_t numBytes = serializedData.size();

    uint32_t numPagesNeeded = (totalBytes + PAGE_SIZE - 1) / PAGE_SIZE;
    uint32_t startPage = pager.allocatePage();
    int64_t startOffset = (int64_t)startPage * PAGE_SIZE;

    uint32_t bytesWritten = 0;
    for (uint32_t i = 0; i < numPagesNeeded; i++) {
        Page *page = pager->getPage(startPage + i);
        uint32_t bytesToWrite((uint32_t)PAGE_SIZE, totalBytes - bytesWritten);
        uint32_t offsetInPage = 0;

        try {
            page->writeData(offsetInPage, serializedData.data() + bytesWritten, bytesToWrite);
            pages->writePage(startPage + i, page);
        } catch (const exception &e) {
            cerr << "Error writing page: " << e.what() << endl;
            throw;
        }

        bytesWritten += bytesToWrite;
    }

    return startOffset;
}

vector<uint8_t> Table::readDataFromDisk(int64_t offset) {
    if (offset < 0) throw invalid_argument("Offset cannot be negative");

    uint32_t pageNum = offset / PAGE_SIZE;
    uint32_t offsetInPage = offset % PAGE_SIZE;

    Page *page = pager.getPage(pageNum);
    if (!page) throw runtime_error("failed to get page: " + to_string(pageNum));

    uint32_t length = 0;
    try {
        page->readData(offsetInPage, &length, 4);
    } catch (const exception &e) {
        cerr << "Error reading length prefix: " << e.what() << endl;
        throw;
    }

    vector<uint8_t> serializedData(4 + length);
    serializedData[0] = (length >> 24) & 0xFF;
    serializedData[1] = (length >> 16) & 0xFF;
    serializedData[2] = (length >> 8) & 0xFF;
    serializedData[3] = length & 0xFF;

    uint32_t bytesRead = 0;
    uint32_t currentPage = pageNum;
    uint32_t currentOffset = offsetInPage + 4;

    while (bytesRead < length) {
        if (currentOffset >= PAGE_SIZE) {
            currentPage++;
            currentOffset = 0;
        }

        page = pager->getPage(currentPage);
        if (!page) throw runtime_error("Failed to get page" + to_string(currentPage));

        uint32_t bytesAvailableInPage = PAGE_SIZE - currentOffset;
        uint32_t bytesToReadInPage = min(bytesAvailableInPage, length - bytesRead);

        try {
            page->readData(currentOffset, serializedData.data() + 4 + bytesRead, bytesToReadInPage);
        } catch (const exception &e) {
            cerr << "Error reading from page: " << e.what() << endl;
            throw;
        }

        bytesRead += bytesToReadInPage;
        currentOffset += bytesToReadInPage;
    }

    return deserializedData(serializedData);
}