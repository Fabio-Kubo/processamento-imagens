# The name of your C compiler:
CC= cc
CFLAGS= -O3
LIB=./lib
INCLUDE = ./include
SRC=./source
OBJ=./obj
BIN=./bin

# Put here the object file name for the correct system-dependent memory
# manager file.  For Unix this is usually jmemnobs.o, but you may want
# to use jmemansi.o or jmemname.o if you have limited swap space.
SYSDEPMEM= jmemnobs

libjpeg: $(LIB)/libjpeg.a
	echo "libjpeg.a built..."

$(LIB)/libjpeg.a: \
$(OBJ)/jcapimin.o \
$(OBJ)/jcapistd.o \
$(OBJ)/jcarith.o \
$(OBJ)/jctrans.o \
$(OBJ)/jcparam.o \
$(OBJ)/jdatadst.o \
$(OBJ)/jcinit.o \
$(OBJ)/jcmaster.o \
$(OBJ)/jcmarker.o \
$(OBJ)/jcmainct.o \
$(OBJ)/jcprepct.o \
$(OBJ)/jccoefct.o \
$(OBJ)/jccolor.o \
$(OBJ)/jcsample.o \
$(OBJ)/jchuff.o \
$(OBJ)/jcdctmgr.o \
$(OBJ)/jfdctfst.o \
$(OBJ)/jfdctflt.o \
$(OBJ)/jfdctint.o \
$(OBJ)/jdapimin.o \
$(OBJ)/jdapistd.o \
$(OBJ)/jdarith.o \
$(OBJ)/jdtrans.o \
$(OBJ)/jdatasrc.o \
$(OBJ)/jdmaster.o \
$(OBJ)/jdinput.o \
$(OBJ)/jdmarker.o \
$(OBJ)/jdhuff.o \
$(OBJ)/jdmainct.o \
$(OBJ)/jdcoefct.o \
$(OBJ)/jdpostct.o \
$(OBJ)/jddctmgr.o \
$(OBJ)/jidctfst.o \
$(OBJ)/jidctflt.o \
$(OBJ)/jidctint.o \
$(OBJ)/jdsample.o \
$(OBJ)/jdcolor.o \
$(OBJ)/jquant1.o \
$(OBJ)/jquant2.o \
$(OBJ)/jdmerge.o \
$(OBJ)/jaricom.o \
$(OBJ)/jcomapi.o \
$(OBJ)/jutils.o \
$(OBJ)/jerror.o \
$(OBJ)/jmemmgr.o \
$(OBJ)/$(SYSDEPMEM).o \


	ar csr $(LIB)/libjpeg.a \
$(OBJ)/jcapimin.o \
$(OBJ)/jcapistd.o \
$(OBJ)/jcarith.o \
$(OBJ)/jctrans.o \
$(OBJ)/jcparam.o \
$(OBJ)/jdatadst.o \
$(OBJ)/jcinit.o \
$(OBJ)/jcmaster.o \
$(OBJ)/jcmarker.o \
$(OBJ)/jcmainct.o \
$(OBJ)/jcprepct.o \
$(OBJ)/jccoefct.o \
$(OBJ)/jccolor.o \
$(OBJ)/jcsample.o \
$(OBJ)/jchuff.o \
$(OBJ)/jcdctmgr.o \
$(OBJ)/jfdctfst.o \
$(OBJ)/jfdctflt.o \
$(OBJ)/jfdctint.o \
$(OBJ)/jdapimin.o \
$(OBJ)/jdapistd.o \
$(OBJ)/jdarith.o \
$(OBJ)/jdtrans.o \
$(OBJ)/jdatasrc.o \
$(OBJ)/jdmaster.o \
$(OBJ)/jdinput.o \
$(OBJ)/jdmarker.o \
$(OBJ)/jdhuff.o \
$(OBJ)/jdmainct.o \
$(OBJ)/jdcoefct.o \
$(OBJ)/jdpostct.o \
$(OBJ)/jddctmgr.o \
$(OBJ)/jidctfst.o \
$(OBJ)/jidctflt.o \
$(OBJ)/jidctint.o \
$(OBJ)/jdsample.o \
$(OBJ)/jdcolor.o \
$(OBJ)/jquant1.o \
$(OBJ)/jquant2.o \
$(OBJ)/jdmerge.o \
$(OBJ)/jaricom.o \
$(OBJ)/jcomapi.o \
$(OBJ)/jutils.o \
$(OBJ)/jerror.o \
$(OBJ)/jmemmgr.o \
$(OBJ)/$(SYSDEPMEM).o \

