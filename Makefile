CC=g++ --std=c++17

all: big.o operators.o comp.o utils.o
	$(CC) -o big operators.o big.o comp.o utils.o

comp.o: comp.cpp
	$(CC) -c comp.cpp

operators.o: operators.cpp
	$(CC) -c operators.cpp

big.o: big.cpp 
	$(CC) -c big.cpp

utils.o: utils.cpp
	$(CC) -c utils.cpp

clear:
	rm -f big *.o
