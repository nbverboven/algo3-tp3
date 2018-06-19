greedy_player: GreedyPlayer.cpp GenericPlayer.hpp LogicalBoard.cpp Util.cpp main.cpp board_status.hpp
	g++ -ggdb -std=c++11 GreedyPlayer.cpp -c
	g++ -ggdb -std=c++11 LogicalBoard.cpp -c
	g++ -ggdb -std=c++11 Util.cpp -c
	g++ -ggdb -std=c++11 LogicalBoard.o GreedyPlayer.o Util.o main.cpp -o greedy_player

clean:
	rm -f greedy_player *.o *~ *.pyc
