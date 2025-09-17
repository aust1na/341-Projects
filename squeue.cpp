// UMBC - CMSC 341 - Spring 2025 - Project 3                                                                                                                                                                
// Name: Austin Phan                                                                                                                                                                                        
#include "squeue.h"
using namespace std;

/*  Name SQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)                                                                                                                                      
    Desc: This is the constructor.                                                                                                                                                                          
          It must be provided with a pointer to the prioritization function,                                                                                                                                
          the type of heap, and the desired data structure.                                                                                                                                                 
*/
SQueue::SQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) {
    // initialize all member variables                                                                                                                                                                      
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = structure;
    m_heap = nullptr;
    m_size = 0;
}


/*  Name: ~SQueue()                                                                                                                                                                                         
    Desc: The destructor deallocates the memory and re-initializes the member variables.                                                                                                                    
*/
SQueue::~SQueue() {
    // call clear() function to destruct                                                                                                                                                                    
    clear();
}


/*  Name: clear()                                                                                                                                                                                           
    Desc: This function clears the queue.                                                                                                                                                                   
          It must delete all the nodes in the heap, leaving the heap empty.                                                                                                                                 
          Moreover, it re-initializes the member variables.                                                                                                                                                 
*/
void SQueue::clear() {
    // destroy the heap using helper function                                                                                                                                                               
    destroyHeapHelper(m_heap);

    // change all member variables to perspective defaults                                                                                                                                                  
    m_heap = nullptr;
    m_size = 0;
    m_priorFunc = nullptr;
}


/*  Name: destroyHeapHelper(Post* node)                                                                                                                                                                     
    Desc: Helper function to destroy the heap                                                                                                                                                               
*/
void SQueue::destroyHeapHelper(Post* node) {
    // continue if node isn't nullptr                                                                                                                                                                       
    if (node != nullptr) {
        // recursively destroy left and right subtree of node using helper function                                                                                                                         
        destroyHeapHelper(node->m_left);
        destroyHeapHelper(node->m_right);

        // delete current node after children are deleted                                                                                                                                                   
        delete node;
    }
}

/*  Name: SQueue(const SQueue& rhs)                                                                                                                                                                         
    Desc: The copy constructor must make a deep copy of the rhs object.                                                                                                                                     
          It must function correctly if rhs is empty.                                                                                                                                                       
          This function creates an exact same copy of rhs.                                                                                                                                                  
*/
SQueue::SQueue(const SQueue& rhs) {
    // copy of everything to rhs                                                                                                                                                                            
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_size = rhs.m_size;

    // recursively deep copy the heap using helper function                                                                                                                                                 
    m_heap = deepCopyHelper(rhs.m_heap);
}


/*  Name: deepCopyHelper(Post* node)                                                                                                                                                                        
    Desc: Helper function that creates a deep copy                                                                                                                                                          
*/
Post* SQueue::deepCopyHelper(Post* node) {
    // base case: node is nullptr, return nullptr (branch)                                                                                                                                                  
    if (node == nullptr) {
        return nullptr;
    }

    // create new post object with original nodes data                                                                                                                                                      
    Post* newNode = new Post(node->m_postID, node->m_likes, node->m_connectLevel, node->m_postTime, node->m_interestLevel);

    // copy npl                                                                                                                                                                                             
    newNode->m_npl = node->m_npl;

    // recursively deep copy left and right subtree using helper function                                                                                                                                   
    newNode->m_left = deepCopyHelper(node->m_left);
    newNode->m_right = deepCopyHelper(node->m_right);

    // return pointer to new node                                                                                                                                                                           
    return newNode;
}


/*  Name: operator=(constSQueue& rhs)                                                                                                                                                                       
    Desc: The assignment operator creates an exact same copy of rhs.                                                                                                                                        
          You should not call the copy constructor in the implementation of the assignment operator.                                                                                                        
*/
SQueue& SQueue::operator=(const SQueue& rhs) {
    // check for self-assignment                                                                                                                                                                            
    if (this == &rhs) {
        // no work needed if assigning to self                                                                                                                                                              
        return *this;
    }

    // clean up current objects heap to prevent memory leaks using helper function                                                                                                                          
    destroyHeapHelper(m_heap);

    // copy over data to rhs                                                                                                                                                                                
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_size = rhs.m_size;

    // deep copy the heap from rhs using helper function                                                                                                                                                    
    m_heap = deepCopyHelper(rhs.m_heap);

    // return current object                                                                                                                                                                                
    return *this;
}

