CC := g++ -Wno-pointer-arith
SRC := main.cpp list.cpp
DIR := C:\Users\USER\Documents\List

.PHONY: all clean

main: $(SRC)
	$(CC) $^ -o $(DIR)\$@




