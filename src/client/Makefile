FLAGS = -Wextra -Wall -pedantic -std=c17 -O3 -lncurses
TEST_FLAGS = -Wextra -Wall -pedantic -std=c17 -ggdb -lncurses

chess: chess.o
	gcc -o chess chess.o $(FLAGS)

chess.o: chess.c
	gcc -c -g chess.c $(FLAGS)

test: chessTest.o
	gcc -o chess chess.o $(TEST_FLAGS)
	
chessTest.o: chess.c
	gcc -c -g chess.c $(TEST_FLAGS)

clean:
	rm *.o chess
