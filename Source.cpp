#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include "StackList.h"
#include"Searching.h"
#include"WordSuggestion.h"
#include"SentenceSuggestions.h" 
#include <cstring>


using namespace std;

StackList Undo;
StackList Redo;

struct LoadFiles {
    string arr;
    LoadFiles* next;
};

LoadFiles* firstFile = nullptr;

void borderform(int width, int height);
void xycorrdinate(int x, int y);

class Node {
public:
    char data;
    Node* up;
    Node* down;
    Node* left;
    Node* right;
    Node() :data(), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
    Node(char val) : data(val), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};

class WriterAndEditor {
private:
    Node* head;
    Node* RowPointer;
    Node* mover;
    const int maxWidth;
    const int maxHeight;
    int width;
    string text;
    int moverX;
    int moverY;
    string check;
    bool flag;
    string inssertion;
    SuggTree suggestions;
    string byteByte;
    SentenceCompletionTree senTree;

public:
    WriterAndEditor() : head(new Node('\0')), RowPointer(head), mover(head), maxWidth(80), maxHeight(20), width(0), text(""), moverX(2), moverY(2) {

        suggestions.readFile("file.txt");
    }

    ~WriterAndEditor() {

        Node* RowPointer = head;

        while (RowPointer) {
            Node* current = RowPointer;
            RowPointer = RowPointer->down;

            while (current) {
                Node* temp = current;
                current = current->right;
                delete temp;
            }
        }
    }

    string convertListToString() {

        Node* row = head;
        string content;
        while (row != nullptr) {
            Node* temp = row->right;
            while (temp != nullptr) {
                content += temp->data;
                temp = temp->right;
            }
            content += '\n';
            row = row->down;
        }
        return content;
    }

    void ClearCurrentContent() {

        Node* RowPointer = head->down;
        while (RowPointer != nullptr) {
            Node* currentChar = RowPointer->right;
            while (currentChar != nullptr) {
                Node* tempChar = currentChar;
                currentChar = currentChar->right;
                delete tempChar;
            }
            Node* tempRow = RowPointer;
            RowPointer = RowPointer->down;
            delete tempRow;
        }

        head->right = nullptr;
        head->down = nullptr;

        mover = head;
        moverX = 2;
        moverY = 2;
        width = 0;

        system("cls");
        borderform(maxWidth, maxHeight);
        PrintCurrentLine();
    }


    void ListEverySpaceWord() {

        Node* row = head;
        text = "";
        while (row != nullptr) {
            Node* temp = row->right;

            while (temp != nullptr) {
                if (temp->data == ' ') {
                    if (!text.empty()) {
                        Undo.push(text);
                        text = "";
                    }
                }
                else {
                    text += temp->data;
                }

                temp = temp->right;
            }

            if (!text.empty()) {
                Undo.push(text);
                text = "";
            }

            row = row->down;
        }
    }

    void DeleteFromLast(int size) {

        Node* curr = mover;

        while (curr->right != nullptr) {
            curr = curr->right;
        }

        for (int i = 0; i < size; ++i) {
            if (curr->left != nullptr) {
                Node* temp = curr;
                curr = curr->left;
                delete temp;
                curr->right = nullptr;
            }
        }

        mover = curr;
    }

    void UndoStack() {
        if (Undo.isEmpty()) {
            return;
        }

        string lastWord = Undo.pop();
        int size = lastWord.size();
        DeleteFromLast(size);
        int nonspace = size;
        Node* temp = mover;
        for (; mover->left != nullptr && mover->data != ' ' && mover->data != '\n'; mover = mover->left) {
            size++;
        }


        DeleteFromLast(size - nonspace);
        Redo.push(lastWord);

        UpdateScreenAndCursor();

    }

    void RedoStack() {
        if (Redo.isEmpty()) {
            return;
        }

        string textToRedo = Redo.pop();
        Insertion(' ');

        size_t i = 0;
        while (i < textToRedo.size()) {
            Insertion(textToRedo[i]);
            i++;
        }


        Undo.push(textToRedo);

        UpdateScreenAndCursor();

    }


