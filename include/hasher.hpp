#pragma once
#include <memory>
#include <string>
#include <vector>

using namespace std;

template <typename T>
class Hasher {
    vector<pair<size_t, shared_ptr<T>>>& ring;

    size_t HashFunction(string key) const {
        std::hash<string> hasher;
        return hasher(key) % ring.size();
    }

public:
    Hasher(vector<pair<size_t, shared_ptr<T>>>& vec) : ring(vec) {}

    shared_ptr<T> GetElement(string key) {
        size_t index = HashFunction(key);
        return ring[index].second;
    }

    void AddElement() {
        // Add a new segment (as per your logic)
        ring.emplace_back(make_pair(ring.size(), make_shared<T>()));
    }
};

//when utilising b+ tree:
// #pragma once

// #include <memory>
// #include <string>
// #include <vector>

// using namespace std;

// template <typename T> class Hasher {
//     vector<pair<size_t, shared_ptr<T>>>& ring;
//     size_t HashFunction(string key) const;

//   public:
//     Hasher(vector<pair<size_t, shared_ptr<T>>>& vec);
//     shared_ptr<T> GetElement(string key);
//     void AddElement();
//     void HandleSplit(shared_ptr<T> segment, vector<pair<string, string>> splitPairs); // Handle segment split
// };
