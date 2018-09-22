IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./obj
LDIR =../lib

LIBS=-lm

_DEPS = montador.h ligador.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = montador.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ2 = ligador.o
OBJ2 = $(patsubst %,$(ODIR)/%,$(_OBJ2))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: clean montador ligador

montador: $(OBJ)
	gcc -std=c99 -o $@ $^ $(CFLAGS) $(LIBS)

ligador: $(OBJ2)
	gcc -std=c99 -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ montador ligador
