#pragma once
#include <iostream>
#include <fstream>
#include <cstring> 
#include <cctype> 
#include<string.h>
#include <string>



using namespace std;

const int MAX_SUGGESTIONS = 10; // Max number of suggestions
const int MAX_SUGGESTION_LENGTH = 100; // Max length of a suggestion

class NodeList
{
public:
    char ch;
    NodeList* ptr[26]; 
    bool isEndOfWord;  

    NodeList() : ch('\0'), isEndOfWord(false)
    {
        for (int i = 0; i < 26; ++i)
        {
            ptr[i] = nullptr;
        }
    }
};

class SuggTree
{
private:
    NodeList* root;

public:
    SuggTree() : root(new NodeList()) {}

    ~SuggTree()
    {
        delete root;
    }

    void readFile(string filename)
    {
        ifstream file(filename);

        if (!file.is_open())
        {
            cout << "Error opening file!" << endl;
            return;
        }

        // Read the entire file into a character array (fixed size buffer)
        file.seekg(0, ios::end);
        int fileSize = file.tellg();
        file.seekg(0, ios::beg);
        char* buffer = new char[fileSize + 1];
        file.read(buffer, fileSize);
        buffer[fileSize] = '\0'; // Null terminate the string
        file.close();

        // Process the file character by character
        int i = 0;
        while (buffer[i] != '\0')
        {
            // Skip any non-alphabetic characters
            while (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t')
                i++;

            // Now 'i' points to the start of a word, we will extract the word
            if (buffer[i] != '\0')
            {
                int start = i;
                while (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0')
                    i++; // Find the end of the word

                // Create a new word from the characters between start and i
                char word[100]; // Max word length can be 100
                int wordLength = 0;
                for (int j = start; j < i; ++j)
                {
                    word[wordLength++] = buffer[j];
                }
                word[wordLength] = '\0'; // Null terminate the word

                insert(word); // Insert the word into the trie
            }
        }

        delete[] buffer; // Clean up the buffer
    }

    // Insert a word into the trie
    void insert(const char* word)
    {
        NodeList* current = root;
        for (int i = 0; word[i] != '\0'; ++i)
        {
            char ch = word[i];
            int index = ch - 'a'; // Convert character to index (0 for 'a', 1 for 'b', etc.)

            // Ensure that we're dealing with lowercase letters
            if (index < 0 || index >= 26)
                continue;

            if (current->ptr[index] == nullptr)
            {
                current->ptr[index] = new NodeList();
                current->ptr[index]->ch = ch;
            }

            current = current->ptr[index];
        }

        current->isEndOfWord = true;
    }

    // Function to find all words starting with a given prefix and return them as suggestions
    void getSuggestions(const string& prefix, char suggestions[MAX_SUGGESTIONS][MAX_SUGGESTION_LENGTH], int& count) {
        NodeList* node = root;  // Assuming `root` is the root of the Trie
        count = 0;

        // Traverse the Trie according to the prefix
        for (char ch : prefix) {
            int index = ch - 'a';  // Assuming lowercase letters
            if (node->ptr[index] == nullptr) {
                return;  // No suggestions found if the prefix isn't in the Trie
            }
            node = node->ptr[index];
        }

        // Now we are at the end of the prefix, recursively collect suggestions
        collectSuggestions(node, prefix, suggestions, count);
    }

    void collectSuggestions(NodeList* node, string currentPrefix, char suggestions[MAX_SUGGESTIONS][MAX_SUGGESTION_LENGTH], int& count) {
        if (node == nullptr || count >= MAX_SUGGESTIONS) {
            return;
        }

        // If we reach a node that is the end of a word, add the current prefix to suggestions
        if (node->isEndOfWord) {
            strcpy_s(suggestions[count++], currentPrefix.c_str());
        }

        // Recursively search for all possible words
        for (int i = 0; i < 26; ++i) {
            if (node->ptr[i] != nullptr) {
                collectSuggestions(node->ptr[i], currentPrefix + char(i + 'a'), suggestions, count);
            }
        }
    }


    // Function to find the node that matches the prefix
    NodeList* findPrefixNode(const char* prefix)
    {
        NodeList* current = root;
        for (int i = 0; prefix[i] != '\0'; ++i)
        {
            char ch = prefix[i];
            int index = ch - 'a'; // Convert character to index

            // Ensure that we're dealing with lowercase letters
            if (index < 0 || index >= 26)
                return nullptr;

            if (current->ptr[index] == nullptr)
                return nullptr;

            current = current->ptr[index];
        }

        return current; // Return the node where the prefix ends
    }

    bool search(const std::string& word) {
        NodeList* current = root;
        for (char ch : word) {
            int index = ch - 'a'; // Convert character to index (assuming lowercase letters)
            if (index < 0 || index >= 26) {
                // If not a lowercase letter, return false (or you can handle uppercase/conversion as needed)
                return false;
            }
            if (current->ptr[index] == nullptr) {
                // If no node exists for the current character, word not found
                return false;
            }
            current = current->ptr[index];
        }
        // If we've reached this point, we've traversed the entire word
        // Return whether the final node marks the end of a word
        return current->isEndOfWord;
    }

    void printTree() {
        cout << "Trie Structure:" << endl;
        printTreeHelper(root, "");  // Start with the root and an empty prefix 
    }

    void printTreeHelper(NodeList* node, const string& currentPrefix) {
        if (node == nullptr)
            return;

        // If this node marks the end of a word, print the word
        if (node->isEndOfWord) {
            cout << currentPrefix << endl;
        }

        // Recursively visit all child nodes
        for (int i = 0; i < 26; ++i) {
            if (node->ptr[i] != nullptr) {
                // Append the current character to the prefix and recurse
                printTreeHelper(node->ptr[i], currentPrefix + char(i + 'a'));
            }
        }
    }
};

