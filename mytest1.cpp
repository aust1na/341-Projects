// UMBC - CMSC 341 - Spring 2025 - Proj4 test file                                                                                                                                                          
// Name: Austin Phan                                                                                                                                                                                        
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include "dnadb.h"
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

unsigned int hashCode(const string str) {
    // initialize hash value                                                                                                                                                                                
    unsigned int hashValue = 0;
    // magic number                                                                                                                                                                                         
    const unsigned int thirtyThree = 33;

    // terate through character in string                                                                                                                                                                   
    for (int i = 0; i < int(str.length()); i++)
        // multiply current hash by ASCII value of char                                                                                                                                                     
        hashValue = hashValue * thirtyThree + str[i];

    // return hash value                                                                                                                                                                                    
    return hashValue;
}

class Tester {
public:
    // Test cases                                                                                                                                                                                           
    bool constructorTest();
    bool changePolicyTest();
    bool insertionNormal();
    bool findOperationError();
    bool findOperationNonCollidingKeys();
    bool findOperationCollidingKeys();
    bool removeOperationNonCollidingKeys();
    bool removeOperationCollidingKeys();
    bool updateLocationTest();
    bool lambdaTest();
    bool rehashAfterInsert();
    bool rehashLoadFactor();
    bool rehashAfterRemoval();
    bool deletedRatioTest();
    bool rehashDeleteRatio();

    /******************************************                                                                                                                                                             
     * Private function declarations go here! *                                                                                                                                                             
    ******************************************/

};

int main() {
    // create tester object                                                                                                                                                                                 
    Tester test;

    // Test cases below                                                                                                                                                                                     

    if (test.constructorTest()) {
        cout << "Pass: constructor capacity bounds and prime logic\n";
    } else {
        cout << "Fail: constructor capacity bounds or prime logic\n";
    }

    if (test.changePolicyTest()) {
        cout << "Pass: changeProbPolicy updates m_newPolicy correctly\n";
    } else {
        cout << "Fail: changeProbPolicy did not update or incorrectly modified m_currProbing\n";
    }

    if (test.insertionNormal()) {
        cout << "Pass: inserted into hash table" << endl;
    } else {
        cout << "Fail: could not insert into hash table" << endl;
    }

    if (test.findOperationError()) {
        cout << "Pass: DNA objected doesn't exist in database" << endl;
    } else {
        cout << "Fail: DNA object exists in database" << endl;
    }

    if (test.findOperationNonCollidingKeys()) {
        cout << "Pass: got DNA with non-colliding keys" << endl;
    } else {
        cout << "Fail: could not get DNA with non-colliding keys" << endl;
    }

    if (test.findOperationCollidingKeys()) {
        cout << "Pass: got DNA with colliding keys w/o rehash" << endl;
    } else {
        cout << "Fail: did not get DNA with colliding keys w/o rehash" << endl;
    }

    if (test.removeOperationNonCollidingKeys()) {
        cout << "Pass: removed with non-colliding keys" << endl;
    } else {
        cout << "Fail: did not remove with non-colliding keys" << endl;
    }

    if (test. removeOperationCollidingKeys()) {
        cout << "Pass: removed with colliding keys w/o rehash" << endl;
    } else {
        cout << "Fail: did not remove with colliding keys w/o rehash" << endl;
    }
    if (test.updateLocationTest()) {
        cout << "Pass: updated location ID successfully\n";
    } else {
        cout << "Fail: failed to update location ID\n";
    }

    if (test.lambdaTest()) {
        cout << "Pass: lambda (load factor) computed correctly\n";
    } else {
        cout << "Fail: lambda (load factor) is incorrect\n";
    }

    if (test.rehashAfterInsert()) {
        cout << "Pass: rehash after insertions" << endl;
    } else {
        cout << "Fail: did not rehash after insertions" << endl;
    }

    if (test.rehashLoadFactor()) {
        cout << "Pass: rehash due to load factor" << endl;
    } else {
        cout << "Fail: did not rehash due to load factor" << endl;
    }

    if (test.rehashAfterRemoval()) {
        cout << "Pass: rehash after removal" << endl;
    } else {
        cout << "Fail: did not rehash after removal" << endl;
    }

    if (test.deletedRatioTest()) {
        cout << "Pass: delete ratio test" << endl;
    } else {
        cout << "Fail: delete ratio test failed" << endl;
    }

    if (test.rehashDeleteRatio()) {
        cout << "Pass: rehash with delete ratio" << endl;
    } else {
        cout << "Fail: rehash without delete ratio" << endl;
    }

    return 0;
}

