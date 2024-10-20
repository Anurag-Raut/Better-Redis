#include "../include/hasher.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

template <typename T>
Hasher<T>::Hasher(vector<pair<size_t, shared_ptr<T>>>& ring) : ring(ring) {}

template <typename T> shared_ptr<T> Hasher<T>::GetElement(string key) {
	if (ring.empty()) {
		throw std::runtime_error("No Segments available");
	}

	size_t hashValue = HashFunction(key);
	auto it = lower_bound(
		ring.begin(), ring.end(), make_pair(hashValue, shared_ptr<T>{}),
		[](const pair<size_t, shared_ptr<T>>& a,
		   const pair<size_t, shared_ptr<T>>& b) { return a.first < b.first; });

	if (it == ring.end()) {
		cout << "HASHER FOUND ELEmENT: " << ring.front().first << endl;

		return ring.front().second;
	}
	cout << "HASHER FOUND ELEmENT: " << it->first << endl;
	return it->second;
}

template <typename T> void Hasher<T>::AddElement() {
	// TODO: Instead of sort Directly insert in correct position and move
	size_t hashValue = HashFunction(to_string(ring.size()));

	ring.push_back(make_pair(hashValue, make_shared<T>()));
	sort(
		ring.begin(), ring.end(),
		[](const pair<size_t, shared_ptr<T>>& a,
		   const pair<size_t, shared_ptr<T>>& b) { return a.first < b.first; });
}

template <typename T> size_t Hasher<T>::HashFunction(string key) const {
	// TODO: Improve hash function
	hash<string> hash_fn;
	cout << "KEY: " << key << endl;
	size_t hashValue = hash_fn(key);
	cout << "Hash Value: " << hashValue << endl;
	return hashValue;
}

//with b+ tree:
// #include "../include/hasher.hpp"

// template <typename T>
// Hasher<T>::Hasher(vector<pair<size_t, shared_ptr<T>>>& vec) : ring(vec) {}

// template <typename T>
// shared_ptr<T> Hasher<T>::GetElement(string key) {
//     size_t hashValue = HashFunction(key);
//     for (auto& segmentPair : ring) {
//         if (segmentPair.first >= hashValue) {
//             return segmentPair.second;
//         }
//     }
//     return ring.front().second; // Default to the first segment if none found
// }

// template <typename T>
// void Hasher<T>::AddElement() {
//     size_t newHashValue = ring.size(); // New segment gets the next hash value
//     ring.push_back({newHashValue, make_shared<T>()});
// }

// template <typename T>
// void Hasher<T>::HandleSplit(shared_ptr<T> segment, vector<pair<string, string>> splitPairs) {
//     // Create a new segment to hold the split keys
//     AddElement();
//     auto newSegment = ring.back().second;

//     // Redistribute the keys from the split
//     for (const auto& pair : splitPairs) {
//         newSegment->Put(pair.first, pair.second);
//     }

//     // Update the hasher's mapping
//     cout << "New segment added after split with hash value: " << ring.back().first << endl;
// }

// template <typename T>
// size_t Hasher<T>::HashFunction(string key) const {
//     hash<string> hasher;
//     return hasher(key) % ring.size(); // Simple hash mod ring size
// }
