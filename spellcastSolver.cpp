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
using namespace std;


/*
    The cell is what will contain the letter and its respective point value
*/
struct Cell {
    Cell(char letter, int points) : letter(letter), points(points) {}
    char letter;
    int points;
};

/*
    Converts the string of the board into the board used for the solver
*/
vector<vector<Cell>> convertBoard(const string& board)
{
    // Default point values for each character
    unordered_map<char, int> defaults;
    defaults = {{'a', 1}, {'b', 4}, {'c', 5}, {'d', 3}, {'e', 1},
            {'f', 5}, {'g', }, {'h', }, {'i', 1}, {'j', },
            {'k', }, {'l', 3}, {'m', 4}, {'n', 2}, {'o', 1},
            {'p', }, {'q', 8}, {'r', 2}, {'s', }, {'t', 2},
            {'u', 4}, {'v', }, {'w', }, {'x', 7}, {'y', }, {'z', }};

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

/*
    Helper used to find the largest word recursively, and generate a list of valid words
*/
pair<int,string> solverHelper(const vector<vector<Cell>>& board, const unordered_set<string>& dictionary, vector<vector<bool>>& alreadyUsed, int row, int col, string prevWord, int prevVal, unordered_map<string,int>& listOfValids)
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
    alreadyUsed[row][col] = true;
    // right
    if(col < board[row].size() - 1 && alreadyUsed[row][col + 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row, col + 1, currWord, currVal, listOfValids);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // left
    if(col > 0 && alreadyUsed[row][col - 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row, col - 1, currWord, currVal, listOfValids);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // up
    if(row < board.size() - 1 && alreadyUsed[row + 1][col] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row + 1, col, currWord, currVal, listOfValids);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // down
    if(row > 0 && alreadyUsed[row - 1][col] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row - 1, col, currWord, currVal, listOfValids);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // up right
    if(col < board[row].size() - 1 && row < board.size() - 1 && alreadyUsed[row + 1][col + 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row + 1, col + 1, currWord, currVal, listOfValids);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // down right
    if(col < board[row].size() - 1 && row > 0 && alreadyUsed[row - 1][col + 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row - 1, col + 1, currWord, currVal, listOfValids);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // up left
    if(col > 0 && row < board.size() - 1 && alreadyUsed[row + 1][col - 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row + 1, col - 1, currWord, currVal, listOfValids);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    // down left
    if(col > 0 && row > 0 && alreadyUsed[row - 1][col - 1] == false)
    {
        pair<int,string> tempVal = solverHelper(board, dictionary, alreadyUsed, row - 1, col - 1, currWord, currVal, listOfValids);
        if(currMax.first < tempVal.first)
        {
            currMax = tempVal;
        }
    }
    alreadyUsed[row][col] = false;
    return currMax;
}

pair<int,string> solver(const vector<vector<Cell>>& board, const unordered_set<string>& dictionary)
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
            pair<int,string> tempBest = solverHelper(board, dictionary, alreadyUsed, row, col, "" , 0, listOfValids);
            if(tempBest.first > bestWord.first)
            {
                bestWord = tempBest;
            }
        }
    }
    return bestWord;
}

int main(int argc, char** argv) {
    string boardStr;
    for(int i = 1; i < argc; i++)
    {
        boardStr += argv[i];
    }
    vector<vector<Cell>> board = convertBoard(boardStr);
    unordered_set<string> dictionary;
    // TODO implement the dictionary
    cout << solver(board, dictionary).second << endl;
}