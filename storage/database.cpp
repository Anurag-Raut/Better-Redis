#include "database.hpp"
#include <iostream>
#include <stdexcept>

Database::Database(size_t numThreads) : threadPool(numThreads) {
    hasher = std::make_unique<Hasher<Segment>>(segments);
}

void Database::Put(string key, string value) {
    threadPool.EnqueueTask([this, key, value] {
        try {
            shared_ptr<Segment> segment = hasher->GetElement(key);
            segment->Put(key, value);
        } catch (const exception& e) {
            hasher->AddElement();
            this->Put(key, value);  // Retry after adding element
        }
    });
}

string Database::Get(string key) {
    auto resultFuture = threadPool.EnqueueTask([this, key] {
        shared_ptr<Segment> segment = hasher->GetElement(key);
        return segment->Get(key);
    });

    return resultFuture.get();  // Wait for the result and return
}


// when using b+ tree:
// #include "../include/database.hpp"
// #include <iostream>

// Database::Database() {
//     this->hasher = make_unique<Hasher<Segment>>(this->segments);
// }

// void Database::Put(string key, string value) {
//     auto segment = hasher->GetElement(key);
//     segment->Put(key, value);

//     // Check if the segment is full and handle the split
//     if (segment->IsFull()) {
//         auto splitPairs = segment->Split();
//         hasher->HandleSplit(segment, splitPairs); // Redistribute keys after split
//     }
// }

// string Database::Get(string key) {
//     auto segment = hasher->GetElement(key);
//     return segment->Get(key);
// }
