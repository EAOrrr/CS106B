// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include"console.h"
#include"simpio.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
/*Trim all punctuation from the beginning and end of str, but not from inside str.
*Confirm the token contains at least one letter character.
*Convert the token to lowercase*/
string cleanToken(string s)
{
    // Trim all punctuation from the beginning and end of str, but not from inside str.
    size_t begin = 0, end = s.length();
    for(size_t i = 0, len =  s.length();i < len; i++){
        if(!ispunct(s[i])) break;
        begin++;
    }
    for(size_t j = s.length()-1; j >= 0; j--){
        if(!ispunct(s[j])) break;
        end--;
    }
    if(begin >= end) return "";

    s = s.substr(begin, end-begin);
//    Confirm the token contains at least one letter character.
    bool has_ch = false;
    for(char ch: s){
        if(isalpha(ch)){
            has_ch = true;
            break;
        }
    }
    if(!has_ch) return "";
//  *Convert the token to lowercase*/
    return toLowerCase(s);
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
/*The helper function gatherTokens extracts the set of unique tokens from the body text.*/
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> text_tokens = stringSplit(text, " ");
    for(auto& elem: text_tokens){
        string word = cleanToken(elem);
        if(word != "")
            tokens.add(word);
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream input(dbfile);
    if(!input.is_open()){
        error("failed to open " + dbfile);
    }
    int page = 0;
    string url, text;
    while(getline(input, url) && getline(input, text)){
        Set<string> tokens = gatherTokens(text);
        for(auto& token: tokens){
            if(!index.containsKey(token)){
                index[token] = {};
            }
            index[token].add(url);
        }
        page++;
    }
    return page;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result{};
    // TODO: your code here
    // split queue
    Vector<string> vec = stringSplit(query, " ");
    Vector<char> tokens{};
    for(auto& word: vec){
        word = toLowerCase(word);
        if(ispunct(word[0])){
            tokens.add(word[0]);
            word = word.substr(1);
        }
        else{
            tokens.add(0);
        }
    }
    for(int i = 0, len = vec.size(); i < len; i++){
        Set<string>temp = index[vec[i]];
        if(tokens[i] == '+'){
            result.intersect(temp);
        }
        else if(tokens[i] == '-'){
            result.difference(temp);
        }
        else{
            result.unionWith(temp);
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    cout << "Stand by while building index..." << endl;
    Map<string, Set<string>> index{};
    int npages = buildIndex(dbfile, index);
    cout << "Indexed "<<npages << " pages containing "<< index.size() <<" unique terms";
    while(true){
        string query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if(query == "") break;
        Set<string> result = findQueryMatches(index, query);
        cout << "Found " << result.size() << " matching pages"<< endl;
        cout << result << endl;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("cleanToken on all tokens"){
    EXPECT_EQUAL(cleanToken("!!!!!!!"), "");
    EXPECT_EQUAL(cleanToken("+(..."), "");
}

STUDENT_TEST("cleanToken on all both punct in the middle and in the front and end"){
    EXPECT_EQUAL(cleanToken("!!<awe--some..."), "awe--some");
}
//STUDENT_TEST("buildIndex print out for testing"){
//    Map<string, Set<string>> index{};
//    buildIndex("res/tiny.txt", index);
//    auto printset = [](Set<string> &urls){
//        int index = 0;
//        for(auto& url: urls){
//            cout << "  " << index++ << ":" << url << endl;
//        }
//    };
//    auto printmap = [&printset](Map<string, Set<string>>& index){
//        for(auto& word: index){
//            cout << word<< endl;
//            printset(index[word]);
//        }
//    };
//    printmap(index);
//}
STUDENT_TEST("buildIndex-operation time >"){
    Map<string, Set<string>> index{};
    TIME_OPERATION(100, buildIndex("res/website.txt", index));
}

STUDENT_TEST("findQueryMatches on additional test"){
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "REd fISh");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "rEd +fIsh");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "reD -Fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}
