BIN=.
CC = g++

#FLAGS= -g -O0 -Wall -Wno-unused-result -std=gnu99 -pedantic
FLAGS= -O3  -Wall -Wno-unused-result  -pedantic -fopenmp -Wno-write-strings -fsanitize=address
#-std=gnu99

FLLIBPNGINCLUDES = -I../externals/libpng/include
FLLIBJPEGINCLUDES = -I../externals/libjpeg/include

FL = -L../lib -lFL
LINKER =  $(FL)

FLFLAGS  = -I../include
FLAGSIM =  $(FLLIBPNGINCLUDES) $(FLLIBJPEGINCLUDES) -I../include $(FLAGS)


libFL:
	cd ../; $(MAKE) ; cd -\


$@.cpp: libFL $@.cpp
	make
	$(CC) $(FLAGSIM) $@.cpp -o $(BIN)/$@  $(LINKER); \

#Removing everything without an extension
clean:	
	find . -type f -executable -exec rm '{}' \;

cleanImages:
	rm *.ppm *.pgm *.gif *.png *.jpg *.jpeg

cleanText:
	rm *.txt
