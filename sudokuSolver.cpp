/*
Program: Sudoku Solver
Description: Solves a valid sudoku board 
Made By: Steven Han
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

bool isValid(vector<vector<char>>& board, int row, int col, char num)
{
    for(int i = 0; i < 9; i++)
    {
        if(board[i][col] == num)
            return false;
        if(board[row][i] == num)
            return false;
        if(board[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == num)
            return false;
    }
    return true;
}

bool solveSudokuHelp(vector<vector<char>>& board, int col, int row)
{
    if(col == 9)
    {
        row += 1;
        col = 0;
    }
    if(row == 9)
        return true;
    if(board[row][col] == '.')
    {
        for(char i = '1'; i <= '9'; i++)
        {
            if(isValid(board, row, col, i))
            {
                board[row][col] = i;
                if (solveSudokuHelp(board, col + 1, row))
                    return true;
            }
            board[row][col] = '.';
        }
        return false;
    }
    return solveSudokuHelp(board, col + 1, row);
}
void solveSudoku(vector<vector<char>>& board) {
    solveSudokuHelp(board, 0, 0);
}


vector<vector<char>> convertBoard(const string& board)
{
    vector<vector<char>> vec(9, vector<char>(9, '.'));
    int col = 0;
    int row = 0;
    for(int i = 0; i < board.size(); i++)
    {
        if (col == 9)
        {
            row++;
            col = 0;
        }
        if(board[i] >= 48 && board[i] <= 57)
        {
            vec[row][col] = board[i];
            col++;
        }
        if(board[i] == '.')
        {
            col++;
        }
    }
    return vec;
}

int main(int argc, char** argv) {
    string boardStr;
    for(int i = 1; i < argc; i++)
    {
        boardStr += argv[i];
    }
    vector<vector<char>> board = convertBoard(boardStr);
    solveSudoku(board);
    for(const vector<char>& row : board)
    {
        cout << '[';
        for(char val : row)
        {
            cout << val << ",";
        }
        cout << "]" << endl;
    }
}