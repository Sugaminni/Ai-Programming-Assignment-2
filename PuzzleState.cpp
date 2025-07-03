#include "PuzzleState.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

// Constructor to initialize board, parent, and costs
PuzzleState::PuzzleState(vector<vector<int>> b, PuzzleState* p, int g, int h) {
    board = b;
    parent = p;
    gCost = g;
    hCost = h;
    fCost = gCost + hCost;
}

// Checks if current board matches goal board
bool PuzzleState::isGoal(const vector<vector<int>>& goalBoard) {
    return board == goalBoard;
}

// Prints the puzzle board
void PuzzleState::print() {
    for (auto row : board) {
        for (int tile : row) {
            cout << tile << " ";
        }
        cout << endl;
    }
    cout << "---------" << endl;
}

// Converts board to string for hashing/comparison
string PuzzleState::toString() {
    stringstream ss;
    for (auto row : board) {
        for (int tile : row) {
            ss << tile;
        }
    }
    return ss.str();
}

// Generates all valid next states by moving empty tile
vector<PuzzleState*> PuzzleState::generateNeighbors() {
    vector<PuzzleState*> neighbors;

    // Finds location of empty tile (0)
    int zeroRow, zeroColumn;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                zeroRow = i;
                zeroColumn = j;
            }
        }
    }

    // Possible move directions: up, down, left, right
    vector<pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    for (auto dir : directions) {
        int newRow = zeroRow + dir.first;
        int newColumn = zeroColumn + dir.second;

        // Checks bounds
        if (newRow >= 0 && newRow < 3 && newColumn >= 0 && newColumn < 3) {
            vector<vector<int>> newBoard = board;
            swap(newBoard[zeroRow][zeroColumn], newBoard[newRow][newColumn]);

            // Creates new PuzzleState for this move
            PuzzleState* newState = new PuzzleState(newBoard, this, gCost + 1);
            neighbors.push_back(newState);
        }
    }

    return neighbors;
}
