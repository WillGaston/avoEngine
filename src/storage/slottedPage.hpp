#pragma once
#include "page.hpp"
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cstring>

// page layout
// pageheader [0 .. 19]
// slot directory [20 .. ?]
// free space [ ? .. ? ]
// tuple data [ ? .. end of page ]

#pragma pack(1)

struct PageHeader {
    uint32_t pageNum;
    uint16_t numSlots;
    uint16_t freeSpaceOffset;
    uint16_t endOfTuples;
    uint16_t checksum; // for future use :)
    uint32_t flags; // for future use :)
};

struct SlotEntry {
    uint16_t offset;
    uint16_t length;
    uint16_t flags; // for future use :) e.g. mvcc
    uint16_t padding;
};

static_assert(sizeof(PageHeader) == 16);
static_assert(sizeof(SlotEntry) == 8);

static constexpr uint32_t PAGE_HEADER_SIZE = sizeof(PageHeader);
static constexpr uint32_t SLOT_ENTRY_SIZE = sizeof(SlotEntry);

class SlottedPage {
public:
    SlottedPage(Page *page);
    void init(uint32_t pageNum);
    int insertTuple(const uint8_t *data, uint16_t length); // returns slot index
    uint16_t getTuple(int slotIndex, uint8_t *buffer, uint16_t bufferSize);
    void deleteTuple(int slotIndex);
    void compact();

    // helpers / getters
    uint16_t getFreeSpace(); // get the amount of free space available
    uint16_t actualDataSize(); // get non-tombestoned data size
    bool shouldCompact(); // check if dead space crossed threshold
    bool isSlotAlive(); // check if slot is tombstoned
private:
    Page *page;

    PageHeader readPageHeader();
    void writePageHeader(PageHeader &pageHeader);

    SlotEntry readSlot(int slotIndex);
    void writeSlot(int slotIndex, const SlotEntry &slot);

    // btree address encoding (32 bit pageNum, 32 bit slotindex)
    int64_t encodeRowAddress(uint32_t pageNum, uint32_t slotIndex);
    uint32_t getPageNum(uint64_t encodedAddress);
    uint32_t getSlotIndex(uint64_t encodedAddress);
};