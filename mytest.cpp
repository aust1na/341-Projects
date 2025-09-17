#include <iostream>
#include "centcom.h"
using namespace std;

class Tester {
public:
    bool testConstructorNormal();
    bool testConstructorError();
    bool testAddElevatorNormal();
    bool testAddElevatorError();
    bool testSetUpError();
    bool testInsertFloorError();
    bool testPushButtonNormal();
    bool testPushButtonError();
    bool testProcessNextRequestNormal();
    bool testProcessNextRequestError();
};

int main() {
    Tester test;

    if (test.testConstructorNormal()) {
        cout << "Constructor test passed" << endl;
    } else {
        cout << "Constructor test failed" << endl;
    }

    if (test.testConstructorError()) {
        cout << "Constructor error test passed" << endl;
    } else {
        cout << "Constructor error test failed" << endl;
    }

    if (test.testAddElevatorNormal()) {
        cout << "Add elevator test passed" << endl;
    } else {
        cout << "Add elevator test failed" << endl;
    }

    if (test.testAddElevatorError()) {
        cout << "Add elevator error test passed" << endl;
    } else {
        cout << "Add elevator error test failed" << endl;
    }

    if (test.testSetUpError()) {
        cout << "Set up error test passed" << endl;
    } else {
        cout << "Set up error test failed" << endl;
    }

    if (test.testInsertFloorError()) {
        cout << "Insert floor error test passed" << endl;
    } else {
        cout << "Insert floor error test failed" << endl;
    }

    if (test.testPushButtonNormal()) {
        cout << "Push button test passed" << endl;
    } else {
        cout << "Push button test failed" << endl;
    }

    if (test.testPushButtonError()) {
        cout << "Push button error test passed" << endl;
    } else {
        cout << "Push button error test failed" << endl;
    }

    if (test.testProcessNextRequestNormal()) {
        cout << "Process next request test passed" << endl;
    } else {
        cout << "Process next request test failed" << endl;
    }

    if (test.testProcessNextRequestError()) {
        cout << "Process next request error test passed" << endl;
    } else {
        cout << "Process next request error test failed" << endl;
    }

    return 0;
}

bool Tester::testConstructorNormal() {
    // create centcom object                                                                                                                                                                                
    CentCom centCom(3, 1001);
    // check if id and numElevator match                                                                                                                                                                    
    return centCom.getId() == 1001 && centCom.getNumElevators() == 3;
}

bool Tester::testConstructorError() {
    // create invalid object                                                                                                                                                                                
    CentCom centCom(-1, -1);
    // check if id and numElevator don't match                                                                                                                                                              
    return centCom.getId() != -1 && centCom.getNumElevators() == 0;
}

bool Tester::testAddElevatorNormal() {
    // create object                                                                                                                                                                                        
    CentCom centCom(2, 1001);
    // store addElevator() within result                                                                                                                                                                    
    bool result = centCom.addElevator(0, 0, 10);
    // return the result to test and also if the elevator is not nullptr                                                                                                                                    
    return result && centCom.getElevator(0) != nullptr;
}

bool Tester::testAddElevatorError() {
    // create object                                                                                                                                                                                        
    CentCom centCom(2, 1001);
    // add an elevator to object                                                                                                                                                                            
    centCom.addElevator(0, 0, 10);
    // store result of adding elevator                                                                                                                                                                      
    bool result = centCom.addElevator(0, 0, 20);
    // should return true                                                                                                                                                                                   
    return result && centCom.getElevator(0) != nullptr;
}

