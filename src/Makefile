all: greedy_player busqueda_local optimize_genomes

greedy_player: GreedyPlayer.cpp GenericPlayer.hpp LogicalBoard.cpp Util.cpp board_status.hpp main.cpp
	g++ -ggdb -std=c++11 GreedyPlayer.cpp -c
	g++ -ggdb -std=c++11 LogicalBoard.cpp -c
	g++ -ggdb -std=c++11 Util.cpp -c
	g++ -ggdb -std=c++11 LogicalBoard.o GreedyPlayer.o Util.o main.cpp -o greedy_player

busqueda_local: BusquedaLocal.cpp
	g++ -ggdb -O3 -Wall -Wextra -pedantic -std=c++11 BusquedaLocal.cpp -o busqueda_local

optimize_genomes:
	g++ -ggdb -O3 -Wall -Wextra -pedantic -std=c++11 OptimizeGenomes.cpp -o optimize_genomes

clean:
	rm -f greedy_player busqueda_local *.o *~ *.pyc
