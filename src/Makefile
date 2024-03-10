CC = gcc -Wall -Werror -Wextra
CFLAGS = -g -std=c11
OBJ = arithmetic.c comparison.c onverters.c another_func.c help_func.c
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  	OPEN_CMD = xdg-open
	ADD_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
endif

ifeq ($(UNAME_S),Darwin)
	OPEN_CMD = open
	ADD_LIB = -lcheck
endif

all: d3_decimal.a test clean

test:
	@$(CC) $(CFLAGS) --coverage *.c TEST/*.c -o tests $(ADD_LIB)
	@./tests

d3_decimal.a:
	@$(CC) $(CFLAGS) *.c -c
	@ar rc d3_decimal.a *.o
	@ranlib d3_decimal.a

gcov_report: test
	@lcov -t "d3_decimal_test" -o fizzbuzz.info -c -d .
	@genhtml -o report fizzbuzz.info
	@open -a "Google Chrome" report/src/index.html

style:
	@clang-format -style=Google -n *.c *.h TEST/*.c TEST/*.h

clean:
	@-rm -rf *.o *.gcno *.gcda *.info *.gcov *.a *.dSYM

all_clean: clean
	@ -rm -rf tests report

.PHONY:	d3_decimal.a test gcov_report