CC=g++

.PHONY: clean

all: main

main: main.cpp
	$(CC) -std=c++17 -o ./main.exe \
		./main.cpp \
		-lssl \
		-lcrypto \
		./hasher/hash-suite/MurmurHash3.cpp

clean:
	rm -f ./main.exe
