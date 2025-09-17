// CMSC 341 - Spring 25 - Project 4                                                                                                                                                                         
// Austin Phan (aphan3)                                                                                                                                                                                     
#include "dnadb.h"

/*  name: DnaDb(int size, hash_fn hash, prob_t probing)                                                                                                                                                     
    desc: Constructor for the DnaDb class. Initializes the hash table using the given size, hash function,                                                                                                  
          and probing strategy. Ensures the capacity is a prime within [MINPRIME, MAXPRIME].                                                                                                                
          Allocates memory and sets all member variables.                                                                                                                                                   
*/
DnaDb::DnaDb(int size, hash_fn hash, prob_t probing) {
    // set hash function and probing policy                                                                                                                                                                 
    m_hash = hash;
    m_currProbing = probing;
    m_newPolicy = probing;

    // initialize old table member variables                                                                                                                                                                
    m_oldTable = nullptr;
    m_oldCap = 0;
    m_oldSize = 0;
    m_oldNumDeleted = 0;
    m_transferIndex = 0;

    // determine capacity                                                                                                                                                                                   
    if (size < MINPRIME) {
        // set to MINPRIME if too small                                                                                                                                                                     
        m_currentCap = MINPRIME;
    } else if (size > MAXPRIME) {
        // set to MAXPRIME if too big                                                                                                                                                                       
        m_currentCap = MAXPRIME;
    } else if (!isPrime(size)) {
        // if not a prime find next prime greater in size                                                                                                                                                   
        m_currentCap = findNextPrime(size);
    } else {
        // else use user specified size                                                                                                                                                                     
        m_currentCap = size;
    }

    // initialize current size and deleted count                                                                                                                                                            
    m_currentSize = 0;
    m_currNumDeleted = 0;

    // allocate memory for table                                                                                                                                                                            
    m_currentTable = new DNA*[m_currentCap];

    // set all pointers in table to nullptr                                                                                                                                                                 
    for (int i = 0; i < m_currentCap; i++) {
        m_currentTable[i] = nullptr;
    }
}

/*  name: ~DnaDb                                                                                                                                                                                            
    desc: Destructor for the DnaDb class. Frees memory allocated for current and old tables.                                                                                                                
*/
DnaDb::~DnaDb() {
    // check if current table exists                                                                                                                                                                        
    if (m_currentTable != nullptr) {
        // delete each DNA pointer in table                                                                                                                                                                 
        for (int i = 0; i < m_currentCap; i++) {
            delete m_currentTable[i];
        }
        // delete array                                                                                                                                                                                     
        delete[] m_currentTable;
        // set to nullptr as precaution                                                                                                                                                                     
        m_currentTable = nullptr;
    }

    // check if old table exists                                                                                                                                                                            
    if (m_oldTable != nullptr) {
        // delete each pointer                                                                                                                                                                              
        for (int i = 0; i < m_oldCap; i++) {
            delete m_oldTable[i];
        }
        // delete array itself                                                                                                                                                                              
        delete[] m_oldTable;
        // set to nullptr as precaution                                                                                                                                                                     
        m_oldTable = nullptr;
    }
}

/*     name: changeProbPolicy(prob_t policy)                                                                                                                                                                
       desc: Sets the new collision resolution strategy to be applied on the next rehash.                                                                                                                   
*/
void DnaDb::changeProbPolicy(prob_t policy) {
    // set policy to policy being passed in                                                                                                                                                                 
    m_newPolicy = policy;
}

