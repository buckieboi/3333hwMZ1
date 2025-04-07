#include "solve.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <unordered_map>

using namespace std;

string solve(string maze) {
    // Parse input into grid of rows
    vector<string> grid;
    size_t pos = 0;
    while (pos < maze.size()) {
        size_t nl = maze.find('\n', pos);
        if (nl == string::npos) break;
        grid.emplace_back(maze.substr(pos, nl - pos));
        pos = nl + 1;
    }
    int rows = grid.size();
    int cols = rows ? grid[0].size() : 0;

    // Locate the two exits on the boundary
    vector<int> exits;
    exits.reserve(2);
    unordered_set<int> seen;
    auto try_exit = [&](int r, int c) {
        int idx = r * cols + c;
        if (grid[r][c] == ' ' && seen.insert(idx).second) {
            exits.push_back(idx);
        }
    };
    for (int c = 0; c < cols && exits.size() < 2; ++c) {
        try_exit(0, c);
        try_exit(rows - 1, c);
    }
    for (int r = 0; r < rows && exits.size() < 2; ++r) {
        try_exit(r, 0);
        try_exit(r, cols - 1);
    }

    int start = exits[0];
    int goal  = exits[1];

    // BFS setup
    vector<bool> visited(rows * cols, false);
    unordered_map<int,int> parent;
    queue<int> q;
    visited[start] = true;
    q.push(start);

    static const int dr[4] = {-1,1,0,0};
    static const int dc[4] = {0,0,-1,1};

    // BFS loop
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        if (cur == goal) break;
        int r = cur / cols, c = cur % cols;
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] == '#') continue;
            int ni = nr * cols + nc;
            if (visited[ni]) continue;
            visited[ni] = true;
            parent[ni] = cur;
            q.push(ni);
        }
    }

    // Backtrack from goal to start, marking 'o'
    for (int cur = goal; ; cur = parent[cur]) {
        int r = cur / cols, c = cur % cols;
        grid[r][c] = 'o';
        if (cur == start) break;
    }

    // Reconstruct solution string
    string sol;
    sol.reserve(maze.size());
    for (auto& row : grid) {
        sol += row;
        sol += '\n';
    }
    return sol;
}
