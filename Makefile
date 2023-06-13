# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s --trace-children=yes --track-fds=yes --log-file=valgrind.log ./test
CFLAGS = -Wno-implicit-function-declaration -Wall -Werror -Wextra -pedantic -std=c11 -g
SOURSES = ./Backend/Sourses/affinity_transformations.c ./Backend/Sourses/parser.c
OBJECTS = $(SOURSES:.c=.o)
SOURSE_TEST = ./Backend/Tests/tests.c
LIB = lib_viewer.a

UNAME =  $(shell uname -s)
FLAGS = 
ifeq ($(UNAME), Linux)
	FLAGS += -lcheck_pic -lm -lsubunit -lrt -lpthread
else
	FLAGS += -lcheck -lm
endif

all: install

install: $(LIB)
	cmake 3D_Viewer -B App
	cd App && make
	cd App/3D_Viewer.app/Contents/MacOS && ./3D_Viewer

uninstall:
	rm -rf App

dvi:
	doxygen doxygen_file
	open ./Docs/html/index.html

dist: clean uninstall
	tar -cvzf viewer.tgz .	

test: $(LIB) $(SOURSE_TEST)
	@gcc $(SOURSE_TEST) $(CFLAGS) $(SOURSES) -o test_viewer $(FLAGS)
	@./test_viewer

lib_viewer.a: $(OBJECTS)
	ar rc $(LIB) $(OBJECTS)
	ranlib $(LIB)
	# mv $(LIB) ./qt/lib_viewer.a
	# rm -rf *.o

affinity_transformations.o: affinity_transformations.c
	gcc $(CFLAGS) affinity_transformations.c -c -o affinity_transformations.o

parser.o: parser.c
	gcc $(CFLAGS) parser.c -c -o parser.o

gcov_report: $(LIB)
	gcc $(CFLAGS) $(SOURSE_TEST) -o testlcov --coverage $(SOURSES) $(FLAGS)
	./testlcov
	lcov -t "a.out" -o test.info -c -d .
	genhtml -o report test.info
	open ./report/index.html
	rm -rf test *.o *.a *gcda *gcno *info

valgrind: clean $(LIB)
	gcc $(SOURSE_TEST) $(CFLAGS) $(SOURSES) -o testvalg $(FLAGS)
	valgrind --leak-check=full --track-origins=yes --track-fds=yes ./testvalg
	# grep ERROR valgrind.log

clean:
	@rm -rf ./Backend/Sourses/a.out.dSYM
	@find ./Backend/ -type f -name "*.o" -delete
	@find ./Backend/ -type f -name "*.out" -delete
	@find ./Backend/ -type f -name "*.dSYM" -delete
	@rm -rf *.a a.out *.dSYM test_viewer testlcov testvalg *gcda report *gcno *info *.log Documentation \
	build-qt-Desktop_x86_darwin_generic_mach_o_64bit-Debug *.tgz Docs build-3D_Viewer-Desktop_x86_darwin_generic_mach_o_64bit-Debug
