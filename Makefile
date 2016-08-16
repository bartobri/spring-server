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

OBJ_L1=obj/l1
SRC_L1=src/l1
OBJ_L2=obj/l2
SRC_L2=src/l2

CC = gcc
CFLAGS = -Wextra -Wall -iquote$(SRC)

.PHONY: all install uninstall clean

EXES = server client

all: $(EXES)

server: $(OBJ_IF)/timestamps.o $(OBJ_IF)/comfunctions.o $(OBJ_IF)/readlist.o $(OBJ_IF)/socklist.o $(OBJ_LOGIC)/socket.o $(OBJ_LOGIC)/netio.o $(OBJ_LOGIC)/comfunction.o $(OBJ_L1)/prdtime.o $(OBJ_L1)/prdfunction.o $(OBJ_L1)/payload.o $(OBJ_L1)/command.o $(OBJ_L2)/periodic.o $(OBJ_L2)/inputparser.o $(OBJ)/server.o $(OBJ)/main.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^


client: $(OBJ_IF)/timestamps.o $(OBJ_IF)/comfunctions.o $(OBJ_IF)/readlist.o $(OBJ_IF)/socklist.o $(OBJ_LOGIC)/socket.o $(OBJ_LOGIC)/netio.o $(OBJ_LOGIC)/comfunction.o $(OBJ_L1)/prdtime.o $(OBJ_L1)/prdfunction.o $(OBJ_L1)/payload.o $(OBJ_L1)/command.o $(OBJ_L2)/periodic.o $(OBJ_L2)/inputparser.o $(OBJ)/client.o $(OBJ)/main.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_IF)/%.o: $(SRC_IF)/%.c | $(OBJ_IF)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(OBJ_LOGIC)/%.o: $(SRC_LOGIC)/%.c | $(OBJ_LOGIC)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_L1)/%.o: $(SRC_L1)/%.c | $(OBJ_L1)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(OBJ_L2)/%.o: $(SRC_L2)/%.c | $(OBJ_L2)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN):
	mkdir $(BIN)

$(OBJ):
	mkdir $(OBJ)

$(OBJ_IF): $(OBJ)
	mkdir $(OBJ_IF)

$(OBJ_LOGIC): $(OBJ)
	mkdir $(OBJ_LOGIC)

$(OBJ_L1): $(OBJ)
	mkdir $(OBJ_L1)

$(OBJ_L2): $(OBJ)
	mkdir $(OBJ_L2)

clean:
	rm -rf $(BIN)
	rm -rf $(OBJ)
