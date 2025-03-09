// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* TODO: Replace this comment with a descriptive function
 * header comment.
 * generate the neighbors for a given location:
 * Valid moves are those GridLocations that are:

    *Exactly one "step" away from cur in one of the four cardinal directions (N, S, E, W)
    *Within bounds for the Grid
    *An open corridor, not a wall
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    Set<GridLocation> nexts = {GridLocation(cur.row+1, cur.col),
                                   GridLocation(cur.row-1, cur.col),
                                   GridLocation(cur.row, cur.col+1),
                                   GridLocation(cur.row, cur.col-1)};
    /* TODO: Fill in the remainder of this function. */
    for(auto neighbor: nexts){
        if(maze.inBounds(neighbor) && maze.get(neighbor) == true){
           neighbors.add(neighbor);
        }
    }
    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 * A path is a valid solution to a maze if it meets the following criteria:

    *The path must start at the entry (upper left corner) of the maze
    *The path must end at the exit (lower right corner) of the maze
    *Each location in the path is a valid move from the previous location in the path
        Hint: rather than re-implement the same logic you already did for generateValidMoves, simply call that function and check whether a move is contained in the set of valid moves.
    *The path must not contain a loop, i.e. the same location cannot appear more than once in the path
        Hint: a Set<GridLocation> is a good data structure for tracking seen locations and avoiding a repeat.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    /* TODO: Fill in the remainder of this function. */
    Set<GridLocation> visited{};
    while(path.size() > 1){
        GridLocation curr = path.pop();
        GridLocation back = path.peek();
        if(visited.contains(curr)){
           error("Revist: The path contains a loop");
        }
        Set<GridLocation> validForCurr = generateValidMoves(maze, curr);
        Set<GridLocation> validForBack = generateValidMoves(maze, back);
        if(!validForBack.contains(curr) || !validForCurr.contains(back)){
           error("Each location in the path is a valid move from the previous location in the path");
        }
        visited.add(curr);
    }
    GridLocation mazeEntry{0, 0};
    GridLocation start = path.pop();
    if(visited.contains(start)){
        error("Revist: The path contains a loop");
    }
    if(start != mazeEntry){
        error("Path does not start at maze entry");
    }
    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */
    GridLocation entry = {0, 0}, exit = {maze.numRows()-1, maze.numCols()-1};
    Set<GridLocation> visited;
    Queue<Stack<GridLocation>> allPath;
    path.push(entry);
    allPath.enqueue(path);

    while(!allPath.isEmpty()){
        Stack<GridLocation> currPath = allPath.dequeue();
        MazeGraphics::highlightPath(currPath, "blue");
        if(currPath.peek() == exit){
           MazeGraphics::highlightPath(currPath, "green");
           return currPath;
        }
        Set<GridLocation> neighbors = generateValidMoves(maze, currPath.peek());
        for(auto neighbor: neighbors){
           if(!visited.contains(neighbor)){
               Stack<GridLocation> newPath = currPath;
               newPath.push(neighbor);
               allPath.enqueue(newPath);
               visited.add(neighbor);
           }
        }
    }
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("generateValidMoves on only a valid way of a 3x3 Maze"){
    Grid<bool> maze = {{false, true, false},
                       {false, true, false},
                       {false, false, false}};
    GridLocation start = {1, 1};
    Set<GridLocation> expected={{0, 1}};
    EXPECT_EQUAL(generateValidMoves(maze, start), expected);
}

STUDENT_TEST("generateValidMoves on no valid path of a 3x3 maze"){
    Grid<bool> maze = {{false, false, false},
                        {false, true, false},
                       {false, false, false}};
    GridLocation start = {1, 1};
    Set<GridLocation> expected{};
    EXPECT_EQUAL(generateValidMoves(maze, start), expected);
}

STUDENT_TEST("generateValidMoves on 2 valid path and 2 invaild path of a 3x3 maze"){
    Grid<bool> maze = {{true, true, false},
                        {false, true, true},
                       {false, false, true}};
    GridLocation start = {1, 1};
    Set<GridLocation> expected{{0, 1}, {1, 2}};
    EXPECT_EQUAL(generateValidMoves(maze, start), expected);
}

STUDENT_TEST("validatePaths on a 3x3 maze"){
    Grid<bool> maze = {{true, true, false},
                       {false, true, true},
                       {false, false, true}};
    Stack<GridLocation> soln{{0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 2}};
    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePaths on a 3x3 all true matrix"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    Stack<GridLocation> teleport{{0, 0}, {1, 1}, {2, 2}};
    Stack<GridLocation> walk_all{{0, 0}, {0, 1}, {0, 2}, {1, 2}, {1, 1}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
    Stack<GridLocation> revisited{{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisited));
    EXPECT_NO_ERROR(validatePath(maze, walk_all));
}

STUDENT_TEST("validatePath on a 3x3 matrix"){
    Grid<bool> maze = {{true, false, false},
                       {true, true, false},
                       {true, true, true}};
    Stack<GridLocation> walk_all{{0, 0}, {0, 1}, {0, 2}, {1, 2}, {1, 1}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
    Stack<GridLocation> out_of_bound{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {3, 1}, {2, 1}, {2, 2} };
    EXPECT_ERROR(validatePath(maze, walk_all));
    EXPECT_ERROR(validatePath(maze, out_of_bound));
}

STUDENT_TEST("solveMaze on file 17x37"){
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 33x41"){
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 19x35"){
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}


