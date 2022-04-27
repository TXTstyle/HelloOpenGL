CXX	:=	clang++
FLAGS	:=	-Wall -Wextra -std=c++17 -fdelayed-template-parsing

INCLUDE	:=	include
SRC	:=	src
BIN	:=	bin

LIBS	:=	-lglfw -lGLEW -lGLU -lGL
EXE	:=	main

all: $(BIN)/$(EXE)

run: clean all
		./$(BIN)/$(EXE)

$(BIN)/$(EXE): $(SRC)/*.cpp
	$(CXX) $(FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBS)

start:
		./$(BIN)/$(EXE)

clean:
		-rm $(BIN)/*
