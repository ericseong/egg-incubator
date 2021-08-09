g++ -c ../Env.cpp -o Env.o -std=c++11
g++ -c env_test.cpp -o env_test.o -std=c++11
g++ Env.o env_test.o -ljsoncpp -o env_test

