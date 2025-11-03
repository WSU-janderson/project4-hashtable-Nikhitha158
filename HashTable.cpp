#include "HashTable.h"
#include <algorithm>
#include <random>
#include <stdexcept>

// Initialize the static constant
const size_t HashTable::DEFAULT_INITIAL_CAPACITY;

// ============================================================================
// HashTableBucket CLASS IMPLEMENTATION
// ============================================================================

HashTableBucket::HashTableBucket() : key(""), value(0), type(BucketType::ESS) {}

HashTableBucket::HashTableBucket(string key, size_t value)
    : key(key), value(value), type(BucketType::NORMAL) {}

void HashTableBucket::load(string newKey, size_t newValue) {
    key = newKey;
    value = newValue;
    type = BucketType::NORMAL;
}

void HashTableBucket::clear() {
    key = "";
    value = 0;
    type = BucketType::EAR;
}

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

string HashTableBucket::getKey() const {
    return key;
}

size_t HashTableBucket::getValue() const {
    return value;
}

BucketType HashTableBucket::getType() const {
    return type;
}

void HashTableBucket::setValue(size_t newValue) {
    value = newValue;
}

ostream& operator<<(ostream& os, const HashTableBucket& bucket) {
    if (bucket.isNormal()) {
        os << "<" << bucket.key << ", " << bucket.value << ">";
    }
    return os;
}

// ============================================================================
// HashTable CLASS IMPLEMENTATION
// ============================================================================

HashTable::HashTable(size_t initCapacity) : numItems(0) {
    tableData.resize(initCapacity);
    generateOffsets(initCapacity);
}

size_t HashTable::hashFunction(const string& key) const {
    size_t hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    return hash % tableData.size();
}

void HashTable::generateOffsets(size_t size) {
    offsets.clear();
    for (size_t i = 1; i < size; i++) {
        offsets.push_back(i);
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(offsets.begin(), offsets.end(), g);
}

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

        if (tableData[currentIndex].isEmptySinceStart()) {
            return tableData.size(); // Not found
        }

        if (tableData[currentIndex].isNormal() && tableData[currentIndex].getKey() == key) {
            return currentIndex;
        }
    }

    return tableData.size(); // Not found
}

bool HashTable::insert(string key, size_t value) {
    resizeIfNeeded();

    // Check for duplicate key
    if (contains(key)) {
        return false;
    }

    size_t home = hashFunction(key);
    size_t currentIndex = home;

    // Try home position first
    if (tableData[home].isEmpty()) {
        tableData[home].load(key, value);
        numItems++;
        return true;
    }

    // Use probing sequence to find empty slot
    for (size_t i = 0; i < offsets.size(); i++) {
        currentIndex = (home + offsets[i]) % tableData.size();

        if (tableData[currentIndex].isEmpty()) {
            tableData[currentIndex].load(key, value);
            numItems++;
            return true;
        }
    }

    return false;
}

bool HashTable::remove(string key) {
    size_t index = findKeyIndex(key);
    if (index < tableData.size()) {
        tableData[index].clear();
        numItems--;
        return true;
    }
    return false;
}

bool HashTable::contains(const string& key) const {
    return findKeyIndex(key) < tableData.size();
}

optional<size_t> HashTable::get(const string& key) const {
    size_t index = findKeyIndex(key);
    if (index < tableData.size()) {
        return tableData[index].getValue();
    }
    return nullopt;
}

// Fixed operator[] implementation
size_t& HashTable::operator[](const string& key) {
    // First, ensure the key exists (insert if it doesn't)
    if (!contains(key)) {
        insert(key, 0);
    }

    // Find the key's index
    size_t index = findKeyIndex(key);

    if (index < tableData.size()) {
        // Return a direct reference to the value in the bucket
        return tableData[index].getValueRef();
    }

    // Fallback - should never reach here if insert worked
    static size_t dummy;
    dummy = 0;
    return dummy;
}

vector<string> HashTable::keys() const {
    vector<string> keyList;
    for (size_t i = 0; i < tableData.size(); i++) {
        if (tableData[i].isNormal()) {
            keyList.push_back(tableData[i].getKey());
        }
    }
    return keyList;
}

double HashTable::alpha() const {
    if (tableData.size() == 0) return 0.0;
    return static_cast<double>(numItems) / static_cast<double>(tableData.size());
}

size_t HashTable::capacity() const {
    return tableData.size();
}

size_t HashTable::size() const {
    return numItems;
}

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