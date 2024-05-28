WWW = -Wall -Wextra -Werror -std=c11
LINUX= -lrt -lpthread -lm -lsubunit -lcheck
LIBS=-lcheck
LFLAGS=-fprofile-arcs -ftest-coverage
OS=$(shell uname -s)

all: clean test gcov_report check

re: clean all 

test: s21_decimal.a 
ifeq ($(OS), Darwin)
	gcc $(WWW) tests.c s21_decimal.a -o test $(LIBS)
else
	gcc $(WWW) tests.c s21_decimal.a -o test $(LIBS) $(LINUX)
endif
	./test

gcov_report: clean_test
ifeq ($(OS), Darwin)
	gcc $(WWW) -fprofile-arcs -ftest-coverage tests.c s21_*.c -o test $(LIBS)
else
	gcc $(WWW) -fprofile-arcs -ftest-coverage tests.c s21_*.c -o test $(LIBS) $(LINUX)
endif
	./test
	lcov -t "tests" -o test.info -c -d .
	genhtml -o report test.info
	open ./report/index.html

s21_decimal.a: build_s21_decimal 

build_s21_decimal:
	gcc -c s21_*.c
	ar rcs s21_decimal.a s21_*.o
	ranlib s21_decimal.a

install_brew:
	cd ~
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

install_cpch:
	brew install cppcheck

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

check: cppcheck
	cp ../materials/linters/.clang-format ./
	clang-format -n *.c
	clang-format -n *.h 
	# rm -rf .clang-format

install_valgrind:
	brew install --HEAD LouisBrunner/valgrind/valgrind

vc:
	CK_FORK=no valgrind --tool=memcheck ./test

vb:
	K_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
	 
valgrind:
	valgrind -q --tool=memcheck --leak-check=yes --log-file=RESULT_VALGRIND.txt ./test -e while -e void ./s21_decimal.c Makefile -f pattern.txt -s -l
	cat RESULT_VALGRIND.txt


clean_obj:
	rm -rf *.o

clean_lib: 
	rm -rf *.a

clean_test:
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf test
	rm -rf report

clean: clean_lib clean_lib clean_test clean_obj
	rm -rf s21_test

print:
	gcc $(WWW) *.c 