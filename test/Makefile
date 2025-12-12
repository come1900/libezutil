#-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
#
# Makefile - Z:\Dropbox\stock\trunk\code\library\linux_device\libplatform\test\
#
# Copyright (C) 2013 ezlibs.com, All Rights Reserved.
#
# $Id: Makefile 1 2013-06-13 10:05:26 WuJunjie Exp $
#
# explain
#      exp
#
# Update:
#    2013-06-13 10:05 WuJunjie Create
#
#-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

PLATFORM = linux
#PLATFORM = linux-x86-32
#PLATFORM = linux-x86-64
#PLATFORM = linux-hi3515
#PLATFORM = linux-arm
#PLATFORM = linux-x86-64-centos5
#PLATFORM = linux-am335x
#PLATFORM = linux-mips-mipsl-openwrt

VERSION =	1.0
DIFF_VERSION =	0.9

DIST =		exef

CFLAGS =	-Wall -g -O2 -MD
CFLAGS += -D_DEBUG #debugme

include         ../../lazaru/Makefile.Defines/Makefile.Defines.$(PLATFORM)

CFLAGS += -DLINUX
#CFLAGS	+= -I../include
CFLAGS += -Wall -O2 -Os
CFLAGS += -D_REENTRANT -msoft-float

#LDFLAGS += -L../lib/\
#	-lezthread-linux-x86-32
EZLIBS_BASEDIR_LIBS=$(HOME)/libs
CFLAGS += -I${EZLIBS_BASEDIR_LIBS}/include
#LIBS += -L${EZLIBS_BASEDIR_LIBS}/lib -lezutil-$(PLATFORM)

LDFLAGS += $(LIBS) -lpthread

AFLAGS += -r

#LIB_OBJS = ../src/api_exa.o
PREFIX_BASEDIR_SRC=../src
#include $(PREFIX_BASEDIR_SRC)/Makefile.SrcLists
LIB_OBJS =      \
                $(PREFIX_BASEDIR_SRC)/base64.o  \
                $(PREFIX_BASEDIR_SRC)/crc16.o  \
                $(PREFIX_BASEDIR_SRC)/crc32.o  \
		$(PREFIX_BASEDIR_SRC)/aes.o \
                $(PREFIX_BASEDIR_SRC)/ez_bit.o  \
                $(PREFIX_BASEDIR_SRC)/ez_fs.o  \
                $(PREFIX_BASEDIR_SRC)/ez_log.o  \
                $(PREFIX_BASEDIR_SRC)/ez_system_api.o  \
                $(PREFIX_BASEDIR_SRC)/http_parser.o  \
                $(PREFIX_BASEDIR_SRC)/md5.o  \
                $(PREFIX_BASEDIR_SRC)/sha1.o  \
                $(PREFIX_BASEDIR_SRC)/str_opr.o

CFLAGS += -I$(PREFIX_BASEDIR_SRC)

TARGET = test_ezutil-$(PLATFORM) test_sha1-$(PLATFORM)
TARGET += test_ez_md5_calc_ascii_digit-$(PLATFORM)
TARGET += test_sha_str-$(PLATFORM)
TARGET += test_ezuuid-$(PLATFORM)
TARGET += test_ez_fs-$(PLATFORM)
TARGET += test_aes-$(PLATFORM)

#test_api_exa-$(PLATFORM)\
#		test_api_exa_2-$(PLATFORM)

# test_net_gate_way

all: $(TARGET)
#	./test_net_work_interface

test_ezutil-$(PLATFORM):test_ezutil.o $(LIB_OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS)
test_api_exa-$(PLATFORM):  test_api_exa.o $(LIB_OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS)
	#$(STRIP) $@
	
test_api_exa_2-$(PLATFORM):  test_api_exa_2.o $(LIB_OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS)
	#$(STRIP) $@

test_sha1-$(PLATFORM):  test_sha1.o $(LIB_OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS)

test_ez_md5_calc_ascii_digit-$(PLATFORM):  test_ez_md5_calc_ascii_digit.o $(LIB_OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS)
	
test_sha_str-$(PLATFORM):  test_sha_str.o $(LIB_OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS)

test_ezuuid-$(PLATFORM):test_ezuuid.o $(LIB_OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS)

test_ez_fs-$(PLATFORM):test_ez_fs.o $(LIB_OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS)

test_aes-$(PLATFORM):test_aes.o $(LIB_OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(TARGET) $(LIB_OBJS) *.o *.cfbk *.d;

dbg:$(TARGET)
	cp $(TARGET) /home/share

