#include "PuzzleState.h"
#include "UCS.h"
#include "AStar.h"
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;


    // Generates a random puzzle by making random moves from the goal state.
vector<vector<int>> generateRandomBoard(const vector<vector<int>>& goalBoard, int numMoves = 50) {
    PuzzleState* state = new PuzzleState(goalBoard);

    for (int i = 0; i < numMoves; i++) {
        vector<PuzzleState*> neighbors = state->generateNeighbors();
        if (!neighbors.empty()) {
            int idx = rand() % neighbors.size();
            state = neighbors[idx];
        }
    }
    return state->board;
}

int main() {
    srand(time(0));

    // Goal board to solve for
    vector<vector<int>> goalBoard = {
        {1, 2, 3},
        {8, 0, 4},
        {7, 6, 5}
    };

    // Generate a random starting board
    vector<vector<int>> startBoard = generateRandomBoard(goalBoard, 50);

    // Prints the starting board
    cout << "Random Start Board:" << endl;
    for (auto row : startBoard) {
        for (int tile : row) {
            cout << tile << " ";
        }
        cout << endl;
    }
    cout << "---------" << endl;

    // Creates separate start states for each solver
    PuzzleState* startStateUCS = new PuzzleState(startBoard);
    PuzzleState* startStateAStar1 = new PuzzleState(startBoard);
    PuzzleState* startStateAStar2 = new PuzzleState(startBoard);

    // Runs UCS solver
    UCS ucsSolver(startStateUCS, goalBoard);
    ucsSolver.solve();

    // Runs A* with Manhattan heuristic
    AStar astarSolver1(startStateAStar1, goalBoard);
    astarSolver1.solveManhattan();

    // Runs A* with Nilsson heuristic
    AStar astarSolver2(startStateAStar2, goalBoard);
    astarSolver2.solveNilsson();

    return 0;
}
