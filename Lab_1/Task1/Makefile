
#output: learn_makefile.o printfunc.o
#	gcc -o output learn_makefile.o printfunc.o -I.
#printfunc.o: printfunc.c
#	gcc -c -o printfunc.o printfunc.c -I.
#learn_makefile.o: learn_makefile.c
#	gcc -c -o learn_makefile.o learn_makefile.c -I.
#clean:
#	\rm -rf *.o output

#CC =gcc
#CFLAGS = -I.
#Target= output

#INCDIR =./include
#SRCDIR = ./src
#OBJDIR = ./obj

#OBJECTS = learn_makefile.o printfunc.o
#SOURCE = learn_makefile.c printfunc.c
#Include = learn_makefile.h

#all: $(Target)

#$(Target): $(OBJECTS)
#	$(CC) -o $@ $^ $(CFLAGS)
#clean: 
#	\rm *.o $(Target)

EXEC = output
CC = gcc    
#CFLAGS = -I.     

INCDIR =./include
OBJDIR = ./obj
SRCDIR = ./src

CFLAGS += -I$(INCDIR) -I$(SRCDIR)

SOURCES = learn_makefile.c printfunc.c

_OBJ = $(SOURCES:.c=.o)
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -vf $(EXEC) $(OBJ)