/*  name: transfer25Percent()                                                                                                                                                                               
    desc: helper function that transfers 25% of the old table’s live entries into the current table.                                                                                                        
          Deletes old table once transfer completes.                                                                                                                                                        
*/
void DnaDb::transfer25Percent() {
    // check if old table exists or capacity is invalid                                                                                                                                                     
    if ((m_oldTable == nullptr) || (m_oldCap <= 0)) {
        return;
    }

    // calculate number of transfers (25% of old capacity)                                                                                                                                                  
    int numToTransfer = floor(m_oldCap * 0.25);
    if (numToTransfer <= 0) {
        return;
    }

    // determine ending index                                                                                                                                                                               
    int endIndex = min(m_transferIndex + numToTransfer, m_oldCap);

    // iterate through transfer range and move live entries                                                                                                                                                 
    for (int i = m_transferIndex; i < endIndex; i++) {
        if ((m_oldTable[i] != nullptr) && (m_oldTable[i]->getUsed())) {
            // insert deep copy into new table                                                                                                                                                              
            insertToCurrentTable(new DNA(*m_oldTable[i]));
        }
    }

    // update transfer index                                                                                                                                                                                
    m_transferIndex = endIndex;

    // check if all elements have transferred                                                                                                                                                               
    if (m_transferIndex >= m_oldCap) {
        // clean table                                                                                                                                                                                      
        for (int i = 0; i < m_oldCap; i++) {
            delete m_oldTable[i];
        }
        // delete array                                                                                                                                                                                     
        delete[] m_oldTable;
        // set to nullptr as precaution                                                                                                                                                                     
        m_oldTable = nullptr;
        // reset capacity                                                                                                                                                                                   
        m_oldCap = 0;
    }
}

/*  name: insertToCurrentTable(DNA* newDna)                                                                                                                                                                 
    desc: helper function that inserts a given DNA pointer into the current table using current probing strategy.                                                                                           
          Returns true if successful, false if table is full or a duplicate.                                                                                                                                
*/
bool DnaDb::insertToCurrentTable(DNA* newDna) {
    // compute initial hash index                                                                                                                                                                           
    int hashIndex = m_hash(newDna->getSequence()) % m_currentCap;
    int attempt = 0;

    // try to probe to resolve collisions                                                                                                                                                                   
    while (attempt < m_currentCap) {
        int probeIndex = 0;

        // determine probe index on current probing strategy                                                                                                                                                
        if (m_currProbing == LINEAR)
            probeIndex = (hashIndex + attempt) % m_currentCap;
        else if (m_currProbing == QUADRATIC)
            probeIndex = (hashIndex + attempt * attempt) % m_currentCap;
        else if (m_currProbing == DOUBLEHASH) {
            // compute step size using second hash for double hashing                                                                                                                                       
            int stepSize = 11 - (m_hash(newDna->getSequence()) % 11);
            probeIndex = (hashIndex + attempt * stepSize) % m_currentCap;
        }

        // if bucket is empty or has deleted item, insert there                                                                                                                                             
        if ((m_currentTable[probeIndex] == nullptr) || (!m_currentTable[probeIndex]->getUsed())) {
            // if deleted node, free memory                                                                                                                                                                 
            if (m_currentTable[probeIndex] != nullptr)
                delete m_currentTable[probeIndex];

            // insert new DNA object and increment size                                                                                                                                                     
            m_currentTable[probeIndex] = newDna;
            m_currentSize++;
            return true;
        }

        // continue probing                                                                                                                                                                                 
        attempt++;
    }

    // case in which insertion fails, delete to avoid memory leak                                                                                                                                           
    delete newDna;
    return false;
}

