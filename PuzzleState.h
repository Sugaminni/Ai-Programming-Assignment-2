#ifndef PUZZLESTATE_H
#define PUZZLESTATE_H

#include <vector>
#include <string>

using namespace std;

class PuzzleState {
public:
    vector<vector<int>> board;
    PuzzleState* parent;
    int gCost;
    int hCost;
    int fCost;

    // Constructor for puzzle state
    PuzzleState(vector<vector<int>> b, PuzzleState* p = nullptr, int g = 0, int h = 0);

    // Checks if this state is the goal
    bool isGoal(const vector<vector<int>>& goalBoard);

    // Prints the board
    void print();

    // Converts board to a string for hashing
    string toString();

    // Generates all valid next states
    vector<PuzzleState*> generateNeighbors();
};

#endif // PUZZLESTATE_H
