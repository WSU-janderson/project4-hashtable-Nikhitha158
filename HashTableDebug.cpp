#include <iostream>
#include "HashTable.h"

using namespace std;

int main() {
    cout << "=== Testing HashTable Implementation ===" << endl;

    HashTable table(4);

    cout << "\n1. Testing Insert Operations:" << endl;
    cout << "Insert 'Alice': " << (table.insert("Alice", 1001) ? "Success" : "Failed") << endl;
    cout << "Insert 'Bob': " << (table.insert("Bob", 1002) ? "Success" : "Failed") << endl;
    cout << "Insert 'Charlie': " << (table.insert("Charlie", 1003) ? "Success" : "Failed") << endl;
    cout << "Insert 'Alice' again: " << (table.insert("Alice", 9999) ? "Success" : "Failed") << endl;

    cout << "\n2. Testing Contains Operations:" << endl;
    cout << "Contains 'Alice': " << (table.contains("Alice") ? "Yes" : "No") << endl;
    cout << "Contains 'David': " << (table.contains("David") ? "Yes" : "No") << endl;

    cout << "\n3. Testing Get Operations:" << endl;
    auto bobValue = table.get("Bob");
    if (bobValue) {
        cout << "Value for 'Bob': " << *bobValue << endl;
    }

    cout << "\n4. Testing Size and Capacity:" << endl;
    cout << "Current size: " << table.size() << endl;
    cout << "Table capacity: " << table.capacity() << endl;
    cout << "Load factor: " << table.alpha() << endl;

    cout << "\n5. Testing Remove Operation:" << endl;
    cout << "Remove 'Bob': " << (table.remove("Bob") ? "Success" : "Failed") << endl;
    cout << "Contains 'Bob' after removal: " << (table.contains("Bob") ? "Yes" : "No") << endl;
    cout << "New size: " << table.size() << endl;

    cout << "\n6. Testing Array Operator []:" << endl;
    cout << "table['Alice'] = " << table["Alice"] << endl;
    table["Alice"] = 9999;
    cout << "After table['Alice'] = 9999: " << table["Alice"] << endl;

    cout << "\n7. Testing Keys Method:" << endl;
    vector<string> allKeys = table.keys();
    cout << "All keys in table: ";
    for (const string& key : allKeys) {
        cout << key << " ";
    }
    cout << endl;

    cout << "\n8. Final Table State:" << endl;
    cout << table << endl;

    return 0;
}