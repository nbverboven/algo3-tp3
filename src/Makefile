greedy_player: GreedyPlayer.cpp LogicalBoard.cpp Util.cpp main.cpp
	g++ -ggdb -std=c++11 GreedyPlayer.cpp -c
	g++ -ggdb -std=c++11 LogicalBoard.cpp -c
	g++ -ggdb -std=c++11 Util.cpp -c
	g++ -ggdb -std=c++11 LogicalBoard.o GreedyPlayer.o Util.o main.cpp -o greedy_player

clean:
	rm -f greedy_player *.o *~ *.pyc
