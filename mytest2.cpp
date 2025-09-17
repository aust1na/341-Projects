// UMBC - CMSC 341 - Spring 2025 - Proj3 test file                                                                                                                                                          
// Name: Austin Phan                                                                                                                                                                                        
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include "squeue.h"
using namespace std;

// Random number generator class (given and copied to mytest.cpp)                                                                                                                                           
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) :
        m_min(min), m_max(max), m_type(type) {
        if (type == NORMAL) {
            m_generator = std::mt19937(m_device());
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            m_generator = std::mt19937(10);
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) {
            m_generator = std::mt19937(10);
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else {
            m_generator = std::mt19937(m_device());
        }
    }

    int getRandNum() {
        int result = 0;
        if(m_type == NORMAL) {
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT) {
            result = m_unidist(m_generator);
        }
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;
    std::uniform_int_distribution<> m_unidist;
    std::uniform_real_distribution<double> m_uniReal;
};

class Tester {
public:
    bool insertMinHeapNormal();
    bool insertMaxHeapNormal();
    bool removeMinHeapNormal();
    bool removeMaxHeapNormal();
    bool leftistHeapNPLValue();
    bool leftistHeapProperty();
    bool changePriorityFunction();
    bool mergeQueue();
    bool squeueCopyConstructorNormal();
    bool squeueCopyConstructorEdge();
    bool squeueAssignmentOpNormal();
    bool squeueAssignmentOpEdge();
    bool dequeueEmptyQueue();
    bool mergeQueueAndDifferentPriorityFunction();

    /******************************************                                                                                                                                                             
     * Private function declarations go here! *                                                                                                                                                             
    ******************************************/
    typedef int (*prifn_t)(const Post&); // priority function                                                                                                                                               
    bool isValidMinHeapHelper(Post* node, prifn_t priFn); // helper function to test validity of minheap                                                                                                    
    bool isValidMaxHeapHelper(Post* node, prifn_t priFn); // helper function to test validity of maxheap                                                                                                    
    bool validateNPLHelper(Post* node); // helper function to validate NPL                                                                                                                                  
    bool validateLeftistPropertyHelper(Post* node); // helper function to validate leftist property                                                                                                         
};

int main() {
    // create tester object                                                                                                                                                                                 
    Tester test;

    // test cases below:                                                                                                                                                                                    

    if (test.insertMinHeapNormal()) {
        cout << "Pass: min-heap inserted (normal)" << endl;
    } else {
        cout << "Fail: didn't insert min-heap (normal) fail" << endl;
    }

    if (test.insertMaxHeapNormal()) {
        cout << "Pass: inserted max-heap (normal)" << endl;
    } else {
        cout << "Fail: didn't insert max-heap (normal)" << endl;
    }

    if (test.removeMinHeapNormal()) {
        cout << "Pass: removed min-heap (normal) " << endl;
    } else {
        cout << "Fail: didn't remove min-heap (normal) " << endl;
    }

    if (test.removeMaxHeapNormal()) {
        cout << "Pass: remove max-heap (normal)" << endl;
    } else {
        cout << "Fail: didn't remove max-heap (normal)" << endl;
    }

    if (test.leftistHeapNPLValue()) {
        cout << "Pass: leftist heap have correct NPL values" << endl;
    } else {
        cout << "Fail: leftist heap doesn't have correct NPL values" << endl;
    }

   if (test.leftistHeapProperty()) {
        cout << "Pass: leftist heap preserves property of such a heap" << endl;
    } else {
        cout << "Fail: Leftist heap doesn't preserve property of such a heap" << endl;
    }

    if (test.changePriorityFunction()) {
        cout << "Pass: correct heap rebuilt with same data and different priority function" << endl;
    } else {
        cout << "Fail: incorrect heap rebuilt" << endl;
    }

    if (test.mergeQueue()) {
        cout << "Pass: merged empty queue with normal queue" << endl;
    } else {
        cout << "Fail: could not merge empty queue with normal queue" << endl;
    }

    if (test.squeueCopyConstructorNormal()) {
        cout << "Pass: Copy Constructor (normal) works" << endl;
    } else {
        cout << "Fail: Copy Constructor (normal) failed" << endl;
    }

    if (test.squeueCopyConstructorEdge()) {
        cout << "Pass: copy constructor (edge) works" << endl;
    } else {
        cout << "Fail: copy constructor (edge) failed" << endl;
    }

    if (test.squeueAssignmentOpNormal()) {
        cout << "Pass: assignment operator (normal) works" << endl;
    } else {
        cout << "Fail: assignment operator (normal) failed" << endl;
    }

    if (test.squeueAssignmentOpEdge()) {
        cout << "Pass: assignment operator (edge) works" << endl;
    } else {
        cout << "Fail: assignment operator (edge) failed" << endl;
    }

    if (test.dequeueEmptyQueue()) {
        cout << "Pass: dequeued an empty queue" << endl;
    } else {
        cout << "Fail: didn't dequeue an empty queue (no exception thrown)" << endl;
    }

    if (test.mergeQueueAndDifferentPriorityFunction()) {
        cout << "Pass: merged queues with different priority functions" << endl;
    } else {
        cout << "Fail: didn't merge queues with different priority functions (no exception thrown)" << endl;
    }
}
/*  Name: priorityFn1(const Post& post)                                                                                                                                                                     
    Desc: priority function for MINHEAP type                                                                                                                                                                
*/
int priorityFn1(const Post& post) {
    // lower likes = higher priority                                                                                                                                                                        
    return post.getNumLikes();
}


/*  Name priorityFn2(const Post& post)                                                                                                                                                                      
    Desc: priority function for MAXHEAP type                                                                                                                                                                
*/
int priorityFn2(const Post& post) {
    // higher interest = higher priority                                                                                                                                                                    
    return post.getInterestLevel();
}


/*  Name: isValidMinHeapHelper(Post* node, prifn_t priFn)                                                                                                                                                   
    Desc: Helper tester function to test the validity of the heap (min)                                                                                                                                     
*/
bool Tester::isValidMinHeapHelper(Post* node, prifn_t priFn) {
    // base case: null node is valid                                                                                                                                                                        
    if (node == nullptr) {
        return true;
    }

    bool valid = true;

    // check left child (expected to have > or = priority)                                                                                                                                                  
    if (node->m_left != nullptr) {
        // validate min heap conditions and recurse                                                                                                                                                         
        valid = valid && (priFn(*node) <= priFn(*node->m_left)) && isValidMinHeapHelper(node->m_left, priFn);
    }
    // check right child (expected to have > or = priority)                                                                                                                                                 
    if (node->m_right != nullptr) {
        // validate min heap conditions and recurse                                                                                                                                                         
        valid = valid && (priFn(*node) <= priFn(*node->m_right)) && isValidMinHeapHelper(node->m_right, priFn);
    }

    // return true if checks are passed                                                                                                                                                                     
    return valid;
}


/*  Name: isValidMaxHeapHelper(Post* node, prifn_t priFn)                                                                                                                                                   
    Desc: Helper tester function to test the validity of the heap (max)                                                                                                                                     
*/
bool Tester::isValidMaxHeapHelper(Post* node, prifn_t priFn) {
    // base case: null node is valid by default                                                                                                                                                             
    if (node == nullptr) {
        return true;
    }

    bool valid = true;

    // check left child                                                                                                                                                                                     
    if (node->m_left != nullptr) {
        // validate max heap conditions and recurse                                                                                                                                                         
        valid = valid && (priFn(*node) >= priFn(*node->m_left)) && isValidMaxHeapHelper(node->m_left, priFn);
    }
    // check right child                                                                                                                                                                                    
    if (node->m_right != nullptr) {
        // validate max heap conditions and recurse                                                                                                                                                         
        valid = valid && (priFn(*node) >= priFn(*node->m_right)) && isValidMaxHeapHelper(node->m_right, priFn);
    }

    // return true if all conditions are met                                                                                                                                                                
    return valid;
}

/*  Name: insertMinHeapNormal()                                                                                                                                                                             
    Desc: Test insertion for a normal case of min-heap.                                                                                                                                                     
          After a decent number of insertion (e.g. 300 nodes)                                                                                                                                               
          we traverse the tree and check that the heap property is satisfied at every node.                                                                                                                 
*/
bool Tester::insertMinHeapNormal() {
    // create a new min heap squeue object with skew structure and priorityFn1                                                                                                                              
    SQueue queue(priorityFn1, MINHEAP, SKEW);

    // generate random id and likes number                                                                                                                                                                  
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);

    // insert 300 posts with random data into heap                                                                                                                                                          
    for (int i = 0; i < 300; i++) {
        int id = idGen.getRandNum();
        int likes = likesGen.getRandNum();

        // create a post (likes determine priority)                                                                                                                                                         
        Post p(id, likes, 1, 1, 5);

        // insert post, if any fails return false                                                                                                                                                           
        if (!queue.insertPost(p)) {
            return false;
        }
    }

    // confirm total number of posts being exactly 300                                                                                                                                                      
    if (queue.m_size != 300) {
        return false;
    }

    // check min-heap property                                                                                                                                                                              
    return isValidMinHeapHelper(queue.m_heap, priorityFn1);
}

