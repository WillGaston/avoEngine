using namespace std;

#include "slottedPage.hpp"

SlottedPage::SlottedPage(Page *page) : page(page) {
    if (!page) throw invalid_argument("SlottedPage: null page pointer is not allowed");
}

void SlottedPage::init(uint32_t pageNum) {
    page->clear();

    PageHeader header;
    header.pageNum = pageNum;
    header.numSlots = 0;
    header.freeSpaceOffset = static_cast<uint16_t>(PAGE_HEADER_SIZE);
    header.endOfTuples = static_cast<uint16_t>(PAGE_SIZE);
    header.checksum = 0;
    header.flags = 0;

    writePageHeader(header);
}

// return slot index or -1,. also throws exceptions if necessary
int SlottedPage::insertTuple(const uint8_t *data, uint16_t length) {
    if (!data) throw invalid_argument("null data on tuple insert - slotted page");
    
    PageHeader header = readPageHeader();

    uint16_t newSlotEnd = header.freeSpaceOffset + static_cast<uint16_t>(SLOT_ENTRY_SIZE);
    uint16_t newTupleStart = header.endOfTuples - length;

    // check if the page is too full to add tuple
    if (newTupleStart < newSlotEnd) return -1;

    page->writeData(newTupleStart, data, length);

    SlotEntry slotEntry;
    slotEntry.offset = newTupleStart;
    slotEntry.length = length;
    slotEntry.flags = 0;
    slotEntry.padding = 0;

    page->writeData(header.freeSpaceOffset, &slotEntry, SLOT_ENTRY_SIZE);

    int slotIndex = header.numSlots;
    header.numSlots++;
    header.freeSpaceOffset = newSlotEnd;
    header.endOfTuples = newTupleStart;
    writePageHeader(header);

    return slotIndex;
}

// returns tuple length, tuple data in buffer
uint16_t SlottedPage::getTuple(int slotIndex, uint8_t *buffer, uint16_t bufferSize) {
    SlotEntry slotEntry = readSlot(slotIndex);

    if (slotEntry.length == 0) return 0;
    if (slotEntry.length > bufferSize) throw runtime_error("buffer does not have enough space for getTuple in slottedPage");

    page->readData(slotEntry.offset, buffer, slotEntry.length);
    return slotEntry.length;
}

void SlottedPage::deleteTuple(int slotIndex) {
    SlotEntry slotEntry = readSlot(slotIndex);

    if (slotEntry.length == 0) return;

    slotEntry.length = 0;
    slotEntry.offset = 0;
    writeSlot(slotIndex, slotEntry);
}

void SlottedPage::compact() {
    // to do
}

uint16_t SlottedPage::getFreeSpace() {
    PageHeader header = readPageHeader();
    uint16_t freeSpaceGap = header.endOfTuples - header.freeSpaceOffset;
    if (freeSpaceGap < SLOT_ENTRY_SIZE) return 0;
    return freeSpaceGap - static_cast<uint16_t>(SLOT_ENTRY_SIZE);
}

uint16_t SlottedPage::actualDataSize() {
    PageHeader header = readPageHeader();
    uint16_t totalSize = 0;
    for (int i = 0; i < header.numSlots; i++) {
        SlotEntry slotEntry = readSlot(i);
        totalSize += slotEntry.length;
    }
    return totalSize;
}

// should compact if 40% tuple area is dead
bool SlottedPage::shouldCompact() {
    PageHeader header = readPageHeader();

    uint16_t usedSpace = PAGE_SIZE - header.endOfTuples;
    uint16_t liveSpace = actualDataSize();

    return usedSpace > 0 && (usedSpace - liveSpace) > (usedSpace * 0.4);
}

bool SlottedPage::isSlotAlive(int slotIndex) {
    PageHeader header = readPageHeader();

    if (slotIndex < 0 || slotIndex >= header.numSlots) return false;
    return readSlot(slotIndex).length > 0;
}

uint16_t SlottedPage::getNumSlots() {
    return readPageHeader().numSlots;
}

uint32_t SlottedPage::getPageNum() {
    return readPageHeader().pageNum;
}

PageHeader SlottedPage::readPageHeader() {
    PageHeader header;
    page->readData(0, &header, PAGE_HEADER_SIZE);
    return header;
}

void SlottedPage::writePageHeader(PageHeader &pageHeader) {
    page->writeData(0, &pageHeader, PAGE_HEADER_SIZE);
}

SlotEntry SlottedPage::readSlot(int slotIndex) {
    PageHeader header = readPageHeader();

    if (slotIndex < 0 || slotIndex >= static_cast<int>(header.numSlots)) throw out_of_range("slotted page slot index out of bounds");

    SlotEntry slotEntry;
    uint32_t byteOffset = PAGE_HEADER_SIZE + (slotIndex * SLOT_ENTRY_SIZE);
    page->readData(byteOffset, &slotEntry, SLOT_ENTRY_SIZE);
    return slotEntry;
}

void SlottedPage::writeSlot(int slotIndex, const SlotEntry &slot) {
    if (slotIndex < readPageHeader().numSlots) throw out_of_range("incorrect slotIndex");
    uint32_t byteOffset = PAGE_HEADER_SIZE + (slotIndex * SLOT_ENTRY_SIZE);
    page->writeData(byteOffset, &slot, SLOT_ENTRY_SIZE);
}

uint64_t SlottedPage::encodeRowAddress(uint32_t pageNum, uint32_t slotIndex) {
    return (static_cast<int64_t>(pageNum) << 32) | static_cast<int64_t>(slotIndex);
}

uint32_t ::getPageNum(uint64_t encodedAddress) {
    return static_cast<uint32_t>(encodedAddress >> 32);
}

uint32_t SlottedPage::getSlotIndex(uint64_t encodedAddress) {
     return static_cast<uint32_t>(encodedAddress & 0xFFFFFFFF);
}