    void MoveCursorLeftByWord(int size) {
        while (size > 0 && mover->left != nullptr && mover->data != ' ') {
            mover = mover->left;
            size--;
        }
        width -= size;
    }

    void search() {
        string val;
        Node* row = head;
        int x = 2;
        int count = 2;
        char ch1;


        xycorrdinate(2, maxHeight + 3);
        cout << "Input the word You want to search: ";
        cin >> val;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White on Black
        int i = 0;
        while (row != nullptr) {
            Node* temp = row->right;
            xycorrdinate(2, count);
            while (temp != nullptr) {
                string textToCheck = "";
                Node* checkTemp = temp;

                while (checkTemp != nullptr && textToCheck.length() < val.length()) {
                    textToCheck += checkTemp->data;
                    checkTemp = checkTemp->right;
                    /* string e21;
                     cin >> ch1;
                     cout << "eNTER THE R";
                     if (ch1 == 'r') {
                         cin >> e21;
                         textToCheck += e21;
                     }*/
                }

                if (textToCheck == val) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5); // cyan on Black
                    // Highlight the entire word
                    for (int j = 0; j < val.length(); j++) {
                        cout << temp->data;
                        temp = temp->right;
                        /*xycorrdinate(2, maxHeight + 4);
                        char check;
                        string check2;
                        cout << "Do You Want to Replace ?? (Y/N) ";
                        cin >> check;
                        if (check == 'y' || check == 'Y') {
                            cout << "Enter the word : ";
                            cin >> check2;
                            int length1 = check2.length();
                            for (int i = 0; i < length1; i++) {
                                temp->data = check2[i];
                                temp = temp->right;
                            }
                        }*/
                    }
                    // Restore text color for non-matching characters
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White on Black
                }
                else {
                    cout << temp->data;
                    temp = temp->right;
                }
            }
            row = row->down;
            count++;


        }
    }

    Node* stringToList(string rep) {
        if (rep.empty()) return nullptr;

        Node* head = new Node();
        head->data = rep[0];
        Node* temp = head;

        for (int i = 1; i < rep.length(); i++) {
            temp->right = new Node();
            temp->right->data = rep[i];
            temp = temp->right;
        }
        return head;
    }

    void replace() {
        string valToReplace, replacement;
        Node* row = head;
        int x = 2;
        int count = 2;

        xycorrdinate(2, maxHeight + 3);
        cout << "Input the word You want to replace: ";
        cin >> valToReplace;

        xycorrdinate(2, maxHeight + 4);
        cout << "Input word you want to replace with: ";
        cin >> replacement;

        Node* replacementList = stringToList(replacement);

        // Traverse the original list to find and replace the word
        while (row != nullptr) {
            Node* temp = row->right;
            xycorrdinate(2, count);
            while (temp != nullptr) {
                string textToCheck = "";
                Node* checkTemp = temp;
                int matchLength = 0;

                // Check if the current sequence matches the word to replace
                while (checkTemp != nullptr && matchLength < valToReplace.length()) {
                    textToCheck += checkTemp->data;
                    checkTemp = checkTemp->right;
                    matchLength++;
                }

                if (textToCheck == valToReplace) {
                    // Highlight the matched word (optional)
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5); // Cyan on Black

                    // Actual Replacement Logic
                    Node* nextNode = checkTemp; // Node after the matched word
                    Node* repTemp = replacementList; // Temp pointer for replacement list

                    // Link the nodes before the matched word to the replacement list
                    temp->left->right = replacementList;

                    // Link the replacement list to the node after the matched word
                    while (repTemp->right != nullptr) {
                        repTemp = repTemp->right;
                    }
                    repTemp->right = nextNode;
                    if (nextNode != nullptr) {
                        nextNode->left = repTemp;
                    }

                    // Update temp to point after the replacement
                    temp = nextNode;

                    // Restore text color
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White on Black
                }
                else {
                    cout << temp->data;
                    temp = temp->right;
                }
            }
            row = row->down;
            count++;
        }
        UpdateScreenAndCursor(); // Update the screen after replacement
    }
    void PrintAllContent() {
        Node* RowPointer = head->down;
        while (RowPointer != nullptr) {
            Node* currentChar = RowPointer->right;
            while (currentChar != nullptr) {
                cout << currentChar->data;
                currentChar = currentChar->right;
            }
            cout << endl;
            RowPointer = RowPointer->down;
        }
    }



    void UpdateScreenAndCursor() {
        system("cls");
        borderform(maxWidth, 30);
        PrintCurrentLine();
    }

    void Insertion(char c) {

        if (c == '@') {  // Enter suggestion mode when '@' is pressed
            EnterSuggestionMode();
            return;
        }

        if (c == '\n' || width == maxWidth - 3) {
            EnterMovement();
            width = 0;
            return;
        }

        if (c == '%') {
            replace();
            return;
        }
        if (c == '*') {
            this->SentenceSuggestions();
            return;
        }

        if (c == '#') {
            search();
            return;
        }

        if (c != ' ' && width >= maxWidth - 4) {
            HandleLongWordInsertion();
        }

        Node* newNode = new Node(c);
        LinkNewNode(newNode);
        UpdateCursorAndWord(c);

        UpdateScreenAndCursor();
    }
    void EnterSuggestionMode() {
        string prefix = "";
        char ch;
        char suggestionsList[MAX_SUGGESTIONS][MAX_SUGGESTION_LENGTH]; // Array to hold suggestions
        int suggestionCount = 0;

        system("cls");
        borderform(maxWidth, maxHeight); // Display the border form
        PrintCurrentLine(); // Print the current line of text

        // Move the cursor to the bottom of the border form for input
        xycorrdinate(2, maxHeight + 1);
        cout << "\nEnter word prefix (after @): ";

        // Capture prefix input
        while (true) {
            ch = _getch();  // Get character input without Enter
            if (ch == '\r')  // Enter key to finish input
                break;
            if (ch == 8 && !prefix.empty()) {  // Backspace to delete a character
                prefix.pop_back();
                cout << "\b \b";  // Remove last character from screen
            }
            else if (ch != 8) {
                prefix += ch;
                cout << ch;  // Display character as it's typed
            }
        }

        // Fetch suggestions from the SuggestionTree
        suggestions.getSuggestions(prefix, suggestionsList, suggestionCount);

        if (suggestionCount > 0) {
            // Clear the input line and move the cursor to display suggestions
            xycorrdinate(2, maxHeight + 1);
            for (int i = 0; i < prefix.length() + 24; ++i) cout << " ";
            xycorrdinate(2, maxHeight + 1);

            // Display suggestions under the border form
            cout << "\nSuggestions:" << endl;
            for (int i = 0; i < suggestionCount; ++i) {
                xycorrdinate(4, maxHeight + 3 + i); // Adjust the y-coordinate for each suggestion
                cout << i + 1 << ". " << suggestionsList[i] << endl;
            }
            xycorrdinate(2, maxHeight + suggestionCount + 3); // Move the cursor below the suggestions
            cout << "Select a suggestion (number): ";

            // Get the selected suggestion number
            ch = _getch();  // Get character input without Enter
            int choice = ch - '0';  // Convert char to int

            if (choice > 0 && choice <= suggestionCount) {
                InsertSuggestion(suggestionsList[choice - 1]);  // Insert the chosen suggestion
            }
        }
        else {
            xycorrdinate(2, maxHeight + 1);
            for (int i = 0; i < prefix.length() + 24; ++i) cout << " ";
            xycorrdinate(2, maxHeight + 1);
            cout << "\nNo suggestions found for the prefix '" << prefix << "'." << endl;
            Sleep(1000); // Wait for 1 second before returning
        }

        UpdateScreenAndCursor(); // Update the screen and cursor position
    }

    void HandleLongWordInsertion() {
        width = 0;
        while (mover->left != nullptr && mover->left->data != ' ') {
            mover = mover->left;
        }
        EnterMovement();
    }

    void InsertSuggestion(const string& suggestion) {
        for (char c : suggestion) {
            Insertion(c);
        }
    }

    void LinkNewNode(Node* newNode) {

        if (mover->right != nullptr) {
            newNode->right = mover->right;
            mover->right->left = newNode;
        }
        mover->right = newNode;
        newNode->left = mover;
        mover = newNode;
        width++;
        moverX++;
    }

    void UpdateCursorAndWord(char c) {
        if (c == ' ') {
            if (!text.empty()) {
                Undo.push(text);
                text = "";
            }
        }
        else {
            text += c;
        }
    }

    void MoveCursor(const string& direction) {
        if (direction == "left" && mover->left != nullptr) {
            mover = mover->left;
            moverX--;
        }
        else if (direction == "right" && mover->right != nullptr) {
            mover = mover->right;
            moverX++;
        }
        else if (direction == "up" && RowPointer->up != nullptr) {
            RowPointer = RowPointer->up;
            MoveToSamePositionInRow();
            moverY--;
        }
        else if (direction == "down" && RowPointer->down != nullptr) {
            RowPointer = RowPointer->down;
            MoveToSamePositionInRow();
            moverY++;
        }

        xycorrdinate(moverX, moverY);
    }

    void MoveToSamePositionInRow() {

        Node* temp = mover;
        int len = 0;
        for (; temp->left != nullptr; temp = temp->left) len++;
        mover = RowPointer;
        for (int i = 0; i < len && mover->right != nullptr; mover = mover->right, i++);
        moverX = len + 2;
    }



    void EnterMovement() {

        if (moverY == 27) {
            return;
        }

        Node* newRow = new Node('\0');
        newRow->up = RowPointer;

        if (RowPointer->down != nullptr) {
            newRow->down = RowPointer->down;
            RowPointer->down->up = newRow;
        }
        RowPointer->down = newRow;

        Node* moveCursor = mover->right;
        mover->right = nullptr;

        Node* newRowCursor = newRow;

        while (moveCursor != nullptr) {
            Node* temp = moveCursor;
            moveCursor = moveCursor->right;

            newRowCursor->right = temp;
            temp->left = newRowCursor;
            newRowCursor = temp;
            width++;
        }

        RowPointer = newRow;

        MoveToSamePositionInRow();

        moverY++;

        MoveContentBelow(RowPointer);

        UpdateScreenAndCursor();
    }


    void MoveContentBelow(Node* RowPointer) {
        if (RowPointer->down != nullptr) {
            Node* tempCursor = mover;
            mover = RowPointer->down;

            while (mover->down != nullptr) {
                EnterMovement();
                mover = mover->down;
            }

            mover = tempCursor;
        }
    }



    void PrintCurrentLine() {

        Node* row = head;
        int x = 2;
        int count = 2;

        while (row != nullptr) {
            Node* temp = row->right;
            xycorrdinate(2, count);
            while (temp != nullptr) {
                cout << temp->data;
                temp = temp->right;
            }
            row = row->down;
            count++;
        }
    }

    void Deletion() {

        int counter = 0;
        bool check = true;
        bool flag = false;
        int index = -1;
        int ptr = flag;


        if (mover == RowPointer && RowPointer->up != nullptr) {
            RowPointer = RowPointer->up;
            mover = RowPointer;
            moverX = 2;
            moverY--;
            while (mover->right != nullptr) {
                mover = mover->right;
                moverX++;
            }
            width = moverX;
        }
        else if (mover->left != nullptr) {
            Node* temp = mover;
            mover = mover->left;
            mover->right = temp->right;
            if (temp->right != nullptr) {
                temp->right->left = mover;
            }
            moverX--;
            delete temp;
            width--;

            if (!text.empty()) {
                text.pop_back();
            }
        }

        for (int i = 0; i < 1; i++) {
            counter++;
        }

        UpdateScreenAndCursor();

        xycorrdinate(moverX, moverY);
    }

    void InsertExistingFileName(string fileName) {

        LoadFiles* newFile = new LoadFiles;
        newFile->arr = fileName;
        newFile->next = nullptr;

        if (firstFile == nullptr) {
            firstFile = newFile;
        }
        else {
            LoadFiles* temp = firstFile;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newFile;
        }
    }

    void createNewFile(string fileName) {
        fstream file(fileName + ".txt", ios::out);
        if (file.is_open()) {
            cout << "New file created: " << fileName << endl;
            file.close();
        }

    }

    void LoadExistingFiles(string file) {

        LoadFiles* temp = firstFile;

        while (temp != nullptr) {
            if (temp->arr == file) {

                ifstream file(temp->arr + ".txt");
                if (file.is_open()) {
                    char c;
                    while (file.get(c)) {
                        Insertion(c);
                    }
                    file.close();
                }
                else {
                    cout << "Error opening file: " << temp->arr << endl;
                }
                return;
            }
            temp = temp->next;
        }
        cout << "File not found in the list." << endl;
    }

    int totalFiles() {
        LoadFiles* temp = firstFile;
        int i = 1;
        while (temp != nullptr) {
            temp = temp->next;
            i++;
        }

        return i;
    }

    void printExistingFiles() {

        LoadFiles* temp = firstFile;
        int i = 1;
        while (temp != nullptr) {
            cout << i << ". " << temp->arr << endl;
            temp = temp->next;
            i++;
        }
    }

    void SentenceSuggestions() {
        this->byteByte = this->convertListToString();
        int length = this->byteByte.length();
        this->senTree.insertSentence(byteByte.c_str());

        system("cls");
        borderform(maxWidth, maxHeight);  
        PrintCurrentLine();  

        xycorrdinate(0, maxHeight + 3);

        this->senTree.printTree();
        int j = 1;
        cout << endl;
        cout << endl;
        cout << "In the sentence form: " << endl;
        cout << j << ".";

        for (int i = 0; i < length; i++) {
            if (this->byteByte[i] == '.') {
                cout << endl;
                j++;
                if (i != length - 1) {
                    cout << j << ".";  
                }
                else {
                    return;  
                }
            }
            else {
                cout << byteByte[i];  
            }
        }

        cout << "\nEnter the suggestion word for the sentence: ";
        string gen1;
        cin >> gen1; 

        for (char& c : gen1) {
            c = tolower(c); 
        }

        int len = gen1.length();
        string temp;
        bool found = false;  
        string result;

        for (int i = 0; i < length; i++) {
            if (byteByte[i] != ' ' && byteByte[i] != '.') {
                temp += tolower(byteByte[i]); 
            }
            else {
                if (temp == gen1) {
                    found = true;
                    result = "";  

                    for (int j = i + 1; j < length; j++) {
                        if (byteByte[j] == '.') { 
                            break;
                        }
                        if (byteByte[j] != ' ' && byteByte[j] != '.') {  
                            result += byteByte[j];
                        }
                        else if (byteByte[j] == ' ' && !result.empty() && result.back() != ' ') {
                            result += ' ';  
                        }
                    }

                    break;
                }

                temp = "";
            }
        }

        if (found) {
            cout << "Suggested sentence continuation: " << result << endl;  
            Sleep(1000);
            int len1 = result.length();
            for(int o = 0; o<len1; o++)
            this->Insertion(result[o]);
        }
        else {
            cout << "No match found for the word '" << gen1 << "' in the sentence." << endl;  
        }
    }


    string getFileName(int val) {

        LoadFiles* temp = firstFile;
        int i = 1;
        while (temp != nullptr) {
            if (i == val) {
                return temp->arr;
            }
            temp = temp->next;
            i++;
        }
        return "";
    }

    void SaveFiles(string fileName) {
        InsertExistingFileName(fileName);

        fstream file(fileName + ".txt", ios::out);
        if (file.is_open()) {
            Node* row = head;
            while (row != nullptr) {
                Node* temp = row->right;
                while (temp != nullptr) {
                    file << temp->data;
                    temp = temp->right;
                }
                file << endl;
                row = row->down;
            }
            file.close();
            cout << "File saved: " << fileName << endl;
        }
        else {
            cout << "Error saving file: " << fileName << endl;
        }
    }

    void Menu() {
        system("cls");

        cout << "------------------------------------------------------------------------" << endl;
        cout << "                  M  A  I  N             M  E  N  U                     " << endl;
        cout << "------------------------------------------------------------------------" << endl;
        cout << "                         1. CREATE A NEW FILE                           " << endl;
        cout << "                         2. LOAD EXISTING FILES                         " << endl;
        cout << "                         3. SAVE THE FILE                               " << endl;
        cout << "                         4. EXIT THE PROGRAM" << endl;
    }

    void StartWriting() {
        system("cls");
        borderform(maxWidth, 30);
        PrintCurrentLine();
    }

