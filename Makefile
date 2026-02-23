CC = gcc
CFLAGS = -Wall -Wextra -g
RM = del /q

default: all
all: regex_lite

regex_lite: regex_lite.o search.o match.o
	$(CC) $(CFLAGS) -o regex_lite regex_lite.o search.o match.o
regex_lite.o: regex_lite.c match.h
	$(CC) $(CFLAGS) -c regex_lite.c
search.o: search.c match.h
	$(CC) $(CFLAGS) -c search.c
match.o: match.c match.h
	$(CC) $(CFLAGS) -c match.c
tests: tests.o match.o
	$(CC) $(CFLAGS) -o tests tests.o match.o
tests.o: tests.c match.h
	$(CC) $(CFLAGS) -c tests.c
bench: benchmark.o match.o
	$(CC) $(CFLAGS) -o bench benchmark.o match.o
benchmark.o: benchmark.c match.h
	$(CC) $(CFLAGS) -c benchmark.c

clean:
	-$(RM) regex_lite regex_lite.exe *.o 2>nul || true