/*  name: insert(DNA dna)                                                                                                                                                                                   
    desc: Inserts a DNA object into the current hash table using the current probing policy.                                                                                                                
          Triggers rehashing if load factor exceeds 0.5. Prevents duplicate and out-of-bound locId entries.                                                                                                 
          Returns true on success, false otherwise.                                                                                                                                                         
*/
bool DnaDb::insert(DNA dna) {
    // return false if out of bounds location ids                                                                                                                                                           
    if ((dna.getLocId() < MINLOCID) || (dna.getLocId() > MAXLOCID)) {
        return false;
    }

    // transfer 25% of old table incase rehash is currently present                                                                                                                                         
    transfer25Percent();

    // calculate initial hash index using hash function and table size                                                                                                                                      
    unsigned int hashVal = m_hash(dna.getSequence());
    int index = hashVal % m_currentCap;
    int origIndex = index;

    // track number of probes during collision resolution                                                                                                                                                   
    int probeAttempt = 0;
    // store value of first deleted slot found                                                                                                                                                              
    int firstDeleted = -1;


    while (probeAttempt < m_currentCap) {
        // case 1: empty slot found                                                                                                                                                                         
        if (m_currentTable[index] == nullptr) {
            // if deleted spot was found earlier, use that instead                                                                                                                                          
            if (firstDeleted != -1) {
                index = firstDeleted;
            }

            // insert new DNA object                                                                                                                                                                        
            m_currentTable[index] = new DNA(dna);
            m_currentSize++;

            // check if load factor > 0.5, if so trigger rehash                                                                                                                                             
            if (((float)m_currentSize / m_currentCap) > 0.5) {
                // save current table as old table                                                                                                                                                          
                m_oldTable = m_currentTable;
                m_oldCap = m_currentCap;
                m_oldSize = m_currentSize;
                m_oldNumDeleted = m_currNumDeleted;
                m_oldProbing = m_currProbing;
                m_transferIndex = 0;

                // calculate new capacity: 4 * live elements, then find next prime                                                                                                                          
                int newCap = findNextPrime(4 * (m_currentSize - m_currNumDeleted));

                // allocate and initialize new table                                                                                                                                                        
                m_currentTable = new DNA*[newCap];
                for (int i = 0; i < newCap; i++) {
                    m_currentTable[i] = nullptr;
                }

                // update data for current table                                                                                                                                                            
                m_currentCap = newCap;
                m_currentSize = 0;
                m_currNumDeleted = 0;
                m_currProbing = m_newPolicy;
            }
            return true;

        // case 2: deleted node found                                                                                                                                                                       
        } else if (!m_currentTable[index]->getUsed()) {
            // mark first deleted bucket found                                                                                                                                                              
            if (firstDeleted == -1) {
                firstDeleted = index;
            }

        // case 3: duplicate DNA object found                                                                                                                                                               
        } else if (*(m_currentTable[index]) == dna) {
            // found duplicate                                                                                                                                                                              
            return false;
        }

        // collision occured, increment probeAttempt                                                                                                                                                        
        probeAttempt++;
        // calculations for next index                                                                                                                                                                      
        if (m_currProbing == LINEAR) {
            index = (origIndex + probeAttempt) % m_currentCap;
        } else if (m_currProbing == QUADRATIC) {
            index = (origIndex + probeAttempt * probeAttempt) % m_currentCap;
        } else {
            // DOUBLEHASH                                                                                                                                                                                   
            index = (origIndex + probeAttempt * (11 - (hashVal % 11))) % m_currentCap;
        }
    }
    // returns false if table is full or attempt fails                                                                                                                                                      
    return false;
}
/*  name: remove                                                                                                                                                                                            
    desc: Removes a DNA object by marking it deleted in either the current or old table.                                                                                                                    
          Triggers rehashing if delete ratio exceeds 0.8. Returns true if deleted, false otherwise.                                                                                                         
*/
bool DnaDb::remove(DNA dna) {
    // continue rehashing even if in progrees                                                                                                                                                               
    transfer25Percent();

    // check both current tables                                                                                                                                                                            
    for (int i = 0; i < 2; i++) {
        DNA** table;
        int cap;
        prob_t policy;

        // select appropriate table, capacity, and strategy                                                                                                                                                 
        if (i == 0) {
            table = m_currentTable;
            cap = m_currentCap;
            policy = m_currProbing;
        } else {
            table = m_oldTable;
            cap = m_oldCap;
            policy = m_oldProbing;
        }

        // skip if table is null                                                                                                                                                                            
        if (table == nullptr) {
            continue;
        }

        // compute initial hash index                                                                                                                                                                       
        unsigned int hashVal = m_hash(dna.getSequence());
        int index = hashVal % cap;
        int probeAttempt = 0;
        int originalIndex = index;

        // probe until we find item or hit nullptr                                                                                                                                                          
        while ((i < cap) && (table[index] != nullptr)) {

            // check if match is found and not marked deleted                                                                                                                                               
            if ((table[index]->getUsed()) && (*(table[index]) == dna)) {
                // mark as deleted                                                                                                                                                                          
                table[index]->setUsed(false);

                // if deleted from current table, update eleted count                                                                                                                                       
                if (i == 0) {
                    m_currNumDeleted++;

                    // if delete ratio exceeds threshold, rehash                                                                                                                                            
                    if ((float)m_currNumDeleted / m_currentSize > 0.8) {
                        // save current table to old table                                                                                                                                                  
                        m_oldTable = m_currentTable;
                        m_oldCap = m_currentCap;
                        m_oldSize = m_currentSize;
                        m_oldNumDeleted = m_currNumDeleted;
                        m_oldProbing = m_currProbing;
                        m_transferIndex = 0;

                        // allocate new table with size = next prime after 4 * live elements                                                                                                                
                        int newCap = findNextPrime(4 * (m_currentSize - m_currNumDeleted));
                        m_currentTable = new DNA*[newCap];
                        for (int i = 0; i < newCap; i++) m_currentTable[i] = nullptr;

                        // reset data                                                                                                                                                                       
                        m_currentCap = newCap;
                        m_currentSize = 0;
                        m_currNumDeleted = 0;
                        m_currProbing = m_newPolicy;
                    }
                } else {
                    // if deleted from old table, update old deleted count                                                                                                                                  
                    m_oldNumDeleted++;
                }
                return true;
            }

            // continue probing based on strategy                                                                                                                                                           
            probeAttempt++;
            if (policy == LINEAR) {
                index = (originalIndex + probeAttempt) % cap;
            } else if (policy == QUADRATIC) {
                index = (originalIndex + probeAttempt * probeAttempt) % cap;
            } else {
                // DOUBLEHASH                                                                                                                                                                               
                index = (originalIndex + probeAttempt * (11 - (hashVal % 11))) % cap;
            }
        }
    }
    // DNA object not found in either table                                                                                                                                                                 
    return false;
}