/*  name: constructorTest                                                                                                                                                                                   
    desc: Tests whether the constructor properly clamps or adjusts capacity values                                                                                                                          
          based on MINPRIME, MAXPRIME, and primality conditions.                                                                                                                                            
*/
bool Tester::constructorTest() {
    // get values for MINPRIME, MAXPRIME, exact, and non prime                                                                                                                                              
    DnaDb dbLow(MINPRIME - 50, hashCode, LINEAR);
    DnaDb dbHigh(MAXPRIME + 50000, hashCode, DOUBLEHASH);
    DnaDb dbExact(103, hashCode, QUADRATIC);
    // 104 is not prime                                                                                                                                                                                     
    DnaDb dbNonPrime(104, hashCode, QUADRATIC);

    // Check bounds and adjustment logic                                                                                                                                                                    
    bool pass = true;
    bool passLow = dbLow.m_currentCap == MINPRIME;
    bool passHigh = dbHigh.m_currentCap == MAXPRIME;
    bool passExact = dbExact.m_currentCap == 103;
    bool passNonPrime = dbNonPrime.m_currentCap > 104 && dbNonPrime.isPrime(dbNonPrime.m_currentCap);

    pass = passLow && passHigh && passExact && passNonPrime;

    return pass;
}

/*  name: changePolicyTest                                                                                                                                                                                  
    desc: Verifies that changeProbPolicy correctly sets m_newPolicy while leaving                                                                                                                           
          m_currProbing unchanged.                                                                                                                                                                          
*/
bool Tester::changePolicyTest() {
    // create database with linear probing and minimum size                                                                                                                                                 
    DnaDb db(MINPRIME, hashCode, LINEAR);

    // change to DOUBLEHASH probing                                                                                                                                                                         
    db.changeProbPolicy(DOUBLEHASH);

    // Check that current probing policy remains unchanged                                                                                                                                                  
    bool unchanged = (db.m_currProbing == LINEAR);

    // Check that new policy is updated                                                                                                                                                                     
    bool updated = (db.m_newPolicy == DOUBLEHASH);

    // test passes if conditions check                                                                                                                                                                      
    return unchanged && updated;
}

/*  name: insertionNormal                                                                                                                                                                                   
    desc: Tests insertion of multiple unique DNA entries with no collisions.                                                                                                                                
          Also tests prevention of duplicate insertions.                                                                                                                                                    
*/
bool Tester::insertionNormal() {
    // create database with linear probing and minimum size                                                                                                                                                 
    DnaDb db(MINPRIME, hashCode, LINEAR);

    // create 3 different unique DNA entries                                                                                                                                                                
    DNA dna1("AAAAC", 100000, true);
    DNA dna2("AAAAG", 100001, true);
    DNA dna3("AAAAT", 100002, true);

    // attempt to insert all 3                                                                                                                                                                              
    bool insert1 = db.insert(dna1);
    bool insert2 = db.insert(dna2);
    bool insert3 = db.insert(dna3);
    // should fail                                                                                                                                                                                          
    bool duplicate = db.insert(dna1);

    // pass if all insertions pass except duplicate                                                                                                                                                         
    return insert1 && insert2 && insert3 && !duplicate;
}

