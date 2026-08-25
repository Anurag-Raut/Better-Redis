# Better Redis

An experimental multithreaded in-memory key-value store written in C++.

Better Redis explores how a Redis-like storage engine can use multiple CPU
cores without putting every operation behind one global lock. The keyspace is
partitioned across shards, and each shard owns a custom hash table that grows
by splitting and rehashing only the segment that becomes full.

This is a learning project inspired by the architecture of
[DragonflyDB](https://www.dragonflydb.io/). It is not intended to be a
production replacement for Redis.

## What it explores

- Sharding a keyspace across worker-owned storage.
- Routing `GET`, `PUT`, and `DELETE` operations to the correct shard.
- Concurrent transaction submission with C++ queues, promises, and futures.
- A custom hash table (`Dashtable`) backed by a hash ring.
- Segment-level growth instead of resizing and rehashing the entire table.
- Fixed-size buckets with key-value slots and collision handling.
- Workload comparisons against Redis through [Hiredis](https://github.com/redis/hiredis).

## Architecture

```text
Database
   │
   ├── Hash key and choose shard
   │
   ▼
Shard workers
   │
   ├── Queue transactions
   │
   ▼
Dashtable
   │
   ├── Hash ring
   │
   ▼
Segments
   │
   ▼
Buckets
   │
   ▼
Key/value slots
```

When a segment becomes full, the table creates another segment in the
affected hash range, redistributes that segment's keys, and retries the
operation. Unaffected segments do not need to be rehashed.

## Example

```cpp
#include "database.hpp"

int main() {
    Database db(4);

    db.Put("name", "anurag");
    std::string value = db.Get("name");
    db.Delete("name");
}
```

The public database interface currently provides:

```cpp
std::string Get(std::string key);
void Put(std::string key, std::string value);
void Delete(std::string key);
```

## Build

The project uses C++17, CMake, GoogleTest, Hiredis, and the
[ConcurrentQueue](https://github.com/cameron314/concurrentqueue) library.

Clone the repository with its dependencies:

```bash
git clone --recurse-submodules https://github.com/Anurag-Raut/Better-Redis.git
cd Better-Redis
```

If the repository was cloned without submodules:

```bash
git submodule update --init --recursive
```

Configure and build:

```bash
cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build build -j
```

The policy flag is required with newer CMake versions because the pinned
Hiredis submodule uses an older `cmake_minimum_required` declaration. The
sample executable is produced at `build/better_redis`.


## Tests and Redis comparison

Build the test executable:

```bash
cmake --build build --target run_tests
```

The test suite includes a one-million-key workload for Better Redis and a
matching Hiredis workload against Redis on `127.0.0.1:6379`. Start a local
Redis server before running the comparison:

```bash
redis-server
./build/run_tests
```

These tests are useful for exploring the design and workload shape. They are
not a controlled benchmark or a claim that Better Redis is faster than Redis.

## Repository layout

```text
include/    Public headers for the database, shards, segments, and buckets
database/   Database and shard execution logic
storage/    Hashing, buckets, segments, and Dashtable implementation
tests/      GoogleTest workloads and Redis comparison
libs/       Git submodules for GoogleTest, Hiredis, and ConcurrentQueue
```

## Scope and limitations

Better Redis intentionally focuses on storage and concurrency mechanics. It
does not currently implement the complete Redis protocol, persistence,
replication, Pub/Sub, Redis data types, or crash recovery. The code is a
prototype built to make the trade-offs behind sharding and incremental
hash-table growth easier to understand.

## License

No license has been declared yet.
