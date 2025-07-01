#include "AStar.h"
#include <queue>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <iostream>

using namespace std;

// Comparison structure to sort priority queue by fCost
struct CompareF {
    bool operator()(PuzzleState* a, PuzzleState* b) {
        return a->fCost > b->fCost;
    }
};

// Constructor for A* method
AStar::AStar(PuzzleState* start, vector<vector<int>> goal) {
    startState = start;
    goalBoard = goal;
}

// Runs A* using Manhattan heuristic
void AStar::solveManhattan() {
    cout << "Running A* with Manhattan Distance Heuristic:" << endl;
    solve(&AStar::manhattanHeuristic);
}

// Runs A* using Nilsson's heuristic
void AStar::solveNilsson() {
    cout << "Running A* with Nilsson Sequence Heuristic:" << endl;
    solve(&AStar::nilssonHeuristic);
}

// Core A* logic, accepts a heuristic function pointer
void AStar::solve(HeuristicFunction heuristic) {
    auto startTime = chrono::high_resolution_clock::now();

    priority_queue<PuzzleState*, vector<PuzzleState*>, CompareF> openList;
    unordered_set<string> visited;

    // Calculates heuristic for start state
    startState->hCost = (this->*heuristic)(startState);
    startState->fCost = startState->gCost + startState->hCost;
    openList.push(startState);

    int nodesVisited = 0;

    while (!openList.empty()) {
        PuzzleState* current = openList.top();
        openList.pop();
        nodesVisited++;

        if (current->isGoal(goalBoard)) {
            auto endTime = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = endTime - startTime;

            cout << "A* found the solution in " << elapsed.count() << " seconds!" << endl;
            cout << "Nodes visited: " << nodesVisited << endl;

            reconstructPath(current);
            return;
        }

        visited.insert(current->toString());

        vector<PuzzleState*> neighbors = current->generateNeighbors();

        for (PuzzleState* neighbor : neighbors) {
            if (visited.find(neighbor->toString()) == visited.end()) {
                neighbor->hCost = (this->*heuristic)(neighbor);
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                openList.push(neighbor);
            }
        }
    }

    cout << "A* could not find a solution." << endl;
}

// Prints the path from start to goal
void AStar::reconstructPath(PuzzleState* goalNode) {
    vector<PuzzleState*> path;

    while (goalNode != nullptr) {
        path.push_back(goalNode);
        goalNode = goalNode->parent;
    }

    reverse(path.begin(), path.end());

    for (auto state : path) {
        state->print();
    }
}

// Calculates Manhattan Distance heuristic
int AStar::manhattanHeuristic(PuzzleState* state) {
    int distance = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int tile = state->board[i][j];

            if (tile != 0) {
                int goalRow = (tile - 1) / 3;
                int goalCol = (tile - 1) % 3;
                distance += abs(i - goalRow) + abs(j - goalCol);
            }
        }
    }

    return distance;
}

 // For Nilsson's heuristic
int AStar::nilssonHeuristic(PuzzleState* state) {
    // Need to implement Nilsson's Sequence Score heuristic
    return 0;
}

