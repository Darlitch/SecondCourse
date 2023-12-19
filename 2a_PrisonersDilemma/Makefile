CFLAGS = -Wall -Werror -Wextra
LIBS =--coverage -lgtest -lgmock -pthread -lgtest_main -lrt
SOURCE = $(wildcard *.cpp)
OBGMAIN = PrisonersDilemma.o Strategies.o Menu.o
CC = g++
OUTFILE = dilema

all: clean build run_test 

build: create_objects $(OBJMAIN)
	$(CC) $(OBGMAIN) $(LIBS) -o $(OUTFILE)

create_objects: $(SOURCE)
	$(CC) $(CFLAGS) -c PrisonersDilemma.cpp
	$(CC) $(CFLAGS) -c Strategies.cpp
	$(CC) $(CFLAGS) -c Menu.cpp

run_test:
	./$(OUTFILE) strat1 strat2 strat1 strat3 --steps=5 --mode=tournament

lcov: build
	./$(OUTFILE)
	lcov --no-external -t "test" -o $(OUTFILE).info -c -d .
	genhtml -o report $(OUTFILE).info
	rm -f $(OUTFILE) $(OUTFILE).info .gc

clean:
	rm -rf $(OUTFILE) report *.gcda *.gcno *.o