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

server: $(OBJ_LOGIC)/socket.o $(OBJ_LOGIC)/netio.o $(OBJ_L1)/sockettimes.o $(OBJ_L1)/mainsocket.o $(OBJ_L1)/commandfunctions.o $(OBJ_L1)/periodictime.o $(OBJ_L1)/periodicfunctions.o $(OBJ_L2)/inputpayload.o $(OBJ_L2)/inputcommand.o $(OBJ_L2)/nextperiodic.o $(OBJ_L2)/sockettime.o $(OBJ_L2)/readlist.o $(OBJ_L2)/socketlist.o $(OBJ_L2)/socket.o $(OBJ_L2)/command.o $(OBJ_L2)/periodic.o $(OBJ)/server.o $(OBJ)/main.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^


client: $(OBJ_LOGIC)/socket.o $(OBJ_LOGIC)/netio.o $(OBJ_L1)/sockettimes.o $(OBJ_L1)/mainsocket.o $(OBJ_L1)/commandfunctions.o $(OBJ_L1)/periodictime.o $(OBJ_L1)/periodicfunctions.o $(OBJ_L2)/inputpayload.o $(OBJ_L2)/inputcommand.o $(OBJ_L2)/nextperiodic.o $(OBJ_L2)/sockettime.o $(OBJ_L2)/readlist.o $(OBJ_L2)/socketlist.o $(OBJ_L2)/socket.o $(OBJ_L2)/command.o $(OBJ_L2)/periodic.o $(OBJ)/client.o $(OBJ)/main.o | $(BIN)
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