/*  name: findOperationError                                                                                                                                                                                
    desc: Verifies that getDNA correctly fails to retrieve DNA objects that                                                                                                                                 
          were never inserted or have incorrect sequence/location.                                                                                                                                          
*/
bool Tester::findOperationError() {
    // create database with linear probing and minimum size                                                                                                                                                 
    DnaDb db(MINPRIME, hashCode, LINEAR);

    // insert one DNA object                                                                                                                                                                                
    DNA inserted("AACCG", 123456, true);
    db.insert(inserted);

    // try to find a different one with same sequence but wrong location                                                                                                                                    
    // wrong location                                                                                                                                                                                       
    DNA notInserted1 = db.getDNA("AACCG", 654321);
    // wrong sequence                                                                                                                                                                                       
    DNA notInserted2 = db.getDNA("GGGTT", 123456);
    // wrong location and sequence                                                                                                                                                                          
    DNA notInserted3 = db.getDNA("GGGTT", 654321);

    // all of these should return an empty DNA object (i.e., sequence is "")                                                                                                                                
    return ((notInserted1.getSequence() == "") && (notInserted2.getSequence() == "") && (notInserted3.getSequence() == ""));
}

/*  name: findOperationNonCollidingKeys                                                                                                                                                                     
    desc: Confirms that DNA objects with distinct, non-colliding keys                                                                                                                                       
          are inserted and retrieved correctly.                                                                                                                                                             
*/
bool Tester::findOperationNonCollidingKeys() {
    // create database with linear probing and minimum size                                                                                                                                                 
    DnaDb db(MINPRIME, hashCode, LINEAR);

    // define 2 DNA entries with different sequences and ids                                                                                                                                                
    DNA dna1("GGGGG", 100005, true);
    DNA dna2("TTTTT", 100006, true);

    // insert both entries into database                                                                                                                                                                    
    db.insert(dna1);
    db.insert(dna2);

    // attempt to find them using getDNA()                                                                                                                                                                  
    DNA found1 = db.getDNA("GGGGG", 100005);
    DNA found2 = db.getDNA("TTTTT", 100006);

    // return if found                                                                                                                                                                                      
    return (found1 == dna1 && found2 == dna2);
}

/*  name: findOperationCollidingKeys                                                                                                                                                                        
    desc: Tests insertion and retrieval of colliding keys using the same sequence                                                                                                                           
          with different locIds, ensuring probing resolves correctly.                                                                                                                                       
*/
bool Tester::findOperationCollidingKeys() {
    // create database with DOUBLEHASH probing and minimum size                                                                                                                                             
    DnaDb db(MINPRIME, hashCode, DOUBLEHASH);

    // define sequence for collision                                                                                                                                                                        
    string sequence = "CCCCC";

    // have both DNA entries have the same sequence                                                                                                                                                         
    DNA dna1(sequence, 200000, true);
    DNA dna2(sequence, 200001, true);

    // insert both entries into database                                                                                                                                                                    
    db.insert(dna1);
    db.insert(dna2);

    // try to find entries                                                                                                                                                                                  
    DNA found1 = db.getDNA(sequence, 200000);
    DNA found2 = db.getDNA(sequence, 200001);

    // true if found                                                                                                                                                                                        
    return (found1 == dna1 && found2 == dna2);
}
/*  name: removeOperationNonCollidingKeys                                                                                                                                                                   
    desc: Inserts and then removes non-colliding keys. Checks that deleted entries                                                                                                                          
          are no longer retrievable, and live ones remain.                                                                                                                                                  
*/
bool Tester::removeOperationNonCollidingKeys() {
    // create database with linear probing and minimum size                                                                                                                                                 
    DnaDb db(MINPRIME, hashCode, LINEAR);

    // create 3 different dna entries                                                                                                                                                                       
    DNA dna1("AAAAA", 100000, true);
    DNA dna2("AAAAC", 100001, true);
    DNA dna3("AAAAG", 100002, true);

    // insert all 3 entries into database                                                                                                                                                                   
    db.insert(dna1);
    db.insert(dna2);
    db.insert(dna3);

    // remove dna1 and dna2 from database                                                                                                                                                                   
    db.remove(dna1);
    db.remove(dna2);

    // retrieve all 3 entries                                                                                                                                                                               
    DNA found1 = db.getDNA("AAAAA", 100000);
    DNA found2 = db.getDNA("AAAAC", 100001);
    DNA found3 = db.getDNA("AAAAG", 100002);

    // retrieve1 and retrieve2 should be missing, found3 should still exist                                                                                                                                 
    return ((found1.getSequence() == "") && (found2.getSequence() == "") && (found3 == dna3));
}

