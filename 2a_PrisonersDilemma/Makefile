FLAGS = -Wall -Werror -Wextra
LIBS =--coverage -lgtest -lgmock -pthread -lgtest_main -lrt
FILES = $(wildcard *.cpp)
CC = g++
OUTFILE = dilema

all: build run_test

build: $(FILES)
	$(CC) $(FLAGS) $(FILES) $(LIBS) -o $(OUTFILE)

run_test:
	./$(OUTFILE) strat1 strat2 strat1 --steps=5 --mode=fast

lcov: build
	./$(OUTFILE)
	lcov --no-external -t "test" -o $(OUTFILE).info -c -d .
	genhtml -o report $(OUTFILE).info
	rm -f $(OUTFILE) $(OUTFILE).info .gc

clean:
	rm -rf $(OUTFILE) report *.gcda *.gcno