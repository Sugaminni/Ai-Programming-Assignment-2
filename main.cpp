#include <iostream>          // for cout printing
#include <vector>            // for vector
#include "PuzzleState.h"
#include "UCS.h"
#include "AStar.h"

using namespace std;

int main() {
    vector<vector<int>> startBoard = { // Initial puzzle board to solve (Randomizes)
        {2, 8, 3},
        {1, 6, 4},
        {7, 0, 5}
    };

    vector<vector<int>> goalBoard = { // Goal end of solved board
        {1, 2, 3},
        {8, 0, 4},
        {7, 6, 5}
    };

    // Creates a puzzlestate object
    PuzzleState* startState = new PuzzleState(startBoard);

    // Creates UCS solver and runs it
    UCS ucsSolver(startState, goalBoard);
    ucsSolver.solve();

    // Creates A* solver and runs it
    AStar astarSolver(startState, goalBoard);
    astarSolver.solveManhattan();
    astarSolver.solveNilsson();

    return 0;
}
