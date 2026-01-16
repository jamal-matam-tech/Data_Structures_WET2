
#ifndef HASH_TABLE_H
#define HASH_TABLE_H


#include <stdio.h>
#include <memory>
#include "Hunter.h"

#define INITIALDIVIDOR 20
class HashTable {
private:
    struct HashNode {
        int key;
        Hunter* value;
        std::shared_ptr<HashNode> next;

        HashNode(int k, Hunter* v) : key(k), value(v), next(nullptr) {}
    };

    std::shared_ptr<HashNode>* table;
    int dividor;
    int size;

    int hash(int key) const {
        return key % dividor;
    }
    void rehash();

    public:

      HashTable(int initialDividor = INITIALDIVIDOR);
      ~HashTable();

      bool insert(int key, Hunter* value);
      Hunter* find(int key) const;
      bool remove(int key);

    int getSize() const { return size;}

};
#endif //HASH_TABLE_H
