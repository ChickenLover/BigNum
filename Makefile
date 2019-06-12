CC=g++ --std=c++17

all: big.o operators.o comp.o
	$(CC) -o big operators.o big.o comp.o

comp.o: comp.cpp
	$(CC) -c comp.cpp

operators.o: operators.cpp
	$(CC) -c operators.cpp

big.o: big.cpp
	$(CC) -c big.cpp

clear:
	rm -f big big.o operators.o
