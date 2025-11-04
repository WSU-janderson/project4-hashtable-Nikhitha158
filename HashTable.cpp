/* Name : Nikhitha Palakurla
Project-4 : Map ADT : Hash Table
This project implements a Hash Table data structure in C++ that functions as a Map ADT (Dictionary)
we will use open addressing with pseudo-random probing for collision resolution.
The hash table stores key-value pairs where keys are strings and values are integers.
 */

#include "HashTable.h"
#include <algorithm>  // For std::shuffle
#include <random>     // For random number generation
#include <stdexcept>  // For exception handling

// Initialize the static constant which is required for static class members
const size_t HashTable::DEFAULT_INITIAL_CAPACITY;

// HASHTABLEBUCKET CLASS IMPLEMENTATION
/*
Default constructor - creates an empty bucket marked as Empty Since Start
This is the initial state for all buckets when the hash table is created
 */
HashTableBucket::HashTableBucket() : key(""), value(0), type(BucketType::ESS) {}

/*
 Parameterized constructor - creates a bucket with specific key-value pair
 and marks it as Normal (actively storing data)
 */
HashTableBucket::HashTableBucket(string key, int value)
    : key(key), value(value), type(BucketType::NORMAL) {}

/*
Load a new key-value pair into this bucket
Sets the bucket state to Normal
 */
void HashTableBucket::load(string newKey, int newValue) {
    key = newKey;           // Store the new key
    value = newValue;       // Store the new value
    type = BucketType::NORMAL; // Mark as actively storing data
}

/*
Clear the bucket - removes key-value pair and marks as Empty After Remove
This allows the space to be reused for future insertions
 */
void HashTableBucket::clear() {
    key = "";               // Clear the key
    value = 0;              // Clear the value
    type = BucketType::EAR; // Mark as empty but previously used
}

/*
 Check if bucket is empty (either ESS or EAR state)
 Used during insertion to find available slots
 */
bool HashTableBucket::isEmpty() const {
    return type == BucketType::ESS || type == BucketType::EAR;
}

/*
Check if bucket has never been used (Empty Since Start)
if we hit ESS during probing, key doesn't exist
 */
bool HashTableBucket::isEmptySinceStart() const {
    return type == BucketType::ESS;
}

/*Check if bucket had data that was removed (Empty After Remove).
 Can be reused for new insertions
 */
bool HashTableBucket::isEmptyAfterRemove() const {
    return type == BucketType::EAR;
}

//Check if bucket has valid data (Normal state)
bool HashTableBucket::isNormal() const {
    return type == BucketType::NORMAL;
}

// GETTER METHODS - Provide read-only access to private members
//Get the key stored in this bucket

string HashTableBucket::getKey() const {
    return key;
}

// Get the value stored in this bucket

int HashTableBucket::getValue() const {
    return value;
}

// Get the current state of this bucket

BucketType HashTableBucket::getType() const {
    return type;
}

/* Update the value in this bucket (key remains the same)
 Used by operator[] for assignment operations
 */
void HashTableBucket::setValue(int newValue) {
    value = newValue;
}

/* Output operator for HashTableBucket - prints key-value pair if bucket has data
Only prints buckets in Normal state, ignores empty buckets
 */
ostream& operator<<(ostream& os, const HashTableBucket& bucket) {
    if (bucket.isNormal()) {
        os << "<" << bucket.key << ", " << bucket.value << ">";
    }
    return os;
}
// HASHTABLE CLASS IMPLEMENTATION

/*
Constructor - initializes hash table with specified capacity
initCapacity: initial number of buckets (defaults to 8)
Creates empty buckets and generates probing sequence
 */
HashTable::HashTable(size_t initCapacity) : numItems(0) {
    tableData.resize(initCapacity);  // Create vector with specified capacity
    generateOffsets(initCapacity);   // Generate pseudo-random probing sequence
}

