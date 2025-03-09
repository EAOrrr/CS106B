// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void computePowerIndexesHelper(Vector<int>& blocks, Vector<int>& result,int index, int soFar, int total){
    // Base Case
//    cout << blocks.size();
    if(index == blocks.size()){
        if(soFar <= total/2) return;
        for(int i = 0; i < blocks.size(); ++i){
            if(blocks[i] + soFar <= total/2){
                result[i]++;
            }
        }
        return;
    }
    // Explore
    // Exclude
    computePowerIndexesHelper(blocks, result, index+1, soFar, total);
    // Include
    blocks[index] = -blocks[index];
    computePowerIndexesHelper(blocks, result, index+1, soFar-blocks[index], total);
    // Unchoose
    blocks[index] = -blocks[index];
}
int computePowerIndexesHelper(Vector<int>& blocks, int target, int index, int soFar, int total){
    if(index >= blocks.size()){
        if(soFar <= total/2  && soFar + blocks[target] > total / 2) return 1;
        return 0;
    }
    if(soFar > total/2) return 0;
    if(index == target){
        // exclude
        return computePowerIndexesHelper(blocks, target, index+1, soFar, total);
    }
    int exclude = computePowerIndexesHelper(blocks, target, index+1, soFar, total);
    int include = computePowerIndexesHelper(blocks, target, index+1, soFar+blocks[index], total);
    return include + exclude;

}
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result(blocks.size(), 0);
    Vector<int> record{};
    // TODO your code here
    int total  = 0;
    for(int i = 0; i < blocks.size(); i++){
        total += blocks[i];
    }

//    computePowerIndexesHelper(blocks, result, 0, 0, total); // helper function 1: 24.380s

    for(int i = 0; i< result.size(); i++){
        result[i] = computePowerIndexesHelper(blocks, i, 0, 0, total);
    }  // helper function 2: 26.259s
    cout << result;
    int sum = 0;
    for(int i = 0; i < result.size(); i++){
        sum += result[i];
    }
    for(int i = 0; i < result.size(); i++){
        result[i] = result[i] * 100 / sum;
    }
    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
    TIME_OPERATION(27, computePowerIndexes(blocks));
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