/*  name: removeOperationCollidingKeys                                                                                                                                                                      
    desc: Inserts colliding keys and removes a subset of them. Confirms proper deletion                                                                                                                     
          while retaining the correct remaining entry.                                                                                                                                                      
*/
bool Tester::removeOperationCollidingKeys() {
    // create database with quadratic probing to resolve collisions and minimum size                                                                                                                        
    DnaDb db(MINPRIME, hashCode, QUADRATIC);
    string sequence = "AGTAC";

    // create 3 DNA entries with same sequence                                                                                                                                                              
    DNA dna1(sequence, 100000, true);
    DNA dna2(sequence, 100001, true);
    DNA dna3(sequence, 100002, true);

    // insert 3 entries into database                                                                                                                                                                       
    db.insert(dna1);
    db.insert(dna2);
    db.insert(dna3);

    // remove dna1 and dna3 from entry                                                                                                                                                                      
    db.remove(dna1);
    db.remove(dna3);

    // try to find the entries                                                                                                                                                                              
    // removed                                                                                                                                                                                              
    DNA found1 = db.getDNA(sequence, 100000);
    // should still be present                                                                                                                                                                              
    DNA found2 = db.getDNA(sequence, 100001);
    // removed                                                                                                                                                                                              
    DNA found3 = db.getDNA(sequence, 100002);

    // return if all conditions meet                                                                                                                                                                        
    return ((found1.getSequence() == "") && (found3.getSequence() == "") && (found2 == dna2));
}

/*  name: updateLocationTest                                                                                                                                                                                
    desc: Tests if an inserted DNA object’s location ID can be updated successfully                                                                                                                         
          and found at its new location.                                                                                                                                                                    
*/
bool Tester::updateLocationTest() {
    // create database with linear probing and minimum size                                                                                                                                                 
    DnaDb db(MINPRIME, hashCode, LINEAR);

    // create 1 dna entry                                                                                                                                                                                   
    DNA dna1("AACGT", 123456, true);
    db.insert(dna1);

    // try to update its location                                                                                                                                                                           
    bool update = db.updateLocId(dna1, 654321);

    // look up using the new location                                                                                                                                                                       
    DNA found = db.getDNA("AACGT", 654321);

    return (update) && (found.getSequence() == "AACGT") && (found.getLocId() == 654321);
}

/*  name: lambdaTest                                                                                                                                                                                        
    desc: Tests whether the lambda (load factor) is calculated correctly                                                                                                                                    
          after inserting entries.                                                                                                                                                                          
*/
bool Tester::lambdaTest() {
    // create database with linear probing and 101 capacity                                                                                                                                                 
    DnaDb db(101, hashCode, LINEAR);

    // insert 3 dna entries                                                                                                                                                                                 
    db.insert(DNA("AAAAA", 123456, true));
    db.insert(DNA("CCCCC", 234567, true));
    db.insert(DNA("GGGGG", 345678, true));

    // calculate load factor                                                                                                                                                                                
    float lambda = db.lambda();

    // expected load factor of 3 entries / 101 capacity                                                                                                                                                     
    float expected = float(3.0) / float(101.0);

    // check if lambda is close to value                                                                                                                                                                    
    return abs(lambda - expected) < 1e-6;
}

/*  name: rehashAfterInsert                                                                                                                                                                                 
    desc: Inserts enough entries to trigger a rehash due to load factor.                                                                                                                                    
          Verifies that lambda remains ≤ 0.5 afterwards.                                                                                                                                                    
*/
bool Tester::rehashAfterInsert() {
    // initialize database with linear probing and 101 capacity                                                                                                                                             
    DnaDb db(101, hashCode, LINEAR);
    // random generator for ids                                                                                                                                                                             
    Random rng(MINLOCID, MAXLOCID);

    // insert 60 DNA objects to exceed 0.5 load factor                                                                                                                                                      
    for (int i = 0; i < 60; i++) {
        // create unique dna entry sequences                                                                                                                                                                
        string sequence = "A" + to_string(i);
        db.insert(DNA(sequence, rng.getRandNum(), true));
    }

    // after rehash, lambda should be <= 0.5                                                                                                                                                                
    return db.lambda() <= float(0.5);
}

