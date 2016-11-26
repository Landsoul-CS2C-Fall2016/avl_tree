// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once




template <typename Key, typename Value>
struct AVLTreeNode {
   AVLTreeNode *parent, *left, *right;
   Key key;
   Value value;
   // Note that the height of an empty tree is 0.
   int height;

   template<typename Callable>
   void traverse(Callable& callable)
   {
      if (left != nullptr)
         left->traverse(callable);
      callable(key, value);
      if (right != nullptr)
         right->traverse(callable);
   }

   AVLTreeNode(const Key& key, const Value& value)
      : parent(nullptr), left(nullptr), right(nullptr),
      key(key), value(value), height(0) {}

   ~AVLTreeNode() {
      if (this->right != nullptr)
         delete right;
      if (this->left != nullptr)
         delete left;
   
   }
};

template <typename Key, typename Value>
class AVLTree {
public:
   AVLTreeNode<Key, Value> *root;

   AVLTree() : root(nullptr) {}

   ~AVLTree() {
      if (root != nullptr)
         delete root;
   }

   // ---------------- HELPER METHODS FOR INSERT/REMOVE ---------------------------------------------------

   void insertHelper(AVLTreeNode<Key, Value> *myRoot, AVLTreeNode<Key, Value> *cur, const Key& key) {
      if (key >= myRoot->key) { // if its greater than
         if (myRoot->right != nullptr) {
            insertHelper(myRoot->right, cur, key);
         } else {
            myRoot->right = cur;
            myRoot->right->parent = myRoot;
            cur->height = 1;
         }
      }
      else if (key < myRoot->key ) { // if its less than
         if (myRoot->left != nullptr) {
            insertHelper(myRoot->left, cur, key);
         } else {
            myRoot->left = cur;
            myRoot->left->parent = myRoot;
            cur->height = 1;
         }
      }
      updateHeight(myRoot);
      reBalance(myRoot);
      /* NOTE TO RAY:
      you need to update the height of the node you are rotating after you rotated it. 
      otherwise, you will have wrong heights.
      */
   }

   // this method recursively finds the node to delete, then after its found, does something, then updates height and rebalances up each step of recursion.
   void removeHelper(const Key& key, AVLTreeNode<Key, Value> *myRoot) {
      if (myRoot != nullptr && myRoot->key == key) { // you have found the node
         if (myRoot->left == nullptr && myRoot->right == nullptr) { // no children
            if ((myRoot->parent != nullptr && (myRoot->parent->key > key)) 
               || myRoot->parent->left == myRoot) {
               myRoot->parent->left = nullptr;
            }
            else {
               myRoot->parent->right = nullptr;
            }
            myRoot->parent = nullptr;
            delete myRoot;
         }
         else if (myRoot->left != nullptr && myRoot->right == nullptr) { // only left children
            if (myRoot->parent != nullptr) {
               myRoot->left->parent = myRoot->parent;
               if ((myRoot->parent->key > key) || myRoot->parent->left == myRoot) {
                  myRoot->parent->left = myRoot->left;
               }
               else {
                  myRoot->parent->right = myRoot->left;
               }
               updateHeight(myRoot->parent);
            }
            else {
               root = myRoot->left;
               myRoot->left->parent = nullptr;
            }
            myRoot->left = nullptr;
            delete myRoot;
         }
         else if (myRoot->right != nullptr && myRoot->left == nullptr) { // only right children

            if (myRoot->parent != nullptr) { // you are not at the root
               myRoot->right->parent = myRoot->parent;
               if ((myRoot->parent->key > key) || myRoot->parent->left == myRoot) {
                  myRoot->parent->left = myRoot->right;
               }
               else {
                  myRoot->parent->right = myRoot->right;
               }
               updateHeight(myRoot->parent);
            }
            else {
               root = myRoot->right;
               myRoot->right->parent = nullptr;
            }
            myRoot->right = nullptr;
            delete myRoot;
         }
         else { // has two children
            swapWithReplacement(myRoot->left, myRoot); // first one is to recurse with, other is to swap once a replacement is found.
         }
      }
      else if(myRoot != nullptr) { // recurse down until you find it.
         if (myRoot->key > key)
            removeHelper(key, myRoot->left);
         else
            removeHelper(key, myRoot->right);
         updateHeight(myRoot); // updates height as it goes back up the steps of recursion
         reBalance(myRoot);
      }

   }

