/*
 *  __________________________
 *  Sample #2: map structure using hashtable
 *  __________________________
 *  # Available input commands:
 *  ## put string key string value -- adds pair key: value to map or updates existing key value
 *  ## delete string key -- delete key (if key is not in map, nothing will happen)
 *  ## get string key -- returns key value or "none" if key not found
 *  __________________________
 *  # Input sample at main/cpp/input/map.in
 *  # Output sample at main/cpp/output/map.out
 *
 */

#include <fstream>
#include <vector>

using namespace std;

struct Node {
    string key;
    string value;
};

class map {
private:
    const int SIZE = 100000;
    vector<vector<Node>> hashTable;

    int hash(auto *needle) {
        int x = 0;
        for (char i : *needle) {
            x = (int) abs(26 * x + (i - 'a') + 1);
            x %= SIZE;
        }
        return abs(x);
    }

public:
    map() {
        hashTable.resize(SIZE);
    }

    Node *find(string *key) {
        int keyIndex = hash(key);
        for (int i = 0; i < hashTable[keyIndex].size(); ++i) {
            if (hashTable[keyIndex][i].key == *key) return &hashTable[keyIndex][i];
        }
        return nullptr;
    }

    void add(string *key, string *value) {
        int keyIndex = hash(key);
        Node *currentNode = find(key);
        if (currentNode == nullptr) {
            Node tmp;
            tmp.key = *key;
            tmp.value = *value;
            hashTable[keyIndex].push_back(tmp);
        } else {
            currentNode->value = *value;
        }
    }

    void remove(string *key) {
        int keyIndex = hash(key);
        Node *currentNode = find(key);
        if (currentNode == nullptr) return;

        for (int i = 0; i < hashTable.size(); ++i) {
            if (hashTable[keyIndex][i].key == *key) {
                auto currentIterator = hashTable[keyIndex].begin();
                hashTable[keyIndex].erase(currentIterator + i);
                return;
            }
        }
    }

    ~map() {
        hashTable.clear();
    }
};

int main() {
    ifstream fin("map.in");
    ofstream fout("map.out");

    //      ios_base::sync_with_stdio(false);
    //      fin.tie(NULL);

    string command, key, value;
    map sampleMap;

    while (fin >> command) {
        if (command == "put") {
            fin >> key >> value;
            sampleMap.add(&key, &value);
        } else if (command == "get") {
            fin >> key;
            Node *tmp = sampleMap.find(&key);
            fout << ((tmp == nullptr) ? "none" : tmp->value) << '\n';
        } else if (command == "delete") {
            fin >> key;
            sampleMap.remove(&key);
        }
    }

    fout.close();
    fin.close();

    return 0;
}