private:

};

void borderform(int maxWidth, int maxHeight) {
    int i = 0;

    // Top border
    while (i <= maxWidth) {
        cout << "=";
        i++;
    }
    cout << endl;

    int h = 0;
    while (h < maxHeight) {
        cout << "||";

        int j = 0;
        while (j < maxWidth - 2) {
            cout << " ";
            j++;
        }
        cout << "||" << endl;
        h++;
    }

    i = 0;
    while (i <= maxWidth) {
        cout << "=";
        i++;
    }
    cout << endl;
}


void xycorrdinate(int x, int y) {
    COORD c = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int main(int argc, char* argv[]) {
    // Clears the console screen

    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD events = 0;
    DWORD eventsRead = 0;

    WriterAndEditor editor;

    bool running = true;
    bool inMenu = true;
    string fileName;

    while (running) {

        if (inMenu) {
            editor.Menu();
            int choice;

            bool validChoice = false;

            while (!validChoice) {
                cout << "Select an option: ";
                cin >> choice;

                switch (choice) {
                case 1: // Create new file
                    cout << "Enter new file name: ";
                    cin >> fileName;
                    editor.createNewFile(fileName);
                    system("cls");
                    borderform(80, 30);

                    inMenu = false;
                    validChoice = true;
                    break;
                case 2: // Load an existing file
                    editor.printExistingFiles();
                    cout << "Enter file name to load: ";
                    cin >> fileName;
                    editor.LoadExistingFiles(fileName); // Load the selected file
                    system("cls");
                    borderform(80, 30); // Redraw border
                    editor.PrintCurrentLine(); // Print the current line of text
                    inMenu = false;  // Switch to writing mode
                    validChoice = true;
                    break;
                case 3: // Save the current file
                    cout << "Enter file name to save: ";
                    cin >> fileName;
                    editor.SaveFiles(fileName);  // Save the file
                    validChoice = true;
                    break;
                case 4: // Exit the application
                    running = false;
                    validChoice = true;
                    break;
                default:  // Invalid choice
                    cout << "Please select a valid option (1-4)" << endl;
                    break;
                }
            }
        }
        else {
            GetNumberOfConsoleInputEvents(inputHandle, &events);

            if (events != 0) {
                INPUT_RECORD eventBuffer[200];
                ReadConsoleInput(inputHandle, eventBuffer, events, &eventsRead);

                for (DWORD i = 0; i < eventsRead; ++i) {
                    if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {
                        switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
                        case VK_ESCAPE: // Return to the menu on ESC key
                            inMenu = true;
                            break;
                        case VK_UP:
                            editor.MoveCursor("up"); // Call MoveCursor with "up" argument
                            break;
                        case VK_DOWN:
                            editor.MoveCursor("down"); // Call MoveCursor with "down" argument
                            break;
                        case VK_LEFT:
                            editor.MoveCursor("left"); // Call MoveCursor with "left" argument
                            break;
                        case VK_RIGHT:
                            editor.MoveCursor("right"); // Call MoveCursor with "right" argument
                            break;
                        case VK_BACK:
                            editor.Deletion(); // Handle Backspace
                            break;
                        case VK_RETURN:
                            editor.Insertion('\n'); // Handle Enter key
                            break;
                        case 'Z': // Undo on Ctrl+Z
                            if (eventBuffer[i].Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) {
                                editor.UndoStack();
                            }
                            break;
                        case VK_TAB: // Redo on Tab key
                            editor.RedoStack();
                            break;
                        default:
                            char ch = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;

                            editor.Insertion(ch);// play /playing
                            break;
                        }
                    }
                }
            }
        }
    }

    return 0;
}