/*  Name: mergeWithQueue(SQueue& rhs)                                                                                                                                                                       
    Desc: This function merges the host queue with the rhs; it leaves rhs empty;                                                                                                                            
          it transfers all nodes from rhs to the current heap.                                                                                                                                              
          Two heaps can only be merged if they have the same priority function and they are of the same data structure.                                                                                     
          If the user attempts to merge queues with different priority functions,                                                                                                                           
          or two different data structures a domain_error exception should be thrown.                                                                                                                       
          This function requires protection against self-merging.                                                                                                                                           
          Merging a heap with itself is not allowed.                                                                                                                                                        
*/
void SQueue::mergeWithQueue(SQueue& rhs) {
    // protect against self-merging                                                                                                                                                                         
    if (this == &rhs) {
        throw domain_error("mergeWithQueue(): cannot merge queue with itself");
    }

    // check compatibility between both heaps                                                                                                                                                               
    if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure) {
        throw domain_error("mergeWithQueue(): priority function or structure mismatch");
    }

    // preform merge based on structure (skew or leftist)                                                                                                                                                   
    if (m_structure == SKEW) {
	m_heap = mergeSkewHelper(m_heap, rhs.m_heap);
    } else {
        m_heap = mergeLeftistHelper(m_heap, rhs.m_heap);
    }

    // update size to reflect merged heap                                                                                                                                                                   
    m_size += rhs.m_size;

    // clear rhs queue since empty now                                                                                                                                                                      
    rhs.m_heap = nullptr;
    rhs.m_size = 0;
}

/*  Name: insertPost(constPost& post)                                                                                                                                                                       
    Desc: Inserts a Post object into the queue.                                                                                                                                                             
          It must maintain the min-heap or the max-heap property depending on the settings.                                                                                                                 
          Moreover, if the selected data structure is leftist heap,                                                                                                                                         
          it needs to maintain a correct value of Null Path Length (NPL) in the node.                                                                                                                       
          If the Post object does not provide a valid priority value the object is not inserted and the function returns false,                                                                             
          otherwise the function returns true.                                                                                                                                                              
*/
bool SQueue::insertPost(const Post& post) {
    // check for valid ID range                                                                                                                                                                             
    if ((post.getPostID() < MINPOSTID) || (post.getPostID() > MAXPOSTID)) {
        return false;
    }

    // try computing priority                                                                                                                                                                               
    try {
        // make sure post doesn't crash                                                                                                                                                                     
        m_priorFunc(post);
    } catch (...) {
        // reject post if priority function fails                                                                                                                                                           
        return false;
    }

    // deep copy the post into a new heap node                                                                                                                                                              
    Post* newNode = new Post(post);

    // merge new node with current heap, depending on structure type (skew or leftist) using helper function                                                                                                
    if (m_structure == SKEW) {
        m_heap = mergeSkewHelper(m_heap, newNode);
    } else {
        m_heap = mergeLeftistHelper(m_heap, newNode);
    }

    // increase size of heap to reflect insertion of post                                                                                                                                                   
    m_size++;

    // inserted so true                                                                                                                                                                                     
    return true;
}
/*  Name: mergeSkewHelper(Post* a, Post* b)                                                                                                                                                                 
    Desc: Helper function to merge skews                                                                                                                                                                    
*/
Post* SQueue::mergeSkewHelper(Post* skewA, Post* skewB) {
    // base case: if either heap is empty, return the other                                                                                                                                                 
    if (skewA == nullptr) {
        return skewB;
    }
    if (skewB == nullptr) {
        return skewA;
    }

    // get priority values                                                                                                                                                                                  
    int prioritySkewA = m_priorFunc(*skewA);
    int prioritySkewB = m_priorFunc(*skewB);

    // determine if skewA has higher priority based on heap type                                                                                                                                            
    bool skewAHigherPriority = false;
    // MINHEAP type (priorityFn1)                                                                                                                                                                           
    if (m_heapType == MINHEAP) {
        if (prioritySkewA < prioritySkewB) {
            skewAHigherPriority = true;
        }
    // MAXHEAP type (priorityFn2)                                                                                                                                                                           
    } else {
        if (prioritySkewA > prioritySkewB) {
            skewAHigherPriority = true;
        }
    }

    // ensure skewA has the higher priority, if not, swap pointers                                                                                                                                          
    if (!skewAHigherPriority) {
        Post* tempSkew = skewA;
        skewA = skewB;
        skewB = tempSkew;
    }
    // swap left and right children of skewA                                                                                                                                                                
    Post* tempChild = skewA->m_left;
    skewA->m_left = skewA->m_right;
    skewA->m_right = tempChild;

    // recursively merge skewB into skewA's left child using helper function                                                                                                                                
    skewA->m_left = mergeSkewHelper(skewA->m_left, skewB);

    // return the new root                                                                                                                                                                                  
    return skewA;
}


