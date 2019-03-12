FLAGS = -Wextra -Wall -pedantic -std=c17 -O3 -I../shared

TEST_FLAGS = -Wextra -Wall -pedantic -std=c17 -ggdb

server: chess_server.o
	gcc -o chess_server chess_server.o $(FLAGS)

chess_server.o: chess_server.c
	gcc -c -g chess_server.c $(FLAGS)

test: chess_server_test.o
	gcc -o chess_server chess_server.o $(TEST_FLAGS)

chess_server_test.o: chess_server.c
	gcc -c -g chess_server.c $(TEST_FLAGS)

clean:
	rm *.o chess_server