# compression library object files
$(OBJ)/jcapimin.o: $(SRC)/jcapimin.c
	$(CC) $(CFLAGS) -c $(SRC)/jcapimin.c -I$(INCLUDE) \
	-o $(OBJ)/jcapimin.o

$(OBJ)/jcapistd.o: $(SRC)/jcapistd.c
	$(CC) $(CFLAGS) -c $(SRC)/jcapistd.c -I$(INCLUDE) \
	-o $(OBJ)/jcapistd.o

$(OBJ)/jcarith.o: $(SRC)/jcarith.c
	$(CC) $(CFLAGS) -c $(SRC)/jcarith.c -I$(INCLUDE) \
	-o $(OBJ)/jcarith.o

$(OBJ)/jctrans.o: $(SRC)/jctrans.c
	$(CC) $(CFLAGS) -c $(SRC)/jctrans.c -I$(INCLUDE) \
	-o $(OBJ)/jctrans.o

$(OBJ)/jcparam.o: $(SRC)/jcparam.c
	$(CC) $(CFLAGS) -c $(SRC)/jcparam.c -I$(INCLUDE) \
	-o $(OBJ)/jcparam.o

$(OBJ)/jdatadst.o: $(SRC)/jdatadst.c
	$(CC) $(CFLAGS) -c $(SRC)/jdatadst.c -I$(INCLUDE) \
	-o $(OBJ)/jdatadst.o

$(OBJ)/jcinit.o: $(SRC)/jcinit.c
	$(CC) $(CFLAGS) -c $(SRC)/jcinit.c -I$(INCLUDE) \
	-o $(OBJ)/jcinit.o

$(OBJ)/jcmaster.o: $(SRC)/jcmaster.c
	$(CC) $(CFLAGS) -c $(SRC)/jcmaster.c -I$(INCLUDE) \
	-o $(OBJ)/jcmaster.o

$(OBJ)/jcmarker.o: $(SRC)/jcmarker.c
	$(CC) $(CFLAGS) -c $(SRC)/jcmarker.c -I$(INCLUDE) \
	-o $(OBJ)/jcmarker.o

$(OBJ)/jcmainct.o: $(SRC)/jcmainct.c
	$(CC) $(CFLAGS) -c $(SRC)/jcmainct.c -I$(INCLUDE) \
	-o $(OBJ)/jcmainct.o

$(OBJ)/jcprepct.o: $(SRC)/jcprepct.c
	$(CC) $(CFLAGS) -c $(SRC)/jcprepct.c -I$(INCLUDE) \
	-o $(OBJ)/jcprepct.o

$(OBJ)/jccoefct.o: $(SRC)/jccoefct.c
	$(CC) $(CFLAGS) -c $(SRC)/jccoefct.c -I$(INCLUDE) \
	-o $(OBJ)/jccoefct.o

$(OBJ)/jccolor.o: $(SRC)/jccolor.c
	$(CC) $(CFLAGS) -c $(SRC)/jccolor.c -I$(INCLUDE) \
	-o $(OBJ)/jccolor.o

$(OBJ)/jcsample.o: $(SRC)/jcsample.c
	$(CC) $(CFLAGS) -c $(SRC)/jcsample.c -I$(INCLUDE) \
	-o $(OBJ)/jcsample.o