bool Tester::testSetUpError() {
    // create centcom object                                                                                                                                                                                
    CentCom centCom(2, 1001);
    // create elevator object                                                                                                                                                                               
    Elevator* elevator = new Elevator(0);
    // set up elevator with invalid parameters                                                                                                                                                              
    elevator->setUp(0, 10);
    // grab initial floor                                                                                                                                                                                   
    int initialFloor = elevator->m_currentFloor->m_floorNum;
    // set up again                                                                                                                                                                                         
    elevator->setUp(1, 5);
    // store result                                                                                                                                                                                         
    bool result = (elevator->m_currentFloor->m_floorNum == initialFloor);
    // Free memory                                                                                                                                                                                          
    delete elevator;
    // return result                                                                                                                                                                                        
    return result;
}

bool Tester::testInsertFloorError() {
    // initialize centncom object                                                                                                                                                                           
    CentCom centCom(2, 1001);
    // create elevator object                                                                                                                                                                               
    Elevator* elevator = new Elevator(0);
    // set up elevator                                                                                                                                                                                      
    elevator->setUp(0, 10);
    // insert invalid number of floors                                                                                                                                                                      
    bool result = elevator->insertFloor(1000);
    // Free memory                                                                                                                                                                                          
    delete elevator;
    return result == false;
}

bool Tester::testPushButtonNormal() {
    // intialize centcom obbject                                                                                                                                                                            
    CentCom centCom(2, 1001);
    // create elevator object                                                                                                                                                                               
    Elevator* elevator = new Elevator(0);
    // set up elevator                                                                                                                                                                                      
    elevator->setUp(0, 10);
    // push the button and store within result                                                                                                                                                              
    bool result = elevator->pushButton(5);
    // Free memory                                                                                                                                                                                          
    delete elevator;
    return result == true;
}

bool Tester::testPushButtonError() {
    // intiialize centcom object                                                                                                                                                                            
    CentCom centCom(2, 1001);
    // create elevator object                                                                                                                                                                               
    Elevator* elevator = new Elevator(0);
    // set up elevator                                                                                                                                                                                      
    elevator->setUp(0, 10);

    // have floor point to elevators head                                                                                                                                                                   
    Floor* floor = elevator->m_bottom;
    // iterate                                                                                                                                                                                              
    while (floor != nullptr && floor->m_floorNum != 5) {
        // move pointer                                                                                                                                                                                     
        floor = floor->m_next;
    }
    // check if floor is nullptr                                                                                                                                                                            
    if (floor != nullptr) {
        // if not it's secured                                                                                                                                                                              
        floor->m_secured = true;
    }

    // check secure and store in isSecure                                                                                                                                                                   
    bool isSecure = elevator->checkSecure(5);
    // store pushButton within result                                                                                                                                                                       
    bool result = elevator->pushButton(5);
    // Free memory                                                                                                                                                                                          
    delete elevator;
    return isSecure && result == false;
}

bool Tester::testProcessNextRequestNormal() {
    // initialize centcom object                                                                                                                                                                            
    CentCom centCom(2, 1001);
    // create new elevator object                                                                                                                                                                           
    Elevator* elevator = new Elevator(0);
    // set up elevator                                                                                                                                                                                      
    elevator->setUp(0, 10);
    // push button                                                                                                                                                                                          
    elevator->pushButton(5);
    // store processNextRequest within result                                                                                                                                                               
    bool result = elevator->processNextRequest();
    // get the current floors num and store in currentFloor                                                                                                                                                 
    int currentFloor = elevator->m_currentFloor->m_floorNum;
    // Free memory                                                                                                                                                                                          
    delete elevator;
    return result == true && currentFloor == 5;
}

bool Tester::testProcessNextRequestError() {
    // initialize centcom object                                                                                                                                                                            
    CentCom centCom(2, 1001);
    // create new elevator object                                                                                                                                                                           
    Elevator* elevator = new Elevator(0);
    // set up elevator                                                                                                                                                                                      
    elevator->setUp(0, 10);
    // enter invalid num elevator                                                                                                                                                                           
    elevator->enter(2100);
    // process and store within result                                                                                                                                                                      
    bool result = elevator->processNextRequest();
    // Free memory                                                                                                                                                                                          
    delete elevator;
    return result == false;
}