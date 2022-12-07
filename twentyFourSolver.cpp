/*
Program: 24 Game Solver
Description: Solves the 24 Game, where the player is given 4 numbers and must return a sequence of arithmetic functions 
            with all 4 of the numbers to get the result of 24.
Made By: Steven Han
*/
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

string solverEveryPermutation(vector<pair<double,string>>& nums);

// Removes Duplicates in the Solution String
string duplicateRemover(string& orig)
{
    int ptr = 0;
    unordered_set<string> strings;
    for(int i = 0; i < orig.size(); i++)
    {
        if(orig[i] == '\n')
        {
            strings.insert(orig.substr(ptr, i - ptr));
            ptr = i + 1;
        }
    }
    string ans = "";
    for(const string& str : strings)
    {
        ans += str + '\n';
    }
    return ans;
}

// Solves the problem, returning a string of the answers
string solver(vector<pair<double,string>>& nums) 
{
    if(nums.size() == 1 && nums[0].first > 23.99 && nums[0].first <= 24)
    {
        return (nums[0].second + "\n");
    }
    else if(nums.size() == 1)
    {
        return "";
    }
    else
    {
        string combinations = "";
        for(int i = 0; i < nums.size() - 1; i++)
        {
            for(int j = i + 1; j < nums.size(); j++)
            {
                vector<pair<double,string>> newNums = nums;
                // for(auto num : newNums) { cout << num.first << " ";} cout << endl;
                // +
                newNums[i].first = newNums[i].first + newNums[j].first;
                newNums[i].second = "(" + newNums[i].second + "+" +  newNums[j].second + ")";
                newNums.erase(newNums.begin()+j);         
                combinations += solver(newNums);

                // - (l - r)
                newNums = nums;
                bool see = false;
                newNums[i].first = newNums[i].first - newNums[j].first;
                newNums[i].second = "(" + newNums[i].second + "-" + newNums[j].second + ")";
                newNums.erase(newNums.begin()+j);
                combinations += solver(newNums);

                // *
                newNums = nums;
                newNums[i].first = newNums[i].first * newNums[j].first;
                newNums[i].second = "(" + newNums[i].second + "*" + newNums[j].second + ")";
                newNums.erase(newNums.begin()+j);
                combinations += solver(newNums);

                // / (l / r)
                newNums = nums;
                if(newNums[j].first != 0)
                {
                    newNums[i].first = newNums[i].first / newNums[j].first;
                    newNums[i].second = "(" + newNums[i].second + "/" + newNums[j].second + ")";
                    newNums.erase(newNums.begin()+j);
                    combinations += solver(newNums);
                }

                // - (r - l)
                newNums = nums;
                newNums[i].first = newNums[j].first - newNums[i].first;
                newNums[i].second = "(" + newNums[j].second + "-" + newNums[i].second + ")";
                newNums.erase(newNums.begin()+j);
                combinations += solver(newNums);

                // / (r / l)
                newNums = nums;
                see = false;
                if(newNums[i].first != 0)
                {
                    newNums[i].first = newNums[j].first / newNums[i].first;
                    newNums[i].second = "(" + newNums[j].second + "/" + newNums[i].second + ")";
                    newNums.erase(newNums.begin()+j);
                    combinations += solver(newNums);
                }
            }
            
        }
        return duplicateRemover(combinations);
    }
}

// Main function, directs the user
int main(int argc, char** argv) 
{
    double input[argc - 2];
    for (int i = 1; i < argc - 1; i++) 
    {
        input[i - 1] = stod(argv[i]);
    }
    vector<pair<double,string>> toSolve;
    for(double num : input)
    {
        toSolve.push_back(make_pair(num, to_string(int(num))));
    }
    string solved = solver(toSolve);
    cout << (solved == "" ? "There is not a solution" : "There is at least one solution") << endl;
    if(solved != "")
    {
        string resp = argv[argc - 1];
        if(resp == "y")
        {
            cout << solved;
        }
    }
}