/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include<cmath>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 * This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to sqrt(n), testing for a
 * zero remainder from the division.
 */
long smarterSum(long n) {
    /* TODO: Fill in this function. */
    if(n <= 0) return 0;
    long total = 0;
    for(long divisor = 1; divisor < sqrt(n); divisor++){
        if(n % divisor == 0){
            total += (divisor + n / divisor);
        }
    }
    return total - n;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 * This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfectSmarter(long n) {
    /* TODO: Fill in this function. */
    return n > 0 && n == smarterSum(n);
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void findPerfectsSmarter(long stop) {
     /* TODO: Fill in this function. */
        for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;

}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 * Taking Euclid's approach, we instead hop through the numbers by powers of two, checking each Mersenne number to see if it is prime, and if so, calculate its corresponding perfect number. The general strategy is outlined below:
 *Start by setting .k = 1
 *Calculate (Note: C++ has no exponentiation operator, instead use library function powm = 2k-1)
 *Determine whether is prime or composite. (Hint: a prime number has a equal to one. Code reuse is your friend!)mdivisorSum (and smarterSum)
 *If is prime, then the value is a perfect number. If this is the perfect number you have found, stop here.m2(k-1) * (2k-1)nth
 *Increment and repeat from step 2.k
 *The call should return the th perfect number.
 */
long findNthPerfectEuclid(long n) {
    /* TODO: Fill in this function. */
    long k = 1, num = 0;
    for(long m = 0; m < n; m++){
        while(true){
            long maybePrime = pow(2, k) - 1;
            if(smarterSum(maybePrime) == 1){
                num = pow(2, k-1) * (pow(2, k) - 1);
                k++;
                break;
            }
            k++;
        }
    }

    return num;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here
//STUDENT_TEST("practice with time_operation"){
//    TIME_OPERATION(10000, findPerfects(10000));
//    TIME_OPERATION(20000, findPerfects(20000));
//    TIME_OPERATION(40000, findPerfects(40000));
//    TIME_OPERATION(80000, findPerfects(80000));
//}

//STUDENT_TEST("time with smarter function"){
//    TIME_OPERATION(10000, findPerfectsSmarter(10000));
//    TIME_OPERATION(20000, findPerfectsSmarter(20000));
//    TIME_OPERATION(40000, findPerfectsSmarter(40000));
//    TIME_OPERATION(80000, findPerfectsSmarter(80000));
//}

//STUDENT_TEST("isPerfectfalse"){
//    EXPECT(!isPerfect(-6));
//    EXPECT(!isPerfect(-28));
//    EXPECT(!isPerfect(-1));
//    EXPECT(!isPerfect(-33243));
//}

STUDENT_TEST("SmarterSum"){
    EXPECT_EQUAL(smarterSum(8128), 8128);
    EXPECT_EQUAL(smarterSum(98), 73);
    EXPECT_EQUAL(smarterSum(27), 13);
    EXPECT_NO_ERROR(smarterSum(-6));
}

STUDENT_TEST("findNthPerfectEuclid"){
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}
