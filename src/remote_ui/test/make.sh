g++ -c producer.cpp -std=c++14 -o producer.o
g++ -c ../../LockedFileAccess.cpp -std=c++14 -o LockedFileAccess.o
g++ producer.o LockedFileAccess.o -o producer