/*  Name: mergeLeftistHelper(Post* a, Post* b)                                                                                                                                                              
    Desc: Helper function to merge leftist                                                                                                                                                                  
*/
Post* SQueue::mergeLeftistHelper(Post* leftistA, Post* leftistB) {
    // base case: if either heap is empty, return the other                                                                                                                                                 
    if (leftistA == nullptr) {
        return leftistB;
    }
    if (leftistB == nullptr) {
        return leftistA;
    }

    // get the priority values of both nodes                                                                                                                                                                
    int priorityLeftistA = m_priorFunc(*leftistA);
    int priorityLeftistB = m_priorFunc(*leftistB);

    // determine which root has higher priority depending on heap type                                                                                                                                      
    bool aHasHigherPriority = false;
    // MINHEAP type (priorityFn1)                                                                                                                                                                           
    if (m_heapType == MINHEAP) {
        // smaller = higher priority                                                                                                                                                                        
        aHasHigherPriority = (priorityLeftistA < priorityLeftistB);
    // MAXHEAP type (priorityFn2)                                                                                                                                                                           
    } else {
        // larger = smaller priority                                                                                                                                                                        
        aHasHigherPriority = (priorityLeftistA > priorityLeftistB);
    }

    // make sure leftistA points to the node with higher priority                                                                                                                                           
    if (!aHasHigherPriority) {
        Post* leftistTemp = leftistA;
        leftistA = leftistB;
        leftistB = leftistTemp;
    }

    // recursively merge into right subtree                                                                                                                                                                 
    leftistA->m_right = mergeLeftistHelper(leftistA->m_right, leftistB);

    // get NPL values of the left and right children                                                                                                                                                        
    int leftNPL = -1;
    int rightNPL = -1;
    if (leftistA->m_left != nullptr) {
        leftNPL = leftistA->m_left->getNPL();
    }
    if (leftistA->m_right != nullptr) {
        rightNPL = leftistA->m_right->getNPL();
    }

    // swap children if left NPL is smaller than right NPL                                                                                                                                                  
    if (leftNPL < rightNPL) {
        Post* leftistTemp = leftistA->m_left;
        leftistA->m_left = leftistA->m_right;
        leftistA->m_right = leftistTemp;
    }

    // set new NPL for this node                                                                                                                                                                            
    if (leftistA->m_right == nullptr) {
        leftistA->m_npl = 0;
    } else {
        leftistA->m_npl = leftistA->m_right->getNPL() + 1;
    }

    // return root of merged heap                                                                                                                                                                           
    return leftistA;
}

/*  Name: numPosts() const                                                                                                                                                                                  
    Desc: Returns current number of posts in the queue                                                                                                                                                      
*/
int SQueue::numPosts() const {
    // return number of posts in the queue                                                                                                                                                                  
    return m_size;
}


/*  Name: getPriorityFn() const                                                                                                                                                                             
    Desc: This function returns the current priority function.                                                                                                                                              
*/
prifn_t SQueue::getPriorityFn() const {
    // return current priority function                                                                                                                                                                     
    return m_priorFunc;
}

