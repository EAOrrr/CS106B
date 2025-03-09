/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int points(string str) {
    /* TODO: Implement this function. */
    if(str.length() > 3) return str.length() - 3;
    return 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Set<GridLocation> generateValidNeighbors(Grid<char>& board, GridLocation curr){
    Set<GridLocation> neighbors{};
    for(int i = -1; i <= 1; ++i){
        for(int j = -1; j <= 1; ++j){
            if(i == 0 && j == 0) continue;
            if(curr.row + i < 0 || curr.row + i >= board.numRows() ||
                curr.col + j < 0 || curr.col + j >= board.numCols()){
                continue;
            }
            GridLocation neighbor{curr.row+i, curr.col+j};
            if(board[neighbor] != '_')
                neighbors.add(neighbor);
        }
    }
    return neighbors;
}

int scoreBoardHelper(Grid<char> &board, Lexicon &lex, Set<string>& visited, GridLocation curr, string soFar){
    cout << soFar << ' ';
    // Pruning
    if(!lex.containsPrefix(soFar)){
        return 0;
    }
    // Base Case
    int cnt = 0;
    if(lex.contains(soFar)){
        // check if soFar in visited
        if(!visited.contains(soFar)){
            // add soFar to delete
            visited.add(soFar);
            // add some point
            cnt += points(soFar);
        }
    }
    Set<GridLocation> neighbors = generateValidNeighbors(board, curr);
    // mark
    char letter = board[curr];
    board[curr] = '_';
    for(GridLocation neighbor: neighbors){
        // explore
        cnt += scoreBoardHelper(board, lex, visited, neighbor, soFar + board[neighbor]);
    }
    // undo
    board[curr] = letter;
    return cnt;
}
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /* TODO: Implement this function. */
    Grid<char> playBoard = board;
    int cnt = 0;
    Set<string> visited{};
    for(int i = 0; i < board.numRows(); ++i){
        for(int  j = 0; j < board.numCols(); ++j){

            GridLocation start {i, j};
            string str = charToString(playBoard[start]);
            cnt += scoreBoardHelper(board, lex, visited, start, str);
        }
    }
//    cout << visited << endl;
    cout << endl;
    return cnt;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("generateValidNeighbors tests"){
    Grid<char> board = {{'E','_','A','R'},
                        {'L','V','T','S'},
                        {'R','A','_','_'},
                        {'O','I','S','E'}};
    GridLocation startT{1, 2};
    std::cout << board[startT];
    Set<GridLocation> expectedT= {{0,2}, {0,3},{1,1},{1,3},{2,1}};
    EXPECT_EQUAL(generateValidNeighbors(board, startT), expectedT);
    GridLocation startR{2, 0};
    Set<GridLocation> expectedR= {{1,0}, {1,1},{2,1},{3,0},{3,1}};
    EXPECT_EQUAL(generateValidNeighbors(board, startR), expectedR);
    GridLocation startE{3, 3};
    Set<GridLocation> expectedE= {{3, 2}};
    EXPECT_EQUAL(generateValidNeighbors(board, startE), expectedE);
}

STUDENT_TEST("scoreBoard to beat computer"){
    Grid<char> board = {{'S','T','N','G'},
                        {'I','L','E','Y'},
                        {'N','P','S','V'},
                        {'A','Y','I','Q'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 199);
}
