/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result{};
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}
/*The function is intended to return a string which converts s to the digit
 * of soundextable
*/
string toTableDigit(string s){
    vector<string> table{"AEIOUHWY", "BFPV", "CGJKQSXZ", "DT", "L", "MN", "R"};
    string result{};
    for(char c: s){
        for(size_t i = 0; i < 7; i++){
            if(table[i].find(c) != string::npos){
                result += integerToString(i);
                break;
            }
        }
    }
    return result;
}
/*This function is intended to Coalesce adjacent duplicate digits from the code*/
string removeDuplicate(string s){
    char pre = 'a';
    string result{};
    for(char c:s){
        if(c != pre){
            result += c;
        }
        pre = c;
    }
    return result;
}
/*This function is intended to Remove all zeros from the code.*/
string removeZero(string s){
    string result{};
    for(char c:s){
        if(c != '0')
            result += c;
    }
    return result;
}
string truncate(string s){
    if(s.length() > 3)
        return s.substr(0, 3);
    string zeros(3-s.length(), '0');
    return s + zeros;
}
/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
    /* TODO: Fill in this function. */
    // O'Conner -> OConner
    s = removeNonLetters(s);
    // OConner -> OCONNER
    s = toUpperCase(s);
    // OCONNER ->0205506
    string result = toTableDigit(s);
    // 0205506 -> 020506
    result = removeDuplicate(result);
    // 020506 -> s[0]+256
    result = removeZero(result.substr(1));
    // 2445 -> 244; 2->200
    result = truncate(result);
    return s[0]+result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
    while(true){
        string surname = getLine("Please enter a surname (ENTER/RETURN to quit): ");
        if(surname =="") break;
        string sdx = soundex(surname);
        cout << "Soundex code is " << sdx << endl;
        Vector<string> vec;
        for(string& name: databaseNames){
            if(sdx == soundex(name)) vec.add(name);
        }
        vec.sort();
        cout << "Matches from database: ";
        cout << vec << endl;
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("My test for removeNonLetters"){
    EXPECT_EQUAL(removeNonLetters("'H ello"), "Hello");
}

STUDENT_TEST("test for digitable"){
    EXPECT_EQUAL(toTableDigit("OCONNER"), "0205506");
    EXPECT_EQUAL(toTableDigit("ANGELOU"), "0520400");
}

STUDENT_TEST("TEST for removeDuplicate"){
    EXPECT_EQUAL(removeDuplicate("22250666"), "2506");
    EXPECT_EQUAL(removeDuplicate("4324444343"), "4324343");
    EXPECT_EQUAL(removeDuplicate("0205506"), "020506");
}
STUDENT_TEST("TEST for removeZero"){
    EXPECT_EQUAL(removeZero("2506"), "256");
    EXPECT_EQUAL(removeZero("0320043"), "3243");
    EXPECT_EQUAL(removeZero("43433"), "43433");
}
STUDENT_TEST("TEST for truncate"){
    EXPECT_EQUAL(truncate("256"), "256");
    EXPECT_EQUAL(truncate("32423424353"), "324");
    EXPECT_EQUAL(truncate("1"), "100");
    EXPECT_EQUAL(truncate(""), "000");
}
