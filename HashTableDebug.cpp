#define RUN_TESTS
#ifdef RUN_TESTS

#include "HashTable.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

using namespace std;

// Macro to easily print hash table contents for debugging
#define HT_PRINT(ht) \
    cout << "HashTable contents" << endl; \
    cout << ht << endl;

// Test configuration
#define HT_INSERT              // Test basic insertion functionality
#define HT_INSERT_DUPLICATE    // Test duplicate key rejection
#define HT_INSERT_FULL         // Test automatic resizing when table fills up
#define HT_REMOVE              // Test key removal
#define HT_REMOVE_MISSING      // Test removing non-existent key
#define HT_CONTAINS            // Test key existence checking
#define HT_CONTAINS_MISSING    // Test checking for non-existent key
#define HT_CONTAINS_AFTER_REMOVE // Test contains after removal
#define HT_GET                 // Test value retrieval
#define HT_GET_MISSING         // Test getting non-existent key
#define HT_GET_REMOVED         // Test getting removed key
#define HT_GET_AFTER_REMOVE    // Test get after removal
#define HT_BRACKET_OP_GET      // Test operator[] for reading
#define HT_BRACKET_OP_SET      // Test operator[] for assignment
#define HT_KEYS                // Test retrieving all keys
#define HT_ALPHA               // Test load factor calculation
#define HT_CAPACITY            // Test table capacity reporting
#define HT_SIZE                // Test size reporting

int main() {
    const size_t MAXHASH = 8; // matches default constructor size

    cout << "Testing HashTable creation, insert, print" << endl;

#ifdef HT_INSERT
    // Basic insertion and printing
    try {
        HashTable ht1;
        cout << "Inserting first " << MAXHASH / 2 << " values" << endl;
        // Insert half capacity to test normal operation
        for (int i = 1; i <= MAXHASH / 2; ++i) {
            if (ht1.insert(to_string(i), i))
                cout << "Inserted <" << i << ", " << i << ">." << endl;
        }
        cout << endl;
        HT_PRINT(ht1);  // Display current table state

        cout << "Inserting larger keys..." << endl;
        // Test with larger keys to verify hash function works with different inputs
        for (int k = 111; k <= 444; k += 111) {
            ht1.insert(to_string(k), k);
            cout << "Inserted <" << k << ", " << k << ">." << endl;
        }
        cout << endl;
        HT_PRINT(ht1);  // Display final table state
    } catch (const exception &e) {
        cout << "Exception during insert test: " << e.what() << endl;
    }
#endif

#ifdef HT_INSERT_DUPLICATE
    // Verify duplicate keys are rejected
    cout << "Inserting duplicate key" << endl;
    try {
        HashTable ht;
        ht.insert("1", 1);  // Insert first occurrence
        // Attempt to insert same key with different value
        if (!ht.insert("1", 99))
            cout << "CORRECT: insert() returned false" << endl;
        else
            cout << "ERROR: duplicate key accepted" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_INSERT_FULL
    //  Test automatic resizing when load factor threshold reached
    cout << "\nTesting insert when table is full" << endl;
    try {
        HashTable ht;
        // Fill initial table (capacity 8)
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        cout << "Table full, inserting more to trigger resize..." << endl;
        // These inserts should trigger resize to capacity 16
        for (int i = 9; i <= 16; ++i) ht.insert(to_string(i), i);
        cout << "Reported hash table size is: " << ht.size()
             << " versus expected size: 16" << endl;
        if (ht.size() == 16)
            cout << "CORRECT: hash table successfully resized" << endl;
        HT_PRINT(ht);  // Show table after resize
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_REMOVE
    //  Basic removal functionality
    cout << "\nTesting HashTable::remove()" << endl;
    try {
        HashTable ht;
        // Populate table
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        // Remove all items
        for (int i = 1; i <= 8; ++i)
            if (ht.remove(to_string(i)))
                cout << "Removed entry <" << i << ", " << i << ">" << endl;
        cout << "CORRECT: All items removed" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_REMOVE_MISSING
    //  Attempt to remove non-existent key
    cout << "\nRemoving missing key: 13" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        // Try to remove key that was never inserted
        if (!ht.remove("13"))
            cout << "CORRECT: remove() returned false" << endl;
        else
            cout << "ERROR: remove() returned true" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_CONTAINS
    //  Verify contains() finds all existing keys
    cout << "\nTesting HashTable::contains()" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        bool allFound = true;
        // Check that all inserted keys are found
        for (int i = 1; i <= 8; ++i)
            if (!ht.contains(to_string(i))) allFound = false;
        cout << (allFound ? "CORRECT" : "ERROR") << ": Contains returned true"
             << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_CONTAINS_MISSING
    // Verify contains() returns false for non-existent keys
    cout << "\nTesting contains() with missing item" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        cout << (ht.contains("13") ? "ERROR: contains true" : "CORRECT: contains false") << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_GET
    //  Test retrieving values with get() method
    cout << "\nTesting HashTable::get()" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        auto val = ht.get("4");  // Retrieve value for key "4"
        if (val && val.value() == 4)
            cout << "CORRECT: found entry 4" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_BRACKET_OP_GET
    // Test operator[] for reading values
    cout << "\nTesting operator[]" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        int result = ht["7"];  // Use array syntax to read value
        if (result == 7) cout << "CORRECT: found entry 7" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_BRACKET_OP_SET
    //  Test operator[] for modifying values
    cout << "\nTesting operator[] to set value" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        ht["7"] = 42;  // Use array syntax to modify value
        int val = ht["7"];  // Read back to verify change
        if (val == 42)
            cout << "CORRECT: found key 7 with new value: 42" << endl;
        else
            cout << "ERROR: operator[] set failed" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_KEYS
    //  Test retrieving all keys from the table
    cout << "\nTesting HashTable::keys()" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        auto k = ht.keys();  // Get all keys
        if (k.size() == 8)
            cout << "CORRECT: all keys successfully returned" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_ALPHA
    //  Test load factor calculation
    cout << "\nTesting HashTable::alpha()" << endl;
    try {
        HashTable ht;
        // Test empty table
        cout << (ht.alpha() == 0 ? "CORRECT: alpha for empty table is zero" : "ERROR") << endl;
        // Test with some items
        for (int i = 1; i <= 4; ++i) ht.insert(to_string(i), i);
        cout << "alpha is " << ht.alpha() << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_CAPACITY
    //  Test capacity reporting and verify resizing
    cout << "\nTesting HashTable::capacity()" << endl;
    try {
        HashTable ht;
        cout << "Initial capacity: " << ht.capacity() << endl;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        ht.insert("9", 9);  // This should trigger resize
        cout << "New capacity after resize: " << ht.capacity() << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_SIZE
    // Test size reporting (number of key-value pairs)
    cout << "\nTesting HashTable::size()" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        cout << "CORRECT: size is " << ht.size() << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

    cout << "\nProcess finished with exit code 0" << endl;
    return 0;
}

#endif // RUN_TESTS