$(OBJ)/jchuff.o: $(SRC)/jchuff.c
	$(CC) $(CFLAGS) -c $(SRC)/jchuff.c -I$(INCLUDE) \
	-o $(OBJ)/jchuff.o

$(OBJ)/jcdctmgr.o: $(SRC)/jcdctmgr.c
	$(CC) $(CFLAGS) -c $(SRC)/jcdctmgr.c -I$(INCLUDE) \
	-o $(OBJ)/jcdctmgr.o

$(OBJ)/jfdctfst.o: $(SRC)/jfdctfst.c
	$(CC) $(CFLAGS) -c $(SRC)/jfdctfst.c -I$(INCLUDE) \
	-o $(OBJ)/jfdctfst.o

$(OBJ)/jfdctflt.o: $(SRC)/jfdctflt.c
	$(CC) $(CFLAGS) -c $(SRC)/jfdctflt.c -I$(INCLUDE) \
	-o $(OBJ)/jfdctflt.o

$(OBJ)/jfdctint.o: $(SRC)/jfdctint.c
	$(CC) $(CFLAGS) -c $(SRC)/jfdctint.c -I$(INCLUDE) \
	-o $(OBJ)/jfdctint.o


# decompression library object files
$(OBJ)/jdapimin.o: $(SRC)/jdapimin.c
	$(CC) $(CFLAGS) -c $(SRC)/jdapimin.c -I$(INCLUDE) \
	-o $(OBJ)/jdapimin.o

$(OBJ)/jdapistd.o: $(SRC)/jdapistd.c
	$(CC) $(CFLAGS) -c $(SRC)/jdapistd.c -I$(INCLUDE) \
	-o $(OBJ)/jdapistd.o

$(OBJ)/jdarith.o: $(SRC)/jdarith.c
	$(CC) $(CFLAGS) -c $(SRC)/jdarith.c -I$(INCLUDE) \
	-o $(OBJ)/jdarith.o

$(OBJ)/jdtrans.o: $(SRC)/jdtrans.c
	$(CC) $(CFLAGS) -c $(SRC)/jdtrans.c -I$(INCLUDE) \
	-o $(OBJ)/jdtrans.o

$(OBJ)/jdatasrc.o: $(SRC)/jdatasrc.c
	$(CC) $(CFLAGS) -c $(SRC)/jdatasrc.c -I$(INCLUDE) \
	-o $(OBJ)/jdatasrc.o

$(OBJ)/jdmaster.o: $(SRC)/jdmaster.c
	$(CC) $(CFLAGS) -c $(SRC)/jdmaster.c -I$(INCLUDE) \
	-o $(OBJ)/jdmaster.o

$(OBJ)/jdinput.o: $(SRC)/jdinput.c
	$(CC) $(CFLAGS) -c $(SRC)/jdinput.c -I$(INCLUDE) \
	-o $(OBJ)/jdinput.o

$(OBJ)/jdmarker.o: $(SRC)/jdmarker.c
	$(CC) $(CFLAGS) -c $(SRC)/jdmarker.c -I$(INCLUDE) \
	-o $(OBJ)/jdmarker.o

$(OBJ)/jdhuff.o: $(SRC)/jdhuff.c
	$(CC) $(CFLAGS) -c $(SRC)/jdhuff.c -I$(INCLUDE) \
	-o $(OBJ)/jdhuff.o

$(OBJ)/jdmainct.o: $(SRC)/jdmainct.c
	$(CC) $(CFLAGS) -c $(SRC)/jdmainct.c -I$(INCLUDE) \
	-o $(OBJ)/jdmainct.o

$(OBJ)/jdcoefct.o: $(SRC)/jdcoefct.c
	$(CC) $(CFLAGS) -c $(SRC)/jdcoefct.c -I$(INCLUDE) \
	-o $(OBJ)/jdcoefct.o

