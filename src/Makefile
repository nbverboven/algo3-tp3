CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -ggdb -std=c++11
EXE = greedy_player busqueda_local #optimize_genomes
SRC = $(filter-out main.cpp BusquedaLocal.cpp OptimizeGenomes.cpp GraspPlayer.cpp, $(wildcard *.cpp))
HEADERS = $(wildcard *.h, *hpp)
OBJS = $(SRC:.cpp=.o)

.PHONY: all clean

all: $(EXE)

greedy_player: $(OBJS) $(HEADERS) main.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) main.cpp -o $@

busqueda_local: $(OBJS) $(HEADERS) BusquedaLocal.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) BusquedaLocal.cpp -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(EXE) *.pyc

