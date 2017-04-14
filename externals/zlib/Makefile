# makefile for zlib using gcc (generic, static library)
#
# This code is released under the zlib license.
# For conditions of distribution and use, see the disclaimer
# and license in zlib.h

LIB=./lib
INCLUDE =./include
SRC=./source
OBJ=./obj
BIN=./bin

#LOC = -DASMV
#LOC = -DZLIB_DEBUG -g
CC = gcc
CFLAGS = $(LOC) -W -Wall -O3


zlib: $(LIB)/zlib.a
	echo "zlib.a built..."

$(LIB)/zlib.a: \
$(OBJ)/adler32.o \
$(OBJ)/crc32.o \
$(OBJ)/deflate.o \
$(OBJ)/gzclose.o \
$(OBJ)/gzlib.o \
$(OBJ)/gzread.o \
$(OBJ)/gzwrite.o \
$(OBJ)/infback.o \
$(OBJ)/inffast.o \
$(OBJ)/inflate.o \
$(OBJ)/inftrees.o \
$(OBJ)/trees.o \
$(OBJ)/uncompr.o \
$(OBJ)/zutil.o \

	ar csr $(LIB)/zlib.a \
$(OBJ)/adler32.o \
$(OBJ)/crc32.o \
$(OBJ)/deflate.o \
$(OBJ)/gzclose.o \
$(OBJ)/gzlib.o \
$(OBJ)/gzread.o \
$(OBJ)/gzwrite.o \
$(OBJ)/infback.o \
$(OBJ)/inffast.o \
$(OBJ)/inflate.o \
$(OBJ)/inftrees.o \
$(OBJ)/trees.o \
$(OBJ)/uncompr.o \
$(OBJ)/zutil.o \


$(OBJ)/adler32.o: $(SRC)/adler32.c
	$(CC) $(CFLAGS) -c $(SRC)/adler32.c -I$(INCLUDE) \
	-o $(OBJ)/adler32.o

$(OBJ)/crc32.o: $(SRC)/crc32.c
	$(CC) $(CFLAGS) -c $(SRC)/crc32.c -I$(INCLUDE) \
	-o $(OBJ)/crc32.o

$(OBJ)/deflate.o: $(SRC)/deflate.c
	$(CC) $(CFLAGS) -c $(SRC)/deflate.c -I$(INCLUDE) \
	-o $(OBJ)/deflate.o

$(OBJ)/gzclose.o: $(SRC)/gzclose.c
	$(CC) $(CFLAGS) -c $(SRC)/gzclose.c -I$(INCLUDE) \
	-o $(OBJ)/gzclose.o

$(OBJ)/gzlib.o: $(SRC)/gzlib.c
	$(CC) $(CFLAGS) -c $(SRC)/gzlib.c -I$(INCLUDE) \
	-o $(OBJ)/gzlib.o

$(OBJ)/gzread.o: $(SRC)/gzread.c
	$(CC) $(CFLAGS) -c $(SRC)/gzread.c -I$(INCLUDE) \
	-o $(OBJ)/gzread.o

$(OBJ)/gzwrite.o: $(SRC)/gzwrite.c
	$(CC) $(CFLAGS) -c $(SRC)/gzwrite.c -I$(INCLUDE) \
	-o $(OBJ)/gzwrite.o

$(OBJ)/infback.o: $(SRC)/infback.c
	$(CC) $(CFLAGS) -c $(SRC)/infback.c -I$(INCLUDE) \
	-o $(OBJ)/infback.o

$(OBJ)/inflate.o: $(SRC)/inflate.c
	$(CC) $(CFLAGS) -c $(SRC)/inflate.c -I$(INCLUDE) \
	-o $(OBJ)/inflate.o

$(OBJ)/inffast.o: $(SRC)/inffast.c
	$(CC) $(CFLAGS) -c $(SRC)/inffast.c -I$(INCLUDE) \
	-o $(OBJ)/inffast.o

$(OBJ)/inftrees.o: $(SRC)/inftrees.c
	$(CC) $(CFLAGS) -c $(SRC)/inftrees.c -I$(INCLUDE) \
	-o $(OBJ)/inftrees.o

$(OBJ)/trees.o: $(SRC)/trees.c
	$(CC) $(CFLAGS) -c $(SRC)/trees.c -I$(INCLUDE) \
	-o $(OBJ)/trees.o

$(OBJ)/uncompr.o: $(SRC)/uncompr.c
	$(CC) $(CFLAGS) -c $(SRC)/uncompr.c -I$(INCLUDE) \
	-o $(OBJ)/uncompr.o

$(OBJ)/zutil.o: $(SRC)/zutil.c
	$(CC) $(CFLAGS) -c $(SRC)/zutil.c -I$(INCLUDE) \
	-o $(OBJ)/zutil.o

clean:
	rm $(LIB)/zlib*.a; rm $(OBJ)/*.o;