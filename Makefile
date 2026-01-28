#-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
#
# Makefile - \library\libezutil
#
# Copyright (C) 2011 ezlibs.com, All Rights Reserved.
#
# $Id: Makefile 1 2011-10-22 08:14:10 WuJunjie Exp $
#
# explain
#      for joy.woo only
#      use build/Makefile pls
#
# Update:
#    2011-10-22 08:14 WuJunjie Create
#
#-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
PUB_MAKE_FILE_PREFIX =	../lazaru/Makefile.Defines
include		$(PUB_MAKE_FILE_PREFIX)/Makefile.Batch.Build

# define default platform
ifndef PLATFORM
PLATFORM = linux
endif
#PLATFORM = linux-mipsel-openwrt-linux
VERSION="1.0.0"

# CPP, CFLAGS, LIBS
include		$(PUB_MAKE_FILE_PREFIX)/Makefile.Defines.$(PLATFORM)
#for LIB_OBJS
PREFIX_BASEDIR_SRC=./src
include $(PREFIX_BASEDIR_SRC)/Makefile.SrcLists

PREFIX =	$(HOME)/libs
INS_DIR_LIB = $(PREFIX)/lib
INS_DIR_INC = $(PREFIX)/include/ezutil

LIB_TARGET = libezutil-$(PLATFORM).a

# uncomment these lines if the library should be in its own namespace
CFLAGS +=      -std=gnu99
#CFLAGS +=      -DEZUTIL_NAMESPACE=ezutil
#CFLAGS +=      -DEZUTIL_NAMESPACE_STR='"ezutil"'

# Enable insane amounts of debug output to stdout/stderr:
#CFLAGS +=      -D_DEBUG
#CFLAGS +=      -DENABLE_DEBUG
#CFLAGS +=      -DENABLE_TRACE=1
#CFLAGS +=      -I/usr/local/include

#CFLAGS +=      -DHAVE_ICONV

TARGET	= $(LIB_TARGET)
all: $(TARGET)

$(LIB_TARGET): $(LIB_OBJS)
	#mkdir -p $(LIBDIR)
	$(RM) $@;
	$(AR) $(AR_FLAG) $@ $^
	$(RANLIB) $@

install:	all 
		@mkdir -p $(INS_DIR_LIB)
		cp $(LIB_TARGET) $(INS_DIR_LIB)
		@mkdir -p $(INS_DIR_INC)
		cp ./src/*.h $(INS_DIR_INC)
.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(TARGET) $(LIB_OBJS) *.a *.cfbk ./lib/* ./include/*
	find . -name "*.d" |xargs rm -f
	find . -name "*.cfbk" |xargs rm -f
	make -C tutorial_libezutil clean
	make -C test clean
