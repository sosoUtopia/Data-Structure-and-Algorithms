/*
 * Name: Alvin Yang
 * Date Submitted: 10/18/22
 * Lab Section: 001
 * Assignment Name: Spell Checker Using a Hash Table
 */

#include "stringset.h"
#include <algorithm>

Stringset::Stringset() : table(4), num_elems(0), size(4) {} 

//Used in test cases and testStringset() in main.cpp, do not modify 
vector<list<string>> Stringset::getTable() const
{
    return table;
}

//Used in test cases and testStringset() in main.cpp, do not modify 
int Stringset::getNumElems() const
{
    return num_elems;
}

//Used in test cases and testStringset() in main.cpp, do not modify 
int Stringset::getSize() const
{
    return size;
}

// function to insert word
void Stringset::insert(string word)
{
    if (find(word)) return; // if word already exists, return

    size_t num; // variable for hash value

    // if the number of elements is greater than size
    if (num_elems >= size) {
        size = size * 2; // multiply size by 2
        vector<list<string>> newTable(size); // create new table
        list<string>::iterator it;// iterator of string to loop through linked list of old table
        // loop through old table
        for (int i = 0; i < (int)table.size(); i++) {
            // loop through linked list at the index of old table
            for (it = table[i].begin(); it != table[i].end(); it++) {
                num = hash<string>{}(*it); // get hash value
                int hashIndex = (unsigned) num % size; // get hash index
                newTable[hashIndex].push_front(*it); // push string at current index of linked list of old table into linked list of new table 
            }
        }  
        table = newTable; // assign newTable into table
    }
    // if the number of elements is less or equal to size
    if (num_elems <= size) {
        num = hash<string>{}(word); // get hash value
        int hashIndex = (unsigned) num % size; // get hash index
        table[hashIndex].push_front(word); // push word into hash index
        num_elems++; // increment number of elements
    }
}

// function to return if word is in table
bool Stringset::find(string word) const
{
    size_t num = hash<string>{}(word); // get hash value
    int hashIndex = (unsigned) num % size; // get hash index
    // iterate through the linked list and find word in list
    return std::find(table[hashIndex].begin(), table[hashIndex].end(), word)  
    != table[hashIndex].end(); // return if iterated to the end and not found word
}

// function to remove word from table
void Stringset::remove(string word)
{
    // if word is not found, then return 
    if (!find(word)) {
        return;
    }

    size_t num = hash<string>{}(word); // get hash value
    int hashIndex = (unsigned) num % size; // get hash index
    list<string>::iterator it; // iterator to iterate through linked list
    //iterate linked list
    for (it = table[hashIndex].begin(); it != table[hashIndex].end(); it++) {
        if (*it == word) { // if word is equal to value of iterator
            table[hashIndex].erase(it); // erase the iterator
            num_elems--; // decrement number of elmeents
            break; // break from loop
        }
    }
}