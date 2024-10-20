#include "../include/segment.hpp"
#include "../include/bucket.hpp"
#include "gtest/gtest.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
using namespace std;
void Segment::Put(string key, string value) {
	try {

		shared_ptr<Bucket> buck = GetBucket(key);
		buck->Put(key, value);
	} catch (const exception& e) {
		cout << "Exeption " << e.what() << endl;
		// insert new one
	}
}

string Segment::Get(string key) {
	shared_ptr<Bucket> buck = GetBucket(key);
	return buck->Get(key);
}

shared_ptr<Bucket> Segment::GetBucket(string key) const {
	size_t hashValue = hash<string>{}(key);

	std::size_t bucketIndex = hashValue % buckets.size();
	cout << "BUCKET INDEX: " << bucketIndex << endl;
	return (buckets[bucketIndex]);
}

Segment::Segment() {
	cout << "LES GOO" << endl;
	buckets.resize(10, make_shared<Bucket>());
}


void Segment::Split() {
    
    cout << "Segment is full, splitting..." << endl;

    // Double the size of the buckets vector
    size_t oldSize = buckets.size();
    buckets.resize(oldSize * 2, make_shared<Bucket>());

    // Redistribute existing keys
    for (size_t i = 0; i < oldSize; ++i) {
        auto& oldBucket = buckets[i];
        for (auto& slot : oldBucket->slots) {
            if (!slot.first.empty()) {
                size_t newIndex = hash<string>{}(slot.first) % buckets.size();
                if (newIndex != i) {
                    buckets[newIndex]->Put(slot.first, slot.second);
                    slot.first.clear(); 
                }
            }
        }
    }
}

//code with b+ tree
// #include "../include/segment.hpp"
// #include <iostream>
// #include <stdexcept>

// Segment::Segment(size_t maxKeys) : bPlusTree(3), maxKeys(maxKeys), currentKeys(0) {
//     // Initialize the segment with an empty B+ tree
// }

// void Segment::Put(string key, string value) {
//     cout << "Segment::Put: Inserting key-value pair (" << key << ", " << value << ")" << endl;
//     bPlusTree.Insert(key, value);
//     currentKeys++;

//     if (IsFull()) {
//         cout << "Segment is full. Triggering split..." << endl;
//         Split();
//     }
// }

// string Segment::Get(string key) {
//     cout << "Segment::Get: Searching for key: " << key << endl;
//     try {
//         return bPlusTree.Search(key);
//     } catch (const std::exception& e) {
//         throw runtime_error("Segment::Get: " + string(e.what()));
//     }
// }

// bool Segment::IsFull() const {
//     return currentKeys >= maxKeys;
// }

// vector<pair<string, string>> Segment::Split() {
//     vector<pair<string, string>> splitPairs = bPlusTree.Split(); // Retrieve half the keys from B+ tree

//     // Reset current segment after the split
//     currentKeys = bPlusTree.GetKeyCount();
    
//     return splitPairs; // Return the split key-value pairs for redistribution
// }

