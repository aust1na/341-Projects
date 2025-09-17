CXX = g++
CXXFLAGS = -Wall

driver: centcom.o driver.o
        $(CXX) $(CXXFLAGS) -o driver centcom.o driver.o

test: centcom.o mytest.o
        $(CXX) $(CXXFLAGS) -o test centcom.o mytest.o

centcom.o: centcom.h centcom.cpp
        $(CXX) $(CXXFLAGS) -c centcom.cpp

driver.o: driver.cpp
        $(CXX) $(CXXFLAGS) -c driver.cpp

mytest.o: mytest.cpp
        $(CXX) $(CXXFLAGS) -c mytest.cpp

run:
        ./driver

run_test:
        ./test

val:
        valgrind --leak-check=full ./driver

val1:
        valgrind --leak-check=full ./test

clean:
        rm *~
        rm *.o

submit:
        cp centcom.h centcom.cpp mytest.cpp ~/cs341proj/proj1-late1/