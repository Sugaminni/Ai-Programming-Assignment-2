#ifndef UCS_H
#define UCS_H

#include "PuzzleState.h"
#include <vector>
#include <unordered_map>

using namespace std;

class UCS {
public:
    UCS(PuzzleState* start, vector<vector<int>> goal);
    void solve();

private:
    PuzzleState* startState;
    vector<vector<int>> goalBoard;
    void reconstructPath(PuzzleState* goalNode);
};

#endif // UCS_H
