#include "linHashManager.hpp"

void LinHashManager::insert(const uint8_t* tupleData, uint32_t tupleSize) {

}

void LinHashManager::remove(const uint8_t* tupleData, uint32_t tupleSize) {

}

vector<uint8_t*> LinHashManager::search(const uint8_t* keyData, uint32_t key) {

}

uint32_t LinHashManager::getDepth() {
    return header.depth;
}

uint32_t LinHashManager::getSplitPtr() {
    return header.sp;
}

uint32_t LinHashManager::getNumBuckets() {
    return header.numBuckets;
}

uint32_t LinHashManager::getNumTuples() {
    return header.numTuples;
}

ChoiceVector LinHashManager::getChoiceVector() {
    return header.choiceVector;
}

uint32_t LinHashManager::getNumAttributes() {
    return header.numAttributes;
}

void LinHashManager::split() {
    if (header.insertsSinceSplit <= header.loadThreshold) return;
    redistribute();
    freeEmptyOverflow();

}

void LinHashManager::merge() {

}

void LinHashManager::redistribute() {
    // add new page
    // create array of tuples
    // add tuples to the pages
}

void LinHashManager::contract() {
    LinHashHeader header = this->header;
    uint32_t lastBucketId = header.numBuckets - 1;
    int depth = header.depth;

    if (header.sp == 0) depth--;
    uint32_t buddyPageId = lastBucketId ^ (1 << (depth - 1));

    if (header.sp == 0) {
        header.depth--;
        header.sp = (1 << header.depth) - 1;
    } else {
        header.sp--;
    }

    //todo
}

uint32_t LinHashManager::hashToPage(uint32_t bits) {
    LinHashHeader header = this->header;
    uint32_t p;
    if (header.depth == 0)
        p = 0;
    else
        p = getLower(bits, header.depth);

    if (p < header.sp)
        p = getLower(bits, header.depth + 1);

    return p;
}

uint32_t getLower(uint32_t bits, uint32_t num) {
	uint32_t i = 0; 
    uint32_t mask = 0;

	for (i = 0; i < num; i++) mask |= (1 << i);

	return bits & mask;
}