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

uint32_t LinHashManager::getChoiceVector() {
    return header.choiceVector;
}

void LinHashManager::split() {
    if (header.insertsSinceSplit <= header.loadThreshold) return;
    redistribute();
    freeEmptyOverflow();

}

void LinHashManager::merge() {

}

void LinHashManager::redistribute() {

}

void LinHashManager::contract() {

}