   void swapWithReplacement(AVLTreeNode<Key, Value> *myNode, AVLTreeNode<Key, Value> *mySwap) { // myswap is node being replaced.
      if (myNode->right != nullptr) {
         swapWithReplacement(myNode->right, mySwap); // this will recurse until myNode->right is nullptr.
      }
      else {
         // case 1:
         AVLTreeNode<Key, Value> *temp;
         if (mySwap->parent == nullptr) { //has no parents
            if (myNode->parent == mySwap)
            {
               if (mySwap->right != nullptr) {
                  myNode->right = mySwap->right;
                  mySwap->right->parent = myNode;
                  mySwap->right = nullptr;
               }

               myNode->parent = nullptr;
               mySwap->parent = myNode;

               if (myNode->left != nullptr) {
                  mySwap->left = myNode->left;
                  myNode->left->parent = mySwap;
               }
               else {
                  mySwap->left = nullptr;
               }
               myNode->left = mySwap;
               root = myNode;
            }
            else {
               if (mySwap->right != nullptr) {
                  mySwap->right->parent = myNode;
                  myNode->right = mySwap->right;
                  mySwap->right = nullptr;
               }

               mySwap->parent = myNode->parent;
               myNode->parent->right = mySwap;
               myNode->parent = nullptr;

               temp = mySwap->left;
               if (myNode->left != nullptr) {
                  mySwap->left = myNode->left;
                  myNode->left->parent = mySwap;
               }
               else {
                  mySwap->left = nullptr;
               }
               myNode->left = temp;
               temp->parent = myNode;
               root = myNode;

            }
         }
         else if (mySwap->parent->key < mySwap->key) { // mySwap is to the right of parent
            if (myNode->parent == mySwap) { // if there is a node in between mySwap and myNode
               myNode->right = mySwap->right;
               mySwap->right->parent = myNode;
               mySwap->right = nullptr;

               temp = mySwap->parent;
               mySwap->parent->right = myNode;
               mySwap->parent = myNode;
               myNode->parent = temp;

               if (myNode->left != nullptr) {
                  mySwap->left = myNode->left;
                  myNode->left->parent = mySwap;
               }
               else {
                  mySwap->left = nullptr;
               }
               myNode->left = mySwap;

            }
            else { // you are one step down in the recursion
               mySwap->right->parent = myNode;
               myNode->right = mySwap->right;
               mySwap->right = nullptr;

               temp = mySwap->parent;
               mySwap->parent->right = myNode;
               mySwap->parent = myNode->parent;
               myNode->parent->right = mySwap;
               myNode->parent = temp;

               temp = mySwap->left;
               if (myNode->left != nullptr) {
                  myNode->left->parent = mySwap;
                  mySwap->left = myNode->left;
               }
               else {
                  mySwap->left = nullptr;
               }
               myNode->left = temp;
               temp->parent = myNode;
            }
         } 
         else if (mySwap->parent->key >= mySwap->key) { // mySwap is to the left of parent
            if (myNode->parent == mySwap) { // same as above.
               mySwap->right->parent = myNode;
               myNode->right = mySwap->right;
               mySwap->right = nullptr;

               temp = mySwap->parent;
               mySwap->parent->left = myNode;
               mySwap->parent = myNode;
               myNode->parent = temp;

               if (myNode->left != nullptr) {
                  mySwap->left = myNode->left;
                  myNode->left->parent = mySwap;
               }
               else {
                  mySwap->left = nullptr;
               }
               myNode->left = mySwap;
            }
            else { // you are one step down in the recursion
               mySwap->right->parent = myNode;
               myNode->right = mySwap->right;
               mySwap->right = nullptr;

               temp = mySwap->parent;
               mySwap->parent->left = myNode;
               mySwap->parent = myNode->parent;
               myNode->parent->right = mySwap;
               myNode->parent = temp;

               temp = mySwap->left;
               if (myNode->left != nullptr) {
                  myNode->left->parent = mySwap;
                  mySwap->left = myNode->left;
               }
               else {
                  mySwap->left = nullptr;
               }

               myNode->left = temp;
               temp->parent = myNode;
            }
         }

         // done swapping
         temp = mySwap->parent;
         removeHelper(mySwap->key, mySwap);
         updateHeight(temp);
         reBalance(temp);

         // swap the two nodes
         // call remove on the swapped node(at the level of recursion you are in.)
         // this will fall under the one / no child case.
      }
      updateHeight(myNode);
      reBalance(myNode);
   }

