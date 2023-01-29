HOME = .
BIN = $(HOME)/bin
INCLUDE = $(HOME)/include
SRC = $(HOME)/src
OBJ = $(HOME)/obj
LIB = $(HOME)/lib
DATA = $(HOME)/data

all: $(BIN)/QKP

$(BIN)/QKP: $(OBJ)/QKP.o $(OBJ)/AG.o $(OBJ)/main.o
	g++ $^ -o $@

$(OBJ)/main.o: $(SRC)/main.cpp
	g++ -c -O3 $^ -I$(INCLUDE) -o $@

$(OBJ)/QKP.o: $(SRC)/QKP.cpp
	g++ -c -O3 $^ -I$(INCLUDE) -o $@

$(OBJ)/AG.o: $(SRC)/AG.cpp
	g++ -c -O3 $^ -I$(INCLUDE) -o $@
	
clean:
	-rm $(OBJ)/*.o
	-rm $(BIN)/*
