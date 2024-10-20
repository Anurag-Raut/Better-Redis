#include "bplustree.hpp"

// Constructor for BPlusTreeNode
BPlusTreeNode::BPlusTreeNode(bool leaf, int _t) {
    isLeaf = leaf;
    t = _t;
    keys.resize(0);  // initially, no keys
    if (!leaf) {
        children.resize(0);  // no children for non-leaf nodes initially
    } else {
        values.resize(0);  // leaf nodes have values
    }
}

// Insert a new key into the tree rooted with this node
void BPlusTreeNode::InsertNonFull(const string& key, const string& value) {
    int i = keys.size() - 1;

    if (isLeaf) {
        // If this is a leaf node, insert the new key in the correct position
        while (i >= 0 && keys[i] > key) {
            i--;
        }
        keys.insert(keys.begin() + i + 1, key);
        values.insert(values.begin() + i + 1, value);
    } else {
        // If this is an internal node, find the child which is going to have the new key
        while (i >= 0 && keys[i] > key) {
            i--;
        }

        // Check if the child is full
        if (children[i + 1]->keys.size() == 2 * t - 1) {
            SplitChild(i + 1, children[i + 1]);
            if (keys[i + 1] < key) {
                i++;
            }
        }
        children[i + 1]->InsertNonFull(key, value);
    }
}

// Split the child y of this node
void BPlusTreeNode::SplitChild(int i, shared_ptr<BPlusTreeNode> y) {
    shared_ptr<BPlusTreeNode> z = make_shared<BPlusTreeNode>(y->isLeaf, t);

    // Move the second half of the keys to the new node
    z->keys.insert(z->keys.end(), y->keys.begin() + t, y->keys.end());
    y->keys.resize(t - 1);  // Shrink the original node

    // Move the second half of the children if it's not a leaf
    if (!y->isLeaf) {
        z->children.insert(z->children.end(), y->children.begin() + t, y->children.end());
        y->children.resize(t);  // Shrink the original node's children
    }

    // If it's a leaf, move the corresponding values as well
    if (y->isLeaf) {
        z->values.insert(z->values.end(), y->values.begin() + t, y->values.end());
        y->values.resize(t - 1);  // Shrink the original node's values
    }

    // Insert the middle key into the parent node (this node)
    children.insert(children.begin() + i + 1, z);
    keys.insert(keys.begin() + i, y->keys[t - 1]);
}

// Traverse the B+ tree node
void BPlusTreeNode::Traverse() {
    int i;
    for (i = 0; i < keys.size(); i++) {
        if (!isLeaf) {
            children[i]->Traverse();
        }
        cout << " " << keys[i];
    }
    if (!isLeaf) {
        children[i]->Traverse();
    }
}

// Search for a key in this B+ tree node
string BPlusTreeNode::Search(const string& key) {
    int i = 0;
    while (i < keys.size() && key > keys[i]) {
        i++;
    }

    if (keys[i] == key && isLeaf) {
        return values[i];  // Found key in a leaf node
    }

    if (isLeaf) {
        throw runtime_error("Key not found");
    }

    return children[i]->Search(key);
}

// Insert a new key into the B+ Tree
void BPlusTree::Insert(const string& key, const string& value) {
    if (root->keys.size() == 2 * t - 1) {
        // Tree is full, need to split
        shared_ptr<BPlusTreeNode> s = make_shared<BPlusTreeNode>(false, t);

        // Make the old root as child of the new root
        s->children.push_back(root);

        // Split the old root
        s->SplitChild(0, root);

        // New root has two children now, decide which one will get the new key
        int i = 0;
        if (s->keys[0] < key) {
            i++;
        }
        s->children[i]->InsertNonFull(key, value);

        // Change root
        root = s;
    } else {
        root->InsertNonFull(key, value);
    }
}

// Search for a key in the B+ Tree
string BPlusTree::Search(const string& key) {
    if (root) {
        return root->Search(key);
    } else {
        throw runtime_error("Tree is empty");
    }
}
