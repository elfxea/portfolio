/*
 *  __________________________
 *  Sample #2: linkedMap structure using hashtable
 *  __________________________
 *  # Available input commands:
 *  ## put string key string value -- adds pair key: value to map or updates existing key value
 *  ## delete string key -- delete key (if key is not in map, nothing will happen)
 *  ## get string key -- gets key value or "none" if key not found
 *  ## next string key -- gets value of key added after current (or "none" if does not exist)
 *  ## prev string key -- gets value of key added before current (or "none" if does not exists)
 *  __________________________
 *  # Input sample at main/cpp/input/linkedMap.in
 *  # Output sample at main/cpp/output/linkedMap.out
 *
 */

#include <fstream>
#include <vector>

using namespace std;

struct Node {
    string key;
    string value;

    int order = 0; // variable to get previous and next elements from order array faster
};

class linkedMap {
private:
    const int SIZE = 1000000;
    const int ASCII_SIZE = 62;
    int counter = 0;
    vector<vector<Node>> hashTable;
    vector<string> queue;

    int hash(auto *key) { // returns hased key
        int hash = 0;
        for (char symbol : *key) {
            hash = (int) abs(ASCII_SIZE * hash + (symbol - '0') + 1);
            hash %= SIZE;
        }
        return abs(hash);
    }

public:
    linkedMap() { // constructor
        hashTable.resize(SIZE);
    }

    Node *find(auto *key) { // returns pointer to existing key node or nullptr
        int keyIndex = hash(key);
        for (int i = 0; i < hashTable[keyIndex].size(); ++i) {
            if (hashTable[keyIndex][i].key == *key) return &hashTable[keyIndex][i];
        }
        return nullptr;
    }

    void add(string *key, string *value) { // adds key: value pair
        int keyIndex = hash(key);
        Node *currentNode = find(key);
        if (currentNode == nullptr) {
            Node tmp;
            tmp.key = *key;
            tmp.value = *value;
            tmp.order = counter;

            hashTable[keyIndex].push_back(tmp);
            queue.push_back(*key);

            ++counter; // update key order counter
        } else {
            currentNode -> value = *value;
        }
    }

    Node *before(string *key) { // returns pointer to node added before key or nullptr
        Node *currentNode = find(key);

        // if no keys added before current key or current key does not exists
        if (currentNode == nullptr || currentNode -> order < 1)
            return nullptr;

        int previous = currentNode -> order - 1;
        return find(&queue[previous]);
    }

    Node *after(string *key) { // returns pointer to node added after key or nullptr
        Node *currentNode = find(key);

        // if no keys added after current key or current key does not exists
        if (currentNode == nullptr || currentNode -> order + 1 > queue.size() - 1)
            return nullptr;

        int next = currentNode -> order + 1;
        return find(&queue[next]);
    }

    void remove(string *key) { // removes key from map
        int keyIndex = hash(key);
        Node *currentNode = find(key);
        if (currentNode == nullptr) return;

        // for all nodes after current we need no decrease their order
        int j = currentNode -> order;
        for (int i = j; i < queue.size(); ++i) {
            Node *tempNode = find(&queue[i]);
            tempNode -> order -= 1;
        }

        queue.erase(queue.begin() + j);
        --counter;

        auto currentIterator = hashTable[keyIndex].begin();
        for (int i = 0; i < hashTable[keyIndex].size(); ++i) {
            if (hashTable[keyIndex][i].key == *key) {
                hashTable[keyIndex].erase(currentIterator + i);
                return;
            }
        }
    }

    ~linkedMap() {
        hashTable.clear();
        queue.clear();
    }
};

int main() {
    ifstream fin("linkedMap.in");
    ofstream fout("linkedMap.out");


    //      ios_base::sync_with_stdio(false);
    //      fin.tie(NULL);

    string command, key, value;
    linkedMap sampleMap;
    Node *tmp;

    while (fin >> command) {
        if (command == "put") {
            fin >> key >> value;
            sampleMap.add(&key, &value);
        } else if (command == "get") {
            fin >> key;
            tmp = sampleMap.find(&key);
            fout << ((tmp == nullptr) ? "none" : tmp -> value) << '\n';
        } else if (command == "delete") {
            fin >> key;
            sampleMap.remove(&key);
        } else if (command == "prev") {
            fin >> key;
            tmp = sampleMap.before(&key);
            fout << ((tmp == nullptr) ? "none" : tmp -> value) << '\n';
        } else if (command == "next") {
            fin >> key;
            tmp = sampleMap.after(&key);
            fout << ((tmp == nullptr) ? "none" : tmp -> value) << '\n';
        }
    }

    fout.close();
    fin.close();

    return 0;
}
