/*
Program: Spellcast Solver
Description: Tries to find the best response for the discord game, Spellcast.
    Given a 5x5 board of letters, find the best word. Later implement length bonus and later implement power ups
Made By: Steven Han
*/
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <fstream>
using namespace std;

// Define the character size
#define CHAR_SIZE 128

/*
    The cell is what will contain the letter and its respective point value.
*/
struct Cell {
    Cell(char letter, int points) : letter(letter), points(points) {}
    char letter;
    int points;
};

/*
    Converts the string of the board into the board used for the solver.
*/
vector<vector<Cell>> convertBoard(const string& board)
{
    // Default point values for each character
    unordered_map<char, int> defaults;
    defaults = {{'a', 1}, {'b', 4}, {'c', 5}, {'d', 3}, {'e', 1},
            {'f', 5}, {'g', 3}, {'h', 4}, {'i', 1}, {'j', 7},
            {'k', 6}, {'l', 3}, {'m', 4}, {'n', 2}, {'o', 1},
            {'p', 4}, {'q', 8}, {'r', 2}, {'s', 2}, {'t', 2},
            {'u', 4}, {'v', 5}, {'w', 5}, {'x', 7}, {'y', 4}, {'z', 5}};

    vector<vector<Cell>> vec(5, vector<Cell>(5, Cell('/', 0)));
    int col = 0;
    int row = 0;
    for(int i = 0; i < board.size(); i++)
    {
        if (col == 5)
        {
            row++;
            col = 0;
        }
        if(board[i] >= 97 && board[i] <= 122)
        {
            vec[row][col] = Cell(board[i], defaults[board[i]]);
            col++;
        }
    }
    return vec;
}

// A class to store a Trie node
class Trie
{
public:
    bool isLeaf;
    Trie* character[CHAR_SIZE];
 
    // Constructor
    Trie()
    {
        this->isLeaf = false;
 
        for (int i = 0; i < CHAR_SIZE; i++) {
            this->character[i] = nullptr;
        }
    }
 
    void insert(string);
    bool deletion(Trie*&, string);
    bool search(string);
    bool haveChildren(Trie const*);
};
 
// Iterative function to insert a key into a Trie
void Trie::insert(string key)
{
    // start from the root node
    Trie* curr = this;
    for (int i = 0; i < key.length(); i++)
    {
        // create a new node if the path doesn't exist
        if (curr->character[key[i]] == nullptr) {
            curr->character[key[i]] = new Trie();
        }
 
        // go to the next node
        curr = curr->character[key[i]];
    }
 
    // mark the current node as a leaf
    curr->isLeaf = true;
}
 
// Iterative function to search a fragment of a word in the trie. If it exists, return true.
bool Trie::search(string key)
{
    // return false if Trie is empty
    if (this == nullptr) {
        return false;
    }
 
    Trie* curr = this;
    for (int i = 0; i < key.length(); i++)
    {
        // go to the next node
        curr = curr->character[key[i]];
 
        // if the string is invalid (reached end of a path in the Trie)
        if (curr == nullptr) {
            return false;
        }
    }
 
    // return true if the current node exists
    return curr;
}
 
// Returns true if a given node has any children
bool Trie::haveChildren(Trie const* curr)
{
    for (int i = 0; i < CHAR_SIZE; i++)
    {
        if (curr->character[i]) {
            return true;    // child found
        }
    }
 
    return false;
}
 
// Recursive function to delete a key in the Trie
bool Trie::deletion(Trie*& curr, string key)
{
    // return if Trie is empty
    if (curr == nullptr) {
        return false;
    }
 
    // if the end of the key is not reached
    if (key.length())
    {
        // recur for the node corresponding to the next character in the key
        // and if it returns true, delete the current node (if it is non-leaf)
 
        if (curr != nullptr &&
            curr->character[key[0]] != nullptr &&
            deletion(curr->character[key[0]], key.substr(1)) &&
            curr->isLeaf == false)
        {
            if (!haveChildren(curr))
            {
                delete curr;
                curr = nullptr;
                return true;
            }
            else {
                return false;
            }
        }
    }
 
    // if the end of the key is reached
    if (key.length() == 0 && curr->isLeaf)
    {
        // if the current node is a leaf node and doesn't have any children
        if (!haveChildren(curr))
        {
            // delete the current node
            delete curr;
            curr = nullptr;
 
            // delete the non-leaf parent nodes
            return true;
        }
 
        // if the current node is a leaf node and has children
        else {
            // mark the current node as a non-leaf node (DON'T DELETE IT)
            curr->isLeaf = false;
 
            // don't delete its parent nodes
            return false;
        }
    }
 
    return false;
}


