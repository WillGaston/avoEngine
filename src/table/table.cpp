#include "table.hpp"

Table::Table(const string& fileName) {

}

Table::~Table() {

}

bool Table::insert(int64_t key, const vector<uint8_t>& data) {

}

vector<int8_t> Table::find(int64_t key) {

}

bool Table::remove(int64_t key) {

}

int64_t Table::getRowCount() const {

}

int64_t Table::getHeight() const {

}

void Table::close() {

}

vector<uint8_t> Table::serializeData(const vector<uint8_t>& data) {

}

vector<uint8_t> Table::deserializeData(const vector<uint8_t>& serializedData) {

}

int64_t Table::writeDataToDisk(const vector<uint8_t>& data) {

}

vector<uint8_t> Table::readDataFromDisk(int64_t offset) {
    
}