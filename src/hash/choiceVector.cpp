#include "choiceVector.hpp"

ChoiceVector::ChoiceVector(string chvec, uint32_t numAttrs) {
    attributes.resize(32);
    bits.resize(32);
    parseChoiceVector(chvec, numAttrs);
}

uint32_t ChoiceVector::getAttribute(int index) {
    if (index < 0 || index >= 32) return NULL; // exception
    return attributes[index];
}

uint32_t ChoiceVector::getBit(int index) {
    if (index < 0 || index >= 32) return NULL; // exception
    return attributes[index];
}

void ChoiceVector::parseChoiceVector(string chvec, uint32_t numAttrs) {
    uint32_t numC = 0;
    string currChar = chvec;
    //to do;
}