   void updateHeight(AVLTreeNode<Key, Value> *myNode) {
      if (myNode->left != nullptr && myNode->right != nullptr) { // handles if there is a difference in left and right children heights
         myNode->height = (myNode->left->height < myNode->right->height ? myNode->right->height + 1 : myNode->left->height + 1);
      }
      else if (myNode->left != nullptr) {
         myNode->height = myNode->left->height + 1;
      }
      else if (myNode->right != nullptr) {
         myNode->height = myNode->right->height + 1;
      }
      else {
         myNode->height = 1;
      }
   }

   void reBalance(AVLTreeNode<Key, Value> *pivot) {
      const int NULLPTR_HEIGHT = 0;
      int balance = 0; // if positive, more nodes on right side.
      int childBalance;
      // find the balance factor of the pivot node.

      if (pivot->right == nullptr && pivot->left == nullptr)
         balance = NULLPTR_HEIGHT;
      else if (pivot->right == nullptr)
         balance = -pivot->left->height; // will produce negative #
      else if (pivot->left == nullptr)
         balance = pivot->right->height; // will produce positive #
      else
         balance = pivot->right->height - pivot->left->height;

      if (balance > 1) { // only shift when it is right or left heavy.
         // find balance factor of right child
         if (pivot->right->right == nullptr && pivot->right->left == nullptr)
            childBalance = NULLPTR_HEIGHT;
         else if (pivot->right->right == nullptr) // always negative
            childBalance = -pivot->right->left->height;
         else if (pivot->right->left == nullptr) // always positive
            childBalance = pivot->right->right->height;
         else
            childBalance = pivot->right->right->height - pivot->right->left->height;

         if (childBalance <= -1) { // if the right child is left heavy
            // double right rotation
            rotateRight(pivot->right);
            rotateLeft(pivot);
         }
         else {
            // left rotation
            rotateLeft(pivot);
         }
      }
      else if(balance < -1) {
         // find balance factor of left child
         if (pivot->left->right == nullptr && pivot->left->left == nullptr)
            childBalance = NULLPTR_HEIGHT;
         else if (pivot->left->right == nullptr)
            childBalance = -pivot->left->height;
         else if (pivot->left->left == nullptr)
            childBalance = pivot->left->right->height;
         else
            childBalance = pivot->left->right->height - pivot->left->left->height;

         if (childBalance >= 1) { // if the left child is right heavy
            // double left rotation
            rotateLeft(pivot->left);
            rotateRight(pivot);
         }
         else {
            // right rotation
            rotateRight(pivot);
         }
      }
   }

   void rotateRight(AVLTreeNode<Key, Value> *rotAt) {
      if (rotAt != nullptr) {
         int temp = 0;
         AVLTreeNode<Key, Value> *pivot = rotAt->left;
         if (pivot->right != nullptr) {
            pivot->right->parent = rotAt;
            rotAt->left = pivot->right;
         }
         else {
            rotAt->left = nullptr;
         }
         pivot->right = rotAt;
         if (rotAt != root) {
            pivot->parent = rotAt->parent;
            if (pivot->key < pivot->parent->key)
               pivot->parent->left = pivot;
            else
               pivot->parent->right = pivot;
         }
         else {
            pivot->parent = nullptr;
            root = pivot;
         }
         rotAt->parent = pivot;
         temp = rotAt->height;
         this->updateHeight(rotAt);
         this->updateHeight(pivot);

      }
   }

