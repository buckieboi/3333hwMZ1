#include "solve.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <unordered_set>

using namespace std;

// Pseudocode:
// Convert the maze string into a list of row strings.
// This function takes the raw maze string and splits it by newlines,
// storing each row as an element in a vector.
static vector<string> parseMaze(const string& maze) {
    vector<string> grid;
    size_t pos = 0;
    while (pos < maze.size()) {
        size_t newline = maze.find('\n', pos);
        if (newline == string::npos) break;
        grid.emplace_back(maze.substr(pos, newline - pos));
        pos = newline + 1;
    }
    return grid;
}

// Pseudocode:
// Find the entrance of the maze by scanning the boundary rows and columns.
// The first open space (represented by a ' ') found on the outer edges is the entrance.
static pair<int,int> findEntrance(const vector<string>& grid) {
    int rowCount = grid.size();
    int colCount = grid[0].size();
    // Scan top and bottom rows
    for (int c = 0; c < colCount; ++c) {
        if (grid[0][c] == ' ') return {0, c};
        if (grid[rowCount-1][c] == ' ') return {rowCount-1, c};
    }
    // Scan left and right columns
    for (int r = 0; r < rowCount; ++r) {
        if (grid[r][0] == ' ') return {r, 0};
        if (grid[r][colCount-1] == ' ') return {r, colCount-1};
    }
    return {-1, -1}; // Should not happen for valid mazes
}

// Pseudocode:
// Find the exit of the maze by scanning the boundaries again,
// but skip the entrance. The first open space that is not the entrance is the exit.
static pair<int,int> findExit(const vector<string>& grid, pair<int,int> entrance) {
    int rowCount = grid.size();
    int colCount = grid[0].size();
    // Scan top and bottom rows
    for (int c = 0; c < colCount; ++c) {
        if (grid[0][c] == ' ' && !(entrance.first == 0 && entrance.second == c))
            return {0, c};
        if (grid[rowCount-1][c] == ' ' && !(entrance.first == rowCount-1 && entrance.second == c))
            return {rowCount-1, c};
    }
    // Scan left and right columns
    for (int r = 0; r < rowCount; ++r) {
        if (grid[r][0] == ' ' && !(entrance.first == r && entrance.second == 0))
            return {r, 0};
        if (grid[r][colCount-1] == ' ' && !(entrance.first == r && entrance.second == colCount-1))
            return {r, colCount-1};
    }
    return {-1, -1};
}

// Pseudocode:
// For a given cell (specified by a 1D index), return a list of valid neighbor indices.
// Valid neighbors are those that are within the bounds of the grid and not walls ('#').
// Neighbors are checked in the order: up, down, left, right.
static vector<int> getAdjacentIndices(int index, int rowCount, int colCount, const vector<string>& grid) {
    int row = index / colCount;
    int col = index % colCount;
    static const int dRow[4] = {-1, 1, 0, 0};
    static const int dCol[4] = {0, 0, -1, 1};

    vector<int> neighbors;
    neighbors.reserve(4);
    for (int dir = 0; dir < 4; ++dir) {
        int newRow = row + dRow[dir];
        int newCol = col + dCol[dir];
        // Check bounds
        if (newRow < 0 || newRow >= rowCount || newCol < 0 || newCol >= colCount)
            continue;
        // Check wall
        if (grid[newRow][newCol] == '#')
            continue;
        neighbors.push_back(newRow * colCount + newCol);
    }
    return neighbors;
}

// Pseudocode:
// Solve the maze by finding the shortest path from the entrance to the exit.
// Steps:
// 1. Convert the maze string into a grid.
// 2. Find the entrance and exit positions.
// 3. Use Breadth-First Search (BFS) to traverse the maze and record the path.
// 4. Backtrack from the exit to the entrance using the parent mapping and mark the path with 'o'.
// 5. Convert the modified grid back into a string and return it.
string solve(string maze) {
    // 1. Parse maze into grid
    vector<string> grid = parseMaze(maze);
    int rowCount = grid.size();
    int colCount = rowCount ? grid[0].size() : 0;

    // 2. Locate entrance and exit
    auto entrance = findEntrance(grid);
    auto exitPos  = findExit(grid, entrance);
    int startIndex = entrance.first * colCount + entrance.second;
    int goalIndex  = exitPos.first  * colCount + exitPos.second;

    // 3. Prepare for BFS
    vector<bool> visited(rowCount * colCount, false);
    unordered_map<int,int> parent;
    queue<int> frontier;
    visited[startIndex] = true;
    frontier.push(startIndex);

    // 4. BFS loop to find shortest path
    while (!frontier.empty()) {
        int current = frontier.front();
        frontier.pop();
        if (current == goalIndex) break;
        for (int neighbor : getAdjacentIndices(current, rowCount, colCount, grid)) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                frontier.push(neighbor);
            }
        }
    }

    // 5. Backtrack from exit to entrance, marking 'o'
    for (int cur = goalIndex; ; cur = parent[cur]) {
        int r = cur / colCount;
        int c = cur % colCount;
        grid[r][c] = 'o';
        if (cur == startIndex) break;
    }

    // 6. Rebuild and return solution string
    string solution;
    solution.reserve(maze.size());
    for (const auto& rowStr : grid) {
        solution += rowStr;
        solution += '\n';
    }
    return solution;
}
