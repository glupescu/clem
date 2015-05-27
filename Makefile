CC=gcc
CFLAGS=-g -I$(AMDAPPSDKROOT)/include -L$($AMDAPPSDKROOT)/lib/x86_64/
LIBS=-lOpenCL

all: build

build: clem_compiler clem_app_add clem_app_sha3

###############################################
###  APPS        ##############################

clem_app_add: clem_app_add.c clem_helper.o kernel_add.h
	$(CC) $(CFLAGS) clem_app_add.c clem_helper.o $(LIBS) -o clem_app_add
kernel_add.h:	kernel_add.cl clem_compiler
	./clem_compiler kernel_add.cl kernel_add.h
	
clem_app_sha3: clem_app_sha3.c clem_helper.o kernel_sha3.h
	$(CC) $(CFLAGS) clem_app_sha3.c clem_helper.o $(LIBS) -o clem_app_sha3
kernel_sha3.h:	kernel_sha3.cl clem_compiler
	./clem_compiler kernel_sha3.cl kernel_sha3.h
	
###############################################
###  COMPILER    ##############################
	
clem_compiler: clem_compiler.c clem_helper.o
	$(CC) $(CFLAGS) clem_compiler.c clem_helper.o $(LIBS) -o clem_compiler
	
###############################################
###  COMMON      ##############################

clem_helper.o:	clem_helper.c clem_helper.h
	$(CC) $(CFLAGS) -c clem_helper.c

clean:
		rm -f *.o clem_compiler clem_app_add kernel_*.h clem_app_sha3
