#include "tuple.hpp"

using namespace std;

Tuple::Tuple(Schema *schema) : schema(schema) {
    // allocate regions
    buffLen = getHeaderSize();
    buffer = static_cast<uint8_t *>(calloc(1, buffLen));

    *nFieldPtr() = schema->getNumColumns();
}

uint32_t *Tuple::nFieldPtr() {
    return reinterpret_cast<uint32_t *>(buffer + sizeof(uint32_t));
}

uint8_t *Tuple::bitmapPtr() {
    return reinterpret_cast<uint8_t *>(nFieldPtr() + 1);
}

TupleItemField *Tuple::tupleOffsetsPtr() {
    return reinterpret_cast<TupleItemField *>(bitmapPtr() + 1);
}

uint32_t *Tuple::dataOffsetPtr() {
    return reinterpret_cast<uint32_t *>(tupleOffsetsPtr() + (getNumFields()));
}

// header = everything before data region
uint32_t Tuple::getHeaderSize() {
    return (sizeof(uint32_t) * 2) + sizeof(int8_t) + (getNumFields() * sizeof(TupleItemField));
}

Tuple Tuple::fromBytes(const Schema *schema, const uint8_t *data, uint16_t length) {

}

uint8_t *Tuple::serialize() {

}

bool Tuple::isTombstoned() {
    return getBit(0) == 0;
}

void Tuple::setTombstoned() {
    setBit(0, 0);
}

bool Tuple::isNull(int columnIndex) {
    return getBit(columnIndex + 1) == 0;
}

void Tuple::setNull(int columnIndex) {
    setBit(columnIndex,  0);
}

FieldValue Tuple::getField(int columnIndex) {
    validateIndex(columnIndex);
    TupleItemField *tupleOffsetsptr = tupleOffsetsPtr();
    TupleItemField tupleItemField = (TupleItemField)(*(tupleOffsetsptr + (sizeof(TupleItemField) * columnIndex)));

    uint32_t *fieldPtr = dataOffsetPtr() + tupleItemField.offset;
    uint8_t *field = (uint8_t *)calloc(1, sizeof(char) * tupleItemField.len);
    memcpy(field, fieldPtr, tupleItemField.len);

    FieldValue fv = decodeValue(field, tupleItemField.len);

    free(field);
    return fv;
}

void Tuple::setField(int columnIndex, const FieldValue &value) {
    // get offset and length of field
    // index into dataregion using that offset
    // set data there for length
    // we either need to splice in place or append to data region

    validateIndex(columnIndex);
    // check if tuple is null
    if (!isTombstoned()) throw invalid_argument("tuple nulled");

    uint16_t encodedLen;
    uint8_t *encodedValue = encodeValue(value, &encodedLen);

    TupleItemField *offsets = tupleOffsetsPtr();
    uint32_t *dataStart = dataOffsetPtr();
    uint32_t dataSize = buffLen - getHeaderSize();

    // should be inserted linearly, so if null must not be set yet
    if (isNull(columnIndex)) {
        uint16_t newOff = static_cast<uint16_t>(dataSize);

        buffLen += encodedLen;
        buffer = static_cast<uint8_t *>(realloc(buffer, buffLen));
        dataStart = dataOffsetPtr();
    } else { // must slice
        uint16_t oldOffset = offsets[columnIndex].offset;
        uint16_t oldLen = offsets[columnIndex].len;
        uint32_t lenDiff = static_cast<uint32_t>(encodedLen) - oldLen;

        if (lenDiff > 0) {
            // to complete
        }
    }
}

uint32_t Tuple::getNumFields() {
    return *(nFieldPtr());
}

void Tuple::validateIndex(int colIndex) {
    if (colIndex < 0 || colIndex >= this->getNumFields()) throw out_of_range("Tuple field column out of range");
}

// index is 0 - 7
char Tuple::getBit(int index) {
    return (*bitmapPtr() >> index) & 1;
}

void Tuple::setBit(int index, char value) {
    uint8_t *bitmap = bitmapPtr();
    if (value) *bitmap |= (1 << index);
    else *bitmap |= ~(1 << index);
}

uint8_t *Tuple::encodeValue(const FieldValue &value, uint16_t *len) {
    // to do
}

FieldValue Tuple::decodeValue(const uint8_t *data, uint16_t length) {
    // to do
}