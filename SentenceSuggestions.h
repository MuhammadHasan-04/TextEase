#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <string>  // To use std::string

using namespace std;

const int MAX_SUGGESTIONS1 = 10;  // Max number of suggestions
const int MAX_SUGGESTION_LENGTH1 = 256;  // Max length of a suggestion
const int MAX_SENTENCE_LENGTH = 256;  // Max length of a sentence

// Define the NodeList2 class (used for constructing the Trie)
class NodeList2 {
public:
    char ch;
    NodeList2* ptr[26];  // Only support lowercase letters a-z
    bool isEndOfSentence;
    char sentence[MAX_SENTENCE_LENGTH];  // Stores the complete sentence

    NodeList2() : ch('\0'), isEndOfSentence(false) {
        for (int i = 0; i < 26; ++i) {
            ptr[i] = nullptr;
        }
        sentence[0] = '\0';  // Initialize sentence to an empty string
    }
};

// Define the SentenceCompletionTree class
class SentenceCompletionTree {
private:
    NodeList2* root;

    // Utility function to convert a string to lowercase
    void toLowerCase(string& str) {
        for (char& ch : str) {
            ch = tolower(ch);
        }
    }

public:
    SentenceCompletionTree() : root(new NodeList2()) {}

    ~SentenceCompletionTree() {
        delete root;
    }

    // Read sentences from a file and insert them into the tree
    void readSentencesFromFile(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }

        string sentence;
        while (getline(file, sentence)) {
            // Convert the sentence to lowercase before inserting
            toLowerCase(sentence);
            insertSentence(sentence.c_str());
        }
        file.close();
    }

    // Insert a sentence into the tree
    void insertSentence(const char* sentence) {
        NodeList2* current = root;
        int i = 0;

        while (sentence[i] != '\0') {
            if (sentence[i] == ' ') {  // Space indicates end of word, but not sentence
                current = root;  // Restart from root for the next word in the sentence
            }
            else {
                int index = sentence[i] - 'a';
                if (index < 0 || index >= 26) {
                    ++i;
                    continue;  // Ignore non-lowercase letters
                }
                if (current->ptr[index] == nullptr) {
                    current->ptr[index] = new NodeList2();
                    current->ptr[index]->ch = sentence[i];
                }
                current = current->ptr[index];
            }
            ++i;
        }

        current->isEndOfSentence = true;
        // Manually copy sentence to the node's sentence array
        int j = 0;
        while (sentence[j] != '\0' && j < MAX_SENTENCE_LENGTH - 1) {
            current->sentence[j] = sentence[j];
            ++j;
        }
        current->sentence[j] = '\0';  // Null-terminate the sentence
    }

    // Function to find all sentence suggestions starting with a given prefix
    void getSentenceSuggestions(const char* prefix, char suggestions[MAX_SUGGESTIONS1][MAX_SUGGESTION_LENGTH1], int& count) {
        NodeList2* node = root;
        count = 0;

        // Convert the prefix to lowercase before checking
        string lowerPrefix = prefix;
        toLowerCase(lowerPrefix);

        int i = 0;
        while (lowerPrefix[i] != '\0') {
            int index = lowerPrefix[i] - 'a';
            if (node->ptr[index] == nullptr) {
                return;  // No suggestions found if the prefix isn't in the tree
            }
            node = node->ptr[index];
            ++i;
        }

        collectSentenceSuggestions(node, suggestions, count);
    }

    // Recursively collect sentence suggestions
    void collectSentenceSuggestions(NodeList2* node, char suggestions[MAX_SUGGESTIONS1][MAX_SUGGESTION_LENGTH1], int& count) {
        if (node == nullptr || count >= MAX_SUGGESTIONS1) return;

        // If we reach a node that is the end of a sentence, add the current sentence as a suggestion
        if (node->isEndOfSentence) {
            // Manually copy the sentence to the suggestions array
            int i = 0;
            while (node->sentence[i] != '\0' && i < MAX_SUGGESTION_LENGTH1 - 1) {
                suggestions[count][i] = node->sentence[i];
                ++i;
            }
            suggestions[count][i] = '\0';  // Null-terminate the suggestion
            ++count;
        }

        // Continue to explore all possible sentence completions
        for (int i = 0; i < 26; ++i) {
            if (node->ptr[i] != nullptr) {
                collectSentenceSuggestions(node->ptr[i], suggestions, count);
            }
        }
    }

    // Function to print suggestions based on input prefix
    void printSuggestions(const char* inputPrefix) {
        char suggestions[MAX_SUGGESTIONS1][MAX_SUGGESTION_LENGTH1];
        int count = 0;

        // Convert the input prefix to lowercase before processing
        string lowerPrefix = inputPrefix;
        toLowerCase(lowerPrefix);

        NodeList2* node = root;
        int i = 0;
        while (lowerPrefix[i] != '\0') {
            int index = lowerPrefix[i] - 'a';
            if (node->ptr[index] == nullptr) {
                cout << "No suggestions found for '" << inputPrefix << "'." << endl;
                return;
            }
            node = node->ptr[index];
            ++i;
        }

        collectSentenceSuggestions(node, suggestions, count);

        // Print the collected suggestions
        if (count > 0) {
            for (int i = 0; i < count; ++i) {
                cout << suggestions[i] << endl;
            }
        }
        else {
            cout << "No sentence completion suggestions found for '" << inputPrefix << "'." << endl;
        }
    }

    // Function to print the tree structure
    void printTreeStructure(NodeList2* node, int level = 0) {
        if (node == nullptr) return;

        // Print the current node
        cout << string(level * 4, ' ') << (node->ch ? node->ch : '*') << endl;

        for (int i = 0; i < 26; ++i) {
            if (node->ptr[i] != nullptr) {
                printTreeStructure(node->ptr[i], level + 1);
            }
        }
    }

    // Function to print the entire tree from the root
    void printTree() {
        printTreeStructure(root);
    }

    string collectAllSentences(NodeList2* node) {
        string result;

        if (node == nullptr) return result;

        // If we reach a node that is the end of a sentence, add the current sentence to result
        if (node->isEndOfSentence) {
            if (!result.empty()) {
                result += " ";  // Add a space between sentences
            }
            result += node->sentence;
        }

        // Continue to explore all possible sentence completions
        for (int i = 0; i < 26; ++i) {
            if (node->ptr[i] != nullptr) {
                result += collectAllSentences(node->ptr[i]);
            }
        }

        return result;
    }

    // Function to return all sentences in a single string
    string printTreeAsString() {
        return collectAllSentences(root);
    }
};

//// Example main function to load sentences and print them
//int main() {
//    // Create the SentenceCompletionTree
//    SentenceCompletionTree tree;
//
//    // Read sentences from a file (you can replace this with any valid filename)
//    tree.readSentencesFromFile("sentences.txt");  // Load sentences from a file
//
//    // Print the tree structure
//    cout << "Tree structure of the Sentence Completion Trie:" << endl;
//    tree.printTree();
//
//    cout << "\nEnter a sentence prefix for completion suggestions (e.g., 'hello i'):" << endl;
//    char inputPrefix[MAX_SENTENCE_LENGTH];
//    cin.getline(inputPrefix, MAX_SENTENCE_LENGTH);
//
//    tree.printSuggestions(inputPrefix);  // Print sentence suggestions based on the prefix
//
//    return 0;
//}
