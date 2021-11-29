CXX=clang++
INCLUDES=-Iincludes/
CXXEXTRAS=-Wall -Wextra -Werror -pedantic -Wno-error=unused-function -Wno-error=unused-variable
CXXFLAGS=-std=c++20 -g -fstandalone-debug
SRC := src
OBJ := obj

SOURCES := $(filter-out $(SRC)/main.cpp, $(wildcard $(SRC)/*.cpp))
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))


exec: bin/exec
dll: bin/dll


bin/exec: $(SRC)/*.cpp
	$(CXX) $(CXXFLAGS) $(CXXEXTRAS) $(INCLUDES) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.cpp | obj
	$(CXX) $(CXXFLAGS) $(CXXEXTRAS) $(INCLUDES) -c $< -o $@


obj:
	mkdir $@

.DEFAULT_GOAL := exec
.PHONY: clean exec dll

clean:
	rm -fr bin/* obj/* *.o
