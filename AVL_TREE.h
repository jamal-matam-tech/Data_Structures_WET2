
#ifndef AVL_TREE_H
#define AVL_TREE_H




#include <iostream>
#include <memory>

using namespace std;


template <class Key, class Value>
class AVLTree {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        Node* parent;
        int height;

        Node(const Key& k, const Value& v)
          : key(k), value(v),
            left(nullptr), right(nullptr), parent(nullptr),
            height(0) {}
    };

    Node* root;
    int size;

    int balanceFactor(const Node* node) const {
      return (node == nullptr) ? 0 : height(node->left) - height(node->right);
    }

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    int height(const Node* n) const {
        return n ? n->height : -1;
    }

    void updateHeight(Node* n) {
      if (!n) return;
        n->height = 1 + max(height(n->left), height(n->right));
    }

    Node* LL_Rotation(Node* n) {
      Node* temp = n->left;
      Node* B = temp->right;
      Node* parent = n->parent;


      temp->right = n;
      n->left = B;

      temp->parent = n->parent;
      n->parent = temp;
      if(B)
        B->parent = n;

      if (parent) {
        if (parent->left == n)
          parent->left = temp;
        else
          parent->right = temp;
      }

      if(B)
      updateHeight(B);
      updateHeight(n);
      updateHeight(temp);
      return temp;
    }

    Node* RL_Rotation(Node* n) {
      if (!n) return nullptr;
      n->right = LL_Rotation(n->right);
      if (n->right) n->right->parent = n;
      return RR_Rotation(n);
    }

  Node* RR_Rotation(Node *A) {

      if (A == nullptr) {
        return nullptr;
      }
      Node* B=A->right;
      if (B == nullptr) {
        return nullptr;
      }

      Node *parent = A->parent;
      Node *tmp   = B->left;

      if (parent == nullptr) {
        B->parent = nullptr;
        A->right = tmp;
        if (tmp != nullptr) {
          tmp->parent = A;
        }
        B->left = A;
        A->parent = B;
      } else {

        B->parent = parent;
        if (parent->left == A) {
          parent->left = B;
        } else {
          parent->right = B;
        }

        A->right = tmp;
        if (tmp != nullptr) {
          tmp->parent = A;
        }

        B->left = A;
        A->parent = B;
      }

      updateHeight(A);
      updateHeight(B);

      return B;

    }

  Node* LR_Rotation(Node* A) {
      if (A == nullptr){
          return nullptr;
      }
      if (A->left == nullptr){

          return nullptr;
      }
      A->left = RR_Rotation(A->left);
      if (A->left!=nullptr){
          A->left->parent = A;
      }

      return LL_Rotation(A);
    }



  Node* minValueNode(Node* node) {
      Node* current = node;
      while (current->left)
        current = current->left;
      return current;
    }

  Node* insert(Node* n, const Key& key, const Value& info) {
      if (n == nullptr) {
        size++;
        Node* back= new Node(key, info);
        return back;
      }

      if (key == n->key) {
        return n;
      }
      Node* Left=n->left;
      Node* Right=n->right;
      if (key < n->key) {
        Left = insert(Left, key, info);
        n->left=Left;
        if (Left!= nullptr){
          Left->parent = n;
        }
      } else {
        Right = insert(Right, key, info);
        n->right=Right;
        if (Right!= nullptr){
          Right->parent = n;
        }
      }

      return rebalance(n);
    }

  Node* rebalance(Node* n){
    if (!n) return n;

    updateHeight(n);

      if (balanceFactor(n) > 1) {
        if (balanceFactor(n->left) < 0) {
          return LR_Rotation(n);
        }
        return LL_Rotation(n);
      }

    if (balanceFactor(n) < -1) {
      if (balanceFactor(n->right) > 0) {
        return RL_Rotation(n);
      }
      return RR_Rotation(n);
    }

    return n;
  }

    Node* remove(Node* n, const Key& k) {
      if (!n) return nullptr;

      if (k < n->key) {
        n->left = remove(n->left, k);
        if (n->left) n->left->parent = n;
      }
      else if (k > n->key) {
        n->right = remove(n->right, k);
        if (n->right) n->right->parent = n;
      }
      else{ // the node has been found
        if (!n->left || !n->right) { // at least 1 child does not exist
          Node* temp = n->left ? n->left : n->right;

          if (!temp) { // both children do not exist
            delete n;
            size--;
            return nullptr;
          }
          else { // 1 child exists
            temp->parent = n->parent;
            delete n;
            size--;
            return temp;
          }
        }

        else { // both children exist
          Node* successor = minValueNode(n->right);
          n->key = successor->key;
          n->value = successor->value;

          n->right = remove(n->right, successor->key);
          if (n->right) n->right->parent = n;
        }
      }

      return rebalance(n);
    }

  Node* find_node(Node* T,const Key& x){
      Node* target= nullptr;
      if(T== nullptr){
        return target;
      }
      Key key=T->key;
      if(key==x){
        target=T;
        return target;
      }
      if(key>x){
        return find_node(T->left,x);
      }else{
        return find_node(T->right,x);
      }



    }



    public:
      AVLTree() : root(nullptr), size(0) {}
      ~AVLTree() {
        destroy(root);
        root = nullptr;
        size = 0;
      }
      Node* insert(const Key& k, const Value& v) {
        root = insert(root,k,v);
        if(root){ root->parent=nullptr ;}
        return root;
      }
//    void insert(const Key& k, const Value& v) {
//        root = insert(root, k, v);
//        if (root) root->parent = nullptr;
//    }     // צריכה להיות VOID


    void remove(const Key& k) {
        root = remove(root, k); // update the root if deleted
        if (root) root->parent = nullptr;
      }
  Value* find(const Key& x) {
        Node* n = find_node(root, x);
        if (n == nullptr){
          return nullptr;
        }
        return &(n->value);
      }
      int getSize() const{ return size; }
      bool isEmpty() const{return root == nullptr;}


  // check if needed
      template<typename Func>
      void inOrderTraverse(Func func) {
          inOrderHelper(root, func);
      }

private:
      template<typename Func>
      void inOrderHelper(Node* node, Func func) {
          if (!node) return;
          inOrderHelper(node->left, func);
          func(node->key, &node->value);
          inOrderHelper(node->right, func);
      }

};


#endif //AVL_TREE_H