/*  Name: insertMaxHeapNormal()                                                                                                                                                                             
    Desc: Test insertion for a normal case of max-heap.                                                                                                                                                     
          After a decent number of insertion (e.g. 300 nodes)                                                                                                                                               
          we traverse the tree and check that the heap property is satisfied at every node.                                                                                                                 
*/
bool Tester::insertMaxHeapNormal() {
    // create max heap squeue object with skew structure and priorityFn2                                                                                                                                    
    SQueue queue(priorityFn2, MAXHEAP, SKEW);

    // create random id and interest number                                                                                                                                                                 
    Random idGen(MINPOSTID, MAXPOSTID);
    Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);

    // insert 300 posts with random data into heap                                                                                                                                                          
    for (int i = 0; i < 300; i++) {
        int id = idGen.getRandNum();
        int interest = interestGen.getRandNum();

        // only interest affects priority here                                                                                                                                                              
        Post p(id, 0, 1, 1, interest);

        // return false if insertion fails                                                                                                                                                                  
        if (!queue.insertPost(p)) {
            return false;
        }
    }

    // check if queue size is exactly 300                                                                                                                                                                   
    if (queue.m_size != 300) {
        return false;
    }

    // check max-heap property                                                                                                                                                                              
    return isValidMaxHeapHelper(queue.m_heap, priorityFn2);
}

