.PHONY: clean uninstall --make-bin-dir

###### PATHS ######
SRCDIR = src/
HEADDIR = include/
BINDIR = bin/
OBJDIR = $(BINDIR)obj/

EXEC = $(BINDIR)prog


#### Modules path ####
# find all modules (.c)
SRC = $(shell find $(SRCDIR) -name \*.c)
# Build BIN destination src/x/x.c -> bin/x/x.o
OBJ = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRC))
# Build dependencies automatically
DEP = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.d, $(SRC))
SUBMODULES = $(shell dirname $(OBJ))

ARGS =
CC = gcc
LDFLAGS = -lMLV -lm
CFLAGS = -ansi -O2 -g -Wall  \
			-I$(HEADDIR) -I$(HEADDIR)controller/ -I$(HEADDIR)graphics/  -I$(HEADDIR)core/ -I$(HEADDIR)/ \
			-DRESOURCES_DIR='"resources/"' \
			-DDEFAULT_WIN_H_PERCENT=70 \
			-DDEFAULT_WIN_W_PERCENT=70 \

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
# -MMD produit les fichiers de dependances de l'utilisateurs et pas systeme
# on se sert de ses fichiers plus bas dans le make ("-include ...")
# pour permettre la verification des modifications et recompiler en consequence
# -MP ajoute dans le fichier de dependances, celle du fichier compilé par ex. pour Foo.c:
# Foo.o : Foo.c Foo.h
# Foo.h :
$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) -c -MMD -MP $< -o $@ $(CFLAGS)

# reproduces modules dir hierarchy in bin
--make-bin-dir:
	@mkdir -p $(BINDIR)
	@mkdir -p $(shell dirname $(OBJ))

doc:
	doxygen doxygen

###### Cleaning ######
clean:
	@rm -rf $(OBJDIR)
	@rm -rf html

uninstall: clean
	@rm -rf $(BINDIR)


# Donc on evite decrire les depandances a la main.
-include $(DEP)