#ifndef ASTAR_H
#define ASTAR_H

#include "PuzzleState.h"
#include <vector>
#include <unordered_map>

using namespace std;

class AStar {
public:
    AStar(PuzzleState* start, vector<vector<int>> goal);
    void solveManhattan();
    void solveNilsson();

private:
    PuzzleState* startState;
    vector<vector<int>> goalBoard;

    typedef int (AStar::*HeuristicFunction)(PuzzleState*);
    void solve(HeuristicFunction heuristic);
    void reconstructPath(PuzzleState* goalNode);
    int manhattanHeuristic(PuzzleState* state);
    int nilssonHeuristic(PuzzleState* state);
};

#endif // ASTAR_H
