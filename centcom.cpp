// UMBC - CMSC 341 - Spring 2025 - Proj1                                                                                                                                                                    
// Name: Austin Phan                                                                                                                                                                                        

#include "centcom.h"

/*  Name: CentCom(int numElevators, int buildingID)                                                                                                                                                         
    Desc: The constructor creates the object. It initializes all member variables.                                                                                                                          
          It creates memory if required.                                                                                                                                                                    
*/
CentCom::CentCom(int numElevators,int buildingID){
    // check if ID is a non-negative number                                                                                                                                                                 
    if (buildingID < 0) {
        // if so, set to 0                                                                                                                                                                                  
        buildingID = 0;
    }

    // check if number of elevators is a non-negative number                                                                                                                                                
    if (numElevators < 0) {
        // set to 0 if so                                                                                                                                                                                   
        numElevators = 0;
    }
    // initializes all member variables                                                                                                                                                                     
    m_numElevators = numElevators;
    m_id = buildingID;

    // ensure m_elevatorsList is empty                                                                                                                                                                      
    if (m_numElevators > 0) {
        // dynamically alllocate memory for m_elevatorList array                                                                                                                                            
        m_elevatorsList = new Elevator*[numElevators];

        // iteratte through elevatorsList array                                                                                                                                                             
        for (int i = 0; i < numElevators; i++){
            // initialize or set all pointers to nullptr                                                                                                                                                    
            m_elevatorsList[i] = nullptr;
        }
    } else {
        // if not empty then just set to nullptr                                                                                                                                                            
        m_elevatorsList = nullptr;
    }
}

/*  Name: ~CentCom()                                                                                                                                                                                        
    Desc: The destructor removes all memory and re-initializes                                                                                                                                              
          all member variables to their default values.                                                                                                                                                     
*/
CentCom::~CentCom(){
    // Check if m_elevatorsList is not nullptr                                                                                                                                                              
    if (m_elevatorsList != nullptr) {
        // Iterate through elevatorsList array                                                                                                                                                              
        for (int i = 0; i < m_numElevators; i++) {
            // check if elevator at index i is nullptr                                                                                                                                                      
            if (m_elevatorsList[i] != nullptr) {
                // Delete each elevator object                                                                                                                                                              
                delete m_elevatorsList[i];
                m_elevatorsList[i] = nullptr;
            }
        }
        // Deallocate the entire array                                                                                                                                                                      
        delete[] m_elevatorsList;
        // set to default value                                                                                                                                                                             
        m_elevatorsList = nullptr;
    }

    // Re-initialize all member variables                                                                                                                                                                   
    m_id = 0;
    m_numElevators = 0;
}

int CentCom::getId() const {
    return m_id;
}


int CentCom::getNumElevators() const {
    return m_numElevators;
}

/*  Name: addElevator(int ID, int bottomFloor, int topFloor)                                                                                                                                                
    Desc: This function creates and stores an elevator in the current CentCom object.                                                                                                                       
*/
bool CentCom::addElevator(int ID, int bottomFloor, int topFloor){
    // Check if ID is valid                                                                                                                                                                                 
    if (ID < 0 || ID >= m_numElevators) {
        // Invalid ID                                                                                                                                                                                       
        return false;
    }

    // Check if the bottomFloor and topFloor are valid                                                                                                                                                      
    if (bottomFloor > topFloor) {
        // Invalid floor range                                                                                                                                                                              
        return false;
    }

    // check if elevator at ID exists                                                                                                                                                                       
    if (m_elevatorsList[ID] != nullptr) {
        // if so, delete it at that index                                                                                                                                                                   
        delete m_elevatorsList[ID];
        // set to default nullptr                                                                                                                                                                           
        m_elevatorsList[ID] = nullptr;
    }

    // Create a new elevator object at the given ID                                                                                                                                                         
    m_elevatorsList[ID] = new Elevator(ID);

    // Set up the elevator with the provided floor range                                                                                                                                                    
    m_elevatorsList[ID]->setUp(bottomFloor, topFloor);

    // Return true if the operation is successful                                                                                                                                                           
    return true;
}

