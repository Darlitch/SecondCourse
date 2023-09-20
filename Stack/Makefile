CMAKE = -Wall -Weror -Wextra
LIBS = -lgtest -lgmock -pthread
FILES = $(wildcard *.cpp)
CC = g++

all: build run_test

build: $(FILES)
	$(CC) $(FLAGS) $(FILES) $(LIBS) -o test

run_test:
	./test

clean:
	rm -rf test