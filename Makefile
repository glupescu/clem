CC=gcc
CFLAGS=-g -I$(AMDAPPSDKROOT)/include -L$($AMDAPPSDKROOT)/lib/x86_64/
LIBS=-lOpenCL

all: build

build: clem_compiler clem_app_add

clem_app_add: clem_app_add.c clem_helper.o cl_add.h
	$(CC) $(CFLAGS) clem_app_add.c clem_helper.o $(LIBS) -o clem_app_add
cl_add.h:	cl_add.cl clem_compiler
	./clem_compiler cl_add.cl cl_add.h
	
clem_compiler: clem_compiler.c clem_helper.o
	$(CC) $(CFLAGS) clem_compiler.c clem_helper.o $(LIBS) -o clem_compiler

clem_helper.o:	clem_helper.c clem_helper.h
	$(CC) $(CFLAGS) -c clem_helper.c

clean:
		rm -f *.o clem_compiler clem_app_add
