#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

class BPlusTreeNode {
public:
    bool isLeaf;
    vector<string> keys;
    vector<shared_ptr<BPlusTreeNode>> children;
    vector<string> values; // only relevant for leaf nodes
    int t;  // Minimum degree (defines the range for number of keys)

    BPlusTreeNode(bool leaf, int t);

    // Insert a key into the subtree rooted with this node
    void InsertNonFull(const string& key, const string& value);

    // Split the child y of this node. i is the index of y in child array.
    void SplitChild(int i, shared_ptr<BPlusTreeNode> y);

    // Traverse the node and print keys (for testing)
    void Traverse();

    // Search for a key in this node
    string Search(const string& key);
};

class BPlusTree {
public:
    shared_ptr<BPlusTreeNode> root;
    int t;  // Minimum degree

    BPlusTree(int _t) {
        root = make_shared<BPlusTreeNode>(true, _t);
        t = _t;
    }

    // Insert a new key into the B+ Tree
    void Insert(const string& key, const string& value);

    // Search for a key in the B+ Tree
    string Search(const string& key);

    // Traverse the tree (for debugging)
    void Traverse() {
        if (root) root->Traverse();
    }
};
