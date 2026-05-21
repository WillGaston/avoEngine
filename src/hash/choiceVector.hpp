#include <vector>
#include <memory>
#include <string>

using namespace std;

class ChoiceVector {
    public:
        ChoiceVector(string chvec);
        uint32_t getAttribute(int index);
        uint32_t getBit(int index);
    private:
        void parseChoiceVector(string chvec, uint32_t numAttrs);
        vector<uint32_t> attributes;
        vector<uint32_t> bits;
}