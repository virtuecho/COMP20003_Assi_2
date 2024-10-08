# define C compiler & flags, note that -g is for potential use of valgrind/gdb etc.
CC = gcc
CFLAGS = -Wall -g
# define libraries to be linked (for example -lm)
LIB =

# the first target, which includes all executable file names such as dict1, dict2
all = dict3 dict4


# define sets of .c source files and object files
SRC = main.c data.c linkedList.c dict.c utils.c patricia_trie.c
# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)

dict3: $(OBJ)
	$(CC) $(CFLAGS) -o dict3 $(OBJ) $(LIB)

dict4: $(OBJ)
	$(CC) $(CFLAGS) -o dict4 $(OBJ) $(LIB)
	

# for each .o file, make it also depends on the corresponding .h file
%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@


# for re-compiling from scratch, if we use make -B we don't need the target clean
clean:
	rm -f $(OBJ) $(EXE)
