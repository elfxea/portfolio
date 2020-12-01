/*
 *  __________________________
 *  Sample #1: set structure using hashtable
 *  __________________________
 *  # Available input commands:
 *  ## insert string a -- puts a into set (if a is in set, nothing will happen)
 *  ## delete string a -- delete a (if a is not in set, nothing will happen)
 *  ## exists string a -- check if a in set (returns bool)
 *  __________________________
 *  # Input sample at main/cpp/ioSamples/Set/set.in
 *  # Output sample at main/cpp/ioSamples/Set/set.out
 *
 */

#include <fstream>
#include <vector>

using namespace std;

struct Node {
    string value;
};

class set {
private:
    const int SIZE = 1000000;
    const int ASCII_SIZE = 26;

    int hash(auto *value) { // returns hash of key
        int result = 0;
        for (char symbol : *value) {
            result = (int) abs(ASCII_SIZE * result + (symbol - 'a') + 1);
            result %= SIZE;
        }
        return abs(result);
    }

    vector<vector<Node>> hashTable;

public:
    set() { // Constructor
        hashTable.resize(SIZE);
    }

    Node *find(auto *element) { // returns pointer to Node element or nullptr if element does not exist
        int keyIndex = hash(element);
        for (auto &i : hashTable[keyIndex]) {
            if (i.value == *element) return &i;
        }
        return nullptr;
    }

    void add(auto *element) { // Adds element to set if element does not exist
        Node *currentNode = find(element);
        if (currentNode == nullptr) { // if Node not found
            int index = hash(element);
            Node tmp;
            tmp.value = *element;
            hashTable[index].push_back(tmp);
        } else {
            currentNode->value = *element;
        }
    }

    void remove(auto *element) { // removes element from set
        int index = hash(element);
        auto currentIterator = hashTable[index].begin(); // __normal__iterator for hashTable[index]

        for (int i = 0; i < hashTable[index].size(); ++i) {
            if (hashTable[index][i].value == *element) {
                hashTable[index].erase(currentIterator + i);
                return;
            }
        }
    }

    ~set() { // Destructor
        hashTable.clear();
    }
};

int main() {
    ifstream fin("set.in");
    ofstream fout("set.out");
    
    //      ios_base::sync_with_stdio(false);
    //      fin.tie(NULL);

    string command, element;
    set sampleSet;
    
    while (fin >> command) {
        if (command == "insert") {
            fin >> element;
            sampleSet.add(&element);
        } else if (command == "exists") {
            fin >> element;
            Node *tmp = sampleSet.find(&element);
            (tmp == nullptr) ? fout << "false\n" : fout << "true\n";
        } else if (command == "delete") {
            fin >> element;
            sampleSet.remove(&element);
        }
    }
    
    fout.close();
    fin.close();

    return 0;
}
