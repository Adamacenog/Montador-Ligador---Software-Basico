IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./obj
LDIR =../lib

LIBS=-lm

_DEPS = ligador.h preProcess.h stringFunctions.h firstPass.h secondPass.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = montador.o preProcess.o stringFunctions.o firstPass.o secondPass.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ2 = ligador.o
OBJ2 = $(patsubst %,$(ODIR)/%,$(_OBJ2))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

all: clean montador ligador

montador: $(OBJ)
	gcc -std=c99  -o $@ $^ $(CFLAGS) $(LIBS)

ligador: $(OBJ2)
	gcc -std=c99   -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ montador ligador
