CC = gcc
SRCDIR = src/
HEADDIR = include/
BINDIR = bin/
OBJDIR = $(BINDIR)obj/
CFLAGS = -ansi -O2 -g -Wall -Wfatal-errors -I$(HEADDIR) -I$(HEADDIR)/controller/
LDFLAGS = -lMLV
EXEC = $(BINDIR)prog
ARGS =

#### Modules path ####
# find all modules (.c)
SRC = $(shell find $(SRCDIR) -name \*.c)
# Build BIN destination src/x/x.c -> bin/x/x.o
OBJ = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRC))
# Build dependencies automatically
DEP = $(patsubst $(SRCDIR)%.c, $(BINDIR)%.d, $(SRC))
# Search for dependency rules and includes it

###### Run ######
run: install
	./$(EXEC) $(ARGS)

###### Test ######
vtest: install
	valgrind --track-origins=yes --leak-check=yes -s ./$(EXEC) $(ARGS)
gtest: install
	gprof -b ./$(EXEC) gmon.out

###### Compilation ######
install: --make-bin-dir $(EXEC)

# we need to gather all object from bin
# so we build all needed paths bin/Main.o, etc..
${EXEC}: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# if target bin/Thing.o exists compiles it to bin
$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# reproduces modules dir hierarchy in bin
--make-bin-dir:
	@mkdir -p $(BINDIR)
	@mkdir -p $(shell dirname $(OBJ))

doc:
	doxygen doxygen

###### Cleaning ######
.PHONY: clean uninstall
clean:
	@rm -rf $(OBJDIR)
	@rm -rf html

uninstall: clean
	@rm -rf $(BINDIR)
