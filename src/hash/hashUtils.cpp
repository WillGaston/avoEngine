#define bit(i,h) (((h) & (1u << (i))) >> (i))

#include "linHashManager.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>

uint32_t hash_any(const char *value, uint32_t len) {
    uint32_t hash = 2166136261u;
    for (uint32_t i = 0; i < len; ++i) {
        hash ^= static_cast<unsigned char>(value[i]);
        hash *= 16777619u;
    }
    return hash;
}

uint32_t hashTuple(LinHashManager *linHashManager, Tuple *tuple) {
    if (!linHashManager || !tuple) throw std::invalid_argument("null argument to hashTuple");

    uint32_t numAttributes = linHashManager->getNumAttributes();
    uint32_t outputHash = 0;
    std::vector<uint32_t> attrHashArr(numAttributes, 0);
    ChoiceVector choiceVector = linHashManager->getChoiceVector();

    for (uint32_t i = 0; i < numAttributes; i++) {
        const char *attributeValue = reinterpret_cast<const char*>(tuple->getData(i));
        uint32_t attributeLength = tuple->getLength(i);
        attrHashArr[i] = hash_any(attributeValue, attributeLength);
    }

    for (int i = 0; i < 32; i++) {
        int currAttributePos = static_cast<int>(choiceVector.getAttribute(i));
        int corrBitPos = static_cast<int>(choiceVector.getBit(i));
        if (currAttributePos < 0 || currAttributePos >= static_cast<int>(numAttributes)) continue;
        uint32_t b = bit(corrBitPos, attrHashArr[currAttributePos]);
        outputHash |= (b << i);
    }

    return outputHash;
}

bool checkHashMatch(uint32_t numAttrs, Tuple pattern, Tuple tupleToCheck) {
    (void)numAttrs;
    (void)pattern;
    (void)tupleToCheck;
    return false;
}

bool checkHashMatchRec(Tuple pattern, Tuple tupleToCheck) {
    (void)pattern;
    (void)tupleToCheck;
    return false;
}