/*  name: removeMinHeap()                                                                                                                                                                                   
    Desc: Test removal for a normal case of min-heap.                                                                                                                                                       
          After a decent number of insertion (e.g. 300 nodes)                                                                                                                                               
          we check whether all removals happen in the correct order.                                                                                                                                        
*/
bool Tester::removeMinHeapNormal() {
    // create min heap squeue object with skew structure and priorityFn1                                                                                                                                    
    SQueue queue(priorityFn1, MINHEAP, SKEW);

    // generate random id and likes num                                                                                                                                                                     
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);

    // create vector to hold the like values                                                                                                                                                                
    vector<int> likeValues;

    // insert 300 posts with random data into heap                                                                                                                                                          
    for (int i = 0; i < 300; i++) {
        int id = idGen.getRandNum();
        int likes = likesGen.getRandNum();

        // store like values into vector                                                                                                                                                                    
        likeValues.push_back(likes);
        Post p(id, likes, 1, 1, 5);
        queue.insertPost(p);
    }

    // sort expected like values for ascending order (min-heap)                                                                                                                                             
    sort(likeValues.begin(), likeValues.end());

    // remove elements from queue                                                                                                                                                                           
    for (int i = 0; i < int(likeValues.size()); i++) {
        Post top = queue.getNextPost();
        // check if within sorted value                                                                                                                                                                     
        if (top.getNumLikes() != likeValues[i]) {
            // return false if not                                                                                                                                                                          
            return false;
        }
    }

    // all posts were removed in successful order                                                                                                                                                           
    return true;
}
/*  Name: removeMaxHeapNormal()                                                                                                                                                                             
    Desc: Test removal for a normal case of max-heap.                                                                                                                                                       
          After a decent number of insertion (e.g. 300 nodes)                                                                                                                                               
          we check whether all removals happen in the correct order.                                                                                                                                        
*/
bool Tester::removeMaxHeapNormal() {
    // create max heap squeue object with skew structure and priorityFn2                                                                                                                                    
    SQueue queue(priorityFn2, MAXHEAP, SKEW);

    // generate random id and interest number                                                                                                                                                               
    Random idGen(MINPOSTID, MAXPOSTID);
    Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);

    // create vector to hold interest values                                                                                                                                                                
    vector<int> interestValues;

    // insert 300 posts with random data into heap                                                                                                                                                          
    for (int i = 0; i < 300; i++) {
        int id = idGen.getRandNum();
        int interest = interestGen.getRandNum();

        interestValues.push_back(interest);

        // interest level determines priority                                                                                                                                                               
        Post p(id, 0, 1, 1, interest);
        queue.insertPost(p);
    }

    // sort expected interest levels in descending order (max-heap)                                                                                                                                         
    sort(interestValues.begin(), interestValues.end(), greater<int>());

    // remove posts from queue                                                                                                                                                                              
    for (int i = 0; i < int(interestValues.size()); i++) {
        Post top = queue.getNextPost();
        // verify if matches expected order                                                                                                                                                                 
        if (top.getInterestLevel() != interestValues[i]) {
            return false;
        }
    }

    // return true if removals were successful                                                                                                                                                              
    return true;
}