/*  name: findInTable                                                                                                                                                                                       
    desc: Helper function that searches a specific table (current or old) using a given probing policy.                                                                                                     
          Returns the matching DNA object if found and marked as used, otherwise returns an empty DNA object.                                                                                               
*/
DNA DnaDb::findInTable(DNA** table, int cap, prob_t policy, const string& sequence, int location) const {
    // hash sequence                                                                                                                                                                                        
    unsigned int hashVal = m_hash(sequence);
    // compute initial index                                                                                                                                                                                
    int index = hashVal % cap;
    // probing variables                                                                                                                                                                                    
    int i = 0;
    int probeIndex;
    // construct target for comparison                                                                                                                                                                      
    DNA target(sequence, location, true);

    while (i < cap) {
        // determine probe index based on probing policy                                                                                                                                                    
        if (policy == LINEAR)
            probeIndex = (index + i) % cap;
        else if (policy == QUADRATIC)
            probeIndex = (index + i * i) % cap;
        else
            // DOUBLEHASH                                                                                                                                                                                   
            probeIndex = (index + i * (11 - (hashVal % 11))) % cap;

	// if bucket is empty, target not in table                                                                                                                                                          
        if (table[probeIndex] == nullptr)
            return DNA();

        // if match found and still marked as used, return it                                                                                                                                               
        if ((*(table[probeIndex]) == target) && (table[probeIndex]->getUsed()))
            return *table[probeIndex];

        // continue probing                                                                                                                                                                                 
        i++;
    }
    // no match found after probing                                                                                                                                                                         
    return DNA();
}

/*  name: getDNA                                                                                                                                                                                            
    desc: Searches for and returns a DNA object matching a given sequence and location ID.                                                                                                                  
          Looks in both the current and old tables using respective probing policies.                                                                                                                       
          Returns empty DNA object if not found.                                                                                                                                                            
*/
const DNA DnaDb::getDNA(string sequence, int location) const {
    // search in current table first                                                                                                                                                                        
    DNA result = findInTable(m_currentTable, m_currentCap, m_currProbing, sequence, location);
    if (result.getSequence() != "") {
        return result;
    }

    // if not found and old table exists, search there                                                                                                                                                      
    if (m_oldTable != nullptr) {
        return findInTable(m_oldTable, m_oldCap, m_oldProbing, sequence, location);
    }

    // return empty DNA object if not found                                                                                                                                                                 
    return DNA();
}

