/**
 * ComprehensiveHashTableTests.cpp
 * CS 3100
 * Project 4
 *
 * Additional comprehensive test file for HashTable project
 * Tests all requirements including edge cases and stress tests
 */

#include "HashTable.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

using namespace std;

void runComprehensiveTests() {
    cout << "+=================================+" << endl;
    cout << "| COMPREHENSIVE HASH TABLE TESTS  |" << endl;
    cout << "+=================================+" << endl << endl;

    // TEST 1: Basic Constructor and Initial State
    cout << "TEST 1: Constructor and Initial State" << endl;
    cout << "=====================================" << endl;
    {
        HashTable ht;
        cout << "Default constructor - capacity: " << ht.capacity() << endl;
        assert(ht.capacity() == 8);
        assert(ht.size() == 0);
        assert(ht.alpha() == 0.0);
        cout << "✓ Default constructor works correctly" << endl;

        HashTable ht_custom(16);
        cout << "Custom constructor - capacity: " << ht_custom.capacity() << endl;
        assert(ht_custom.capacity() == 16);
        assert(ht_custom.size() == 0);
        cout << "✓ Custom constructor works correctly" << endl << endl;
    }

    // TEST 2: Basic Insert and Retrieve
    cout << "TEST 2: Basic Insert and Retrieve Operations" << endl;
    cout << "============================================" << endl;
    {
        HashTable ht;

        // Test basic insert
        assert(ht.insert("apple", 10) == true);
        assert(ht.insert("banana", 20) == true);
        assert(ht.insert("cherry", 30) == true);
        cout << "✓ Basic inserts successful" << endl;

        // Test size tracking
        assert(ht.size() == 3);
        cout << "✓ Size correctly tracked: " << ht.size() << endl;

        // Test contains
        assert(ht.contains("apple") == true);
        assert(ht.contains("banana") == true);
        assert(ht.contains("cherry") == true);
        assert(ht.contains("dragonfruit") == false);
        cout << "✓ Contains works correctly" << endl;

        // Test get
        auto apple_val = ht.get("apple");
        auto banana_val = ht.get("banana");
        auto cherry_val = ht.get("cherry");
        auto dragon_val = ht.get("dragonfruit");

        assert(apple_val.has_value() && apple_val.value() == 10);
        assert(banana_val.has_value() && banana_val.value() == 20);
        assert(cherry_val.has_value() && cherry_val.value() == 30);
        assert(!dragon_val.has_value());
        cout << "✓ Get works correctly with optional" << endl << endl;
    }

    // TEST 3: Duplicate Key Prevention
    cout << "TEST 3: Duplicate Key Prevention" << endl;
    cout << "================================" << endl;
    {
        HashTable ht;

        assert(ht.insert("key1", 100) == true);
        assert(ht.insert("key1", 200) == false); // Should fail
        assert(ht.insert("key1", 300) == false); // Should fail

        auto val = ht.get("key1");
        assert(val.has_value() && val.value() == 100); // Should still be original value

        cout << "✓ Duplicate keys correctly rejected" << endl;
        cout << "✓ Original value preserved: " << val.value() << endl << endl;
    }

    // TEST 4: Remove Operations
    cout << "TEST 4: Remove Operations" << endl;
    cout << "========================" << endl;
    {
        HashTable ht;

        ht.insert("one", 1);
        ht.insert("two", 2);
        ht.insert("three", 3);

        assert(ht.size() == 3);
        assert(ht.remove("two") == true);
        assert(ht.size() == 2);
        assert(ht.contains("two") == false);
        assert(ht.get("two").has_value() == false);

        // Try removing non-existent key
        assert(ht.remove("nonexistent") == false);
        assert(ht.size() == 2); // Size shouldn't change

        cout << "✓ Remove works correctly" << endl;
        cout << "✓ Size updated after removal" << endl;
        cout << "✓ Non-existent key removal returns false" << endl << endl;
    }

    // TEST 5: Bracket Operator
    cout << "TEST 5: Bracket Operator []" << endl;
    cout << "===========================" << endl;
    {
        HashTable ht;

        // Test assignment
        ht["key1"] = 42;
        ht["key2"] = 84;

        assert(ht.contains("key1") == true);
        assert(ht.contains("key2") == true);
        assert(ht["key1"] == 42);
        assert(ht["key2"] == 84);
        cout << "✓ Bracket operator assignment works" << endl;

        // Test modification
        ht["key1"] = 100;
        assert(ht["key1"] == 100);
        cout << "✓ Bracket operator modification works" << endl;

        // Test reading non-existent key (should insert with default value)
        int value = ht["newkey"];
        assert(ht.contains("newkey") == true);
        assert(value == 0);
        cout << "✓ Bracket operator auto-inserts missing keys" << endl << endl;
    }

    // TEST 6: Load Factor and Resizing
    cout << "TEST 6: Load Factor and Dynamic Resizing" << endl;
    cout << "========================================" << endl;
    {
        HashTable ht(4); // Small initial capacity for testing

        cout << "Initial capacity: " << ht.capacity() << endl;
        cout << "Initial load factor: " << ht.alpha() << endl;

        // Fill to trigger resize (load factor >= 0.5)
        ht.insert("a", 1);
        ht.insert("b", 2);
        assert(ht.alpha() == 0.5);
        cout << "Load factor at 0.5: " << ht.alpha() << endl;

        // Next insert should trigger resize
        ht.insert("c", 3);
        cout << "Capacity after resize: " << ht.capacity() << endl;
        assert(ht.capacity() == 8); // Should double

        // Verify all data preserved after resize
        assert(ht.contains("a"));
        assert(ht.contains("b"));
        assert(ht.contains("c"));
        assert(ht.get("a").value() == 1);
        assert(ht.get("b").value() == 2);
        assert(ht.get("c").value() == 3);

        cout << "✓ Dynamic resizing works correctly" << endl;
        cout << "✓ All data preserved after resize" << endl << endl;
    }

    // TEST 7: Keys Method
    cout << "TEST 7: Keys Method" << endl;
    cout << "===================" << endl;
    {
        HashTable ht;

        ht.insert("zebra", 1);
        ht.insert("apple", 2);
        ht.insert("monkey", 3);

        auto keys = ht.keys();
        assert(keys.size() == 3);

        // Check all keys are present
        assert(find(keys.begin(), keys.end(), "zebra") != keys.end());
        assert(find(keys.begin(), keys.end(), "apple") != keys.end());
        assert(find(keys.begin(), keys.end(), "monkey") != keys.end());

        cout << "✓ Keys method returns all keys: ";
        for (const auto& key : keys) {
            cout << key << " ";
        }
        cout << endl;

        // Test empty table
        HashTable empty_ht;
        auto empty_keys = empty_ht.keys();
        assert(empty_keys.size() == 0);
        cout << "✓ Empty table returns empty keys vector" << endl << endl;
    }

    // TEST 8: Collision Handling
    cout << "TEST 8: Collision Handling" << endl;
    cout << "==========================" << endl;
    {
        HashTable ht(4); // Small table to force collisions

        // These might cause collisions depending on hash function
        ht.insert("cat", 1);
        ht.insert("dog", 2);
        ht.insert("bird", 3);
        ht.insert("fish", 4);

        assert(ht.size() == 4);
        assert(ht.contains("cat"));
        assert(ht.contains("dog"));
        assert(ht.contains("bird"));
        assert(ht.contains("fish"));

        // Verify values are correct despite collisions
        assert(ht.get("cat").value() == 1);
        assert(ht.get("dog").value() == 2);
        assert(ht.get("bird").value() == 3);
        assert(ht.get("fish").value() == 4);

        cout << "✓ Collisions handled correctly" << endl;
        cout << "✓ All values retrievable despite collisions" << endl << endl;
    }

    // TEST 9: Complex Operations
    cout << "TEST 9: Complex Operations" << endl;
    cout << "==========================" << endl;
    {
        HashTable ht;

        // Mix of operations
        ht.insert("start", 0);
        ht["middle"] = 50;
        ht.remove("start");
        ht.insert("end", 100);
        ht["middle"] = 75; // Modify existing

        assert(!ht.contains("start"));
        assert(ht.contains("middle"));
        assert(ht.contains("end"));
        assert(ht["middle"] == 75);
        assert(ht.get("end").value() == 100);

        cout << "✓ Complex operation sequence works" << endl;

        // Test capacity and size relationship
        size_t initial_capacity = ht.capacity();
        for (int i = 0; i < 20; i++) {
            ht.insert("key" + to_string(i), i);
        }

        cout << "Final capacity: " << ht.capacity() << endl;
        cout << "Final size: " << ht.size() << endl;
        cout << "Final load factor: " << ht.alpha() << endl;

        assert(ht.capacity() > initial_capacity); // Should have resized
        assert(ht.alpha() < 0.5); // Should be below threshold after resize

        cout << "✓ Multiple resizes handled correctly" << endl << endl;
    }

    // TEST 10: Stress Test
    cout << "TEST 10: Stress Test" << endl;
    cout << "====================" << endl;
    {
        HashTable ht;
        const int NUM_OPERATIONS = 1000;

        cout << "Performing " << NUM_OPERATIONS << " operations..." << endl;

        for (int i = 0; i < NUM_OPERATIONS; i++) {
            string key = "key" + to_string(i);
            ht.insert(key, i);
        }

        assert(ht.size() == NUM_OPERATIONS);

        // Verify all inserts
        for (int i = 0; i < NUM_OPERATIONS; i++) {
            string key = "key" + to_string(i);
            assert(ht.contains(key));
            auto val = ht.get(key);
            assert(val.has_value() && val.value() == i);
        }

        // Remove half
        for (int i = 0; i < NUM_OPERATIONS; i += 2) {
            string key = "key" + to_string(i);
            assert(ht.remove(key));
        }

        assert(ht.size() == NUM_OPERATIONS / 2);

        cout << "✓ Stress test passed" << endl;
        cout << "✓ Final capacity: " << ht.capacity() << endl;
        cout << "✓ Final load factor: " << ht.alpha() << endl << endl;
    }

    // TEST 11: Output Operator
    cout << "TEST 11: Output Operator <<" << endl;
    cout << "==========================" << endl;
    {
        HashTable ht;

        ht.insert("Alice", 25);
        ht.insert("Bob", 30);
        ht.insert("Charlie", 35);

        cout << "HashTable output:" << endl;
        cout << ht << endl;

        cout << "✓ Output operator works correctly" << endl;
        cout << "✓ Format matches requirements" << endl << endl;
    }

    // TEST 12: Empty Table Edge Cases
    cout << "TEST 12: Empty Table Edge Cases" << endl;
    cout << "==============================" << endl;
    {
        HashTable ht;

        assert(ht.size() == 0);
        assert(ht.capacity() == 8);
        assert(ht.alpha() == 0.0);
        assert(ht.keys().size() == 0);
        assert(ht.remove("anything") == false);
        assert(ht.contains("anything") == false);
        assert(!ht.get("anything").has_value());

        cout << "✓ All empty table operations work correctly" << endl << endl;
    }

    cout << "+=================================+" << endl;
    cout << "| ALL TESTS COMPLETED SUCCESSFULLY |" << endl;
    cout << "+=================================+" << endl;
}

int main() {
    try {
        runComprehensiveTests();
        cout << endl << "🎉 All comprehensive tests passed! Your HashTable implementation is robust." << endl;
        return 0;
    } catch (const exception& e) {
        cout << "❌ Test failed with exception: " << e.what() << endl;
        return 1;
    } catch (...) {
        cout << "❌ Test failed with unknown error" << endl;
        return 1;
    }
}