/*  Name: leftistHeapNPLValue()                                                                                                                                                                             
    Desc: Test all nodes in a leftist heap have the correct NPL value.                                                                                                                                      
*/
bool Tester::leftistHeapNPLValue() {
    // create a squeue min heap object with leftist structure and priorityFn1                                                                                                                               
    SQueue queue(priorityFn1, MINHEAP, LEFTIST);

    // get random id and likes number                                                                                                                                                                       
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);

    // insert 200 posts with random data into heap                                                                                                                                                          
    for (int i = 0; i < 200; i++) {
        int id = idGen.getRandNum();
        int likes = likesGen.getRandNum();
        Post p(id, likes, 1, 1, 5);
        queue.insertPost(p);
    }

    // use helper function to validate NPL of all nodes                                                                                                                                                     
    return validateNPLHelper(queue.m_heap);
}

/*  Name: leftistHeapProperty()                                                                                                                                                                             
    Desc: Test a leftist heap preserves the property of such a heap,                                                                                                                                        
          i.e. at every node the NPL value of the left child is greater than                                                                                                                                
          or equal to the NPL value of the right child.                                                                                                                                                     
*/
bool Tester::leftistHeapProperty() {
    // create min-heap squeue object using priorityFn1 and leftist structure                                                                                                                                
    SQueue queue(priorityFn1, MINHEAP, LEFTIST);

    // generate random id and likes number                                                                                                                                                                  
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);

    // insert 200 posts with random data into heap                                                                                                                                                          
    for (int i = 0; i < 200; i++) {
        int id = idGen.getRandNum();
        int likes = likesGen.getRandNum();
        Post p(id, likes, 1, 1, 5);
        queue.insertPost(p);
    }

    // validate leftist property of all nodes                                                                                                                                                               
    return validateLeftistPropertyHelper(queue.m_heap);
}

