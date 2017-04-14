LIB=./lib
INCLUDE=./include
SRC=./src
OBJ=./obj
BIN=./bin
FLLIBPNG = -L./externals/libpng/lib/ -lMpng
FLLIBPNGINCLUDES = -I./externals/libpng/include


#FLLIJPEG = -L./externals/libjpeg/lib/ -ljpeg
FLLIBJPEGINCLUDES = -I./externals/libjpeg/include

#FLAGS= -g  -O0 -Wall -D _DEBUG -Wno-unused-result -fPIC -std=gnu99 -pedantic
FLAGS=  -O3 -Wall -Wno-unused-result -fPIC  -pedantic -Wno-write-strings -fopenmp -fsanitize=address
FLAGSIM =  $(FLLIBPNGINCLUDES) $(FLLIBJPEGINCLUDES) $(FLAGS)
#-std=gnu99

libFL: miau $(LIB)/libFL.a
	echo "libFL.a built..."

miau:
	echo $(FLLIBJPEGINCLUDES)

$(LIB)/libFL.a: \
$(OBJ)/common.o \
$(OBJ)/file.o \
$(OBJ)/adjacencyRelation.o \
$(OBJ)/featureVector.o \
$(OBJ)/gif.o \
$(OBJ)/kernel.o \
$(OBJ)/image.o \
$(OBJ)/histogram.o \
$(OBJ)/morphology.o \
$(OBJ)/filtering.o \
$(OBJ)/bagOfVisualWords.o \

	ar csr $(LIB)/libFL.a \
$(OBJ)/common.o \
$(OBJ)/file.o \
$(OBJ)/adjacencyRelation.o \
$(OBJ)/featureVector.o \
$(OBJ)/gif.o \
$(OBJ)/kernel.o \
$(OBJ)/image.o \
$(OBJ)/histogram.o \
$(OBJ)/morphology.o \
$(OBJ)/filtering.o \
$(OBJ)/bagOfVisualWords.o \
./externals/zlib/obj/*.o \
./externals/libpng/obj/*.o \
./externals/libjpeg/obj/*.o \

$(OBJ)/common.o:   $(SRC)/common.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/common.cpp -I$(INCLUDE) \
	-o $(OBJ)/common.o

$(OBJ)/file.o: $(SRC)/file.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/file.cpp -I$(INCLUDE) \
	-o $(OBJ)/file.o

$(OBJ)/adjacencyRelation.o: $(SRC)/adjacencyRelation.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/adjacencyRelation.cpp -I$(INCLUDE) \
	-o $(OBJ)/adjacencyRelation.o

$(OBJ)/featureVector.o: $(SRC)/featureVector.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/featureVector.cpp -I$(INCLUDE) \
	-o $(OBJ)/featureVector.o

$(OBJ)/gif.o: $(SRC)/gif.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/gif.cpp -I$(INCLUDE) \
	-o $(OBJ)/gif.o

$(OBJ)/kernel.o: $(SRC)/kernel.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/kernel.cpp -I$(INCLUDE) \
	-o $(OBJ)/kernel.o

$(OBJ)/image.o:  lpng ljpeg $(SRC)/image.cpp
	$(CC) $(FLAGSIM)  -c $(SRC)/image.cpp -I$(INCLUDE)  \
	-o $(OBJ)/image.o

$(OBJ)/histogram.o: $(SRC)/histogram.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/histogram.cpp -I$(INCLUDE) \
	-o $(OBJ)/histogram.o


$(OBJ)/morphology.o: $(SRC)/morphology.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/morphology.cpp -I$(INCLUDE) \
	-o $(OBJ)/morphology.o

$(OBJ)/filtering.o: $(SRC)/filtering.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/filtering.cpp -I$(INCLUDE) \
	-o $(OBJ)/filtering.o

$(OBJ)/bagOfVisualWords.o: $(SRC)/bagOfVisualWords.cpp
	$(CC) $(FLAGSIM) -c $(SRC)/bagOfVisualWords.cpp -I$(INCLUDE) \
	-o $(OBJ)/bagOfVisualWords.o

lpng:
	cd externals/libpng; $(MAKE) ; cd -\

ljpeg:
	cd externals/libjpeg; $(MAKE) ; cd -\

clean: 
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o

cleanAll:
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o;cd externals/libpng; $(MAKE) clean; \
	cd ..; cd libjpeg;  $(MAKE) clean; cd ..;cd zlib; $(MAKE) clean; cd -\
