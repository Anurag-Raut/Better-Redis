#pragma once
#include "hasher.hpp"
#include "segment.hpp"
#include "ThreadPool.hpp"
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Database {
    vector<pair<size_t, shared_ptr<Segment>>> segments;
    unique_ptr<Hasher<Segment>> hasher;
    ThreadPool threadPool; 

public:
    Database(size_t numThreads = 4);  
    int NoOfSegments();
    string Get(string key);
    void Put(string key, string value);
    void Update(string key, string value);
    void Delete(string key);
};