/*  name: updateLocId                                                                                                                                                                                       
    desc: Updates the location ID of a matching DNA object in the database.                                                                                                                                 
          Returns true on success, false if the object is not found.                                                                                                                                        
*/
bool DnaDb::updateLocId(DNA dna, int location) {
    // check current table first                                                                                                                                                                            
    if (m_currentTable != nullptr) {
        unsigned int hashVal = m_hash(dna.getSequence());
        // initial index from hash                                                                                                                                                                          
        int index = hashVal % m_currentCap;
        int probeIndex;
        int i = 0;

        // iterate till we find a hit or empty spot                                                                                                                                                         
        while (i < m_currentCap) {
            // compute probing index based on collision resolution policy                                                                                                                                   
            if (m_currProbing == LINEAR)
                probeIndex = (index + i) % m_currentCap;
            else if (m_currProbing == QUADRATIC)
                probeIndex = (index + i * i) % m_currentCap;
            else  // DOUBLEHASH                                                                                                                                                                             
                probeIndex = (index + i * (11 - (hashVal % 11))) % m_currentCap;

            // if empty slot reached, DNA is not in current table                                                                                                                                           
            if (m_currentTable[probeIndex] == nullptr) {
                // exit loop                                                                                                                                                                                
                i = m_currentCap;
            }

            // if match found and not deleted, update locID                                                                                                                                                 
            if ((*(m_currentTable[probeIndex]) == dna) && (m_currentTable[probeIndex]->getUsed())) {
                m_currentTable[probeIndex]->setLocID(location);
                return true;
            }

            i++;
        }
    }

    // If not found in current table, check old table                                                                                                                                                       
    if (m_oldTable != nullptr) {
        unsigned int hashVal = m_hash(dna.getSequence());
        // initial index from hash                                                                                                                                                                          
        int index = hashVal % m_oldCap;
        int probeIndex;
        int j = 0;

        while (j < m_oldCap) {
            // compute probing index based on old probing policy                                                                                                                                            
            if (m_oldProbing == LINEAR)
                probeIndex = (index + j) % m_oldCap;
            else if (m_oldProbing == QUADRATIC)
                probeIndex = (index + j * j) % m_oldCap;
            else
                // DOUBLEHASH                                                                                                                                                                               
                probeIndex = (index + j * (11 - (hashVal % 11))) % m_oldCap;

            // if empty slot reached, DNA is not in old table                                                                                                                                               
            if (m_oldTable[probeIndex] == nullptr)
            // exit loop                                                                                                                                                                                    
                j = m_oldCap;

            // if match found and not marked deleted, update locID                                                                                                                                          
            if ((*(m_oldTable[probeIndex]) == dna) && (m_oldTable[probeIndex]->getUsed())) {
                m_oldTable[probeIndex]->setLocID(location);
                return true;
            }

            j++;
        }
    }

    // not found in either table                                                                                                                                                                            
    return false;
}

/*  name: lambda                                                                                                                                                                                            
    desc: Computes and returns the current load factor (occupied / capacity).                                                                                                                               
*/
float DnaDb::lambda() const {
    // compute load factor and return it                                                                                                                                                                    
    return (float)m_currentSize / m_currentCap;
}

/*  name: deletedRatio                                                                                                                                                                                      
    desc: Computes and returns the ratio of deleted buckets to total occupied ones.                                                                                                                         
*/
float DnaDb::deletedRatio() const {
    // check for any entries into table                                                                                                                                                                     
    if (m_currentSize == 0) {
        return float(0.0);
    }

    // return ratio of deleted buckets to occupied ones                                                                                                                                                     
    return (float)m_currNumDeleted / m_currentSize;
}

/*  name: dump                                                                                                                                                                                              
    desc: This function is given and it prints the contents of the current and old hash tables (for debugging only).                                                                                        
*/
void DnaDb::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

/*  name: isPrime                                                                                                                                                                                           
    desc: This function is given and it returns true if the provided number is a prime, false otherwise.                                                                                                    
*/
bool DnaDb::isPrime(int number) {
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

/*  name: findNextPrime                                                                                                                                                                                     
    desc: This function is given and it returns the smallest prime number greater than the given input.                                                                                                     
          Caps output at MAXPRIME.                                                                                                                                                                          
*/
int DnaDb::findNextPrime(int current) {
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) {
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0)
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    return MAXPRIME;
}
