#include "../include/database.hpp"
#include "../include/shard.hpp"
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using namespace std;

Database::Database(int noOfShards) {
	this->noOfShards = noOfShards;
	cout << "NO OF SHARDS: " << noOfShards << endl;
	for (int i = 0; i < noOfShards; i++) {
		shards.emplace_back(new Shard());
	}
	// cout<<"DONE"<<endl;
}

int Database::getShard(string key) {
	hash<string> hash_fn;
	size_t hashValue = hash_fn(key);
	int hashval = hashValue % noOfShards;
	// cout<<"SHARD INDEX: "<<hashval<<endl;
	return hashval;
}

string Database::Get(string key) {
	auto start = std::chrono::high_resolution_clock::now();

	int shardIndex = getShard(key);
	auto end = std::chrono::high_resolution_clock::now();

	Shard* shard = shards[shardIndex];
	future<string> resultFuture =
		shard->submitTransaction(Transaction{GET, key, ""});
	string result = resultFuture.get();
	return result;
}

void Database::Put(string key, string value) {

	int shardIndex = getShard(key);
	Shard* shard = shards[shardIndex];
	// auto start = std::chrono::high_resolution_clock::now();

	future<string> resultFuture =
		shard->submitTransaction(Transaction{PUT, key, value});
	// auto end = std::chrono::high_resolution_clock::now();
	// auto duration =
	// 	std::chrono::duration_cast<chrono::microseconds>(end - start);
	// cout << "TIME TAKEN FOR submit transaction and receive: "
	// 	 << duration.count() << endl;
}

void Database::Delete(string key) {
	int shardIndex = getShard(key);
	Shard* shard = shards[shardIndex];
	future<string> resultFuture =
		shard->submitTransaction(Transaction{DELETE, key, ""});
	string result = resultFuture.get();
}

Database::~Database() {
	// cout << "DatabseDESTRUCTOR CALLED" << endl;
	shards.clear();
}
