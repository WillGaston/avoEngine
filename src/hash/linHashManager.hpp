#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include "../storage/pager.hpp"
#include "../storage/slottedPage.hpp"
#include "./choiceVector.hpp"

#define NO_PAGE -1

using namespace std;

struct LinHashHeader {
    uint32_t depth;
    uint32_t sp;
    uint32_t numBuckets;
    uint32_t numTuples;
    uint32_t numAttributes;
    uint32_t loadThreshold;
    uint32_t mergeThreshold;
    uint32_t ovflowFreeList;

    uint32_t insertsSinceSplit;
    uint32_t deletesSinceMerge;
    ChoiceVector choiceVector;
};

class LinHashManager {
public:
    LinHashManager(uint32_t numAttributes, uint32_t loadThreshold, uint32_t mergeThreshold, ChoiceVector choiceVector);

    void insert(const uint8_t* tupleData, uint32_t tupleSize);
    void remove(const uint8_t* tupleData, uint32_t tupleSize);
    vector<uint8_t*> search(const uint8_t* keyData, uint32_t key);

    uint32_t getDepth();
    uint32_t getSplitPtr();
    uint32_t getNumBuckets();
    uint32_t getNumTuples();
    ChoiceVector getChoiceVector();
    uint32_t getNumAttributes();

    uint32_t hashToPage(uint32_t bits);
private:
    Pager *dataPager;
    Pager *overflowPager;
    LinHashHeader header;

    void split();
    void merge();
    void redistribute();
    void contract();
    void freeEmptyOverflow();
    uint32_t getLower(uint32_t bits, uint32_t num);
};