/*  Name: getNextPost()                                                                                                                                                                                     
    Desc: This function extracts (removes the node) and returns the highest priority post from the queue.                                                                                                   
          It must maintain the min-heap or max-heap property.                                                                                                                                               
          The function throws an out_of_range exception if the queue is empty when the function is called.                                                                                                  
*/
Post SQueue::getNextPost() {
    // check if queue is empty                                                                                                                                                                              
    if (m_heap == nullptr) {
        throw out_of_range("getNextPost(): queue is empty");
    }

    // save current root's post data                                                                                                                                                                        
    Post topPost = *m_heap;

    // save pointers to left and right subtrees                                                                                                                                                             
    Post* left = m_heap->m_left;
    Post* right = m_heap->m_right;

    // delete old root                                                                                                                                                                                      
    delete m_heap;

    // change size to reflect removal                                                                                                                                                                       
    m_size--;

    // merge left and right subtrees depending on type of heap with helper function                                                                                                                         
    if (m_structure == SKEW) {
        m_heap = mergeSkewHelper(left, right);
    } else {
        m_heap = mergeLeftistHelper(left, right);
    }

    // return saved post data                                                                                                                                                                               
    return topPost;
}

/*  Name: setPriorityFn(prifn_t priFn, HEAPTYPE heapType)                                                                                                                                                   
    Desc: This function sets a new priority function and its corresponding heap type (min-heap or max-heap).                                                                                                
          It must rebuild the heap! To rebuild the heap this application does not re-allocate memory to the objects;                                                                                        
          it reuses the current memory.                                                                                                                                                                     
          Note: it is the responsibility of the user to pass compatible arguments priFn and heapType.                                                                                                       
          The class implementation does not need to check the compatibility.                                                                                                                                
*/
void SQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    // update priority function and heap type to new inputs                                                                                                                                                 
    m_priorFunc = priFn;
    m_heapType = heapType;

    // save current heap pointer                                                                                                                                                                            
    Post* oldRoot = m_heap;

    // reset current heap                                                                                                                                                                                   
    m_heap = nullptr;
    m_size = 0;

    // rebuild the heap recursively using helper function                                                                                                                                                   
    rebuildHeapHelper(oldRoot);
}

/*  Name: rebuildHeapHelper(Post* node)                                                                                                                                                                     
    Desc: Helper function to rebuild the heap                                                                                                                                                               
*/
void SQueue::rebuildHeapHelper(Post* node) {
    // base case: end of branch (nullptr)                                                                                                                                                                   
    if (node == nullptr) {
        // do nothing                                                                                                                                                                                       
        return;
    }

    // save pointers to left and right children                                                                                                                                                             
    Post* left = node->m_left;
    Post* right = node->m_right;

    // detatch current nodes from children                                                                                                                                                                  
    node->m_left = nullptr;
    node->m_right = nullptr;
    // reset NPL (will be recalculated)                                                                                                                                                                     
    node->m_npl = 0;

    // insert node into new heap using merge depending on structure type (skew or leftist)                                                                                                                  
    if (m_structure == SKEW) {
        m_heap = mergeSkewHelper(m_heap, node);
    } else {
        m_heap = mergeLeftistHelper(m_heap, node);
    }

    // increase size to account for each inserted node                                                                                                                                                      
    m_size++;

    // recursively rebuild left and right subtrees                                                                                                                                                          
    rebuildHeapHelper(left);
    rebuildHeapHelper(right);
}

/*  Name: setStructure(STRUCTURE structure)                                                                                                                                                                 
    Desc: This function sets the data structure, i.e. it is either SKEW or LEFTIST.                                                                                                                         
          It must rebuild a heap with the new structure using the nodes in the current data structure.                                                                                                      
          Note: rebuild means transferring nodes from the current data structure to the new data structure.                                                                                                 
          Rebuild operation does not re-create the nodes.                                                                                                                                                   
*/
void SQueue::setStructure(STRUCTURE structure) {
    // update structure type to input                                                                                                                                                                       
    m_structure = structure;

    // save current heap root                                                                                                                                                                               
    Post* oldRoot = m_heap;

    // reset heap                                                                                                                                                                                           
    m_heap = nullptr;
    m_size = 0;

    // rebuild heap with nodes from old heap into new structure                                                                                                                                             
    rebuildHeapHelper(oldRoot);
}


