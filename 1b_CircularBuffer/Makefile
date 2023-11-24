CMAKE = -Wall -Weror -Wextra
LIBS =--coverage -lgtest -lgmock -pthread -lgtest_main -lrt
FILES = $(wildcard *.cpp)
CC = g++
OUTFILE = ./buffer

all: build run_test

build: $(FILES)
	$(CC) $(FLAGS) $(FILES) $(LIBS) -o buffer

run_test:
	./$(OUTFILE)

lcov: build
	./$(OUTFILE)
	lcov --no-external -t "test" -o $(OUTFILE).info -c -d .
	genhtml -o report $(OUTFILE).info
	rm -f $(OUTFILE) $(OUTFILE).info .gc

clean:
	rm -rf buffer report *.gcda *.gcno