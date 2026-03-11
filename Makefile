CXX = g++
CXXFLAGS = -O2 -std=c++17

all: pathfinding

pathfinding: src/main.cpp
	$(CXX) $(CXXFLAGS) -o pathfinding src/main.cpp

clean:
	rm -f pathfinding
