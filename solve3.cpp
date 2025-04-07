#include "solve.h"
#include "vertex.h" 
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <unordered_set>

using namespace std;

// Pseudocode:
// Convert the maze string into a list of row strings.
// This function splits the maze input (by newline) and returns a vector where each element is a row.
static vector<string> parseMaze(const string& maze) {
    vector<string> grid;
    size_t pos = 0;
    while (pos < maze.size()) {
        size_t newline = maze.find('\n', pos);
        if (newline == string::npos)
            break;
        grid.emplace_back(maze.substr(pos, newline - pos));
        pos = newline + 1;
    }
    return grid;
}

// Pseudocode:
// Find the maze entrance by scanning the outer boundaries for the first open space (' ').
static pair<int,int> findEntrance(const vector<string>& grid) {
    int rowCount = grid.size();
    int colCount = grid[0].size();
    // Scan top and bottom rows
    for (int c = 0; c < colCount; ++c) {
        if (grid[0][c] == ' ') return {0, c};
        if (grid[rowCount - 1][c] == ' ') return {rowCount - 1, c};
    }
    // Scan left and right columns
    for (int r = 0; r < rowCount; ++r) {
        if (grid[r][0] == ' ') return {r, 0};
        if (grid[r][colCount - 1] == ' ') return {r, colCount - 1};
    }
    return {-1, -1}; // Should not happen for valid mazes
}

// Pseudocode:
// Find the maze exit by scanning the outer boundaries for the second open space,
// ignoring the entrance.
static pair<int,int> findExit(const vector<string>& grid, pair<int,int> entrance) {
    int rowCount = grid.size();
    int colCount = grid[0].size();
    // Scan top and bottom rows
    for (int c = 0; c < colCount; ++c) {
        if (grid[0][c] == ' ' && !(entrance.first == 0 && entrance.second == c))
            return {0, c};
        if (grid[rowCount - 1][c] == ' ' && !(entrance.first == rowCount - 1 && entrance.second == c))
            return {rowCount - 1, c};
    }
    // Scan left and right columns
    for (int r = 0; r < rowCount; ++r) {
        if (grid[r][0] == ' ' && !(entrance.first == r && entrance.second == 0))
            return {r, 0};
        if (grid[r][colCount - 1] == ' ' && !(entrance.first == r && entrance.second == colCount - 1))
            return {r, colCount - 1};
    }
    return {-1, -1};
}

// Pseudocode:
// Solve the maze by converting it into a graph of Vertex objects, using BFS to find the shortest path,
// then marking that path on the maze and returning the solution as a string.
string solve(string maze) {
    // 1. Parse maze into grid
    vector<string> grid = parseMaze(maze);
    int rowCount = grid.size();
    if (rowCount == 0) return maze;
    int colCount = grid[0].size();

    // 2. Locate entrance and exit positions (as grid coordinates)
    pair<int,int> entrancePos = findEntrance(grid);
    pair<int,int> exitPos = findExit(grid, entrancePos);

    // 3. Create a 2D grid of Vertex pointers for empty cells
    vector<vector<Vertex*>> vertices(rowCount, vector<Vertex*>(colCount, nullptr));
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            if (grid[r][c] == ' ') {
                vertices[r][c] = new Vertex(r, c);
            }
        }
    }

    // 4. Build the graph by linking each Vertex to its valid (non-wall) adjacent neighbors
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            if (vertices[r][c] != nullptr) {
                // Up
                if (r > 0 && vertices[r - 1][c] != nullptr)
                    vertices[r][c]->neighs.push_back(vertices[r - 1][c]);
                // Down
                if (r < rowCount - 1 && vertices[r + 1][c] != nullptr)
                    vertices[r][c]->neighs.push_back(vertices[r + 1][c]);
                // Left
                if (c > 0 && vertices[r][c - 1] != nullptr)
                    vertices[r][c]->neighs.push_back(vertices[r][c - 1]);
                // Right
                if (c < colCount - 1 && vertices[r][c + 1] != nullptr)
                    vertices[r][c]->neighs.push_back(vertices[r][c + 1]);
            }
        }
    }

    // 5. Set up BFS to find the shortest path from entrance to exit
    Vertex* startVertex = vertices[entrancePos.first][entrancePos.second];
    Vertex* goalVertex  = vertices[exitPos.first][exitPos.second];
    unordered_map<Vertex*, Vertex*> parent;  // To backtrack the path
    unordered_set<Vertex*> visited;
    queue<Vertex*> frontier;
    visited.insert(startVertex);
    frontier.push(startVertex);
    bool found = false;
    
    while (!frontier.empty()) {
        Vertex* current = frontier.front();
        frontier.pop();
        if (current == goalVertex) {
            found = true;
            break;
        }
        for (Vertex* neighbor : current->neighs) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                frontier.push(neighbor);
            }
        }
    }

    // If no path is found, cleanup and return the original maze
    if (!found) {
        for (auto &row : vertices)
            for (auto &v : row)
                delete v;
        return maze;
    }

    // 6. Backtrack from the exit to the entrance, marking the path with 'o'
    for (Vertex* cur = goalVertex; ; cur = parent[cur]) {
        grid[cur->row][cur->col] = 'o';
        if (cur == startVertex)
            break;
    }

    // 7. Cleanup dynamically allocated Vertex objects
    for (auto &row : vertices) {
        for (auto &v : row) {
            delete v;
        }
    }
    
    // 8. Rebuild the solution string from the modified grid
    string solution;
    for (const auto& rowStr : grid) {
        solution += rowStr + "\n";
    }
    return solution;
}
