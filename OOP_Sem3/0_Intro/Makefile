CMAKE = -Wall -Weror -Wextra
LIBS = -lgtest -lgmock -pthread
FILES = $(wildcard *.cpp)
CC = g++

all: build run_test

build: $(FILES)
	$(CC) $(FLAGS) $(FILES) -o converter

run_test:
	./converter Input.txt Output.csv

clean:
	rm -rf converter Output.csv