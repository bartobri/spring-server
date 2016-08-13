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
OBJ_LOGIC=obj/logic
SRC_LOGIC=src/logic

CC = gcc
CFLAGS = -Wextra -Wall -iquote$(SRC)

.PHONY: all install uninstall clean

EXES = server client

all: $(EXES)

server: $(OBJ_IF)/ptime.o $(OBJ_IF)/timestamps.o $(OBJ_IF)/comfunctions.o $(OBJ_IF)/prdfunctions.o $(OBJ_IF)/buffer.o $(OBJ_IF)/command.o $(OBJ_IF)/payload.o $(OBJ_IF)/readlist.o $(OBJ_IF)/socklist.o $(OBJ_LOGIC)/socket.o $(OBJ_LOGIC)/netio.o $(OBJ_LOGIC)/comfunction.o $(OBJ_LOGIC)/prdfunction.o $(OBJ)/server.o $(OBJ)/main.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^


client: $(OBJ_IF)/ptime.o $(OBJ_IF)/timestamps.o $(OBJ_IF)/comfunctions.o $(OBJ_IF)/prdfunctions.o $(OBJ_IF)/buffer.o $(OBJ_IF)/command.o $(OBJ_IF)/payload.o $(OBJ_IF)/readlist.o $(OBJ_IF)/socklist.o $(OBJ_LOGIC)/socket.o $(OBJ_LOGIC)/netio.o $(OBJ_LOGIC)/comfunction.o $(OBJ_LOGIC)/prdfunction.o $(OBJ)/client.o $(OBJ)/main.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_IF)/%.o: $(SRC_IF)/%.c | $(OBJ_IF)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(OBJ_LOGIC)/%.o: $(SRC_LOGIC)/%.c | $(OBJ_LOGIC)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN):
	mkdir $(BIN)

$(OBJ):
	mkdir $(OBJ)

$(OBJ_IF): $(OBJ)
	mkdir $(OBJ_IF)

$(OBJ_LOGIC): $(OBJ)
	mkdir $(OBJ_LOGIC)

clean:
	rm -rf $(BIN)
	rm -rf $(OBJ)
