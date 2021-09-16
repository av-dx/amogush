CC = gcc
MKDIR = mkdir -p

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = ./include
EXEC = $(BINDIR)/main

SRC = common.c pwd.c prompt.c echo.c cd.c main.c

SRC_FILES = $(patsubst %.c,$(SRCDIR)/%.c, $(SRC))
OBJ_FILES = $(patsubst %.c,$(OBJDIR)/%.o, $(SRC))

all: objdir main

main: $(OBJ_FILES)
	$(CC) -g $(OBJ_FILES) -o $(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -I $(INCDIR) -o $@

objdir:
	$(MKDIR) $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*.o $(EXEC)