# Installation directories following GNU conventions
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
sbindir = $(exec_prefix)/sbin
datarootdir = $(prefix)/share
datadir = $(datarootdir)
includedir = $(prefix)/include
mandir = $(datarootdir)/man

BIN=bin
OBJ=obj
SRC=src

OBJ_IF=obj/if
SRC_IF=src/if

CC = gcc
CFLAGS = -Wextra -Wall -I$(SRC)

.PHONY: all install uninstall clean

EXES = server client

all: $(EXES)

server: $(OBJ_IF)/ptime.o $(OBJ_IF)/socktime.o $(OBJ_IF)/ftable.o $(OBJ_IF)/buffer.o $(OBJ_IF)/readlist.o $(OBJ_IF)/socklist.o $(OBJ_IF)/netio.o $(OBJ)/server.o $(OBJ)/main.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^


client: $(OBJ_IF)/ptime.o $(OBJ_IF)/socktime.o $(OBJ_IF)/ftable.o $(OBJ_IF)/buffer.o $(OBJ_IF)/readlist.o $(OBJ_IF)/socklist.o $(OBJ_IF)/netio.o $(OBJ)/client.o $(OBJ)/main.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_IF)/%.o: $(SRC_IF)/%.c | $(OBJ_IF)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN):
	mkdir $(BIN)

$(OBJ):
	mkdir $(OBJ)

$(OBJ_IF):
	mkdir $(OBJ)
	mkdir $(OBJ_IF)

clean:
	rm -rf $(BIN)
	rm -rf $(OBJ)
