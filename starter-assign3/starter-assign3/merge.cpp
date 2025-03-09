/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
void queueCheck(Queue<int> que){
    if(que.isEmpty()) return;
    int prev = que.peek(), cur = que.peek();
    while(!que.isEmpty()){
        if(prev > cur){
            error("Invalid: queue is not acsending");
        }
        prev = cur;
        cur = que.dequeue();
    }
}

Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    queueCheck(a);  queueCheck(b);
    Queue<int> result{};
    /* TODO: Implement this function. */
    while(!a.isEmpty() || !b.isEmpty()){
        if(a.isEmpty()){
            while(!b.isEmpty()){
                result.enqueue(b.dequeue());
            }
            break;
        }
        if(b.isEmpty()){
            while(!a.isEmpty()){
                result.enqueue(a.dequeue());
            }
            break;
        }
        if(a.peek() < b.peek()){
            result.enqueue(a.dequeue());
        }else{
            result.enqueue(b.dequeue());
        }
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
//    Queue<int> result;
    /* TODO: Implement this function. */
    if(all.isEmpty()) return {};
    if(all.size() == 1) return all[0];
    Vector<Queue<int>> left = all.subList(0, all.size()/2);
    Vector<Queue<int>> right = all.subList(all.size()/2);
    Queue<int> leftQ = recMultiMerge(left);
    Queue<int> rightQ = recMultiMerge(right);
    Queue<int> result = binaryMerge(leftQ, rightQ);
    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}


STUDENT_TEST("Invalid input for BinaryMerge"){
    Queue<int> a{1, 4, 9}, b{4, 3, 5};
    EXPECT_ERROR(binaryMerge(a, b));
}

STUDENT_TEST("time operation for binaryMerge"){
    Queue<int> a = createSequence(100000);
    Queue<int> b = createSequence(100000);
    TIME_OPERATION(100000, binaryMerge(a, b));
    Queue<int> c = createSequence(200000);
    Queue<int> d = createSequence(200000);
    TIME_OPERATION(200000, binaryMerge(c, d));
    Queue<int> e = createSequence(400000);
    Queue<int> f = createSequence(400000);
    TIME_OPERATION(400000, binaryMerge(e, f));
}

STUDENT_TEST("more test for naiveMultiMerge"){
    Vector<Queue<int>> emptyVec{};
    Queue<int> expectedEmpty{};
    EXPECT_EQUAL(naiveMultiMerge(emptyVec), expectedEmpty);
    Vector<Queue<int>> singleVec{{1, 2, 3}};
    Queue<int> expectedSingle{1, 2, 3};
    EXPECT_EQUAL(naiveMultiMerge(singleVec), expectedSingle);
}

STUDENT_TEST("more time operation on naiveMultiMerge"){
    for(int n = 100000; n <= 500000; n+= 100000){
        Queue<int> que = createSequence(n);
        for(int k = 20; k < 25; k++){
            Vector<Queue<int>> all(k);
            distribute(que, all);
            TIME_OPERATION(n*k, naiveMultiMerge(all));
        }
    }
}
