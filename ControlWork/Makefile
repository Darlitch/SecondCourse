CMAKE = -Wall -Weror -Wextra
LIBS = -lgtest -lgmock -pthread
FILES = $(wildcard *.cpp)
CC = g++ -std=c++20

all: build run_test

build: $(FILES)
	$(CC) $(FILES) -g -o grammatic

run_test:
	./grammatic

clean:
	rm -rf grammatic