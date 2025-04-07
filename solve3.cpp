#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <utility> // For std::pair, std::make_pair

using namespace std;

// findEntrance and findExit remain the same as in your original code.

// Finds the first open space (' ') on the border of the grid,
// returning its (row, col) coordinates.
pair<int, int> findEntrance(const vector<string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Check top and bottom rows.
    for (int c = 0; c < cols; ++c) {
        if (grid[0][c] == ' ')
            return {0, c};
        if (rows > 1 && grid[rows - 1][c] == ' ') // Add rows > 1 check for safety
            return {rows - 1, c};
    }

    // Check left and right columns.
    for (int r = 1; r < rows - 1; ++r) { // Avoid checking corners twice
        if (grid[r][0] == ' ')
            return {r, 0};
        if (cols > 1 && grid[r][cols - 1] == ' ') // Add cols > 1 check
            return {r, cols - 1};
    }
    return {-1, -1}; // Should not happen in valid mazes per problem description
}

// Finds the exit on the border (an open space) that is not the entrance.
pair<int, int> findExit(const vector<string>& grid, pair<int, int> entrance) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Check top and bottom rows.
    for (int c = 0; c < cols; ++c) {
        if (grid[0][c] == ' ' && make_pair(0, c) != entrance)
            return {0, c};
        if (rows > 1 && grid[rows - 1][c] == ' ' && make_pair(rows - 1, c) != entrance) // Add rows > 1 check
            return {rows - 1, c};
    }

    // Check left and right columns.
    for (int r = 1; r < rows - 1; ++r) { // Avoid checking corners twice
        if (grid[r][0] == ' ' && make_pair(r, 0) != entrance)
            return {r, 0};
        if (cols > 1 && grid[r][cols - 1] == ' ' && make_pair(r, cols - 1) != entrance) // Add cols > 1 check
            return {r, cols - 1};
    }
    return {-1, -1}; // Should not happen in valid mazes per problem description
}


// Main solve function as declared in solve.h.
string solve(string maze) {
    // --- Step 1: Convert maze string to grid (vector of strings) ---
    vector<string> grid;
    size_t start = 0, pos;
    while ((pos = maze.find('\n', start)) != string::npos) {
        string line = maze.substr(start, pos - start);
         // Optional: Handle '\r\n' if necessary, though examples use '\n'
         // if (!line.empty() && line.back() == '\r') { line.pop_back(); }
        grid.push_back(line);
        start = pos + 1;
    }
    if (start < maze.size()) { // Add the last row if no trailing newline
        string line = maze.substr(start);
         // Optional: Handle '\r\n'
         // if (!line.empty() && line.back() == '\r') { line.pop_back(); }
        grid.push_back(line);
    }

    int rows = grid.size();
    if (rows == 0) return maze; // Handle empty input
    int cols = grid[0].size();
    if (cols == 0) return maze; // Handle empty rows

    // --- Step 2: Find the entrance and exit on the maze border ---
    pair<int, int> entrance = findEntrance(grid);
    pair<int, int> exitP = findExit(grid, entrance);

    // Handle cases where entrance or exit isn't found (though problem assumes valid maze)
    if (entrance.first == -1 || exitP.first == -1) {
        return maze;
    }
     // Handle trivial case where entrance is the exit (e.g., single space maze " ")
     if (entrance == exitP) {
        // Decide behavior: return as-is or mark the single cell?
        // Assuming return as-is if they are the same and no path needed.
        // If the single cell should be marked 'o', uncomment below:
        // grid[entrance.first][entrance.second] = 'o';
        // // Reassemble and return grid (or just return maze)
     }


    // --- Step 3: BFS to find the shortest path ---
    queue< pair<int, int> > q;
    unordered_map<int, int> parent;
    unordered_set<int> visited;

    auto encode = [cols](int r, int c) -> int {
        return r * cols + c;
    };

    q.push(entrance);
    visited.insert(encode(entrance.first, entrance.second));
    // No parent for the entrance itself

    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    bool found = false;

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();
        int r = cur.first;
        int c = cur.second;

        // Check if we FOUND the exit in the *previous* iteration's neighbor check.
        // Note: We don't check cur == exitP here anymore, we check neighbors.

        // Explore 4-directional neighbors.
        for (int i = 0; i < 4; i++) {
            int rr = r + dr[i];
            int cc = c + dc[i];

            // Boundary check
            if (rr < 0 || cc < 0 || rr >= rows || cc >= cols)
                continue;

            // Wall check
            if (grid[rr][cc] != ' ')
                continue;

            int enc = encode(rr, cc);

            // Visited check
            if (visited.count(enc))
                continue;

            // Valid, unvisited neighbor found
            visited.insert(enc);
            parent[enc] = encode(r, c); // *** Record parent HERE ***

            // *** Check if this neighbor IS the exit ***
            if (make_pair(rr, cc) == exitP) {
                found = true;
                break; // Exit the neighbor loop (found the exit)
            }

            // If not the exit, add to queue
            q.push({rr, cc});
        }

        if (found) {
            break; // Exit the main BFS loop (while !q.empty())
        }
    }

    // --- Step 4: Backtrack from exit to entrance using the parent map ---
    if (found) { // Only backtrack if a path was found
        pair<int, int> cur = exitP;
        int entranceEnc = encode(entrance.first, entrance.second);

        while (true) {
            int curEnc = encode(cur.first, cur.second);

            // Don't mark the entrance node itself
            if (cur == entrance) {
                break;
            }

            // Mark the current node (including the exit node on first iteration)
            grid[cur.first][cur.second] = 'o';

            // Check if we've reached the node whose parent is the entrance,
            // or if the current node has no parent in the map (should only be the entrance itself)
            if (parent.find(curEnc) == parent.end()) {
                 // This should only happen if cur is the entrance, which is handled above.
                 // Or if entrance == exit initially.
                 break;
            }

            int parEnc = parent[curEnc];

            // If the parent is the entrance, we just marked the node adjacent to it. Stop.
            if (parEnc == entranceEnc) {
                break;
            }

            // Move to the parent for the next iteration
            cur = { parEnc / cols, parEnc % cols };
        }
    } else {
         // If no path found, return the original maze (as per original logic)
         return maze;
    }

    // --- Step 5: Reassemble the grid back into a string ---
    string result = "";
    for (int i = 0; i < rows; i++) {
        result += grid[i];
        if (i < rows - 1)
            result += '\n';
    }

    return result;
}