/*  name: rehashLoadFactor                                                                                                                                                                                  
    desc: Specifically targets rehashing due to load factor by monitoring lambda                                                                                                                            
          after many insertions.                                                                                                                                                                            
*/
bool Tester::rehashLoadFactor() {
    // initialize database with linear probing and 101 capacity                                                                                                                                             
    DnaDb db(101, hashCode, LINEAR);
    // random generator for ids                                                                                                                                                                             
    Random rng(MINLOCID, MAXLOCID);

    // insert 55 DNA objectes to exceed 50% load factor                                                                                                                                                     
    for (int i = 0; i < 55; ++i) {
        string sequence = "B" + to_string(i);
        db.insert(DNA(sequence, rng.getRandNum(), true));
    }

    // after rehash lambda factor should be <= 0.5                                                                                                                                                          
    return db.lambda() <= 0.5f;
}

/*  name: rehashAfterRemoval                                                                                                                                                                                
    desc: Inserts and removes a large number of entries.                                                                                                                                                    
          Confirms rehashing triggers if delete ratio exceeds threshold.                                                                                                                                    
*/
bool Tester::rehashAfterRemoval() {
    DnaDb db(101, hashCode, LINEAR);
    Random rng(MINLOCID, MAXLOCID);
    vector<DNA> entries;

    for (int i = 0; i < 50; ++i) {
        DNA d("C" + to_string(i), rng.getRandNum(), true);
        db.insert(d);
        entries.push_back(d);
    }
    for (int i = 0; i < 45; ++i) {
        db.remove(entries[i]);
    }
    return db.deletedRatio() <= 0.8f;
}

/*  name: deletedRatioTest                                                                                                                                                                                  
    desc: Inserts and deletes entries, then checks if deleted ratio                                                                                                                                         
          is calculated accurately.                                                                                                                                                                         
*/
bool Tester::deletedRatioTest() {
    // create database with linear probing and 101 capacity                                                                                                                                                 
    DnaDb db(101, hashCode, LINEAR);

    // insert 3 DNA entries into database                                                                                                                                                                   
    db.insert(DNA("AAAAA", 123456, true));
    db.insert(DNA("CCCCC", 234567, true));

    // remove one of the 2 entries added in                                                                                                                                                                 
    db.remove(DNA("AAAAA", 123456, true));

    // calculate deleted ratio                                                                                                                                                                              
    float ratio = db.deletedRatio();

    // expected ratio is 1 deleted / 2 total occupies = 0.5                                                                                                                                                 
    float expected = float(1.0) / float(2.0);

    // return if ratio is close to expected                                                                                                                                                                 
    return abs(ratio - expected) < 1e-6;
}

/*  name: rehashDeleteRatio                                                                                                                                                                                 
    desc: Inserts many entries and deletes most of them to trigger rehash                                                                                                                                   
          due to high delete ratio. Verifies that the rehash occurs.                                                                                                                                        
*/
bool Tester::rehashDeleteRatio() {
    // create database with lineaer probing and 101 capacity                                                                                                                                                
    DnaDb db(101, hashCode, LINEAR);
    // create random number for ids                                                                                                                                                                         
    Random rng(MINLOCID, MAXLOCID);
    // create vector for entries                                                                                                                                                                            
    vector<DNA> entries;

    // insert 50 unique DNA entries                                                                                                                                                                         
    for (int i = 0; i < 50; i++) {
        DNA d("D" + to_string(i), rng.getRandNum(), true);
        db.insert(d);
        // insert into vector                                                                                                                                                                               
        entries.push_back(d);
    }

    // remove 45 of them to push deleted ratio                                                                                                                                                              
    for (int i = 0; i < 45; i++) {
        db.remove(entries[i]);
    }

    // after rehash deleted ratio should be <= 0.8                                                                                                                                                          
    return db.deletedRatio() <= 0.8f;
}