$(OBJ)/jdpostct.o: $(SRC)/jdpostct.c
	$(CC) $(CFLAGS) -c $(SRC)/jdpostct.c -I$(INCLUDE) \
	-o $(OBJ)/jdpostct.o

$(OBJ)/jddctmgr.o: $(SRC)/jddctmgr.c
	$(CC) $(CFLAGS) -c $(SRC)/jddctmgr.c -I$(INCLUDE) \
	-o $(OBJ)/jddctmgr.o

$(OBJ)/jidctfst.o: $(SRC)/jidctfst.c
	$(CC) $(CFLAGS) -c $(SRC)/jidctfst.c -I$(INCLUDE) \
	-o $(OBJ)/jidctfst.o

$(OBJ)/jidctflt.o: $(SRC)/jidctflt.c
	$(CC) $(CFLAGS) -c $(SRC)/jidctflt.c -I$(INCLUDE) \
	-o $(OBJ)/jidctflt.o

$(OBJ)/jidctint.o: $(SRC)/jidctint.c
	$(CC) $(CFLAGS) -c $(SRC)/jidctint.c -I$(INCLUDE) \
	-o $(OBJ)/jidctint.o

$(OBJ)/jdsample.o: $(SRC)/jdsample.c
	$(CC) $(CFLAGS) -c $(SRC)/jdsample.c -I$(INCLUDE) \
	-o $(OBJ)/jdsample.o

$(OBJ)/jdcolor.o: $(SRC)/jdcolor.c
	$(CC) $(CFLAGS) -c $(SRC)/jdcolor.c -I$(INCLUDE) \
	-o $(OBJ)/jdcolor.o

$(OBJ)/jquant1.o: $(SRC)/jquant1.c
	$(CC) $(CFLAGS) -c $(SRC)/jquant1.c -I$(INCLUDE) \
	-o $(OBJ)/jquant1.o

$(OBJ)/jquant2.o: $(SRC)/jquant2.c
	$(CC) $(CFLAGS) -c $(SRC)/jquant2.c -I$(INCLUDE) \
	-o $(OBJ)/jquant2.o

$(OBJ)/jdmerge.o: $(SRC)/jdmerge.c
	$(CC) $(CFLAGS) -c $(SRC)/jdmerge.c -I$(INCLUDE) \
	-o $(OBJ)/jdmerge.o

# library object files common to compression and decompression
$(OBJ)/jaricom.o: $(SRC)/jaricom.c
	$(CC) $(CFLAGS) -c $(SRC)/jaricom.c -I$(INCLUDE) \
	-o $(OBJ)/jaricom.o

$(OBJ)/jcomapi.o: $(SRC)/jcomapi.c
	$(CC) $(CFLAGS) -c $(SRC)/jcomapi.c -I$(INCLUDE) \
	-o $(OBJ)/jcomapi.o

$(OBJ)/jutils.o: $(SRC)/jutils.c
	$(CC) $(CFLAGS) -c $(SRC)/jutils.c -I$(INCLUDE) \
	-o $(OBJ)/jutils.o

$(OBJ)/jerror.o: $(SRC)/jerror.c
	$(CC) $(CFLAGS) -c $(SRC)/jerror.c -I$(INCLUDE) \
	-o $(OBJ)/jerror.o

$(OBJ)/jmemmgr.o: $(SRC)/jmemmgr.c
	$(CC) $(CFLAGS) -c $(SRC)/jmemmgr.c -I$(INCLUDE) \
	-o $(OBJ)/jmemmgr.o

$(OBJ)/$(SYSDEPMEM).o: $(SRC)/$(SYSDEPMEM).c
	$(CC) $(CFLAGS) -c $(SRC)/$(SYSDEPMEM).c -I$(INCLUDE) \
	-o $(OBJ)/$(SYSDEPMEM).o

clean:
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o;