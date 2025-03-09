#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    /* TODO: Implement this function. */
    string result{};
    EncodingTreeNode* currNode = tree;
    while(!messageBits.isEmpty()){
        Bit curBit = messageBits.dequeue();
        if(curBit == 0){
            currNode = currNode->zero;
        }else{
            currNode = currNode->one;
        }
        if(currNode->isLeaf()){
            result = result + charToString(currNode->getChar());
            currNode = tree;
        }
    }
    return result;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    Bit currBit = treeShape.dequeue();
    if(currBit == 0){
        return new EncodingTreeNode(treeLeaves.dequeue());
    }
    // assume currBit == 1
    EncodingTreeNode* zero = unflattenTree(treeShape, treeLeaves);
    EncodingTreeNode* one = unflattenTree(treeShape, treeLeaves);
    return new EncodingTreeNode(zero, one);
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    /* TODO: Implement this function. */
    EncodingTreeNode* tree = unflattenTree(data.treeShape, data.treeLeaves);
    string text = decodeText(tree, data.messageBits);
    deallocateTree(tree);
    return text;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    /* TODO: Implement this function. */
    Map<char, int> wordCount{};
    for(char ch: text){
        wordCount[ch]++;
    }
    PriorityQueue<EncodingTreeNode*> pq{};
    for(char ch: wordCount){
        pq.enqueue(new EncodingTreeNode(ch), wordCount[ch]);
    }
    while(true){
        EncodingTreeNode *t1{nullptr}, *t2{nullptr};
        double priority = 0;
        priority += pq.peekPriority();
        t1 = pq.dequeue();
        if(pq.isEmpty()) return t1;
        priority += pq.peekPriority();
        t2 = pq.dequeue();
        EncodingTreeNode* tree = new EncodingTreeNode(t1, t2);
        pq.enqueue(tree, priority);
    }
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void encodeTextHelper(Map<char, string>& dict, EncodingTreeNode* tree, string soFar){
    if(tree->isLeaf()){
        dict[tree->getChar()] = soFar;
    }
    else{
        // traverse zero child
        encodeTextHelper(dict, tree->zero, soFar+"0");
        // traverse one child
        encodeTextHelper(dict, tree->one, soFar+"1");
    }
}
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    /* TODO: Implement this function. */
    Queue<Bit> result{};
    Map<char, string> dict{};
    encodeTextHelper(dict, tree, "");
    for(char ch: text){
        for(char elem: dict[ch]){
            result.enqueue(charToInteger(elem));
        }
    }
    return result;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    if(tree->isLeaf()){
        treeLeaves.enqueue(tree->getChar());
        treeShape.enqueue(0);
        return;
    }
    treeShape.enqueue(1);
    flattenTree(tree->zero, treeShape, treeLeaves);
    flattenTree(tree->one, treeShape, treeLeaves);
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    /* TODO: Implement this function. */
    EncodedData data;
    EncodingTreeNode* tree =buildHuffmanTree(messageText);
    data.messageBits = encodeText(tree, messageText);
    flattenTree(tree, data.treeShape, data.treeLeaves);
    deallocateTree(tree);
    return data;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* T = new EncodingTreeNode('T');
    EncodingTreeNode* E = new EncodingTreeNode('E');
    EncodingTreeNode* tmp2 = new EncodingTreeNode(R, S);
    EncodingTreeNode* tmp1 = new EncodingTreeNode(tmp2, E);
    EncodingTreeNode* tmp = new EncodingTreeNode(T, tmp1);
    return tmp;
}

