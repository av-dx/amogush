CC = gcc
MKDIR = mkdir -p

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = ./include
EXEC = $(BINDIR)/main

SRC = common.c ls.c pinfo.c pwd.c prompt.c echo.c cd.c main.c

SRC_FILES = $(patsubst %.c,$(SRCDIR)/%.c, $(SRC))
OBJ_FILES = $(patsubst %.c,$(OBJDIR)/%.o, $(SRC))

all: objdir builddir main

main: $(OBJ_FILES)
	$(CC) -g -Wall $(OBJ_FILES) -o $(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -Wall $< -I $(INCDIR) -o $@

objdir:
	$(MKDIR) $(OBJDIR)

builddir:
	$(MKDIR) $(BINDIR)

clean:
	rm -rf $(OBJDIR)/*.o $(EXEC)