#include <vector>
#include <memory>
#include <cstring>
#include <stdexcept>
#include <variant>

using namespace std;

#pragma once

enum class FieldType : uint8_t {
    INT32 = 1,
    INT64 = 2,
    FLOAT = 3,
    BOOLEAN = 4,
    VARCHAR = 5
};

using FieldValue = variant<
    int32_t,
    int64_t,
    float,
    bool,
    string,
>;

struct ColumnDef {
    string name;
    FieldType type;
};

class Schema {
public:
    Schema();
    void addColumn(string &name, FieldType type);
    int indexOf(string &name);
private:
    vector<ColumnDef> cols;
};