/*  Name: validateLeftistPropertyHelper(Post* node)                                                                                                                                                         
    Desc: helper function to validate leftist structure property                                                                                                                                            
*/
bool Tester::validateLeftistPropertyHelper(Post* node) {
    // base case: empty node is valid                                                                                                                                                                       
    if (node == nullptr) {
        return true;
    }

    // initialize NPL values for left and right children                                                                                                                                                    
    int leftNPL = -1;
    int rightNPL = -1;

    // get NPL of left child if it exists                                                                                                                                                                   
    if (node->m_left != nullptr) {
        leftNPL = node->m_left->getNPL();
    }

    // get NPL of right child if it exists                                                                                                                                                                  
    if (node->m_right != nullptr) {
        rightNPL = node->m_right->getNPL();
    }

    // check current node's leftist property                                                                                                                                                                
    if (leftNPL < rightNPL) {
        return false;
    }

    // recursively validate left and right subtrees                                                                                                                                                         
    return validateLeftistPropertyHelper(node->m_left) && validateLeftistPropertyHelper(node->m_right);
}
/*  Name: validateNPLHelper(Post* node)                                                                                                                                                                     
    Desc: helper function to validate NPL                                                                                                                                                                   
*/
bool Tester::validateNPLHelper(Post* node) {
    // base case: empty node is valid                                                                                                                                                                       
    if (node == nullptr) {
        return true;
    }

    // get NPL value of left and right children                                                                                                                                                             
    int leftNPL = -1;
    int rightNPL = -1;

    // recursively validate left subtree                                                                                                                                                                    
    if (node->m_left != nullptr) {
        leftNPL = node->m_left->getNPL();
        if (!validateNPLHelper(node->m_left)) {
            return false;
        }
    }

    // recursively validate right subtree                                                                                                                                                                   
    if (node->m_right != nullptr) {
        rightNPL = node->m_right->getNPL();
        if (!validateNPLHelper(node->m_right)) {
            return false;
        }
    }

    // compute expected NPL based on leftist heap                                                                                                                                                           
    int minNPL;
    if (leftNPL < rightNPL) {
        minNPL = leftNPL;
    } else {
        minNPL = rightNPL;
    }
    int expectedNPL = minNPL + 1;

    // return if node's NPL is correct                                                                                                                                                                      
    return node->getNPL() == expectedNPL;
}

/*  Name: changePriorityFunction()                                                                                                                                                                          
    Desc: Test whether after changing the priority function a                                                                                                                                               
          correct heap is rebuilt with the same data (nodes) and the different priority function.                                                                                                           
*/
bool Tester::changePriorityFunction() {
    // create min heap squeue with (priorityFn1 based on likes) and skew structure                                                                                                                          
    SQueue queue(priorityFn1, MINHEAP, SKEW);

    // generate random id, likes, and interest numbers                                                                                                                                                      
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);
    Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);

    // create vector to store interest                                                                                                                                                                      
    vector<int> interestLevels;

    // insert 100 posts with random data into heap                                                                                                                                                          
    for (int i = 0; i < 100; i++) {
        int id = idGen.getRandNum();
        int likes = likesGen.getRandNum();
        int interest = interestGen.getRandNum();

        interestLevels.push_back(interest);

        // likes affect original priorityFn1, interest used later                                                                                                                                           
        Post p(id, likes, 1, 1, interest);
        queue.insertPost(p);
    }

    // change to maxheap with priorityFn2 (interestLevel)                                                                                                                                                   
    queue.setPriorityFn(priorityFn2, MAXHEAP);

    // sort expected interests descending (max-heap)                                                                                                                                                        
    sort(interestLevels.begin(), interestLevels.end(), greater<int>());

    // remove posts and validate they match new (MAXHEAP) priority order                                                                                                                                    
    for (int i = 0; i < int(interestLevels.size()); i++) {
        Post top = queue.getNextPost();

        // checks if heap was rebuilt correctly based on new priority                                                                                                                                       
        if (top.getInterestLevel() != interestLevels[i]) {
            return false;
        }
    }

    // all elements came out in correct new priority order                                                                                                                                                  
    return true;
}

