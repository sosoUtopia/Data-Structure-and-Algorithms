/*
 * Name: Alvin Yang
 * Date Submitted: 10/18/2022
 * Lab Section: 001
 * Assignment Name: Spell Checker Using a Hash Table
 */
 
 #include "stringset.h"
 #include <iostream>
 #include <fstream>
 
 void testStringset(Stringset& words);
 void loadStringset(Stringset& words, string filename);
 vector<string> spellcheck(const Stringset& words, string word);
 
 void testStringset(Stringset& words)
 {
     string choice;
     string word;
     do
     {
         cout << "I: insert word" << endl;
         cout << "F: find word" << endl;
         cout << "R: remove word" << endl;
         cout << "P: print words in stringset" << endl;
         cout << "Q: quit" << endl;
         cin >> choice;
         switch (choice[0])
         {
             case 'I':
             case 'i':
                cout << "Enter word to insert: ";
                cin >> word;
                words.insert(word);
                break;
             case 'F':
             case 'f':
                cout << "Enter word to find: ";
                cin >> word;
                if (words.find(word))
                {
                    cout << word << " in stringset" << endl;
                }
                else
                {
                    cout << word << " not in stringset" << endl;
                }
                break;
             case 'R':
             case 'r':
                cout << "Enter word to remove: ";
                cin >> word;
                words.remove(word);
                break;
             case 'P':
             case 'p':
                vector<list<string>> t = words.getTable();
                int numWords = words.getNumElems();
                int tSize = words.getSize();
                for(int i=0; i<tSize; ++i)
                {
                    list<string>::iterator pos;
                    for (pos = t[i].begin(); pos != t[i].end(); ++pos)
                    {
                        cout << *pos << endl;
                    }
                }
                cout << "Words: " << numWords << endl;
         }
     } while (choice[0] != 'Q' && choice[0] != 'q');
 }
 
 // function to load file into word object
 void loadStringset(Stringset& words, string filename)
 {
    ifstream wordFile; // create file object
    wordFile.open(filename); // open file
    string word; // string to store word

    while (wordFile >> word) { // while reading a word
        words.insert(word); // insert word into stringset obj
    }

    wordFile.close(); // close file
    
 }
 
 // function for spellchecking 
 vector<string> spellcheck(const Stringset& words, string word)
 {
    string tempString; // temporary string
    vector<std::string> alternatives; // list of alternatives for word

    // loop through word
    for (int i = 0; i < word.length(); i++) {
        tempString = word; // set temporary string into word
        // loop from a to z
        for (char ch = 'a'; ch <= 'z'; ch++) {
            tempString[i] = ch; // change character of temporary string at index to another character
            // if the character is not equivalent to the character of original word at index, and found tempString
            if (ch != word[i] && words.find(tempString)) {  
                alternatives.push_back(tempString); // push back temporary string into vector
            }
        }
    }
    return alternatives; // return alternatives list
 }