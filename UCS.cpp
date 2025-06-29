#include "UCS.h"
#include <queue>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <iostream>

using namespace std;

// Comparison structure to sort the priority queue by gCost
struct CompareG {
    bool operator()(PuzzleState* a, PuzzleState* b) {
        return a->gCost > b->gCost;
    }
};

// Constructor for UCS
UCS::UCS(PuzzleState* start, vector<vector<int>> goal) {
    startState = start;
    goalBoard = goal;
}

// UCS search method
void UCS::solve() {
    auto startTime = chrono::high_resolution_clock::now();

    priority_queue<PuzzleState*, vector<PuzzleState*>, CompareG> openList;
    unordered_set<string> visited;

    openList.push(startState);
    int nodesVisited = 0;

    while (!openList.empty()) {
        PuzzleState* current = openList.top();
        openList.pop();
        nodesVisited++;

        if (current->isGoal(goalBoard)) {
            auto endTime = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = endTime - startTime;

            cout << "UCS found the solution in " << elapsed.count() << " seconds!" << endl;
            cout << "Nodes visited: " << nodesVisited << endl;

            reconstructPath(current);
            return;
        }

        visited.insert(current->toString());

        vector<PuzzleState*> neighbors = current->generateNeighbors();

        for (PuzzleState* neighbor : neighbors) {
            // Only adds neighbor if not seen it yet
            if (visited.find(neighbor->toString()) == visited.end()) {
                openList.push(neighbor);
            }
        }
    }

    cout << "UCS could not find a solution." << endl;
}

// Prints the path from start to goal
void UCS::reconstructPath(PuzzleState* goalNode) {
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

