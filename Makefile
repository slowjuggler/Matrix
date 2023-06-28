GCC = gcc -std=c++17 -pedantic -Wall -Werror -Wextra
SOURCES = matrix.cc
TEST = tests.cc
LIB = matrix.a
LIBOBJ = matrix.o
GCOV =--coverage

OS = $(shell uname)

ifeq ($(OS), Darwin)
	LIBFLAGS = -lm -lgtest -lstdc++
else
	LIBFLAGS=-lstdc++ `pkg-config --cflags --libs gtest`
endif

all: clean test

test: matrix.a
	$(GCC) $(TEST) $(SOURCES) *.s -o test $(LIBFLAGS) -L. --coverage
	./test

matrix.a: clean copy mob
	ar -rcs matrix.a *.o
	rm -f *.o

mob:
	gcc -c *.s

gcov_report: matrix.a
	$(GCC) $(GCOV) $(TEST) $(SOURCES) $(LIB) -L. $(LIB)  $(LIBFLAGS) -o test
	./test
	lcov -t "test" -c -d  ./ --no-external --output-file ./coverage.info
	genhtml ./coverage.info --output-directory ./report/
	open ./report/index.html

ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif

clean:
	rm -rf *.s *.o *.a *.so *.cfg *.out *.dSYM test *.txt report *.info *.gcda *.gcno *.gch .clang-format logs

copy:
ifeq ($(OS), Darwin)
	cp Darwin/*.s .
else
	cp Linux/*.s .
endif