/*  Name: mergeQueue()                                                                                                                                                                                      
    Desc: Test merge of an empty queue (an edge case) with a normal queue.                                                                                                                                  
          This is a call to the function SQueue::mergeWithQueue(SQueue& rhs)                                                                                                                                
          where rhs is a normally populated queue.                                                                                                                                                          
*/
bool Tester::mergeQueue() {
    // create 2 min heap squeue objects with same priority (priorityFn1) and structure (skew)                                                                                                               
    SQueue nonEmpty(priorityFn1, MINHEAP, SKEW);
    SQueue empty(priorityFn1, MINHEAP, SKEW);

    // generate random id and likes number                                                                                                                                                                  
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);

    // insert 100 posts with random data into heap                                                                                                                                                          
    for (int i = 0; i < 100; i++) {
        int id = idGen.getRandNum();
        int likes = likesGen.getRandNum();
        Post p(id, likes, 1, 1, 5);
        nonEmpty.insertPost(p);
    }

    // save original size for verification                                                                                                                                                                  
    int originalSize = nonEmpty.m_size;

    // merge queues                                                                                                                                                                                         
    nonEmpty.mergeWithQueue(empty);

    // check that empty is now cleared                                                                                                                                                                      
    if (empty.m_size != 0 || empty.m_heap != nullptr) {
        return false;
    }

    // check that nonEmpty remains unchanged in size                                                                                                                                                        
    if (nonEmpty.m_size != originalSize) {
        return false;
    }

    // all checks passed                                                                                                                                                                                    
    return true;
}


/*  Name: squeueCopyConstructorNormal()                                                                                                                                                                     
    Desc: Test squeue Copy constructor for normal case                                                                                                                                                      
*/
bool Tester::squeueCopyConstructorNormal() {
    // create min heap squeue with skew structure and priorityFn1                                                                                                                                           
    SQueue original(priorityFn1, MINHEAP, SKEW);

    // insert a few test posts into queue                                                                                                                                                                   
    original.insertPost(Post(100101, 50, 2, 10, 5));
    original.insertPost(Post(100102, 30, 1, 20, 6));
    original.insertPost(Post(100103, 70, 3, 30, 7));

    // use the copy constructor                                                                                                                                                                             
    SQueue copy(original);

    // verift if not same memory                                                                                                                                                                            
    if (original.m_heap == copy.m_heap) {
        return false;
    }

    // verify if same size                                                                                                                                                                                  
    if (original.m_size != copy.m_size) {
        return false;
    }

    // verify if same root priority                                                                                                                                                                         
    if (priorityFn1(*original.m_heap) != priorityFn1(*copy.m_heap)) {
        return false;
    }

    // all checks passed                                                                                                                                                                                    
    return true;
}


/*  Name: squeueCopyConstructorEdge()                                                                                                                                                                       
    Desc: Test squeue copy constructor for edge case                                                                                                                                                        
*/
bool Tester::squeueCopyConstructorEdge() {
    // create an empty min heap squeue object with priorityFn1 and skew structure                                                                                                                           
    SQueue original(priorityFn1, MINHEAP, SKEW);

    // create a copy using copy constructor                                                                                                                                                                 
    SQueue copy(original);

    // validate copied queue is also empty                                                                                                                                                                  
    if (copy.m_size != 0 || copy.m_heap != nullptr) {
        return false;
    }

    // return true if conditions are passed                                                                                                                                                                 
    return true;
}