/*
    Helper used to find the largest word recursively, and generate a list of valid words.
*/
pair<int,string> solverHelper(const vector<vector<Cell>>& board, const unordered_set<string>& dictionary, 
    vector<vector<bool>>& alreadyUsed, int row, int col, string prevWord, int prevVal, unordered_map<string,int>& listOfValids, 
    Trie* head)
{
    string currWord = prevWord + board[row][col].letter;
    int currVal = prevVal + board[row][col].points;
    pair<int, string> currMax;
    currMax = make_pair(0, "");
    if(dictionary.find(currWord) != dictionary.end())
    {
        listOfValids[currWord] = currVal;
        currMax = make_pair(currVal, currWord);
    }
    // If the word fragment doesn't exist in the trie, return the current max early
    if(!(head->search(currWord)))
    {
        return currMax;
    }
    alreadyUsed[row][col] = true;
    // right
    if(col < board[row].size() - 1 && alreadyUsed[row][col + 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row, col + 1, 
            currWord, currVal, listOfValids, head);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // left
    if(col > 0 && alreadyUsed[row][col - 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row, col - 1, 
            currWord, currVal, listOfValids, head);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // up
    if(row < board.size() - 1 && alreadyUsed[row + 1][col] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row + 1, col, 
            currWord, currVal, listOfValids, head);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // down
    if(row > 0 && alreadyUsed[row - 1][col] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row - 1, col, 
            currWord, currVal, listOfValids, head);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // up right
    if(col < board[row].size() - 1 && row < board.size() - 1 && alreadyUsed[row + 1][col + 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row + 1, col + 1, 
            currWord, currVal, listOfValids, head);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // down right
    if(col < board[row].size() - 1 && row > 0 && alreadyUsed[row - 1][col + 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row - 1, col + 1, 
            currWord, currVal, listOfValids, head);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // up left
    if(col > 0 && row < board.size() - 1 && alreadyUsed[row + 1][col - 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row + 1, col - 1, 
            currWord, currVal, listOfValids, head);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // down left
    if(col > 0 && row > 0 && alreadyUsed[row - 1][col - 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row - 1, col - 1, 
            currWord, currVal, listOfValids, head);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    alreadyUsed[row][col] = false;
    return currMax;
}

/*
    Primary Solver Function. Returns the largest word and its point value given the board and a dictionary of valid words.
*/
pair<int,string> solver(const vector<vector<Cell>>& board, const unordered_set<string>& dictionary, Trie* head)
{
    // Used to keep track of if the letter is already being used for the word
    vector<vector<bool>> alreadyUsed(board.size(), vector<bool>(board[0].size(), false));
    // Will result in being a list of valid words and their point values
    unordered_map<string,int> listOfValids;
    pair<int,string> bestWord;
    bestWord = make_pair(0, "");
    for(int row = 0; row < board.size(); row++)
    {
        for(int col = 0; col < board[row].size(); col++)
        {
            alreadyUsed = vector<vector<bool>>(board.size(), vector<bool>(board[0].size(), false));
            pair<int,string> tempBest = solverHelper(board, dictionary, alreadyUsed, row, col, "" , 0, listOfValids, head);
            if(tempBest.first > bestWord.first)
            {
                bestWord = tempBest;
            }
        }
    }
    return bestWord;
}

/*
    Main function. Reads the command line and runs the solver. Returns the best word.
*/
int main(int argc, char** argv) {
    string boardStr;
    for(int i = 1; i < argc; i++)
    {
        boardStr += argv[i];
    }
    vector<vector<Cell>> board = convertBoard(boardStr);
    unordered_set<string> dictionary;
    Trie* head = new Trie();
    ifstream ifs("static/files/diction.txt");
    if(!ifs)
    {
        cout << "Dictionary Not Found..." << endl;
        exit(1);
    }
    string word;
    while(ifs >> word)
    {
        dictionary.insert(word);
        head->insert(word);
    }
    auto result = solver(board, dictionary, head);
    cout << result.second << ' ' << result.first << endl;
}

/*
    Things that still must be done

    - Finish the default character values.
    - Implement length boost
    - Implement double, triple words.
    - Implement double cell.
    - Implement power ups.
*/