/*Hash function - converts string key to array index
using Multiplicative String Hashing : it distributes strings uniformly across hash table buckets
Minimizes collisions for better O(1) performance and
Handles string keys effectively by considering character order
Since Prime number reduces pattern-based collisions and avoids arithmetic overflow issues
- 31 = 2⁵ - 1: allows compiler optimization to (hash << 5)
hash = (hash * multiplier) + char_code
Each character influences the entire hash value and Maintains dependency on character sequence
Final modulo operation maps to table size
 */
size_t HashTable::hashFunction(const string& key) const {
    size_t hash = 0;  // Start with initial hash value of 0

    // Process each character in the key
    for (char c : key) {
        // 31 is a common prime number used in hash functions
        hash = hash * 31 + c;
    }

    // Use modulo to ensure index fits within table bounds
    return hash % tableData.size();
}

/*Generate pseudo-random probing sequence for collision resolution
Creates a shuffled sequence of numbers 1 through size-1
This sequence determines the order we check buckets during probing
 */
void HashTable::generateOffsets(size_t size) {
    offsets.clear();  // Clear any existing offsets

    // Create sequence from 1 to size-1
    for (size_t i = 1; i < size; i++) {
        offsets.push_back(i);
    }

    // Shuffle using rand()
    for (int i = offsets.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(offsets[i], offsets[j]);
    }
}

/**
 * Check if table needs resizing and resize if necessary
 * Resizes when load factor reaches 0.5 (50% full)
 * Doubles the table capacity and rehashes all existing elements
 */
void HashTable::resizeIfNeeded() {
    // Check if current load factor exceeds threshold
    if (alpha() >= 0.5) {
        // Store reference to old table data before resizing
        vector<HashTableBucket> oldTable = tableData;

        // Calculate new capacity (double the current size)
        size_t newCapacity = tableData.size() * 2;

        // Clear current table and resize to new capacity
        // This creates a new empty table with double the buckets
        tableData.clear();
        tableData.resize(newCapacity);
        numItems = 0;  // Reset item count

        // Generate new probing sequence for larger table
        generateOffsets(newCapacity);

        // Reinsert all items from old table into new larger table
        // This is necessary because hash indices change with new table size
        for (const HashTableBucket& bucket : oldTable) {
            // Only reinsert buckets that have valid data
            if (bucket.isNormal()) {
                insert(bucket.getKey(), bucket.getValue());
            }
        }
    }
}

/* Helper function to find the array index of a given key
Uses pseudo-random probing to handle collisions
 */
size_t HashTable::findKeyIndex(const string& key) const {
    // Calculate  position using hash function
    size_t home = hashFunction(key);
    size_t currentIndex = home;

    // Check home position first
    if (tableData[home].isNormal() && tableData[home].getKey() == key) {
        return home;  // Key found at home position
    }

    // If not at home, follow probing sequence to search other buckets
    for (size_t i = 0; i < offsets.size(); i++) {
        // Calculate next probe position using offset sequence
        currentIndex = (home + offsets[i]) % tableData.size();

        // If we find a never-used bucket (ESS), stop searching
        // This means the key cannot exist beyond this point
        if (tableData[currentIndex].isEmptySinceStart()) {
            return tableData.size();  // Return "not found" indicator
        }

        // Check if current bucket has our key
        if (tableData[currentIndex].isNormal() && tableData[currentIndex].getKey() == key) {
            return currentIndex;  // Key found at probe position
        }
    }

    return tableData.size();  // Key not found after exhaustive search
}

/*Insert a key-value pair into the hash table
@return: true if inserted successfully, false if key already exists
 */