/*  Name: squeueAssignmentOpNormal()                                                                                                                                                                        
    Desc: Test squeue assignment operator for normal case                                                                                                                                                   
*/
bool Tester::squeueAssignmentOpNormal() {
    // create 2 min heap squeues with same configuration (prioritiyFn1 and skew structure)                                                                                                                  
    SQueue queueA(priorityFn1, MINHEAP, SKEW);
    SQueue queueB(priorityFn1, MINHEAP, SKEW);

    // fill queueB with test data                                                                                                                                                                           
    queueB.insertPost(Post(100101, 50, 2, 10, 5));
    queueB.insertPost(Post(100102, 30, 1, 20, 6));
    queueB.insertPost(Post(100103, 70, 3, 30, 7));

    // fill queueA with different data (to be overwritten)                                                                                                                                                  
    queueA.insertPost(Post(100201, 10, 2, 5, 8));

    // assignment                                                                                                                                                                                           
    queueA = queueB;

    // check size matches                                                                                                                                                                                   
    if (queueA.m_size != queueB.m_size) {
        return false;
    }

    // check root priorities match                                                                                                                                                                          
    if (priorityFn1(*queueA.m_heap) != priorityFn1(*queueB.m_heap)) {
        return false;
    }

    // ensure it’s a deep copy                                                                                                                                                                              
    if (queueA.m_heap == queueB.m_heap) {
        return false;
    }
    // return true if all checks passed                                                                                                                                                                     
    return true;
}

/*  Name: squeueAssignmentOpEdge()                                                                                                                                                                          
    Desc: Test squeue assignment operator for edge case                                                                                                                                                     
*/
bool Tester::squeueAssignmentOpEdge() {
    // create a non-empty min heap squeue object with priorityFn1 and skew structure                                                                                                                        
    SQueue nonEmpty(priorityFn1, MINHEAP, SKEW);

    // insert post into nonEmpty queue                                                                                                                                                                      
    Post p(100123, 100, 1, 1, 5);
    nonEmpty.insertPost(p);

    // create an empty queue                                                                                                                                                                                
    SQueue empty(priorityFn1, MINHEAP, SKEW);

    // assign from empty to nonEmpty                                                                                                                                                                        
    nonEmpty = empty;

    // now nonEmpty should be empty                                                                                                                                                                         
    if (nonEmpty.m_size != 0 || nonEmpty.m_heap != nullptr) {
        return false;
    }

    // return true if all checks passed                                                                                                                                                                     
    return true;
}


/*  Name: dequeueEmptyQueue()                                                                                                                                                                               
    Desc: Test that attempting to dequeue an empty queue throws an out_of_range exception.                                                                                                                  
*/
bool Tester::dequeueEmptyQueue() {
    // create an empty min heap squeue object with priorityFn1 and skew structure                                                                                                                           
    SQueue emptyQueue(priorityFn1, MINHEAP, SKEW);

    try {
        // should throw                                                                                                                                                                                     
        emptyQueue.getNextPost();
    } catch (const out_of_range&) {
        // passed                                                                                                                                                                                           
        return true;
    } catch (...) {
        // threw something else (fail)                                                                                                                                                                      
        return false;
    }

    // no exception thrown (shouldn't get to here)                                                                                                                                                          
    return false;
}

/*  Name: mergeQueueAndDifferentPriorityFunction()                                                                                                                                                          
    Desc: Test that attempting to merge queues with different priority functions                                                                                                                            
          throws a domain_error exception.                                                                                                                                                                  
*/
bool Tester::mergeQueueAndDifferentPriorityFunction() {
    // create 2 min heap squeue objects with different priority functions but same skew structure                                                                                                           
    SQueue queueA(priorityFn1, MINHEAP, SKEW);
    SQueue queueB(priorityFn2, MINHEAP, SKEW);

    try {
        // attempt to merge queues                                                                                                                                                                          
        queueA.mergeWithQueue(queueB);
    } catch (const domain_error&) {
        // correct exception thrown                                                                                                                                                                         
        return true;
    } catch (...) {
        // wrong exception thrown                                                                                                                                                                           
        return false;
    }

    // no exception thrown                                                                                                                                                                                  
    return false;
}