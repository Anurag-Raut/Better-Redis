#include "bucket.hpp"
#include <stdexcept>

Bucket::Bucket() {}

void Bucket::Put(string key, string value) {
    for (auto& slot : slots) {
        if (slot.first == key) {
            slot.second = value;
            return;
        }
    }
    slots.emplace_back(key, value);
}

string Bucket::Get(string key) {
    for (auto& slot : slots) {
        if (slot.first == key) {
            return slot.second;
        }
    }
    throw std::runtime_error("Key not found");
}


// #include "../include/bucket.hpp"
// #include <iostream>
// #include <iterator>
// #include <stdexcept>

// Bucket::Bucket() { slots.resize(10); }

// void Bucket::Put(string key, string value) {
// 	for (auto& slot : slots) {
// 		if (slot.first.empty()) {
// 			// found a clear slot Insert
// 			cout << "FOUND SLOT" << endl;
// 			slot.first = key;
// 			slot.second = value;
// 			break;
// 		}
// 	}

// 	cout << "printing KEYS" << endl;

// 	for (auto& slot : slots) {
// 		cout << slot.first << " ";
// 	}
// 	cout << endl;
// }

// string Bucket::Get(string key) {
// 	cout << "KK: " << key << endl;
// 	for (auto& slot : slots) {
// 		if (slot.first == key) {
// 			return slot.second;
// 		}
// 	}
// 	cout << "printing KEYS" << endl;

// 	for (auto& slot : slots) {
// 		cout << slot.first << " ";
// 	}
// 	cout << endl;

// 	throw runtime_error("VALUE NOT PRESENT");
// }
