#include "UCS.h"
#include <queue>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <unordered_set>

using namespace std;

// Compares nodes by gCost for UCS priority queue
struct CompareG {
    bool operator()(PuzzleState* a, PuzzleState* b) {
        return a->gCost > b->gCost;
    }
};

// Constructor to save start state and goal
UCS::UCS(PuzzleState* start, vector<vector<int>> goal) {
    startState = start;
    goalBoard = goal;
}

void UCS::solve() {
    auto startTime = chrono::high_resolution_clock::now();

    priority_queue<PuzzleState*, vector<PuzzleState*>, CompareG> openList;
    unordered_map<string, int> costSoFar;
    unordered_set<string> expanded;

    openList.push(startState);
    costSoFar[startState->toString()] = startState->gCost;

    int nodesVisited = 0;

    while (!openList.empty()) {
        PuzzleState* current = openList.top();
        openList.pop();
        nodesVisited++;

        string currentStr = current->toString();

        if (current->isGoal(goalBoard)) {
            auto endTime = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = endTime - startTime;

            cout << "UCS found the solution in " << elapsed.count() << " seconds." << endl;
            cout << "Nodes visited: " << nodesVisited << endl;

            reconstructPath(current);
            return;
        }

        if (expanded.find(currentStr) != expanded.end()) {
            continue;
        }
        expanded.insert(currentStr);

        vector<PuzzleState*> neighbors = current->generateNeighbors();

        for (PuzzleState* neighbor : neighbors) {
            string neighborStr = neighbor->toString();
            int newCost = neighbor->gCost;

            // Adds neighbor if new or found via cheaper path
            if (costSoFar.find(neighborStr) == costSoFar.end() || newCost < costSoFar[neighborStr]) {
                costSoFar[neighborStr] = newCost;
                openList.push(neighbor);
            }
        }
    }

    cout << "UCS could not find a solution." << endl;
}

// Prints solution path from start to goal
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
