#include "AStar.h"
#include <queue>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <iostream>

using namespace std;

// Defines how to compare nodes by fCost in the priority queue
struct CompareF {
    bool operator()(PuzzleState* a, PuzzleState* b) {
        return a->fCost > b->fCost;
    }
};

// Constructor to save start state and goal board
AStar::AStar(PuzzleState* start, vector<vector<int>> goal) {
    startState = start;
    goalBoard = goal;
}

// Runs A* using Manhattan Distance
void AStar::solveManhattan() {
    cout << "Running A* with Manhattan Distance Heuristic:" << endl;
    solve(&AStar::manhattanHeuristic);
}

// Runs A* using Nilsson's sequence heuristic
void AStar::solveNilsson() {
    cout << "Running A* with Nilsson Sequence Heuristic:" << endl;
    solve(&AStar::nilssonHeuristic);
}

// Core A* algorithm
void AStar::solve(HeuristicFunction heuristic) {
    auto startTime = chrono::high_resolution_clock::now();

    priority_queue<PuzzleState*, vector<PuzzleState*>, CompareF> openList;
    unordered_map<string, int> costSoFar;

    // Initializes start node's heuristic and total cost
    startState->hCost = (this->*heuristic)(startState);
    startState->fCost = startState->gCost + startState->hCost;

    openList.push(startState);
    costSoFar[startState->toString()] = startState->fCost;

    int nodesVisited = 0;

    while (!openList.empty()) {
        PuzzleState* current = openList.top();
        openList.pop();
        nodesVisited++;

        string currentStr = current->toString();

        // Prints out the solution, time, and confirmation
        if (current->isGoal(goalBoard)) {
            auto endTime = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = endTime - startTime;

            cout << "A* found the solution." << endl;
            cout << "Nodes visited: " << nodesVisited << endl;
            cout << "Time to solve: "
                 << fixed << setprecision(7) << elapsed.count()
                 << " seconds." << endl;

            reconstructPath(current);
            return;
        }


        if (costSoFar.find(currentStr) != costSoFar.end() &&
            current->fCost > costSoFar[currentStr]) {
            continue;
        }

        vector<PuzzleState*> neighbors = current->generateNeighbors();

        for (PuzzleState* neighbor : neighbors) {
            neighbor->hCost = (this->*heuristic)(neighbor);
            neighbor->fCost = neighbor->gCost + neighbor->hCost;

            string neighborStr = neighbor->toString();

            // Only adds neighbor if it’s new or a cheaper path
            if (costSoFar.find(neighborStr) == costSoFar.end() ||
                neighbor->fCost < costSoFar[neighborStr]) {
                costSoFar[neighborStr] = neighbor->fCost;
                openList.push(neighbor);
            }
        }
    }

    cout << "A* could not find a solution." << endl;
}

// Prints the solution path
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

// Calculates Manhattan distance heuristic
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

// Calculates Nilsson's sequence heuristic
int AStar::nilssonHeuristic(PuzzleState* state) {
    int manhattan = manhattanHeuristic(state);

    vector<pair<int, int>> perimeter = {
        {0,0}, {0,1}, {0,2}, {1,2},
        {2,2}, {2,1}, {2,0}, {1,0}
    };

    vector<int> goalSequence = {1, 2, 3, 4, 5, 6, 7, 8};

    int sequencePenalty = 0;

    for (size_t i = 0; i < perimeter.size(); i++) {
        int row = perimeter[i].first;
        int col = perimeter[i].second;
        int tile = state->board[row][col];

        int expectedTile = goalSequence[i];

        if (tile != expectedTile) {
            sequencePenalty++;
        }
    }

    int centerTile = state->board[1][1];
    int centerPenalty = (centerTile != 0) ? 1 : 0;

    int nilsson = manhattan + (2 * sequencePenalty) + centerPenalty;

    return nilsson;
}
