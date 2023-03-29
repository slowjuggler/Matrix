CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -mfpmath=sse -march=native -O3
OS=$(shell uname)
ifeq ($(OS), Darwin)
	TEST_LIBS=-lcheck
else
	TEST_LIBS=-lcheck -subunit -pthread -lrt -lm
endif

OBJECTS=*.o
SOURCES=matrix.c *.s
TEST_FILE_NAME = matrix_test.c

all: gcov_report

matrix.a: clean $(OBJECTS) copy
	ar -rcs matrix.a $(OBJECTS)
	rm -f *.o

test: $(TEST_FILE_NAME) matrix.a
	$(CC) $(CFLAGS) $(TEST_FILE_NAME) $(SOURCES) -o test $(TEST_LIBS) -L. --coverage
	./test

gcov_report: test
	
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

clean:
	rm -rf *.o *.a *.so *.gcda *.gcno *.gch rep.info *.html *.css test report *.txt test.info test.dSYM *.s

install lcov:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif

copy:
ifeq ($(OS), Darwin)
	cp Darwin/*.s .
else
	cp Linux/*.s .
endif
