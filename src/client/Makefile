FLAGS = -Wextra -Wall -pedantic -std=c11 -O3 -I../shared -I../ -ggdb
TEST_FLAGS = -Wextra -Wall -pedantic -std=c17 -ggdb -lncurses

chess: chess.o chess_network.o chess_engine.o
	gcc -o chess chess_engine.o chess.o chess_network.o $(FLAGS) -lncurses

chess.o: chess.c
	gcc -c chess.c $(FLAGS)

chess.c:
	
chess_network.o: ../shared/chess_network.c
	gcc -c ../shared/chess_network.c $(FLAGS)

../shared/chess_network.c:

chess_engine.o: ../chess_engine.c
	gcc -c ../chess_engine.c $(FLAGS)

../chess_engine.c:

test: chessTest.o
	gcc -o chess chess.o $(TEST_FLAGS)
	
chessTest.o: chess.c
	gcc -c chess.c $(TEST_FLAGS)

clean:
	rm *.o chess *.s