   void rotateLeft(AVLTreeNode<Key, Value> *rotAt) {
      if (rotAt != nullptr) {
         AVLTreeNode<Key, Value> *pivot = rotAt->right;
         if (pivot->left != nullptr) { // have it point to rotat and rotat point to it
            pivot->left->parent = rotAt;
            rotAt->right = pivot->left;
         }
         else {
            rotAt->right = nullptr;
         }
         pivot->left = rotAt;
         if (rotAt != root) {
            pivot->parent = rotAt->parent;
            if (pivot->key < pivot->parent->key)
               pivot->parent->left = pivot;
            else
               pivot->parent->right = pivot;
         }
         else {
            pivot->parent = nullptr;
            root = pivot;
         }
         rotAt->parent = pivot;
         this->updateHeight(rotAt);
         this->updateHeight(pivot);
      }
   }

   // ----------------- END OF HELPER METHODS -----------------------------

   // PART 1
   // Insert a value, while making sure the tree is still a AVL tree.
   // This assumes that the key does not exist in the tree.
   void insert(const Key& key, const Value& value)
   {
      AVLTreeNode<Key, Value> *current = new AVLTreeNode<Key, Value>(key, value);
      AVLTreeNode<Key, Value> *per = root;
      if (root == nullptr) { //  nothing in the BS tree
         // add a node and update root
         root = current;
         root->height = 1;
         return;
      }
      else{
         insertHelper(per, current, key);
      }
   }

   // PART 1
   // Given a key, find the corresponding value.
   // This assumes that the key exists.
   Value& get(const Key& key) const {
      AVLTreeNode<Key, Value> *cur = root;
      while (cur->key != key) {
         if (cur->key > key) {
            cur = cur->left;
         } else {
            cur = cur->right;
         }
      }
      return cur->value;
   }

   AVLTreeNode<Key, Value> *findNode(const Key& key) const {
      AVLTreeNode<Key, Value> *cur = root;
      while (cur->key != key) {
         if (cur->key > key) {
            cur = cur->left;
         }
         else {
            cur = cur->right;
         }
      }
      return cur;
   }

   // PART 1
   // Given a callable, that takes a Key and Value, call that callable
   // for every key/value pair in the tree in sorted order.
   // e.g. callable(Key(), Value())
   template <typename Callable>
   void for_each_key_val(const Callable& callable)
   {
      if(root != nullptr)
         root->traverse(callable);
   }

   // PART 2
   // Remove the value associated with key,
   // while making sure the tree is still a AVL tree.
   // This assumes that the key exists.
   void remove(const Key& key)
   {
      AVLTreeNode<Key, Value> *myNode = root;
      if (key == root->key && root->left == nullptr && root->right == nullptr) {
         root = nullptr;
         delete myNode;
      }
      else {
         removeHelper(key, myNode);
         updateHeight(root);
      }
   }


   void testPrint() {
      testPrint(root, "", false);
   }

   void testPrint(AVLTreeNode<Key, Value> *myNode, std::string indent, bool isleft) { // first iteration takes an empty string

      std::string right = "/=", left = "\\=", mid = "|";
      if (myNode != nullptr) {
         if (myNode->right != nullptr) {
            if (myNode == root)
               testPrint(myNode->right, indent, false);
            else
               testPrint(myNode->right, indent + "  ", false);
         }

         if (myNode == root) {
            std::cout << indent << "(" << myNode->key << ", " << myNode->value << ", height="
               << myNode->height /*<< "\n" << indent << ", this:" << myNode << ", par:" << myNode->parent << ", left:" << myNode->left << ", right:" << myNode->right*/ << ")\n";
         }
         else if (!isleft) {
            indent += mid;
            std::cout << indent << right << "(" << myNode->key << ", " << myNode->value << ", height="
               << myNode->height /*<< "\n" << indent << ", this:" << myNode << ", par:" << myNode->parent << ", left:" << myNode->left << ", right:" << myNode->right*/ << ")\n";
         }
         else if (isleft) {
            indent += mid;
            std::cout << indent << left << "(" << myNode->key << ", " << myNode->value << ", height="
               << myNode->height /*<< "\n" << indent << ", this:" << myNode << ", par:" << myNode->parent << ", left:" << myNode->left << ", right:" << myNode->right*/ << ")\n";
         }
         if (myNode->left != nullptr) {
            if (myNode == root)
               testPrint(myNode->left, indent, true);
            else
               testPrint(myNode->left, indent + "  ", true);
         }
      }
   }

};

// TODO: reference additional headers your program requires here