/*  Name: getElevator(int ID)                                                                                                                                                                               
    Desc: This function returns the Elevator object specified by ID parameter,                                                                                                                              
          if the object does not exist the function returns nullptr.                                                                                                                                        
*/
Elevator* CentCom::getElevator(int ID){
    // check if ID is within valid range                                                                                                                                                                    
    if (ID >= 0 && ID < m_numElevators) {
        // return object at given ID                                                                                                                                                                        
        return m_elevatorsList[ID];
    }
    // else return nullptr                                                                                                                                                                                  
    return nullptr;
}

/*  Name: setSecure(int ID, int floorNum, bool yes_no)                                                                                                                                                      
    Desc: This function sets or re-sets the specific floor in the specific elevator as a secure floor.                                                                                                      
          If the function works correctly it returns true.                                                                                                                                                  
          For the error cases the function returns false.                                                                                                                                                   
*/
bool CentCom::setSecure(int ID, int floorNum, bool yes_no) {
    // check if ID is valid                                                                                                                                                                                 
    if (ID < 0 || ID >= m_numElevators) {
        // return invalid ID                                                                                                                                                                                
        return false;
    }

    // get the elevator for the specified ID                                                                                                                                                                
    Elevator *elevator = m_elevatorsList[ID];
    // check if elevator exists                                                                                                                                                                             
    if (elevator == nullptr) {
        // return invalid eleavtor object                                                                                                                                                                   
        return false;
    }

    // Check if the floorNum is valid for this elevator                                                                                                                                                     
    if (floorNum < elevator->m_bottom->m_floorNum || floorNum > elevator->m_top->m_floorNum) {
        // Invalid floor number                                                                                                                                                                             
        return false;
    }

    // iterate through floors to find specified floorNum                                                                                                                                                    
    Floor *currentFloor = elevator->m_bottom;
    // iterate through LL                                                                                                                                                                                   
    while (currentFloor != nullptr) {
        // check if pointer is pointing to floorNum                                                                                                                                                         
        if (currentFloor->m_floorNum == floorNum) {
            // set secured status                                                                                                                                                                           
            currentFloor->m_secured = yes_no;
            // successfully set                                                                                                                                                                             
            return true;
        }
        // move to next floor                                                                                                                                                                               
        currentFloor = currentFloor->m_next;
    }
    // if floor not found set to false                                                                                                                                                                      
    return false;
}

/*  Name: clearEmergency(int ID)                                                                                                                                                                            
    Desc: This function re-sets the emergency state of the specific elevator.                                                                                                                               
          If the function works correctly it returns true.                                                                                                                                                  
          For the error cases the function returns false.                                                                                                                                                   
*/
bool CentCom::clearEmergency(int ID) {
    // check if ID is within valid range                                                                                                                                                                    
    if (ID < 0 || ID >= m_numElevators) {
        // return invalid ID                                                                                                                                                                                
        return false;
    }

    // get elevator at specified ID                                                                                                                                                                         
    Elevator *elevator = getElevator(ID);
    // check if elevator exists                                                                                                                                                                             
    if (elevator == nullptr) {
        // no elevator found                                                                                                                                                                                
        return false;
    }

    // clear emergency state of elevator                                                                                                                                                                    
    elevator->m_emergency = false;

    // return true so that reset is successful                                                                                                                                                              
    return true;
}

/* Name: Elevator(intID)                                                                                                                                                                                    
   Desc: The constructor creates an empty object and initializes all                                                                                                                                        
         member variables to their respective default values.                                                                                                                                               
*/
Elevator::Elevator(int ID){
    // initialize all member variables to default                                                                                                                                                           
    m_id = ID;
    m_bottom = nullptr;
    m_top = nullptr;
    m_upRequests = nullptr;
    m_downRequests = nullptr;
    m_currentFloor = nullptr;
    m_moveState = IDLE;
    m_doorState = CLOSED;
    m_emergency = false;
    m_load = 0;
}

