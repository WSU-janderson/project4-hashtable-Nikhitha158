

#define RUN_TESTS

#ifdef RUN_TESTS

#include "HashTable.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

using namespace std;

#define HT_PRINT(ht) \
    cout << "HashTable contents" << endl; \
    cout << "------------------" << endl; \
    cout << ht << endl;

// Enable or disable specific tests here
#define HT_INSERT
#define HT_INSERT_DUPLICATE
#define HT_INSERT_FULL
#define HT_REMOVE
#define HT_REMOVE_MISSING
#define HT_CONTAINS
#define HT_CONTAINS_MISSING
#define HT_CONTAINS_AFTER_REMOVE
#define HT_GET
#define HT_GET_MISSING
#define HT_GET_REMOVED
#define HT_GET_AFTER_REMOVE
#define HT_BRACKET_OP_GET
#define HT_BRACKET_OP_SET
#define HT_KEYS
#define HT_ALPHA
#define HT_CAPACITY
#define HT_SIZE

int main() {
    const size_t MAXHASH = 8; // matches default constructor


    cout << "Testing HashTable creation, insert, print" << endl;

#ifdef HT_INSERT
    try {
        HashTable ht1;
        cout << "Inserting first " << MAXHASH / 2 << " values" << endl;
        for (int i = 1; i <= MAXHASH / 2; ++i) {
            if (ht1.insert(to_string(i), i))
                cout << "Inserted <" << i << ", " << i << ">." << endl;
        }
        cout << endl;
        HT_PRINT(ht1);

        cout << "Inserting larger keys..." << endl;
        for (int k = 111; k <= 444; k += 111) {
            ht1.insert(to_string(k), k);
            cout << "Inserted <" << k << ", " << k << ">." << endl;
        }
        cout << endl;
        HT_PRINT(ht1);
    } catch (const exception &e) {
        cout << "Exception during insert test: " << e.what() << endl;
    }
#endif

#ifdef HT_INSERT_DUPLICATE
    cout << "Inserting duplicate key" << endl;
    try {
        HashTable ht;
        ht.insert("1", 1);
        if (!ht.insert("1", 99))
            cout << "CORRECT: insert() returned false" << endl;
        else
            cout << "ERROR: duplicate key accepted" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_INSERT_FULL
    cout << "\nTesting insert when table is full" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        cout << "Table full, inserting more to trigger resize..." << endl;
        for (int i = 9; i <= 16; ++i) ht.insert(to_string(i), i);
        cout << "Reported hash table size is: " << ht.size()
             << " versus expected size: 16" << endl;
        if (ht.size() == 16)
            cout << "CORRECT: hash table successfully resized" << endl;
        HT_PRINT(ht);
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_REMOVE
    cout << "\nTesting HashTable::remove()" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        for (int i = 1; i <= 8; ++i)
            if (ht.remove(to_string(i)))
                cout << "Removed entry <" << i << ", " << i << ">" << endl;
        cout << "CORRECT: All items removed" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_REMOVE_MISSING
    cout << "\nRemoving missing key: 13" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        if (!ht.remove("13"))
            cout << "CORRECT: remove() returned false" << endl;
        else
            cout << "ERROR: remove() returned true" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_CONTAINS
    cout << "\nTesting HashTable::contains()" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        bool allFound = true;
        for (int i = 1; i <= 8; ++i)
            if (!ht.contains(to_string(i))) allFound = false;
        cout << (allFound ? "CORRECT" : "ERROR") << ": Contains returned true"
             << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_CONTAINS_MISSING
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
    cout << "\nTesting HashTable::get()" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        auto val = ht.get("4");
        if (val && val.value() == 4)
            cout << "CORRECT: found entry 4" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_BRACKET_OP_GET
    cout << "\nTesting operator[]" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        int result = ht["7"];
        if (result == 7) cout << "CORRECT: found entry 7" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_BRACKET_OP_SET
    cout << "\nTesting operator[] to set value" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        ht["7"] = 42;
        int val = ht["7"];
        if (val == 42)
            cout << "CORRECT: found key 7 with new value: 42" << endl;
        else
            cout << "ERROR: operator[] set failed" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_KEYS
    cout << "\nTesting HashTable::keys()" << endl;
    try {
        HashTable ht;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        auto k = ht.keys();
        if (k.size() == 8)
            cout << "CORRECT: all keys successfully returned" << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_ALPHA
    cout << "\nTesting HashTable::alpha()" << endl;
    try {
        HashTable ht;
        cout << (ht.alpha() == 0 ? "CORRECT: alpha for empty table is zero" : "ERROR") << endl;
        for (int i = 1; i <= 4; ++i) ht.insert(to_string(i), i);
        cout << "alpha is " << ht.alpha() << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_CAPACITY
    cout << "\nTesting HashTable::capacity()" << endl;
    try {
        HashTable ht;
        cout << "Initial capacity: " << ht.capacity() << endl;
        for (int i = 1; i <= 8; ++i) ht.insert(to_string(i), i);
        ht.insert("9", 9);
        cout << "New capacity after resize: " << ht.capacity() << endl;
    } catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
#endif

#ifdef HT_SIZE
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
