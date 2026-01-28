#-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
#
# Makefile - Z:\svn\daily\src\labs\socket_model_linux\src\
#
# Copyright (C) 2016 ezlibs.com, All Rights Reserved.
#
# $Id: Makefile 1 2016-07-13 02:34:43 WuJunjie Exp $
#
# explain
#      general Makefile
#
# Update:
#    2016-07-13 02:34 WuJunjie Create
#
#-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
PLATFORM = linux
#PLATFORM = linux-x86-32
#PLATFORM = linux-x86-64
#PLATFORM = linux-hi3515
#PLATFORM = linux-arm
#PLATFORM = osx
#PLATFORM = linux-mips-openwrt
#PLATFORM = linux-mips-mipsl-openwrt
#PLATFORM = linux-mipsel-openwrt-linux

include         ../../lazaru/Makefile.Defines/Makefile.Defines.$(PLATFORM)

CFLAGS += -Wall 
CFLAGS +=  -g# -std=c99
#
#CFLAGS += -DHAVE_ICONV

#CFLAGS += -I$(HOME)/libs/include/ezutil
CFLAGS += -I$(HOME)/libs/include
CFLAGS += -I$(HOME)/libs/include/ezutil
LIBS   += -L$(HOME)/libs/lib -lezutil-$(PLATFORM)

#ifeq ($(PLATFORM),macosx)
#else
	#LIBS += -lrt
#endif

LIBS += -lm -lpthread
#LIBS += -liconv

C_SRC = $(wildcard *.c)
C_OBJ = $(patsubst %c, %o, $(C_SRC))
CPP_SRC = $(wildcard *.cpp)
CPP_OBJ = $(patsubst %cpp, %o, $(CPP_SRC))

.PHONY:all clean

all:$(CPP_OBJ) $(C_OBJ)

.c.o:
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) 
.cpp.o:
	$(CPP) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm *~ *.o -f *.cfbk *.d test_log.log*

chk:all
	./random_gen.o 18
