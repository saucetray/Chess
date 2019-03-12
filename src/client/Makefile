FLAGS = -Wextra -Wall -pedantic -std=c17 -O3 -I../shared
TEST_FLAGS = -Wextra -Wall -pedantic -std=c17 -ggdb -lncurses

chess: chess.o chess_network.o
	gcc -o chess chess.o chess_network.o $(FLAGS) -lncurses

chess.o: chess.c
	gcc -c chess.c $(FLAGS)

chess_network.o:
	gcc -c ../shared/chess_network.c $(FLAGS)

test: chessTest.o
	gcc -o chess chess.o $(TEST_FLAGS)
	
chessTest.o: chess.c
	gcc -c chess.c $(TEST_FLAGS)

clean:
	rm *.o chess
