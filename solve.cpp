#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <unordered_map>

using namespace std;

// For the mandatory running time, assume that the time for
// operations of queue, unordered_set, and map are O(1). 
// (They are for average-case, but not worst-case).
//
// For the mandatory running time below, s is the length of 
// the input string representing the maze.
// 
// For a complete description of the maze string 
// and maze solution formats, see the assignment pdf.


// Returns a string representing a shortest solution to the maze.
// Has undefined behavior if the maze is not valid or has no solution.
//
// Must run in O(s) time.



//image it like craph
//find the entrance
//move down  every level, pushing in queue and removing from.
//once exit on the last give string matches, update a boolean value and end search.


//loop thru string check for entrance
//

pair<int, int> findEntrance(const vector<string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    // how do i assure that entry wont get picked twice?
    //change  entrance found to  an 'o' so it oesnt get picked again?
    // Check top and bottom rows
    for (int c = 0; c < cols; ++c) {
        if (grid[0][c] == ' '){
            return {0, c};
            grid[0][c] = 'o'
        } 
        if (grid[rows - 1][c] == ' '){
            return {rows - 1, c};
            grid[rows - 1][c] = 'o'
        } 
    }

    // Check left and right columns
    for (int r = 0; r < rows; ++r) {
        if (grid[r][0] == ' '){
            return {r, 0};
            grid[r][0] = 'o'
        } 
        if (grid[r][cols - 1] == ' ')
            return {r, cols - 1};
            grid[r][cols - 1] = 'o'

    }
}


pair<int, int> findExit(const vector<string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    // how do i assure that entry wont get picked twice?
    //change  entrance found to  an 'o' so it oesnt get picked again?
    // Check top and bottom rows
    //idk if to swap exit node wit 'E' or 'o' as to indicate exit reached.
    for (int c = 0; c < cols; ++c) {
        if (grid[0][c] == ' '){
            return {0, c};
            grid[0][c] = 'E'
        } 
        if (grid[rows - 1][c] == ' '){
            return {rows - 1, c};
            grid[rows - 1][c] = 'E'
        } 
    }

    // Check left and right columns
    for (int r = 0; r < rows; ++r) {
        if (grid[r][0] == ' '){
            return {r, 0};
            grid[r][0] = 'E'
        } 
        if (grid[r][cols - 1] == ' ')
            return {r, cols - 1};
            grid[r][cols - 1] = 'E'

    }
}






//exploreNeighbors()
void exploreNeighbors(){
    //make direction row and column vectors as to explore them reach level
    //north,south,east,west
    dr=[-1,1,0,0];
    dc=[0,0,-1,1];


    for i < 4 //all directions
        //traverse the neighboring cells.
        rr = r + dr[i];
        cc = c + dc[i];
        //
        //skip over # as they are the walls of the maze
        if rr == "#" or cc == "#" continue;
        //R and C are length of row and column(dont go beyond maze)
        if rr >= R or cc >= C; continue;
        // what if visited?
        // do i need to make a list that will take all nodes that have been passed?
        // is that all nodes that have been to queue?
        if dr == visited or dc == visited continue;
        //add the neighboring nodes to queue
        rr.enqueue;
        cc.enqueue;


}

// we have a way to to traverse the nodes
// we need to now find the smallest path and return it.
// so like when we are traversing the smallest pasth FIFO of queue will allow the smallest path to return first.
// so we mark that path and come pack to return it as a string.
// professor daid use boolean to make it easier and flip when string is found.
// need to tag exit and user r,c to check if we reached it.

string solve(string maze){
    //
    bool pathFound = False
    while True:
        findExit(maze)
        if grid[r][c] = 'E'
            break
    
    //flip shortest path cells to 'o' on the string grid and return the string back as solution
    //flip all the shortest path to 'o'
    //so do we need to make visieted or was that made in neighbords?
    // so we need to walk down the shortest path and flip ' ' to 'o'
    

    //okay idk when to make the parents to eahc node
    // is it //node now to next node then node to 0?
    // 
    

    

    
}
