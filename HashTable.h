#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <optional>
#include <iostream>

using namespace std;

// Enum for bucket states
enum class BucketType { NORMAL, ESS, EAR };

// Bucket class declaration
class HashTableBucket {
private:
    string key;
    size_t value;
    BucketType type;

public:
    // Constructor declarations
    HashTableBucket();
    HashTableBucket(string key, size_t value);

    // Method declarations
    void load(string key, size_t value);
    void clear();
    bool isEmpty() const;
    bool isEmptySinceStart() const;
    bool isEmptyAfterRemove() const;
    bool isNormal() const;
    string getKey() const;
    size_t getValue() const;
    BucketType getType() const;
    void setValue(size_t newValue);

    // Add this method to get a reference to the value
    size_t& getValueRef() {
        return value;
    }

    // Friend function for output
    friend ostream& operator<<(ostream& os, const HashTableBucket& bucket);
};

// Main HashTable class declaration
class HashTable {
private:
    vector<HashTableBucket> tableData;
    vector<size_t> offsets;
    size_t numItems;

    // Private helper method declarations
    size_t hashFunction(const string& key) const;
    void generateOffsets(size_t size);
    void resizeIfNeeded();
    size_t findKeyIndex(const string& key) const;

public:
    // Constant for default capacity
    static const size_t DEFAULT_INITIAL_CAPACITY = 8;

    // Constructor declaration
    HashTable(size_t initCapacity = DEFAULT_INITIAL_CAPACITY);

    // Public method declarations
    bool insert(string key, size_t value);
    bool remove(string key);
    bool contains(const string& key) const;
    optional<size_t> get(const string& key) const;
    size_t& operator[](const string& key);
    vector<string> keys() const;
    double alpha() const;
    size_t capacity() const;
    size_t size() const;

    // Friend function declaration
    friend ostream& operator<<(ostream& os, const HashTable& hashTable);
};

#endif