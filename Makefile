CXX=clang++
INCLUDES=-Iincludes/
CXXEXTRAS=-Wall -Wextra -Werror -pedantic
CXXFLAGS=-std=c++20 -g -fstandalone-debug
SRC := src
OBJ := obj

SOURCES := $(filter-out $(SRC)/main.cpp, $(wildcard $(SRC)/*.cpp))
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))


exec: bin/exec

bin/exec: $(OBJECTS) $(SRC)/main.cpp
	$(CXX) $(CXXFLAGS) $(CXXEXTRAS) $(INCLUDES) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.cpp | obj
	$(CXX) $(CXXFLAGS) $(CXXEXTRAS) $(INCLUDES) -c $< -o $@


obj:
	mkdir $@

.DEFAULT_GOAL := exec
.PHONY: clean exec

clean:
	rm -fr bin/* obj/* *.o
