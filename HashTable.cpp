#include "HashTable.h"
#include <algorithm>
#include <random>
#include <stdexcept>

// Initialize the static constant - ADDED BACK
const size_t HashTable::DEFAULT_INITIAL_CAPACITY;

// ============================================================================
// HashTableBucket CLASS IMPLEMENTATION
// ============================================================================

// Default constructor - sets bucket type to ESS
HashTableBucket::HashTableBucket() : key(""), value(0), type(BucketType::ESS) {}

// Parameterized constructor - initializes key, value and sets type to NORMAL
HashTableBucket::HashTableBucket(string key, int value)
    : key(key), value(value), type(BucketType::NORMAL) {}

// Load method - loads key-value pair and marks bucket as NORMAL
void HashTableBucket::load(string newKey, int newValue) {
    key = newKey;
    value = newValue;
    type = BucketType::NORMAL;
}

// Clear the bucket - mark as Empty After Remove
void HashTableBucket::clear() {
    key = "";
    value = 0;
    type = BucketType::EAR;
}

// Check if bucket is empty (regardless of ESS/EAR state)
bool HashTableBucket::isEmpty() const {
    return type == BucketType::ESS || type == BucketType::EAR;
}

bool HashTableBucket::isEmptySinceStart() const {
    return type == BucketType::ESS;
}

bool HashTableBucket::isEmptyAfterRemove() const {
    return type == BucketType::EAR;
}

bool HashTableBucket::isNormal() const {
    return type == BucketType::NORMAL;
}

// Getters
string HashTableBucket::getKey() const {
    return key;
}

int HashTableBucket::getValue() const {
    return value;
}

BucketType HashTableBucket::getType() const {
    return type;
}

// Setter
void HashTableBucket::setValue(int newValue) {
    value = newValue;
}

// Stream insertion operator for HashTableBucket
ostream& operator<<(ostream& os, const HashTableBucket& bucket) {
    if (bucket.isNormal()) {
        os << "<" << bucket.key << ", " << bucket.value << ">";
    }
    return os;
}

// ============================================================================
// HashTable CLASS IMPLEMENTATION
// ============================================================================

// Constructor - takes initial capacity, defaults to 8
HashTable::HashTable(size_t initCapacity) : numItems(0) {
    tableData.resize(initCapacity);
    generateOffsets(initCapacity);
}

// Hash function
size_t HashTable::hashFunction(const string& key) const {
    size_t hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    return hash % tableData.size();
}

// Generate pseudo-random probing offsets
void HashTable::generateOffsets(size_t size) {
    offsets.clear();
    for (size_t i = 1; i < size; i++) {
        offsets.push_back(i);
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(offsets.begin(), offsets.end(), g);
}

// Resize table if load factor reaches threshold
void HashTable::resizeIfNeeded() {
    if (alpha() >= 0.5) {
        vector<HashTableBucket> oldTable = tableData;
        size_t newCapacity = tableData.size() * 2;
        tableData.clear();
        tableData.resize(newCapacity);
        numItems = 0;
        generateOffsets(newCapacity);

        for (const HashTableBucket& bucket : oldTable) {
            if (bucket.isNormal()) {
                insert(bucket.getKey(), bucket.getValue());
            }
        }
    }
}

// Helper function to find the index of a key
size_t HashTable::findKeyIndex(const string& key) const {
    size_t home = hashFunction(key);
    size_t currentIndex = home;

    // Check home position first
    if (tableData[home].isNormal() && tableData[home].getKey() == key) {
        return home;
    }

    // Search through probe sequence
    for (size_t i = 0; i < offsets.size(); i++) {
        currentIndex = (home + offsets[i]) % tableData.size();

        // Stop if we find a never-used bucket
        if (tableData[currentIndex].isEmptySinceStart()) {
            return tableData.size(); // Not found
        }

        if (tableData[currentIndex].isNormal() && tableData[currentIndex].getKey() == key) {
            return currentIndex;
        }
    }

    return tableData.size(); // Not found
}

// Insert key-value pair - returns true if successful, false for duplicates
bool HashTable::insert(string key, int value) {
    resizeIfNeeded();

    // Check for duplicate key (no duplicates allowed)
    if (contains(key)) {
        return false; // Insertion failed - duplicate key
    }

    size_t home = hashFunction(key);
    size_t currentIndex = home;

    // Try home position first
    if (tableData[home].isEmpty()) {
        tableData[home].load(key, value);
        numItems++;
        return true; // Successfully inserted
    }

    // Use probing sequence to find empty slot
    for (size_t i = 0; i < offsets.size(); i++) {
        currentIndex = (home + offsets[i]) % tableData.size();

        if (tableData[currentIndex].isEmpty()) {
            tableData[currentIndex].load(key, value);
            numItems++;
            return true; // Successfully inserted
        }
    }

    return false; // Table is full (shouldn't happen with proper resizing)
}

// Remove key-value pair - marks bucket as empty-after-remove
bool HashTable::remove(string key) {
    size_t index = findKeyIndex(key);
    if (index < tableData.size()) {
        tableData[index].clear(); // Mark as EAR
        numItems--;
        return true; // Successfully removed
    }
    return false; // Key not found
}

// Check if key exists in table
bool HashTable::contains(const string& key) const {
    return findKeyIndex(key) < tableData.size();
}

// Get value associated with key - returns optional<int>
optional<int> HashTable::get(const string& key) const {
    size_t index = findKeyIndex(key);
    if (index < tableData.size()) {
        return tableData[index].getValue(); // Returns int
    }
    return nullopt; // Key not found
}

// Bracket operator - returns reference to value, allows assignment
int& HashTable::operator[](const string& key) {
    // For our implementation, we'll insert if key doesn't exist
    // (The requirement says undefined behavior is acceptable for missing keys,
    // but we choose to be more robust)
    if (!contains(key)) {
        insert(key, 0); // Insert with default value 0
    }

    // Find the key's index
    size_t index = findKeyIndex(key);

    if (index < tableData.size()) {
        // Return a direct reference to the value in the bucket
        return tableData[index].getValueRef();
    }

    // Fallback - should never reach here if insert worked
    static int dummy;
    dummy = 0;
    return dummy;
}

// Returns vector with all keys currently in the table
vector<string> HashTable::keys() const {
    vector<string> keyList;
    for (size_t i = 0; i < tableData.size(); i++) {
        if (tableData[i].isNormal()) {
            keyList.push_back(tableData[i].getKey());
        }
    }
    return keyList;
}

// Returns current load factor (size/capacity)
double HashTable::alpha() const {
    if (tableData.size() == 0) return 0.0;
    return static_cast<double>(numItems) / static_cast<double>(tableData.size());
}

// Returns total number of buckets in the table
size_t HashTable::capacity() const {
    return tableData.size();
}

// Returns number of key-value pairs in the table
size_t HashTable::size() const {
    return numItems;
}

// Stream insertion operator for HashTable - only prints occupied buckets
ostream& operator<<(ostream& os, const HashTable& hashTable) {
    bool foundItems = false;
    for (size_t i = 0; i < hashTable.tableData.size(); i++) {
        const HashTableBucket& bucket = hashTable.tableData[i];
        if (bucket.isNormal()) {
            os << "Bucket " << i << ": <" << bucket.getKey()
               << ", " << bucket.getValue() << ">" << endl;
            foundItems = true;
        }
    }
    if (!foundItems) {
        os << "Table is empty" << endl;
    }
    return os;
}