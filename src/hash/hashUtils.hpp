#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>

#include "tuple.hpp"

class LinHashManager {
public:
    uint32_t hashTuple(LinHashManager *linHashManager, Tuple *tuple);
    bool checkHashMatch(uint32_t numAttrs, Tuple pattern, Tuple tupleToCheck);
private:
    uint32_t hash_any(char *value, uint32_t len);
    bool checkHashMatchRec(Tuple pattern, Tuple tupleToCheck)
};