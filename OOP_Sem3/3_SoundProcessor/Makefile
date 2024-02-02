CFLAGS = -Wall -Werror -Wextra
LIBS = -lgtest -lgmock -pthread -lgtest_main -lrt
SOURCE = $(wildcard *.cpp)
SOURCETEST = 
CC = g++
OUTFILE = soundproc
TESTFILE = tests
ARGV = -c config.txt out.wav funkorama.wav district_four.wav
GCOV = --coverag

all: build_main run_main

tests: build_tests run_tests

build_main: clean
	$(CC) $(CFLAGS) $(SOURCE) $(LIBS) -o $(OUTFILE)

build_tests: clean
	$(CC) $(CFLAGS) $(SOURCETEST) $(LIBS) $(GCOV) -o $(TESTFILE)

run_main:
	./$(OUTFILE) $(ARGV)

run_tests:
	./$(TESTFILE) 

lcov: tests
	./$(TESTFILE)
	lcov --no-external -t "test" -o $(TESTFILE).info -c -d .
	genhtml -o report $(TESTFILE).info
	rm -f $(TESTFILE) $(TESTFILE).info .gc

clean:
	rm -rf $(OUTFILE) $(TESTFILE) report *.gcda *.gcno *.o *.info