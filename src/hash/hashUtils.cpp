#define bit(i,h) (((h) & (1 << (i))) >> (i))

#include "linHashManager.hpp"

uint32_t hash_any(char *value, uint32_t len) {
    // todo
    return 0;
}

uint32_t hashTuple(LinHashManager *linHashManager, Tuple *tuple) {
    uint32_t numAttributes = linHashManager->getNumAttributes();
    uint32_t outputHash = 0;

    uint32_t attrHashArr[numAttributes + 1];
    uint32_t outputHash = 0;
    char **attributes;
    ChoiceVector choiceVector = linHashManager->getChoiceVector();

    for (int i = 0; i < numAttributes; i++) {
        attrHashArr[i] = hash_any(attributes[i], strlen(attributes[i]));
    }

    for (int i = 0; i < 32; i++) {
        int currAttributePos = choiceVector.getAttribute(i);
        int corrBitPos = choiceVector.getBit(i);
        int b = bit(corrBitPos, attrHashArr[currAttributePos]);
        outputHash |= (b << i);
    }

    return outputHash;
}

bool checkHashMatch(uint32_t numAttrs, Tuple pattern, Tuple tupleToCheck) {
    // todo
    return false;
}

bool checkHashMatchRec(Tuple pattern, Tuple tupleToCheck) {
    //todo
    return false;
}