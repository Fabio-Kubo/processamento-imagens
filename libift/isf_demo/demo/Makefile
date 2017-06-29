IFTFLAGS    = -L../lib -lift -llapack -lblas -lpng -lz -fopenmp -lm -I ../include

all: $@.cpp

$@.cpp: $@.cpp
	$(CXX) $(FLAGS) $@.cpp -o ../bin/$@ $(IFTFLAGS)

clean:
	rm ../bin/*

