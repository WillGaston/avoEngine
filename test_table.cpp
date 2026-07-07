#include "table.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cstdio>

// ─────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────

static int passed = 0;
static int failed = 0;

#define ASSERT_TRUE(expr, msg)                                          \
    do {                                                                 \
        if (!(expr)) {                                                   \
            std::cerr << "  [FAIL] " << msg << "\n";                   \
            ++failed;                                                    \
        } else {                                                         \
            std::cout << "  [PASS] " << msg << "\n";                   \
            ++passed;                                                    \
        }                                                                \
    } while (0)

#define ASSERT_EQ(a, b, msg) ASSERT_TRUE((a) == (b), msg)

// Convert a std::string to bytes for easy test payloads
static std::vector<uint8_t> toBytes(const std::string& s) {
    return std::vector<uint8_t>(s.begin(), s.end());
}

// Convert bytes back to string for readable assertions
static std::string fromBytes(const std::vector<uint8_t>& v) {
    return std::string(v.begin(), v.end());
}

// ─────────────────────────────────────────────
// Test cases
// ─────────────────────────────────────────────

void test_insert_and_find(Table& t) {
    std::cout << "\n--- Insert & Find ---\n";

    // Insert a few rows
    bool ok1 = t.insert(1, toBytes("hello"));
    bool ok2 = t.insert(2, toBytes("world"));
    bool ok3 = t.insert(42, toBytes("the answer"));

    ASSERT_TRUE(ok1, "insert key=1 succeeds");
    ASSERT_TRUE(ok2, "insert key=2 succeeds");
    ASSERT_TRUE(ok3, "insert key=42 succeeds");

    ASSERT_EQ(t.getRowCount(), 3, "row count is 3 after three inserts");

    // Retrieve and verify
    auto v1 = t.find(1);
    auto v2 = t.find(2);
    auto v3 = t.find(42);

    ASSERT_EQ(fromBytes(v1), "hello",      "find key=1 returns 'hello'");
    ASSERT_EQ(fromBytes(v2), "world",      "find key=2 returns 'world'");
    ASSERT_EQ(fromBytes(v3), "the answer", "find key=42 returns 'the answer'");

    // Find a key that doesn't exist — should return empty vector
    auto missing = t.find(999);
    ASSERT_TRUE(missing.empty(), "find missing key returns empty vector");
}

void test_remove(Table& t) {
    std::cout << "\n--- Remove ---\n";

    // Keys 1, 2, 42 should still be present from previous test
    bool removed = t.remove(2);
    ASSERT_TRUE(removed, "remove key=2 succeeds");
    ASSERT_EQ(t.getRowCount(), 2, "row count is 2 after one removal");

    // Key 2 should now be gone
    auto gone = t.find(2);
    ASSERT_TRUE(gone.empty(), "find removed key=2 returns empty vector");

    // Keys 1 and 42 should be unaffected
    auto v1 = t.find(1);
    auto v3 = t.find(42);
    ASSERT_EQ(fromBytes(v1), "hello",      "key=1 still readable after removing key=2");
    ASSERT_EQ(fromBytes(v3), "the answer", "key=42 still readable after removing key=2");

    // Removing an already-removed key should return false
    bool double_remove = t.remove(2);
    ASSERT_TRUE(!double_remove, "removing already-removed key=2 returns false");

    // Remove remaining keys
    ASSERT_TRUE(t.remove(1),  "remove key=1 succeeds");
    ASSERT_TRUE(t.remove(42), "remove key=42 succeeds");
    ASSERT_EQ(t.getRowCount(), 0, "row count is 0 after removing all keys");
}

// ─────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────

int main() {
    const std::string TEST_DB = "test_engine.db";

    // Clean up any leftover file from a previous run
    std::remove(TEST_DB.c_str());

    std::cout << "=== DB Engine Smoke Test ===\n";

    try {
        Table t(TEST_DB);

        test_insert_and_find(t);
        test_remove(t);

        // t.close() is called by destructor
    } catch (const std::exception& e) {
        std::cerr << "\n[FATAL] Uncaught exception: " << e.what() << "\n";
        ++failed;
    }

    std::cout << "\n=== Results: " << passed << " passed, " << failed << " failed ===\n";

    // Clean up test DB
    std::remove(TEST_DB.c_str());

    return (failed == 0) ? 0 : 1;
}
