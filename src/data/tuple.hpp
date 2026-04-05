#include <vector>
#include <memory>
#include <cstring>
#include <stdexcept>
#include "schema.hpp"

using namespace std;

#pragma once

// for bitmap ==> 0 = null, 1 = not

struct TupleItemField {
    uint16_t offset;
    uint16_t len;
};

// buf layout ==> id (32) | nfields (32) | bitmap (8) | tupleitemoffsets (var) | data region
// bitmap ==> isnull (0 = null, else 1) denotes if tuple still exists | is null for each field

class Tuple {
public:
    Tuple(Schema *schema);
    Tuple fromBytes(const Schema *schema, const uint8_t *data, uint16_t length);
    uint8_t *serialize();

    bool isTombstoned();
    void setTombstoned();
    bool isNull(int columnIndex);
    void setNull(int columnIndex);
    FieldValue getField(int columnIndex);
    void setField(int columnIndex, const FieldValue &value);

    uint32_t getNumFields();
private:
    const Schema *schema;
    uint8_t *buffer;
    uint32_t buffLen;

    uint32_t *nFieldPtr();
    uint8_t *bitmapPtr();
    TupleItemField *tupleOffsetsPtr();
    uint32_t *dataOffsetPtr();

    uint32_t getHeaderSize();

    void validateIndex(int colIndex);
    char getBit(int index);
    void setBit(int index, char value);

    static uint8_t *encodeValue(const FieldValue &value, uint16_t *len);
    static FieldValue decodeValue(const uint8_t *data, uint16_t length);
};