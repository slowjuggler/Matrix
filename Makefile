CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -g
SOURCES=matrix.c
OBJECTS=$(SOURCES:.c=.o)
OS = $(shell uname)
TEST_FILE_NAME = matrix_test.c

ifeq ($(OS), Darwin)
	TEST_LIBS=-lcheck
else
	TEST_LIBS=-lcheck -pthread -lrt -lm 
endif

all: matrix.a

matrix.a: clean copy mob $(OBJECTS)
	ar -rcs matrix.a *.o
	rm -f *.o

mob:
	gcc -c *.s	

test: $(TEST_FILE_NAME) matrix.a
	$(CC) $(CFLAGS) $(TEST_FILE_NAME) $(SOURCES) *.s -o test $(TEST_LIBS) -L. --coverage
	./test

gcov_report: test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

clean:
	rm -rf *.o *.s *.a *.so *.gcda *.gcno *.gch rep.info *.html *.css test report *.txt test.info test.dSYM proj
install_lcov:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

check: test
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config *.c *.h
	rm -rf CPPLINT.cfg
	make test

leaks: test
ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test
endif

copy:
ifeq ($(OS), Darwin)
	cp Darwin/*.s .
else
	cp Linux/*.s .
endif