# makefile for libpng using gcc (generic, static library)

# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h


# Location of the zlib library and include files
ZLIBINC = ../zlib/include
ZLIBLIB = ../zlib/lib



LIB=./lib
INCLUDE = ./include
SRC=./source
OBJ=./obj
BIN=./bin


#FLAGS= -g  -O0 -Wall -D _DEBUG -Wno-unused-result -fPIC -std=gnu99 -pedantic
FLAGS=  -O3 -Wall -Wno-unused-result -fPIC  -pedantic -Wno-write-strings -fopenmp -fsanitize=address
#-std=gnu99



# Compiler, linker, lib and other tools
CC = gcc
LD = $(CC)
AR_RC = ar rcs
RANLIB = ranlib
CP = cp
RM_F = rm -f

WARNMORE = -Wwrite-strings -Wpointer-arith -Wshadow \
	-Wmissing-declarations -Wtraditional -Wcast-align \
	-Wstrict-prototypes -Wmissing-prototypes # -Wconversion
CPPFLAGS = -I$(ZLIBINC) # -DPNG_DEBUG=5
CFLAGS = -I$(ZLIBINC) -W -Wall -O2 # $(WARNMORE) -g
LDFLAGS =
LIBS = -lz -lm

# Pre-built configuration
# See scripts/pnglibconf.mak for more options
#PNGLIBCONF_H_PREBUILT = pnglibconf.h.prebuilt


libMpng: $(LIB)/libMpng.a
	echo "libMpng.a built..."

$(LIB)/libMpng.a: \
$(OBJ)/png.o \
$(OBJ)/pngerror.o \
$(OBJ)/pngget.o \
$(OBJ)/pngmem.o \
$(OBJ)/pngpread.o \
$(OBJ)/pngread.o \
$(OBJ)/pngrio.o \
$(OBJ)/pngrtran.o \
$(OBJ)/pngrutil.o \
$(OBJ)/pngset.o \
$(OBJ)/pngtrans.o \
$(OBJ)/pngwio.o \
$(OBJ)/pngwrite.o \
$(OBJ)/pngwtran.o \
$(OBJ)/pngwutil.o \

	ar csr $(LIB)/libMpng.a \
$(OBJ)/png.o \
$(OBJ)/pngerror.o \
$(OBJ)/pngget.o \
$(OBJ)/pngmem.o \
$(OBJ)/pngpread.o \
$(OBJ)/pngread.o \
$(OBJ)/pngrio.o \
$(OBJ)/pngrtran.o \
$(OBJ)/pngrutil.o \
$(OBJ)/pngset.o \
$(OBJ)/pngtrans.o \
$(OBJ)/pngwio.o \
$(OBJ)/pngwrite.o \
$(OBJ)/pngwtran.o \
$(OBJ)/pngwutil.o \


$(OBJ)/png.o: zlib $(SRC)/png.c
	$(CC) $(CFLAGS) -c $(SRC)/png.c -I$(INCLUDE) \
	-o $(OBJ)/png.o

$(OBJ)/pngerror.o: $(SRC)/pngerror.c
	$(CC) $(CFLAGS) -c $(SRC)/pngerror.c -I$(INCLUDE) \
	-o $(OBJ)/pngerror.o

$(OBJ)/pngget.o: $(SRC)/pngget.c
	$(CC) $(CFLAGS) -c $(SRC)/pngget.c -I$(INCLUDE) \
	-o $(OBJ)/pngget.o

$(OBJ)/pngmem.o: $(SRC)/pngmem.c
	$(CC) $(CFLAGS) -c $(SRC)/pngmem.c -I$(INCLUDE) \
	-o $(OBJ)/pngmem.o

$(OBJ)/pngpread.o: $(SRC)/pngpread.c
	$(CC) $(CFLAGS) -c $(SRC)/pngpread.c -I$(INCLUDE) \
	-o $(OBJ)/pngpread.o

$(OBJ)/pngread.o: $(SRC)/pngread.c
	$(CC) $(CFLAGS) -c $(SRC)/pngread.c -I$(INCLUDE) \
	-o $(OBJ)/pngread.o

$(OBJ)/pngrio.o: $(SRC)/pngrio.c
	$(CC) $(CFLAGS) -c $(SRC)/pngrio.c -I$(INCLUDE) \
	-o $(OBJ)/pngrio.o

$(OBJ)/pngrtran.o: $(SRC)/pngrtran.c
	$(CC) $(CFLAGS) -c $(SRC)/pngrtran.c -I$(INCLUDE) \
	-o $(OBJ)/pngrtran.o

$(OBJ)/pngrutil.o: $(SRC)/pngrutil.c
	$(CC) $(CFLAGS) -c $(SRC)/pngrutil.c -I$(INCLUDE) \
	-o $(OBJ)/pngrutil.o

$(OBJ)/pngset.o: $(SRC)/pngset.c
	$(CC) $(CFLAGS) -c $(SRC)/pngset.c -I$(INCLUDE) \
	-o $(OBJ)/pngset.o

$(OBJ)/pngtrans.o: $(SRC)/pngtrans.c
	$(CC) $(CFLAGS) -c $(SRC)/pngtrans.c -I$(INCLUDE) \
	-o $(OBJ)/pngtrans.o

$(OBJ)/pngwio.o: $(SRC)/pngwio.c
	$(CC) $(CFLAGS) -c $(SRC)/pngwio.c -I$(INCLUDE) \
	-o $(OBJ)/pngwio.o

$(OBJ)/pngwrite.o: $(SRC)/pngwrite.c
	$(CC) $(CFLAGS) -c $(SRC)/pngwrite.c -I$(INCLUDE) \
	-o $(OBJ)/pngwrite.o

$(OBJ)/pngwtran.o: $(SRC)/pngwtran.c
	$(CC) $(CFLAGS) -c $(SRC)/pngwtran.c -I$(INCLUDE) \
	-o $(OBJ)/pngwtran.o

$(OBJ)/pngwutil.o: $(SRC)/pngwutil.c
	$(CC) $(CFLAGS) -c $(SRC)/pngwutil.c -I$(INCLUDE) \
	-o $(OBJ)/pngwutil.o

zlib:
	cd ../zlib; $(MAKE) ; cd -\

clean:
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o;