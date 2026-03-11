CXX = g++
CXXFLAGS = -O2 -std=c++17

all: pathfinding

pathfinding: src/main.cpp src/map.cpp src/heuristics.cpp src/astar.cpp
	$(CXX) $(CXXFLAGS) -o pathfinding $^

clean:
	rm -f pathfinding