/*  Name: getStructure() const                                                                                                                                                                              
    Desc: This function returns the structure of the current heap, i.e. it is either SKEW or LEFTIST.                                                                                                       
*/
STRUCTURE SQueue::getStructure() const {
    // returns structure of current heap                                                                                                                                                                    
    return m_structure;
}


/*  Name: getHeapType() const                                                                                                                                                                               
    Desc: This function returns the heap type, i.e. it is either MINHEAP or MAXHEAP.                                                                                                                        
*/
HEAPTYPE SQueue::getHeapType() const {
    // returns type of heap                                                                                                                                                                                 
    return m_heapType;
}

/*  Name: printPostsQueue() const                                                                                                                                                                           
    Desc: It prints the contents of the queue using preorder traversal.                                                                                                                                     
          Although the first post printed should have the highest priority,                                                                                                                                 
          the remaining posts will not necessarily be in priority order.                                                                                                                                    
          Please refer to the sample output file (driver.txt) for the format of this function's output.                                                                                                     
*/
void SQueue::printPostsQueue() const {
    // check if heap is empty                                                                                                                                                                               
    if (m_heap == nullptr) {
        cout << "The queue is empty." << endl;
        return;
    }

    // print header                                                                                                                                                                                         
    cout << "Preorder traversal of the nodes in the queue with ";
    if (m_heapType == MINHEAP) {
        cout << "priorityFn1 (MINHEAP):" << endl;
    } else {
        cout << "priorityFn2 (MAXHEAP):" << endl;
    }

    // begin traversal                                                                                                                                                                                      
    cout << "Contents of the queue:" << endl;

    // call recursive helper function to do preorder traversal                                                                                                                                              
    printPostsQueueHelper(m_heap);
}


/*  Name: printPostQueueHelper(Post* node) const                                                                                                                                                            
    Desc: helper function to print post queue                                                                                                                                                               
*/
void SQueue::printPostsQueueHelper(Post* node) const {
    // base case: reached end of branch                                                                                                                                                                     
    if (node == nullptr) {
        // do nothing                                                                                                                                                                                       
        return;
    }

    // print current nodes priority and post information                                                                                                                                                    
    cout << "[" << m_priorFunc(*node) << "] Post#: " << node->m_postID << ", likes#: " << node->m_likes << ", connect level: " << node->m_connectLevel << endl;

    // recursively print both left and right subtree                                                                                                                                                        
    printPostsQueueHelper(node->m_left);
    printPostsQueueHelper(node->m_right);
}


/*  Name: dump() const                                                                                                                                                                                      
    Desc: This function prints out the nodes information in an in-order traversal.                                                                                                                          
          For every node, it prints the priority followed by the post's ID;                                                                                                                                 
          and in the case of a leftist heap the output is followed by the value of NPL for the node.                                                                                                        
          The tree viewer tool shows a graphical representation of the output of the dump function.                                                                                                         
          You can copy and paste the dump() output in the viewer.                                                                                                                                           
          This tool facilitates debugging.                                                                                                                                                                  
          Note: The implementation for this function is provided to you.                                                                                                                                    
*/
void SQueue::dump() const {
    if (m_size == 0) {
        cout << "Empty heap.\n" ;
    } else {
        dump(m_heap);
    }
    cout << endl;
}

/*  Name: dump(Post *pos) const                                                                                                                                                                             
    Desc: helper function for dump (provided)                                                                                                                                                               
*/
void SQueue::dump(Post *pos) const {
    if ( pos != nullptr ) {
        cout << "(";
        dump(pos->m_left);
        if (m_structure == SKEW) {
            cout << m_priorFunc(*pos) << ":" << pos->m_postID;
        } else {
            cout << m_priorFunc(*pos) << ":" << pos->m_postID << ":" << pos->m_npl;
        }
        dump(pos->m_right);
        cout << ")";
    }
}


/*  Name: operator<<(ostream& sout, const Post& post)                                                                                                                                                       
    Desc: overloaded insertion operator for post (provided)                                                                                                                                                 
*/
ostream& operator<<(ostream& sout, const Post& post) {
  sout  << "Post#: " << post.getPostID()
        << ", likes#: " << post.getNumLikes()
        << ", connect level: " << post.getConnectLevel();
  return sout;
}
