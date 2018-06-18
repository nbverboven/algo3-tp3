referee_sample: referee_sample.cpp
	g++ -ggdb -std=c++11 MoronicPlayer.cpp -c -o ./MoronicPlayer.o
	g++ -ggdb -std=c++11 ../Referee.cpp -c -o ./Referee.o
	g++ -ggdb -std=c++11 ../LogicalBoard.cpp -c -o ./LogicalBoard.o
	g++ -ggdb -std=c++11 LogicalBoard.o Referee.o MoronicPlayer.o referee_sample.cpp -o referee_sample

clean:
	rm -f referee_sample *.o
