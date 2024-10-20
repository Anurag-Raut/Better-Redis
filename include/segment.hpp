#pragma once
#include "bucket.hpp"
#include <memory>
#include <string>
using namespace std;
class Segment {
	vector<shared_ptr<Bucket>> buckets;
	shared_ptr<Bucket> GetBucket(string key) const;

  public:
	Segment();
	void Put(string key, string value);
	string Get(string key);
};

//code with b+ tree
// #pragma once

// #include "bplustree.hpp"
// #include <string>
// #include <vector>
// using namespace std;

// class Segment {
//     BPlusTree bPlusTree;
//     size_t maxKeys; // Maximum number of keys before splitting
//     size_t currentKeys; // Current number of keys in the segment

//   public:
//     Segment(size_t maxKeys = 10); // Default threshold for splitting
//     void Put(string key, string value);
//     string Get(string key);
//     bool IsFull() const; // Checks if the segment is full
//     vector<pair<string, string>> Split(); // Split the segment and return new key-value pairs for redistribution
// };
