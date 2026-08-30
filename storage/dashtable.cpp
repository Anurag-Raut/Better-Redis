#include "../include/dashtable.hpp"
#include "../include/errors.hpp"
#include "../include/globals.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

Dashtable::Dashtable() { this->hasher = make_unique<Hasher>(this->segments); }
int Dashtable::NoOfSegments() { return this->segments.size(); }

void Dashtable::Put(string key, string value) {
	// cout << "PUTTING: kV" << key << " " << value << endl;
	Segment* segment;
	size_t segmentIndex;
	size_t segmentHash;
	try {
		tie(segment, segmentIndex,segmentHash) = hasher->GetElement(key);
	} catch (const std::exception& e) {
		hasher->AddElement();

		try {
			tie(segment, segmentIndex,segmentHash) = hasher->GetElement(key);
		} catch (const std::exception& e) {
			// std::cout << "Failed to get segment even after adding: " << e.what()
			// 		  << std::endl;
			return;
		}
	}

	try {
		segment->Put(key, value,segmentHash);
		// cout << "putting in SEGMENt INDEX: " << segmentIndex << endl;
	} catch (const SegmentFullException& e) {
		// std::cout << "Segment " << segmentIndex << " is full: " << e.what()
		// 		  << std::endl;
		this->Rehash(segmentIndex);
		this->Put(key, value);
	} catch (const std::exception& e) {
		// std::cout << "Unexpected exception in Put: " << e.what() << std::endl;
	}
}

void Dashtable::Rehash(size_t segmentIndex) {

	this->rehashCounter++;
	Segment* seg = segments[segmentIndex].second;
	hasher->RehashHelper(segmentIndex);
	vector<pair<string, string>> deletedValues = seg->DeleteAll();
	for (const auto& [key, value] : deletedValues) {
		this->Put(key, value);
	};

}

string Dashtable::Get(string key) {
	try {

		// cout << "GETTING: kV" << key << endl;
		auto [segment, segmentIndex,segmentHash] = hasher->GetElement(key);
		// cout << "SEGMENT INDEX: " << segmentIndex << endl;
		return segment->Get(key,segmentHash);

	} catch (const exception& e) {
		// cout << "ERROR WHILE GETTING: " << e.what() << endl;
		// throw e;
		return "";
	}
}

void Dashtable::print() {
	int index = 0;
	for (auto& seg : segments) {
		cout << "Segment " << index++<<" HASH VALUE: "<<seg.first << endl;
		seg.second->print();
	}
	cout << endl;
	cout << endl;
}

void Dashtable::Delete(string key) {
	try {
		auto [segment, segmentIndex,segmentHash] = hasher->GetElement(key);
		segment->Delete(key,segmentHash);
	} catch (const exception& e) {
		// cout << "ERROR WHILE DELETING: " << e.what() << endl;
		throw e;
	}
}

void Dashtable::Update(string key, string value) {}
