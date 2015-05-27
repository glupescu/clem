CC=gcc
CFLAGS=-g -I$(AMDAPPSDKROOT)/include -L$($AMDAPPSDKROOT)/lib/x86_64/
LIBS=-lOpenCL

all: build

build: clem_app_add

###############################################
###  APPS        ##############################

clem_app_add: clem_app_add.c clem_helper.o
	$(CC) $(CFLAGS) clem_app_add.c clem_helper.o $(LIBS) -o clem_app_add
	
###############################################
###  COMMON      ##############################

clem_helper.o:	clem_helper.c clem_helper.h
	$(CC) $(CFLAGS) -c clem_helper.c

clean:
		rm -f *.o clem_app_add kernel_*.h
