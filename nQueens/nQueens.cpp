/*
 * Name: Alvin Yang     
 * Date Submitted: 11/20/2022
 * Lab Section: 001
 * Assignment Name: Using Recursion to Solve the N-Queens Problem
 */

#include <iostream>
#include <vector>

using namespace std;

// helper function to check if queen position is valid
bool checkQueens(vector<vector<int>> & b, int r, int c) {
    int i; // temp variable for rows
    int j; // temp variable for columns

    // if conditions below are true, return false

    // check top left diagonal
    for (i = r, j = c; i >= 0 && j >= 0; i--, j--) {
        if (b[i][j] == 1) {
            return false;
        } 
    }

    // check left of row
    for (i = r; i >= 0; i--) {
        if (b[i][c] == 1) {
            return false;
        }
    }

    // check top right diagonal
    for (i = r, j = c; i >= 0 && j < b.size(); i--, j++) {
        if (b[i][j] == 1) {
            return false;
        } 
    }

    //check column
    for (i = 0; i < c; i++) {
        if (b[r][i] == 1) {
            return false;
        }
    }

    // check bottom left diagonal
    for (i = r, j = c; i < b.size() && i >= 0; i++, j--) {
        if (b[i][j] == 1) {
            return false;
        }
    }

    // return true if no queens are found in directions
    return true;
}

// function to see how many solutions there are 
int numSolutions(int column, int s, vector<vector<int>> & b, int & solutions) {
    // return and increment number of solutions if queen reaches bottom of the board
    if (column == s) {
        return solutions++;
    }
    else {
        // go through all the rows in the column
        for (int row = 0; row < s; row++) {
            // if there is no queens in diagonal and positions left and right
            if (checkQueens(b, row, column)) {
                b[row][column] = 1; // place queen at current point
                numSolutions(column+1, s, b, solutions); // recursively call function to next column
                b[row][column] = 0; // backtrack and reset position to 0
            }
        }
    }
    return solutions; // return numbers of solutions
}

//Uses recursion to count how many solutions there are for
//placing n queens on an nxn chess board
int nQueens(int n)
{                           
    vector<vector<int>> board(n, vector<int>(n, 0)); // n x n vector, initialize all value to 0
    int size = board[0].size(); // how many spaces in the grid
    int solutions = 0; // initialize solutions to 0
    return numSolutions(0, size, board, solutions); // return numbers of solutions
}