#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include"random.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */

void addNode(ListNode*& list, ListNode*&cur){
    if(list == nullptr){
        list = cur;
        cur = cur->next;    // advance cur
        list->next = nullptr; // cut the relationship with cur->next and prevent loop
    }
    else{
        ListNode* temp = cur;
        cur = cur->next;  // advance cur
        temp->next = list;
        list = temp;
    }
}

void partition(ListNode*front, ListNode*& less, ListNode*& equal, ListNode*& greater, int pivot){
    ListNode* curr = front;
    while(curr != nullptr){
        if(curr->data == pivot){
            addNode(equal, curr);
        }
        else if(curr->data > pivot){
            addNode(greater, curr);
        }
        else{// assume curr->data < pivot
            addNode(less, curr);
        }
    }
    return;
}

void concentrate(ListNode*&front, ListNode* less, ListNode*& equal, ListNode*& greater){
    front = less;
    ListNode* tail = front;
    if(front == nullptr){    // front=tail -> nullptr
        front = equal;
        tail = front;
    }else{  //e.g front -> 3->4->5
        // get tail to the end of the front
        while(tail->next != nullptr){
            tail = tail->next;
        }// tail->next =nullptr
        tail->next = equal;
    }
    if(front == nullptr){    // front=tail -> nullptr
        front = greater;
        tail = front;
    }else{  //e.g front -> 3->4->5
        // get tail to the end of the front
        while(tail->next != nullptr){
            tail = tail->next;
        }// tail->next =nullptr
        tail->next = greater;
    }
}

void quickSort(ListNode*& front) {
    /* TODO: Implement this function. */
    // Base case: if ListNode has only 1 or 0 elems, it has already been sorted
    if(front == nullptr || front->next == nullptr){  // thanks to short-hand operation
        return;
    }
    // Recursive case:
    // divide
    ListNode *less = nullptr, *greater = nullptr, *equal = nullptr;
    int pivot = front->data;
    partition(front, less, equal, greater, pivot);
    // sort
    quickSort(less);  quickSort(greater);
    // conquer
    front = nullptr;
    concentrate(front, less, equal, greater);
}


/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    /* TODO: Implement this function. */
   ListNode* cur = front;
   while(cur != nullptr){
       ListNode* temp = cur;
       cur = cur->next;
       delete temp;
   }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    /* TODO: Implement this function. */
   ListNode* head = nullptr, *tail = head;
   for(int i = 0, len = values.size(); i < len; i++){
       if(head == nullptr){
           head = new ListNode{values[i], nullptr};
           tail = head;
       }
       else{
           tail->next = new ListNode{values[i], nullptr};
           tail = tail->next;
       }
   }
    return head;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    /* TODO: Implement this function. */
    ListNode* cur = front;
    for(int i = 0, len = v.size(); i < len; i++, cur = cur->next){
       if(cur == nullptr) return false;
       if(cur->data != v[i]) return false;
    }
    if(cur != nullptr) return false;
    return true;
}

STUDENT_TEST("TEST ON PARTITION"){
    Vector<int> values = {6, 6, 7, 4, 3, 6, 5};
//    Vector<int> values = {3, 5, 1, 2, 8, 9};
    ListNode *less = nullptr, *greater = nullptr, *equal = nullptr;
    ListNode *front = createList(values);
//    printList(front);
    partition(front, less, equal, greater, front->data);
    /*printList(front);*/   printList(less); printList(equal); printList(greater);
    EXPECT(areEquivalent(less, {5, 3, 4}));  EXPECT(areEquivalent(equal, {6, 6, 6})); EXPECT(areEquivalent(greater, {7}));
    deallocateList(less);   deallocateList(equal);  deallocateList(greater);
//    deallocateList(front);
}

STUDENT_TEST("TEST ON CONCENTRATION"){
    ListNode *less = createList({3, 4, 5}), *greater = createList({}), *equal = createList({6, 6, 6});
    ListNode* front = nullptr;
    concentrate(front, less, greater, equal);
    deallocateList(front);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("A test for an empty list"){
    /* Initialize vector to sequence of values */
    Vector<int> values = {};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Test on only single-elem list"){
    Vector<int> values= {3};
    ListNode* list = createList(values);
    quickSort(list);
    EXPECT(areEquivalent(list, values));
    deallocateList(list);

}

STUDENT_TEST("Test on list in reverse order"){
    Vector<int> values= {7,6,5,4,3,2,1};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    printList(list);
    EXPECT(areEquivalent(list, values));
    deallocateList(list);

}

STUDENT_TEST("Test on duplicate elems"){
    Vector<int> values= {5,7,4,2,3,6,7,5,4,3,2,1,1};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    printList(list);
    EXPECT(areEquivalent(list, values));
    deallocateList(list);

}

STUDENT_TEST("Test on all duplicate elems"){
    Vector<int> values= {1, 1, 1};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    printList(list);
    EXPECT(areEquivalent(list, values));
    deallocateList(list);

}

STUDENT_TEST("Test on all both negative and positive elems"){
    Vector<int> values= {5, -9,-7, -87,4,2,3,6,7,5,4,3, -4,2,1,1};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    printList(list);
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}

STUDENT_TEST(
    "Sorting a list of random values, randomly organized"){
    Vector<int> values{};
    for(int i = 0; i < 20000; i++){
       values.add(randomInteger(-10000, 20000));
    }
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}


PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

STUDENT_TEST("Test for recommending helper functions on empty vector"){
    ListNode* studentList = createList({});
    printList(studentList);
    EXPECT(!areEquivalent(studentList, {1, 2}));
    EXPECT(areEquivalent(studentList, {}));
    deallocateList(studentList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