/*  Name: ~Elevator()                                                                                                                                                                                       
    Desc: The destructor deallocates all memory in the                                                                                                                                                      
          object and reinitializes all member variables.                                                                                                                                                    
*/
Elevator::~Elevator(){
    // call clear() for destructing                                                                                                                                                                         
    clear();
}

/*  Name: clear()                                                                                                                                                                                           
    Desc: This function deallocates all memory in the object                                                                                                                                                
          and reinitializes all member variables.                                                                                                                                                           
*/
void Elevator::clear(){
    // Deallocate memory for the floor linked list                                                                                                                                                          
    Floor* currentFloor = m_bottom;
    // iterate through LL                                                                                                                                                                                   
    while (currentFloor != nullptr) {
        // store pointer into nextFloor                                                                                                                                                                     
        Floor* nextFloor = currentFloor->m_next;
        // Delete the current floor                                                                                                                                                                         
        delete currentFloor;
        // Move to the next floor                                                                                                                                                                           
        currentFloor = nextFloor;
    }

    Floor* currentUpRequest = m_upRequests;
    // iterate through LL                                                                                                                                                                                   
    while (currentUpRequest != nullptr) {
        // get the up_requests pointer and store in nextUpRequest                                                                                                                                           
        Floor* nextUpRequest = currentUpRequest->m_next;
        // Delete the current up request                                                                                                                                                                    
        delete currentUpRequest;
        // Move to the next up request                                                                                                                                                                      
        currentUpRequest = nextUpRequest;
    }

    // store downRequests                                                                                                                                                                                   
    Floor* currentDownRequest = m_downRequests;
    // iterate through downRequests                                                                                                                                                                         
    while (currentDownRequest != nullptr) {
        // store the next pointer of downRequests                                                                                                                                                           
        Floor* nextDownRequest = currentDownRequest->m_next;
        // Delete the current down request                                                                                                                                                                  
        delete currentDownRequest;
        // Move to the next down request                                                                                                                                                                    
        currentDownRequest = nextDownRequest;
    }

    // Reinitialize all member variables to their default values                                                                                                                                            
    m_bottom = nullptr;
    m_top = nullptr;
    m_upRequests = nullptr;
    m_downRequests = nullptr;
    m_currentFloor = nullptr;
    m_moveState = IDLE;
    m_doorState = CLOSED;
    m_emergency = false;
    m_load = 0;
}

/*  Name: findFloor(int floor)                                                                                                                                                                              
    Desc: Private helper function to check                                                                                                                                                                  
          if a floor exists in the linked list                                                                                                                                                              
*/
bool Elevator::findFloor(int floor) {
    // start at head of LL                                                                                                                                                                                  
    Floor* currentFloor = m_bottom;
    // iterate through LL                                                                                                                                                                                   
    while (currentFloor != nullptr) {
        // if floor is found                                                                                                                                                                                
        if (currentFloor->m_floorNum == floor) {
            // return true                                                                                                                                                                                  
            return true;
        }
        // move pointer                                                                                                                                                                                     
        currentFloor = currentFloor->m_next;
    }
    // if not found return false                                                                                                                                                                            
    return false;
}

/*  Name: setUp(int firstFloor, int lastFloor)                                                                                                                                                              
    Desc: This function sets up the elevator to its required floors if the current object is empty,                                                                                                         
          otherwise it does not modify the object.                                                                                                                                                          
*/
void Elevator::setUp(int firstFloor, int lastFloor){
    // Check if the elevator is already set up (i.e., m_bottom is not nullptr)                                                                                                                              
    if (m_bottom != nullptr) {
        // Elevator is already set up, do nothing                                                                                                                                                           
        return;
    }

    // Check if the floor range is valid (firstFloor <= lastFloor)                                                                                                                                          
    if (firstFloor > lastFloor) {
        // Invalid floor range, do nothing                                                                                                                                                                  
        return;
    }

    // Create the first floor                                                                                                                                                                               
    m_bottom = new Floor(firstFloor);
    // Initially, the top and bottom are the same                                                                                                                                                           
    m_top = m_bottom;

    // Create the remaining floors and link them                                                                                                                                                            
    Floor* currentFloor = m_bottom;
    // iterate from firstFloor to lastFloor                                                                                                                                                                 
    for (int floorNum = firstFloor + 1; floorNum <= lastFloor; ++floorNum) {
        // Create a new floor                                                                                                                                                                               
        Floor* newFloor = new Floor(floorNum);
        // Link the new floor to the previous one                                                                                                                                                           
        newFloor->m_previous = currentFloor;
         // Link the previous floor to the new one                                                                                                                                                          
        currentFloor->m_next = newFloor;
        // Move to the new floor                                                                                                                                                                            
        currentFloor = newFloor;
    }

    // Update the top floor                                                                                                                                                                                 
    m_top = currentFloor;

    // Set the current floor to the bottom floor                                                                                                                                                            
    m_currentFloor = m_bottom;
}

