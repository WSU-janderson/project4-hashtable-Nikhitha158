#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>       // For std::vector to store the hash table buckets
#include <optional>     // For std::optional for methods that might not return a value
#include <iostream>

using namespace std;

// BUCKET STATE DEFINITIONS
/*
Enum to represent the three possible states of each bucket in the hash table:
NORMAL: Bucket contains a valid key-value pair
ESS: Empty Since Start - bucket has never been used (helps stop probing)
EAR: Empty After Remove - bucket had data that was removed (can be reused)
 */
enum class BucketType { NORMAL, ESS, EAR };

// HASHTABLEBUCKET CLASS - REPRESENTS A SINGLE SLOT IN THE HASH TABLE

class HashTableBucket {
private:
    string key;      // The key stored in this bucket
    int value;       // The value associated with the key
    BucketType type; // Current state of this bucket (NORMAL, ESS, or EAR)

public:
    // CONSTRUCTORS
    HashTableBucket();                          // Default constructor - creates empty bucket
    HashTableBucket(string key, int value);     // Constructor with key-value pair

    // BUCKET OPERATIONS
    void load(string key, int value);           // Load key-value pair and mark as NORMAL
    void clear();                               // Clear bucket and mark as EAR

    // STATE CHECKING METHODS
    bool isEmpty() const;                       // Check if bucket is empty (ESS or EAR)
    bool isEmptySinceStart() const;             // Check if bucket has never been used (ESS)
    bool isEmptyAfterRemove() const;            // Check if bucket was cleared (EAR)
    bool isNormal() const;                      // Check if bucket has valid data (NORMAL)

    // GETTER METHODS
    string getKey() const;                      // Get the key stored in this bucket
    int getValue() const;                       // Get the value stored in this bucket
    BucketType getType() const;                 // Get the current bucket type

    // SETTER METHOD
    void setValue(int newValue);                // Update the value in this bucket

    // REFERENCE ACCESSOR - Needed for operator[] to return modifiable reference
    int& getValueRef() {
        return value;  // Return direct reference to the value for modification
    }

    // FRIEND FUNCTION FOR OUTPUT - Allows printing bucket contents
    friend ostream& operator<<(ostream& os, const HashTableBucket& bucket);
};

// ============================================================================
// HASHTABLE CLASS - MAIN HASH TABLE IMPLEMENTATION USING OPEN ADDRESSING
// ============================================================================

class HashTable {
private:
    // PRIVATE MEMBER VARIABLES
    vector<HashTableBucket> tableData;  // The actual hash table storage (array of buckets)
    vector<size_t> offsets;             // Pseudo-random probing sequence for collision resolution
    size_t numItems;                    // Counter for number of key-value pairs currently stored

    // PRIVATE HELPER METHODS
    size_t hashFunction(const string& key) const;  // Convert key to array index
    void generateOffsets(size_t size);             // Create pseudo-random probing sequence
    void resizeIfNeeded();                         // Check and perform table resizing
    size_t findKeyIndex(const string& key) const;  // Find index of key using probing

public:
    // PUBLIC CONSTANTS
    static const size_t DEFAULT_INITIAL_CAPACITY = 8;  // Default table size

    // CONSTRUCTOR
    HashTable(size_t initCapacity = 8);  // Create hash table with given capacity (default 8)

    //MAP OPERATIONS
    bool insert(string key, int value);           // Insert key-value pair (no duplicates)
    bool remove(string key);                      // Remove key-value pair
    bool contains(const string& key) const;       // Check if key exists
    optional<int> get(const string& key) const;   // Get value for key
    int& operator[](const string& key);           // Array-style access (get/set)

    // UTILITY METHODS
    vector<string> keys() const;  // Get all keys currently in table
    double alpha() const;         // Calculate current load factor
    size_t capacity() const;      // Get total number of buckets
    size_t size() const;          // Get number of key-value pairs

    // FRIEND FUNCTION FOR OUTPUT - Allows printing entire hash table
    friend ostream& operator<<(ostream& os, const HashTable& hashTable);
};

#endif