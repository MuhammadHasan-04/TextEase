#pragma once
#include<iostream>
using namespace std;

class NodeTree {
public:
    char ch;
    NodeTree* pointers[26];
    NodeTree* next;

    NodeTree() {
        ch = '\0';
        for (int i = 0; i < 26; ++i) {
            pointers[i] = nullptr;
        }
        next = nullptr;
    }

    NodeTree(char character) : ch(character) {
        for (int i = 0; i < 26; ++i) {
            pointers[i] = nullptr;
        }
        next = nullptr;
    }

    ~NodeTree() {
        for (int i = 0; i < 26; ++i) {
            delete pointers[i];
        }
        delete next;
    }
};

class Tree
{
private:
    NodeTree* root;
public:
    Tree() {
        root = new NodeTree();
    }

    ~Tree() {
        delete root;
    }

    void Insertion(string ch)
    {
        if (ch.empty()) return;

        int charAscii = 0;

        if (ch[0] >= 'A' && ch[0] <= 'Z')
        {
            charAscii = int(ch[0]) - 65;
        }

        else if (ch[0] >= 'a' && ch[0] <= 'z') {

            charAscii = int(ch[0]) - 97;

        }

        if (root->pointers[charAscii] == nullptr)
        {
            NodeTree* newNodeTree = new NodeTree(ch[0]);
            root->pointers[charAscii] = newNodeTree;
            NodeTree* temp = root->pointers[charAscii];

            int i = 1;
            while (ch[i] != '\0') {
                NodeTree* tempNodeTree = new NodeTree(ch[i]);
                temp->next = tempNodeTree;
                temp = temp->next;
                i++;
            }

        }
        else
        {
            NodeTree* temp = root->pointers[charAscii];
            int count = matchFunc(temp, ch);
            int start = count;
            while (count > 0)
            {
                temp = temp->next;
                count--;

            }

            charAscii = 0;

            if (ch[start] >= 'A' && ch[start] <= 'Z')
            {
                charAscii = int(ch[start]) - 65;
            }

            else if (ch[start] >= 'a' && ch[start] <= 'z') {

                charAscii = int(ch[start]) - 97;

            }

            NodeTree* tempNodeTree = new NodeTree(ch[start]);
            temp->pointers[charAscii] = tempNodeTree;
            temp = tempNodeTree;
            start++;

            while (ch[start] != '\0') {

                NodeTree* newNodeTree = new NodeTree(ch[start]);
                temp->next = newNodeTree;
                temp = temp->next;
                start++;
            }

        }


    }

    int matchFunc(NodeTree* node, string s1) {
        if (s1.empty()) {
            cout << "Error: Input string is empty." << endl;
            return 0;
        }

        NodeTree* temp = node;
        string str = "";

        while (temp != nullptr) {
            str += temp->ch;
            temp = temp->next;
        }

        int count = 0;
        for (int i = 0, j = 0; s1[i] != '\0'; i++, j++) {

            if (s1[i] != str[j] && i > 0) {
                return count;
            }
            count++;
        }

        return 0;
    }

    void printTree(NodeTree* node, string prefix = "", bool isLast = true) {
        if (node != nullptr) {
            if (!prefix.empty()) {
                cout << (isLast ? "    " : "|   ");
                cout << (isLast ? "└── " : "┌── ") << node->ch;
                cout << endl;
            }

            bool hasChildren = false;
            for (int i = 0; i < 26; ++i) {
                if (node->pointers[i] != nullptr) {
                    hasChildren = true;
                    printTree(node->pointers[i], prefix + (isLast ? "    " : "|   "), false);
                }
            }

            if (node->next != nullptr) {
                string newPrefix = prefix + (hasChildren ? "|   " : "    ");
                bool nextIsLast = (hasChildren) ? false : isLast;
                printTree(node->next, newPrefix, nextIsLast);
            }
        }
    }

    void printTree() {
        for (int i = 0; i < 26; ++i) {
            if (root->pointers[i] != nullptr) {
                printTree(root->pointers[i]);
            }
        }
    }
};

//int main() {
//    Tree tree;
//    tree.Insertion("Hello");
//    tree.Insertion("World");
//    tree.Insertion("Hell");
//    tree.Insertion("Halloween");
//    cout << "Printed Tree:" << endl;
//    tree.printTree();
//    return 0;
//}

/*
Printed Tree:
┌── H
│   ├── └── e
│   │   ├── └── l
│   │   │   ├── └── l
│   │   │   │   ├── └── o (Hello)
│   │   │   │   └── └── w
│   │   │   │       ├── └── e
│   │   │   │       │   ├── └── e
│   │   │   │       │   │   └── n (Halloween)
│   │   │   └── (Hell)
├── W
│   ├── └── o
│   │   ├── └── r
│   │   │   └── └── l
│   │   │       └── └── d (World)
└── (Root)
    ├── H (above)
    ├── W (above)
    └── (Other empty roots: A to G, I to V, X to Z)
*/