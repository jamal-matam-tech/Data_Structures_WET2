#include "HASH_TABLE.h"


HashTable::HashTable(int initialDividor)
    : dividor(initialDividor), size(0) {

    table = new std::shared_ptr<HashNode>[dividor];
    for (int i = 0; i < dividor; i++)
        table[i] = nullptr;
}


HashTable::~HashTable() {
    for (int i = 0; i < dividor; i++) {
        auto curr = table[i];
        while (curr != nullptr) {
            delete curr->value;
            curr = curr->next;
        }
    }
    delete[] table;
}

// amoritized O(1) (rehash takes O(n) but rehash happens after n inserts)
bool HashTable::insert(int key, Hunter* value) {
    if (find(key) != nullptr)
        return false;

    // the load factor = elements/dividor = 1 so we double the dividor
    // to double the size of the table
    if (size == dividor)
        rehash();

    int idx = hash(key);

    auto node = std::make_shared<HashNode>(key, value);
    node->next = table[idx];
    table[idx] = node;

    size++;
    return true;
}


// according to what we learned, this takes O(1) on average input
Hunter* HashTable::find(int key) const {
    int idx = hash(key);
    auto curr = table[idx];

    while (curr != nullptr) {
        if (curr->key == key)
            return curr->value;
        curr = curr->next;
    }
    return nullptr;
}

// according to what we learned, this takes O(1) on average input
bool HashTable::remove(int key) {
    int idx = hash(key);
    auto curr = table[idx];
    std::shared_ptr<HashNode> prev = nullptr;


    while (curr != nullptr) {
        if (curr->key == key) {
            delete curr->value;
            if (prev == nullptr)
                table[idx] = curr->next; // sharedptr deletes it self
            else
                prev->next = curr->next; // sharedptr deletes it self

            size--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

// average input O(n)
void HashTable::rehash() {
  auto oldtable = table;
  table = new std::shared_ptr<HashNode>[dividor * 2];
    dividor *=2;
    size = 0;
    for (int i = 0; i < dividor; i++) {
      table[i] = nullptr;
    }
  std::shared_ptr<HashNode> curr;
  for (int i = 0; i < dividor / 2; i++) {
    curr = oldtable[i];
    while (curr != nullptr) {
      insert (curr->key, curr->value);
      curr = curr->next;
    }
  }

  delete[] oldtable;
}