/*  Name: insertFloor(int floor)                                                                                                                                                                            
    Desc: This function inserts floors at the head of the main linked list.                                                                                                                                 
          If the floor is inserted the function returns true otherwise it returns false.                                                                                                                    
*/
bool Elevator::insertFloor(int floor){
    // Check if the elevator is set up (m_bottom is not nullptr)                                                                                                                                            
    if (m_bottom == nullptr) {
        // Elevator is not set up, cannot insert floor                                                                                                                                                      
        return false;
    }

    // Check if the floor is within the valid range                                                                                                                                                         
    if (floor < m_bottom->m_floorNum || floor > m_top->m_floorNum) {
        // Floor is outside the valid range                                                                                                                                                                 
        return false;
    }

    // Check if the floor already exists in the linked list                                                                                                                                                 
    if (findFloor(floor)) {
        // Floor already exists, cannot insert                                                                                                                                                              
        return false;
    }

    // Check if the floor number is valid and not a duplicate                                                                                                                                               
    Floor* currentFloor = m_bottom;
    while (currentFloor != nullptr) {
        if (currentFloor->m_floorNum == floor) {
            // Floor number already exists, can't insert                                                                                                                                                    
            return false;
        }
        currentFloor = currentFloor->m_next;
    }

    // Create a new floor                                                                                                                                                                                   
    Floor* newFloor = new Floor(floor);

    // New floor points to the current bottom                                                                                                                                                               
    newFloor->m_next = m_bottom;
    // Current bottom points back to the new floor                                                                                                                                                          
    m_bottom->m_previous = newFloor;
    // Update the bottom to the new floor                                                                                                                                                                   
    m_bottom = newFloor;

    // Floor successfully inserted                                                                                                                                                                          
    return true;
}

/*  Name: checkSecure(int floor)                                                                                                                                                                            
    Desc: This function returns the truth value of secure state for a floor.                                                                                                                                
          For the error cases the function returns false.                                                                                                                                                   
*/
bool Elevator::checkSecure(int floor){
    // Start from the first floor in the elevator                                                                                                                                                           
    Floor* current = m_bottom;

    // Traverse the list to find the floor                                                                                                                                                                  
    while (current != nullptr) {
        // check if the current floors number is = to floor param                                                                                                                                           
        if (current->m_floorNum == floor) {
            // Return the secure state of the found floor                                                                                                                                                   
            return current->m_secured;
        }
        // Move to the next floor in the list                                                                                                                                                               
        current = current->m_next;
    }

    // If the floor is not found in the list, return false                                                                                                                                                  
    return false;
}

/*  Name: pushEmergency)bool pushed)                                                                                                                                                                        
    Desc: This function sets the emergency state in the elevator.                                                                                                                                           
*/
void Elevator::pushEmergency(bool pushed){
    // if param checks off                                                                                                                                                                                  
    if (pushed) {
        // Set the emergency state if the button is pressed                                                                                                                                                 
        m_emergency = true;
    }
    // If pushed is false do nothing                                                                                                                                                                        
}

