 CC = gcc
 CFLAGS = -ansi -Wall -pedantic -g
 GLOBAL_DEPS = globals.h
 EXE_DEPS = assembler.o utils.o pre_assem.o first_pass.o data_structures.o errors.o lexer.o encode.o
 VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
 PROG = assembler
 ARG1 = first_argument
 ARG2 = second_argument

 ## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o: $(GLOBAL_DEPS)
	$(CC) -c assem.c $(CFLAGS) -o $@

pre_assem.o: $(GLOBAL_DEPS)
	$(CC) -c pre_assem.c $(CFLAGS) -o $@

first_pass.o: $(GLOBAL_DEPS)
	$(CC) -c first_pass.c $(CFLAGS) -o $@

data_structures.o: $(GLOBAL_DEPS)
	$(CC) -c data_structures.c $(CFLAGS) -o $@

utils.o: $(GLOBAL_DEPS)
	$(CC) -c utils.c $(CFLAGS) -o $@

errors.o: $(GLOBAL_DEPS)
	$(CC) -c errors.c $(CFLAGS) -o $@

lexer.o: $(GLOBAL_DEPS)
	$(CC) -c lexer.c $(CFLAGS) -o $@

encode.o: $(GLOBAL_DEPS)
	$(CC) -c encode.c $(CFLAGS) -o $@

clean:
	rm -rf *.o *.am *.ob *.ent *.ext

val:
	valgrind $(VALGRIND_FLAGS) ./$(PROG) $(ARG1) $(ARG2)
