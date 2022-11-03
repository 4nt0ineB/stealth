CC = gcc
SRCDIR = src
HEADDIR = include
BINDIR = bin
CPPFLAGS = -Iinclude/
CFLAGS = -ansi -Wall
LFLAGS = -lMLV
OBJ = Main.o Menu.o Room.o Mana.o Test.o

all : $(BINDIR)/prog clean

$(BINDIR)/prog : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

Menu.o : $(SRCDIR)/Menu.c $(HEADDIR)/Menu.h

Room.o : $(SRCDIR)/Room.c $(HEADDIR)/Room.h

Character.o : $(SRCDIR)/Character.c $(HEADDIR)/Character.h

Guard.o : $(SRCDIR)/Guard.c $(HEADDIR)/Guard.h

Relic.o : $(SRCDIR)/Relic.c $(HEADDIR)/Relic.h 

Graphic.o : $(SRCDIR)/Graphic.c $(HEADDIR)/Graphic.h 

Test.o : $(SRCDIR)/Test.c $(HEADDIR)/Test.h

Main.o : $(SRCDIR)/Main.c $(HEADDIR)/Menu.h $(HEADDIR)/Room.h $(HEADDIR)/Character.h $(HEADDIR)/Guard.h $(HEADDIR)/Relic.h $(HEADDIR)/Graphic.h $(HEADDIR)/Test.h

%.o : $(SRCDIR)/%.c
	$(CC) -c $< $(CPPFLAGS) $(CFLAGS) $(LFLAGS)

clean : 
	rm *.o