/*  Name: pushButton(int floor)                                                                                                                                                                             
    Desc: This function registers (inserts) a Floor object in one of the requests lists,                                                                                                                    
          If the request is registered the function returns true,                                                                                                                                           
          otherwise it returns false.                                                                                                                                                                       
*/
bool Elevator::pushButton(int floor){
    // Return false if the floor is secure, the elevator is in emergency mode, or the requested floor is the current floor                                                                                  
    if (checkSecure(floor) || m_emergency || floor == m_currentFloor->m_floorNum) {
        return false;
    }

    // Create a new request for the specified floor                                                                                                                                                         
    Floor* newRequest = new Floor(floor);

    // Handle upward requests                                                                                                                                                                               
    if (floor > m_currentFloor->m_floorNum) {
        // set prev to default for now                                                                                                                                                                      
        Floor* prev = nullptr;
        // set curr to m_upReuqests                                                                                                                                                                         
        Floor* curr = m_upRequests;

        // Traverse the up requests list to find the correct position                                                                                                                                       
        while (curr != nullptr && curr->m_floorNum < floor) {
            // set prev to curr                                                                                                                                                                             
            prev = curr;
            // move pointer                                                                                                                                                                                 
            curr = curr->m_next;
        }

        // Insert the new request into the up requests list                                                                                                                                                 
        if (prev == nullptr) {
            // newRequest pointer is now pointing to m_upReuqests                                                                                                                                           
            newRequest->m_next = m_upRequests;
            // m_upRequest is now the newReuqest                                                                                                                                                            
            m_upRequests = newRequest;
        } else {{
            // prev pointer points to newReuqest                                                                                                                                                            
            prev->m_next = newRequest;
            // curr is now pointed by newRequest                                                                                                                                                            
            newRequest->m_next = curr;
        }

        // Update the movement state if the elevator is idle                                                                                                                                                
        if (m_moveState == IDLE) {
            // update to UP                                                                                                                                                                                 
            m_moveState = UP;
        }
    }

    // Handle downward requests                                                                                                                                                                             
    else if (floor < m_currentFloor->m_floorNum) {
        // set prev to default for now                                                                                                                                                                      
        Floor* prev = nullptr;
        // set curr to m_downReuqests                                                                                                                                                                       
        Floor* curr = m_downRequests;

        // Traverse the down requests list to find the correct position                                                                                                                                     
        while (curr != nullptr && curr->m_floorNum > floor) {
            // update prev to curr                                                                                                                                                                          
            prev = curr;
            // move to next pointer                                                                                                                                                                         
            curr = curr->m_next;
        }

// Insert the new request into the down requests list                                                                                                                                               
        if (prev == nullptr) {
            // downRequests pointed by newRequest                                                                                                                                                           
            newRequest->m_next = m_downRequests;
            // update the downRequest as the newReuqest                                                                                                                                                     
            m_downRequests = newRequest;
        } else {
            // newReuqest is now being pointed by prev                                                                                                                                                      
            prev->m_next = newRequest;
            // curr is now pointing by newRequest                                                                                                                                                           
            newRequest->m_next = curr;
        }

        // Update the movement state if the elevator is idle                                                                                                                                                
        if (m_moveState == IDLE) {
            // update to DOWN                                                                                                                                                                               
            m_moveState = DOWN;
        }
    }

    // return true if successful                                                                                                                                                                            
    return true;
}

/*  Name: processNextRequests()                                                                                                                                                                             
    Desc: This function processes the next registered request.                                                                                                                                              
          If the request is processed and completed,                                                                                                                                                        
          the function returns true, otherwise it returns false.                                                                                                                                            
*/
bool Elevator::processNextRequest(){
    // Check if the elevator door is open or overloaded                                                                                                                                                     
    if (m_doorState != CLOSED || m_load > LOADLIMIT) {
        return false;
    }

    // If no more requests, set to IDLE                                                                                                                                                                     
    if (m_upRequests == nullptr && m_downRequests == nullptr) {
        m_moveState = IDLE;
        return false;
    }

    // Determine current movement direction and requests                                                                                                                                                    
    Floor* nextRequest = nullptr;
    Floor* currPtr = nullptr;
    bool isUp = (m_moveState == UP && m_upRequests != nullptr);

    if (isUp) {
        // update nextRequest to upRequests                                                                                                                                                                 
        nextRequest = m_upRequests;
        // move the pointer                                                                                                                                                                                 
        m_upRequests = m_upRequests->m_next;
        // set the current floors pointer to the head                                                                                                                                                       
        currPtr = m_bottom;
    } else if (m_moveState == DOWN && m_downRequests != nullptr) {
        // if going down nextRequest is now downRequest                                                                                                                                                     
        nextRequest = m_downRequests;
        // move the pointer                                                                                                                                                                                 
        m_downRequests = m_downRequests->m_next;
        // set pointer to the tail                                                                                                                                                                          
        currPtr = m_top;
            } else {
        // if no requests, set to IDLE                                                                                                                                                                      
        m_moveState = IDLE;
        return false;
    }

    // Move the elevator to the target floor                                                                                                                                                                
    int target = nextRequest->m_floorNum;
    // iterate through the current floors pointer                                                                                                                                                           
    while (currPtr != nullptr && currPtr->m_floorNum != target) {
        if (isUp) {
            // Move to the next element if isUp is true                                                                                                                                                     
            currPtr = currPtr->m_next;
        } else {
            // Move to the previous element if isUp is false                                                                                                                                                
            currPtr = currPtr->m_previous;
        }
    }

    // Update current floor if found and clean up request                                                                                                                                                   
    if (currPtr != nullptr) {
        // set current floor to current floors pointer                                                                                                                                                      
        m_currentFloor = currPtr;
    }
    // clean up request                                                                                                                                                                                     
    delete nextRequest;

// Update the movement state based on remaining requests                                                                                                                                                
    if (isUp && m_upRequests == nullptr && m_downRequests != nullptr) {
        // if conditions check elevator is moving down                                                                                                                                                      
        m_moveState = DOWN;
    } else if (!isUp && m_downRequests == nullptr && m_upRequests != nullptr) {
        // if conditions check elevator is moving up                                                                                                                                                        
        m_moveState = UP;
    } else if (m_upRequests == nullptr && m_downRequests == nullptr) {
        // if no conditions check, stay idle                                                                                                                                                                
        m_moveState = IDLE;
    }

    // return true to prove operation is a success                                                                                                                                                          
    return true;
}

/*  Name: enter(int load)                                                                                                                                                                                   
    Desc: This function adds the new load specified by the                                                                                                                                                  
          parameter load to the current load of the elevator.                                                                                                                                               
*/
void Elevator::enter(int load){
    // check if load is greater than 0 and add new load                                                                                                                                                     
    if (load > 0) {
        m_load += load;
    }
}

/*  Name: exit(int load)                                                                                                                                                                                    
    Desc: This function reduces the elevator load by the amount of the load parameter.                                                                                                                      
          The elevator load cannot be negative.                                                                                                                                                             
          The function returns the current load of the elevator.                                                                                                                                            
          If the load is negative the function returns zero.                                                                                                                                                
*/
int Elevator::exit(int load) {
    if (load > 0) {
        // Reduce the load                                                                                                                                                                                  
        m_load -= load;
        if (m_load < 0) {
            // Ensure the load is not negative                                                                                                                                                              
            m_load = 0;
        }
    }
    // Return the updated load                                                                                                                                                                              
    return m_load;
}

/*  Name: dump()                                                                                                                                                                                            
    Desc: This function prints out the state of the elevator.                                                                                                                                               
*/
void Elevator::dump(){
    if (m_moveState == IDLE) cout << "Elevator " << m_id << " is idle.";
    else if (m_moveState == UP) cout << "Elevator " << m_id << " is moving up.";
    else if (m_moveState == DOWN) cout << "Elevator " << m_id << " is moving down.";
    cout << endl;

    if (m_emergency) cout << "Someone pushed the emergency button!" << endl;

    if (m_top != nullptr) {
        Floor* temp = m_top;
        cout << "Top" << endl;
        while (temp != nullptr) {
            cout << temp->m_floorNum;
            if (temp->m_floorNum == m_currentFloor->m_floorNum) cout << " current ";
            if (temp->m_secured) cout << " secured ";
            cout << endl;
            temp = temp->m_previous;
        }
        cout << "Bottom" << endl;
    }
}