bool HashTable::insert(string key, int value) {
    // First check if we need to resize the table
    resizeIfNeeded();

    // Check for duplicate key
    if (contains(key)) {
        return false;  // Insertion failed - key already exists
    }

    // Calculate home position for the key
    size_t home = hashFunction(key);
    size_t currentIndex = home;

    // Try home position first
    if (tableData[home].isEmpty()) {
        tableData[home].load(key, value);  // Insert at home position
        numItems++;  // Increase count of stored items
        return true;  // Successfully inserted
    }

    // If home position occupied, use probing sequence to find empty slot
    for (size_t i = 0; i < offsets.size(); i++) {
        // Calculate next position in probe sequence
        currentIndex = (home + offsets[i]) % tableData.size();

        // Check if this bucket is empty (can be ESS or EAR)
        if (tableData[currentIndex].isEmpty()) {
            tableData[currentIndex].load(key, value);  // Insert at probe position
            numItems++;  // Increase count of stored items
            return true;  // Successfully inserted
        }
    }
    return false;
}

/*Remove a key-value pair from the hash table
@return: true if removed successfully, false if key not found
 */
bool HashTable::remove(string key) {
    // Find the index of the key using our search helper
    size_t index = findKeyIndex(key);

    // If key was found
    if (index < tableData.size()) {
        tableData[index].clear();  // Mark bucket as Empty After Remove
        numItems--;  // Decrease count of stored items
        return true;  // Successfully removed
    }

    return false;  // Key not found
}

//Check if a key exists in the hash table

bool HashTable::contains(const string& key) const {
    // Uses findKeyIndex helper - if it returns valid index, key exists
    return findKeyIndex(key) < tableData.size();
}

//Get the value associated with a key

optional<int> HashTable::get(const string& key) const {
    // Find the index of the key
    size_t index = findKeyIndex(key);

    // If key was found, return its value wrapped in optional
    if (index < tableData.size()) {
        return tableData[index].getValue();
    }
    // Key not found - return empty optional
    return nullopt;
}

//Array-style access operator - allows both reading and writing values

int& HashTable::operator[](const string& key) {
    // If key doesn't exist, insert it with default value 0
    // This makes the operator more robust than required
    if (!contains(key)) {
        insert(key, 0);  // Auto-insert missing key with value 0
    }

    // Find the key's index
    size_t index = findKeyIndex(key);

    // Return reference to the value for both reading and modification
    if (index < tableData.size()) {
        return tableData[index].getValueRef();
    }

    static int dummy;
    dummy = 0;
    return dummy;
}

/*Get all keys currently stored in the hash table
Useful for iteration and debugging
 */
vector<string> HashTable::keys() const {
    vector<string> keyList;  // Create empty vector to store keys

    // Iterate through all buckets in the table
    for (size_t i = 0; i < tableData.size(); i++) {
        // Only add keys from buckets that have valid data
        if (tableData[i].isNormal()) {
            keyList.push_back(tableData[i].getKey());
        }
    }

    return keyList;  // Return complete list of keys
}

/*Calculate current load factor of the hash table
Load factor = number of items / total buckets
 */
double HashTable::alpha() const {
    // Handle edge case of empty table
    if (tableData.size() == 0) return 0.0;

    // Use static_cast to ensure floating-point division
    return static_cast<double>(numItems) / static_cast<double>(tableData.size());
}

//Get total number of buckets in the hash table (capacity)
size_t HashTable::capacity() const {
    return tableData.size();
}

//Get number of key-value pairs currently stored in the table

size_t HashTable::size() const {
    return numItems;
}

/*Output operator for entire hash table - prints all occupied buckets
Only prints buckets that contain data, shows bucket indices
 */
ostream& operator<<(ostream& os, const HashTable& hashTable) {
    bool foundItems = false;  // Track if  found any items to print

    // Iterate through all buckets in the table
    for (size_t i = 0; i < hashTable.tableData.size(); i++) {
        const HashTableBucket& bucket = hashTable.tableData[i];

        // Only print buckets that have valid data
        if (bucket.isNormal()) {
            os << "Bucket " << i << ": <" << bucket.getKey()
               << ", " << bucket.getValue() << ">" << endl;
            foundItems = true;  // Mark found at least one item
        }
    }

    // If no items found, print empty message
    if (!foundItems) {
        os << "Table is empty" << endl;
    }

    return os;
}