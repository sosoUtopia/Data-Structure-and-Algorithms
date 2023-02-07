/*
 * Name: Alvin Yang
 * Date Submitted: 11/13/2022
 * Lab Section: 001
 * Assignment Name: Storing a Sequence in a Binary Search Tree
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

#include "bstSequence.h"

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


// insert value v at rank r
Node *insert(Node *T, int v, int r)
{
  //Implement Node *insert(Node *T, int v, int r)
  if (T == nullptr) return new Node(v);

  int rank = 0;
  if (T->left != nullptr) {
    rank = T->left->size;
  }

  if (r <= rank) {
    T->left = insert(T->left, v, r);
  }
  else {
    T->right = insert(T->right, v, r-rank-1);
  }
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

// return pointer to node of rank r (with r'th largest key; e.g. r=0 is the minimum)
Node *select(Node *T, int r)
{
  assert(T!=nullptr && r>=0 && r<T->size);

  int rank_of_root = T->left ? T->left->size : 0;
  if (r == rank_of_root) return T;
  if (r < rank_of_root) return select(T->left, r);
  else return select(T->right, r - rank_of_root - 1);
}

// Split tree T on rank r into tree L (containing ranks < r) and 
// a tree R (containing ranks >= r)
void split(Node *T, int r, Node **L, Node **R)
{
  //Implement void split(Node *T, int r, Node **L, Node **R)

  // if T is nullptr, initialize L and R to nullptr then return
  if (T == nullptr) {
    *L = nullptr;
    *R = nullptr;
    return;
  }

  int rank = 0; // initialize rank to 0
  if (T->left != nullptr) { // set rank to the size of the left node if left not empty
    rank = T->left->size;
  }

  // if r is less than rank 
  if (r <= rank) {
    split(T->left, r, L, &T->left); // recursively split left tree 
    *R = T; // set R to the T 
    fix_size(T); // fix size of T
  }
  else {
    split(T->right, r - rank - 1, &T->right, R); // recursively split right tree
    *L = T; // set L to the T
    fix_size(T); // fix size of T
  }
}

// insert value v at rank r
Node *insert_random(Node *T, int v, int r)
{
  // If (v,r) is the Nth node inserted into T, then:
  // with probability 1/N, insert (v,r) at the root of T
  // otherwise, insert_random (v,r) recursively left or right of the root of T
  
  //Implement Node *insert_random(Node *T, int v, int r)

  // return new node with value v if T is nullptr
  if (!T) { 
    return new Node(v);
  }

  int random = rand() % (T->size + 1); // random number mod by size of T + 1

  // if resulting number is 0
  if (random == 0) {
    Node *insertNode = new Node(v); // create new node
    Node *tempLeft = nullptr; // temp node for left tree
    Node *tempRight = nullptr; // temp node for right tree
    split(T, r, &tempLeft, &tempRight); // split and store left and right tree
    T = insertNode; // assign T to new node 
    T->left = tempLeft; // connect left tree to T
    T->right = tempRight; // connect right tree to T
    fix_size(T); // fix T size
    return T; // return T
  }
  else {
    int rank = 0; // set rank to 0
    if (T->left != nullptr) { // set rank to left tree size if not nullptr
      rank = T->left->size;
    }
    if (r <= rank) { // if r is less or equal to rank
      T->left = insert_random(T->left, v, r); // recursively call insert random to left tree
    }
    else { // if r greater than tank
      T->right = insert_random(T->right, v, r - rank - 1); // recursively call insert random to right tree
    }
  }
  fix_size(T); // fix T size
  return T; // return T
}

// Returns true if team x defeated team y
// Leave this function unmodified
bool did_x_beat_y(int x, int y)
{
  assert (x != y);
  if (x > y) return !did_x_beat_y(y,x);
  unsigned long long lx = x;
  unsigned long long ly = y;
  return ((17 + 8321813 * lx + 1861 * ly) % 1299827) % 2 == 0;
}

// Return a BST containing a valid ordering of n teams
Node *order_n_teams(int n)
{
  Node *T = nullptr;

  // start by inserting the first team
  T = insert_random(T, 0, 0);

  // now insert the other teams...
  for (int i=1; i<n; i++) {
    // insert team i so the sequence encoded by the BST remains valid
    if (did_x_beat_y(i, select(T,0)->key)) // can we insert at beginning?
      T = insert_random(T, i, 0);
    else if (did_x_beat_y(select(T,T->size-1)->key, i)) // can we insert at end?
	    T = insert_random(T, i, T->size);
    // if we cannot insert at beginning nor at the end
    else {
	    int high = T->size; // set high bound
      int low = 0; // set low bound
      int mid; // set mid
      // while high bound is greater than low bound
      while (high > low) {
        mid = (low + high) / 2; // mid is in the middle high and low bound
        // if i beats the middle ranked node and loses against the previous node
        if (did_x_beat_y(i, select(T, mid)->key) && did_x_beat_y(select(T, mid-1)->key, i)) {
          T = insert_random(T, i, mid); // insert random at the middle, get in between W and L
          break; // break from loop
        }
        else {
          // set high bound to middle if i beats middle but not against the previous node
          if (did_x_beat_y(i, select(T, mid)->key)) {
            high = mid;
          }
          // else set low bound to middle + 1
          else {
            low = mid + 1;
          }
        }
      }
    }
  }
  return T;
}

void printVector(vector<int> v)
{
    for (int i=0; i<v.size(); i++)
    {
        cout << v[i] << endl;
    }
}