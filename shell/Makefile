shell: shell.o shell_functions.o
	gcc -o shell shell.o shell_functions.o -Wall -std=c11 -lreadline

shell.o: ./src/shell.c ./src/shell.h
	gcc -c ./src/shell.c -Wall -std=c11

test: shell_functions.o unit.o aux_functions.o
	gcc -o unit shell_functions.o unit.o aux_functions.o -lcunit -lreadline -Wall -std=c11

shell_functions.o: ./src/shell_functions.c ./src/shell.h
	gcc -c ./src/shell_functions.c -Wall -std=c11

aux_functions.o: ./src/aux_functions.c
	gcc -c ./src/aux_functions.c -Wall -std=c11

unit.o: ./test/unit.c ./test/unit.h
	gcc -c ./test/unit.c -Wall -std=c11

clean:
	rm *.o; rm *.txt; rm -r *.dSYM; rm ./shell;rm ./unit
