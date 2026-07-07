#include "choiceVector.hpp"
#include <algorithm>

ChoiceVector::ChoiceVector(string chvec, uint32_t numAttrs) {
    attributes.resize(32);
    bits.resize(32);
    parseChoiceVector(chvec, numAttrs);
}

uint32_t ChoiceVector::getAttribute(int index) {
    if (index < 0 || index >= 32) return 0;
    return attributes[index];
}

uint32_t ChoiceVector::getBit(int index) {
    if (index < 0 || index >= 32) return 0;
    return bits[index];
}

void ChoiceVector::parseChoiceVector(string chvec, uint32_t numAttrs) {
    fill(attributes.begin(), attributes.end(), 0);
    fill(bits.begin(), bits.end(), 0);

    if (chvec.empty()) return;

    uint32_t numC = 0;
    for (char c : chvec) {
        if (numC >= numAttrs) break;
        if (c == '1') {
            attributes[numC] = numC;
            bits[numC] = 1;
        } else {
            attributes[numC] = 0;
            bits[numC] = 0;
        }
        numC++;
    }
}