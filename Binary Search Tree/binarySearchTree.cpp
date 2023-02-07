/*
 * Name: Alvin Yang 
 * Date Submitted: 11/9/22  
 * Lab Section: 
 * Assignment Name: 
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

#include "binarySearchTree.h"

using namespace std;

void fix_size(Node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}

// insert key k into tree T, returning a pointer to the resulting tree
Node *insert(Node *T, int k)
{
  if (T == nullptr) return new Node(k);
  if (k < T->key) T->left = insert(T->left, k);
  else T->right = insert(T->right, k);
  fix_size(T);
  return T;
}

// returns a vector of key values corresponding to the inorder traversal of T (i.e., the contents of T in sorted order)
vector<int> inorder_traversal(Node *T)
{
  vector<int> inorder;
  vector<int> rhs;
  if (T == nullptr) return inorder;
  inorder=inorder_traversal(T->left);
  inorder.push_back(T->key);
  rhs=inorder_traversal(T->right);
  inorder.insert(inorder.end(), rhs.begin(), rhs.end());
  return inorder;
}

// return a pointer to the node with key k in tree T, or nullptr if it doesn't exist
Node *find(Node *T, int k)
{
  //Implement Node *find(Node *T, int k)

  if (T == NULL) return NULL;  
if (k == T->key) return T;
if (k < T->key) return find(T->left, k);  
else return find(T->right, k);
}

// return pointer to node of rank r (with r'th largest key; e.g. r=0 is the minimum)
Node *select(Node *T, int r)
{
  assert(T!=nullptr && r>=0 && r<T->size);
  // assert(T->left != nullptr || T->right != nullptr);
  //Implement Node *select(Node *T, int r)

  int rank = 0; // initialize rank to 0
  
  if (T->left != nullptr) { // if there is a left child of T
    rank = T->left->size; // set rank to left child's size
  }

  if (rank == r) { // if rank is r
    return T; // return T
  }
  else if (r < rank && T->left != nullptr) { // if r is less than rank and left child is present
    return select(T->left, r); // recurse select left child and r
  }
  else if (T->right != nullptr) { // if right child is present
    return select(T->right, r - rank - 1); // recursively call right and minus r by rank - 1
  }
}

// Join trees L and R (with L containing keys all <= the keys in R)
// Return a pointer to the joined tree.  
Node *join(Node *L, Node *R)
{
  // choose either the root of L or the root of R to be the root of the joined tree
  // (where we choose with probabilities proportional to the sizes of L and R)

  // return R if L is null
  if (L == nullptr) { 
    return R;
  }
  // return L if R is null
  if (R == nullptr) {
    return L;
  }

  // initialize random number and probability
  double random = rand() / double(RAND_MAX); 
  double probability = L->size / (L->size + R->size);

  // if the random is less or equal to probability 
  if (random <= probability) {
    // recursively call and set right child of L to joining of R and right child of L
    L->right = join(R, L->right); 
    fix_size(L); // fix L size
    return L; // return L
  }
  else { 
    // recursively call and set left child of R to joining of L and left child of R
    R->left = join(L, R->left); 
    fix_size(R); // fix R size
    return R; // return R
  }
}

// remove key k from T, returning a pointer to the resulting tree.
// it is required that k be present in T

// helper function to find minimum of a tree
Node *findMinimum(Node *T) {
  while (T->left) { // go left until null
    T = T->left;
  }
  return T; // return T
}

// functiont to remove a node and return resulting tree
Node *remove(Node *T, int k)
{
  assert(T != nullptr);
  
  //Implement Node *remove(Node *T, int k)
  // if target is less than current key
  if (k < T->key) {
    T->left = remove(T->left, k); // recursively call remove on left child and set to left child of T
  }
  // if target is greater than current key
  else if (k > T->key) {  
    T->right = remove(T->right, k); // recursively call remove on right child and set to right child of T
  }
  // if target is equal to key
  else if (T->key == k) {
    if (T->left == nullptr && T->right == nullptr) { // if both children are null
      delete T; // delete T
      return nullptr; // return nullptr
    }
    // if left child is null
    else if (T->left == nullptr) {
      Node *resultT = T->right; // temporary variable to right of T
      delete T; // delete T
      return resultT; // return right child of T
    }
    // if right child is null
    else if (T->right == nullptr) {
      Node *resultT = T->left; // temporary variable to left of T
      delete T; // delete T
      return resultT; // return left child of T
    }
    // if both children are null
    else if (T->left != nullptr && T->right != nullptr) {
      Node *minimumRight = findMinimum(T->right); // temporary variable to minimum of right of T
      T->key = minimumRight->key; // set current key to right minimum of T
      T->right = remove(T->right, T->key); // recursively call remove on right of T and set to right child of T
    }
  }
  fix_size(T); // fix T size
  return T; // return T
}

// Split tree T on key k into tree L (containing keys <= k) and a tree R (containing keys > k)
void split(Node *T, int k, Node **L, Node **R)
{
  // T is null set L and R to nullptr
  if (T == nullptr) {
    *L = nullptr;
    *R = nullptr;
    return;
  }

  // if k less than key
  if (T->key > k) {
    split(T->left, k, L, &T->left); // split left tree and pass left tree address instead of R
    *R = T; // set R to tree
    fix_size(T); // fix T size
  }
  else {
    split(T->right, k, &T->right, R); // split right tree and pass right tree address instead of L
    *L = T; // set L to T
    fix_size(T); // fix T size
  }
  //Implement void split(Node *T, int k, Node **L, Node **R)
}

// insert key k into the tree T, returning a pointer to the resulting tree
Node *insert_random(Node *T, int k)
{
  // If k is the Nth node inserted into T, then:
  // with probability 1/N, insert k at the root of T
  // otherwise, insert_random k recursively left or right of the root of T
  
  //Implement Node *insert_random(Node *T, int k)

  if (!T) { // return new node initialized to k if T is null
    return new Node(k);
  }

  int random = rand() % (T->size + 1); // random mod by size + 1

  // if modded value is 0
  if (random == 0) {
    Node *insertNode = new Node(k); // create new node with key k
    Node *tempLeft = nullptr; // temporary left node
    Node *tempRight = nullptr; // temporary right node
    split(T, k, &tempLeft, &tempRight); // split and store left and right to temporary nodes
    T = insertNode; // set T to temp new node

    insertNode->left = tempLeft; // set left to temporary left node
    insertNode->right = tempRight; // set right to temporary right node
        
    fix_size(T); // fix size T
    return T; // return T
  }
  else {
    if (k >= T->key) { // if k is greater or equal to key
      T->right = insert_random(T->right, k); // recursively call right child
      fix_size(T); // fix T size
    }
    else {
      T->left = insert_random(T->left, k); // recursively call left child
      fix_size(T); // fix T size
    }
  }
  fix_size(T); // fix T size
  return T; // return T
}

void printVector(vector<int> v)
{
    for (int i=0; i<v.size(); i++)
    {
        cout << v[i] << endl;
    }
}