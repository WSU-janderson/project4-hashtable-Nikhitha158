[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/7RAFczHS)
# Project 4 - HashTable

Project description can be found [in the repo](Project4_HashTable.pdf)
Time Complexity Analysis
1. insert(key, value)
Time Complexity: O(1) average case, O(n) worst case

Justification:

Hash computation is O(1)

Probing sequence checks buckets until finding an empty slot

With good hash distribution and load factor management (resizing at α ≥ 0.5), average probes remain constant

Worst case occurs when all keys hash to same bucket or table needs resizing

2. remove(key)
Time Complexity: O(1) average case, O(n) worst case

Justification:

Uses same probing logic as contains() to find the key

Once found, removal is O(1) (just marking bucket as EAR)

Performance depends on hash distribution and probing efficiency

3. contains(key)
Time Complexity: O(1) average case, O(n) worst case

Justification:

Computes hash in O(1)

Follows probing sequence until key found or ESS bucket encountered

With proper load factor control, average search length remains constant

4. get(key)
Time Complexity: O(1) average case, O(n) worst case

Justification:

Identical search process to contains()

Returns value instead of boolean, but same underlying search algorithm

Hash lookup + probing determines complexity

5. operator[](key)
Time Complexity: O(1) average case, O(n) worst case

Justification:

First performs search (same as contains()/get())

If key not found, performs insert which is also O(1) average

Worst case involves both unsuccessful search and potential resize

Key Factors Affecting Performance:
Load Factor (α): We resize when α ≥ 0.5, maintaining good performance

Hash Function Quality: Our polynomial hash provides decent distribution

Probing Method: Pseudo-random probing reduces clustering compared to linear probing

Resizing Strategy: Doubling capacity ensures amortized O(1) for insertions

Worst Case Scenarios:
All keys hash to the same bucket (O(n) operations)

Poor hash function causing excessive collisions

Table resizing during insertion (O(n) but amortized)

Overall:
With  implementation choices (pseudo-random probing, load factor control, doubling resize), all operations maintain O(1) average case time complexity.



---