void deallocateTree(EncodingTreeNode* t) {
    /* TODO: Implement this utility function needed for testing. */
    if(t == nullptr) return;
    deallocateTree(t->zero);
    deallocateTree(t->one);
    delete t;
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    /* TODO: Implement this utility function needed for testing. */
    if(a == nullptr && b == nullptr) return true;
    if(a == nullptr) return false;
    if(b == nullptr) return false;
    if(a->isLeaf() && b->isLeaf()){
        if(a->getChar() != b ->getChar()){
            return false;
        }
//        return true;
    }
    return areEqual(a->one, b->one) && areEqual(a->zero, b->zero);
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */
STUDENT_TEST("deallocateTree: Test for utility function DeallocateTree"){
    EncodingTreeNode* tree = createExampleTree();
    deallocateTree(tree);
}

STUDENT_TEST("areEqual: a singleton tree of one leaf node compare to an empty tree"){
    EncodingTreeNode* t1 = new EncodingTreeNode('H');
    EncodingTreeNode* t2 = nullptr;
    EXPECT(!areEqual(t1, t2));
    deallocateTree(t1); deallocateTree(t2);
}

STUDENT_TEST("areEqual: Compare a singleton tree of one leaf node with another tree with the same character"){
    EncodingTreeNode* t1 = new EncodingTreeNode('H');
    EncodingTreeNode* t2 = nullptr;
    EXPECT(!areEqual(t1, t2));
    deallocateTree(t1); deallocateTree(t2);
}

STUDENT_TEST("areEqual: Compare a singleton tree of one leaf node with another tree with a different character"){
    EncodingTreeNode* t1 = new EncodingTreeNode('H');
    EncodingTreeNode* t2 = new EncodingTreeNode('H');
    EXPECT(areEqual(t1, t2));
    EXPECT(areEqual(t1, t1));
    deallocateTree(t1); deallocateTree(t2);
}

STUDENT_TEST("areEqual: Compare the example tree with the single-leaf tree"){
    EncodingTreeNode* t1 = new EncodingTreeNode('H');
    EncodingTreeNode* t2 = createExampleTree();
    EXPECT(!areEqual(t1, t2));
    deallocateTree(t1); deallocateTree(t2);
}

STUDENT_TEST("areEqual: Compare the example tree with another example tree"){
    EncodingTreeNode* t1 = createExampleTree();
    EncodingTreeNode* t2 = createExampleTree();
    EXPECT(areEqual(t1, t2));
    EXPECT(areEqual(t1, t1));
    deallocateTree(t1); deallocateTree(t2);
}



STUDENT_TEST("areEqual: Compare the example tree with one of its subtree"){
    EncodingTreeNode* t1 = createExampleTree();
    EXPECT(!areEqual(t1, t1->zero)); EXPECT(!areEqual(t1->one, t1));
    deallocateTree(t1);
}


STUDENT_TEST("decodeText, small example encoding tree") {
    /*
     *      *
     *     / \
     *    *   O
     *   / \
     *  N   *
     *     / \
     *    M   S
     *    O: 1; N: 00;  M: 010; S:011
    */
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* M = new EncodingTreeNode('M');
    EncodingTreeNode* N = new EncodingTreeNode('N');
    EncodingTreeNode* O = new EncodingTreeNode('O');
    EncodingTreeNode* tmp2 = new EncodingTreeNode(M, S);
    EncodingTreeNode* tmp1 = new EncodingTreeNode(N, tmp2);
    EncodingTreeNode* tree = new EncodingTreeNode(tmp1, O);

    EXPECT(tree != nullptr);

    Queue<Bit> messageBits1 = {1}; // 'O'
    EXPECT_EQUAL(decodeText(tree, messageBits1), "O");

    Queue<Bit> messageBits2 = {0, 0}; // 'N'
    EXPECT_EQUAL(decodeText(tree, messageBits2), "N");

    Queue<Bit> messageBits3 = {0, 1, 1}; // 'S'
    EXPECT_EQUAL(decodeText(tree, messageBits3), "S");

    Queue<Bit> messageBits4 = {0,1,0,1,1,0,0,0,1,1}; // MOONS
    EXPECT_EQUAL(decodeText(tree, messageBits4), "MOONS");

    deallocateTree(tree);
}

STUDENT_TEST("unflatten: MORE TEST and DEBUG"){
    Queue<Bit>  expectedShape  = {1,1,0,1,0,0,0};
    Queue<char> expectedLeaves = {'N', 'M', 'S', 'O'};
    EncodingTreeNode* tree1 = unflattenTree(expectedShape, expectedLeaves);
    Queue<Bit>  expectedShape1 = {1,0,1,1,0,0,0};
    Queue<char> expectedLeaves1 = {'A', 'D', 'B', 'N'};
    EncodingTreeNode* tree2 = unflattenTree(expectedShape1, expectedLeaves1);
    deallocateTree(tree1);  deallocateTree(tree2);

}

STUDENT_TEST("decompose:More test on decompse"){
    Queue<Bit> messageBits4 = {0,1,0,1,1,0,0,0,1,1}; // MOONS
    Queue<Bit>  expectedShape  = {1,1,0,1,0,0,0};
    Queue<char> expectedLeaves = {'N', 'M', 'S', 'O'};
    EncodedData data = {expectedShape, expectedLeaves, messageBits4};
    EXPECT_EQUAL(decompress(data), "MOONS");
    Queue<Bit>  expectedShape1 = {1,0,1,1,0,0,0};
    Queue<char> expectedLeaves1 = {'A', 'D', 'B', 'N'};
    Queue<Bit> messageBits = {1,0,1,0,1,1,1,0,0}; // BAND
    data = {expectedShape1, expectedLeaves1, messageBits};
    EXPECT_EQUAL(decompress(data), "BAND");

}

STUDENT_TEST("encodeText: test with decoder"){
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* M = new EncodingTreeNode('M');
    EncodingTreeNode* N = new EncodingTreeNode('N');
    EncodingTreeNode* O = new EncodingTreeNode('O');
    EncodingTreeNode* tmp2 = new EncodingTreeNode(M, S);
    EncodingTreeNode* tmp1 = new EncodingTreeNode(N, tmp2);
    EncodingTreeNode* tree = new EncodingTreeNode(tmp1, O);

    Queue<Bit> b = encodeText(tree, "MOONS");
    EXPECT_EQUAL(decodeText(tree, b), "MOONS");
    Queue<Bit> b1 = encodeText(tree, "SOON");
    EXPECT_EQUAL( decodeText(tree, b1),"SOON");
    Queue<Bit> b2 = encodeText(tree, "SOMOSNOMNOMMMMO");
    EXPECT_EQUAL(decodeText(tree, b2), "SOMOSNOMNOMMMMO");

    deallocateTree(tree);
}





/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}

