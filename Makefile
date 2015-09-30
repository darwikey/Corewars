
CC=gcc
CFLAGS= -std=c99 -W -Wall -g
LDFLAGS= -lncurses
EXEC= corewars
SRCDIR= src
SRC=  $(SRCDIR)/display.c  $(SRCDIR)/main.c  $(SRCDIR)/bot_loader.c  $(SRCDIR)/simulator.c
OBJ= $(SRC:.c=.o)

all: $(EXEC)

corewars: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)


%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(SRCDIR)/*.o

mrproper: clean
	rm -rf $(EXEC)