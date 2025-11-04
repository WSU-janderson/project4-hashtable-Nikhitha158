Time Complexity Analysis :
1. insert(key, value)
Time Complexity: O(1) average case, O(n) worst case
Hash computation is O(1) - just iterates through the string once
Probing sequence checks buckets until finding an empty slot
Worst case occurs when all keys hash to same bucket (O(n) probes) or during resizing (O(n) to rehash all elements)

2. remove(key)
Time Complexity: O(1) average case, O(n) worst case
Uses findKeyIndex() which has same complexity as search
Once found, removal is O(1) (just marking bucket as EAR) and Performance depends on hash distribution and probing efficiency

3. contains(key)
Time Complexity: O(1) average case, O(n) worst case
Computes hash in O(1)
Follows probing sequence until key found or ESS bucket encountered
With proper load factor control (α < 0.5), average search length remains constant
Worst case: all elements collide to same probe sequence

4. get(key)
Time Complexity: O(1) average case, O(n) worst case
Identical search process to contains()
5. operator
Time Complexity: O(1) average case, O(n) worst case
First performs search (same as contains()/get())
If key not found, performs insert which is also O(1) average
Worst case involves both unsuccessful search and potential resize
