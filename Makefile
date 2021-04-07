CC=g++ --std=c++17 -g

all: big.o operators.o comp.o utils.o main.o factorization.o
	$(CC) -o big operators.o big.o comp.o utils.o main.o factorization.o

factorization.o: factorization.cpp
	$(CC) -c factorization.cpp

main.o: main.cpp
	$(CC) -c main.cpp

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
