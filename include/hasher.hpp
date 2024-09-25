
#pragma once
#include "segment.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class Hasher {
  private:
	vector<pair<size_t, shared_ptr<Segment>>>& ring;
	size_t HashFunction(string key) const;

  public:
	Hasher(vector<pair<size_t, shared_ptr<Segment>>>& vec);
	pair<shared_ptr<Segment>, size_t> GetElement(string key);
	void AddElement();
	void RehashHelper(size_t segmentIndex);
};
