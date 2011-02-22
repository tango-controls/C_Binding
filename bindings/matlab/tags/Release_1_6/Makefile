#------------------------------------------------------------------------------
#   Makefile to generate the Tango Binding for Matlab
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# BASE DIR
#------------------------------------------------------------------------------
BASE_DIR = /usr/local/soleil-linux-i386-5.2.0

#------------------------------------------------------------------------------
# OMNIORB BASE DIR
#------------------------------------------------------------------------------
OMNIORB_BASE = $(BASE_DIR)/omniorb

#------------------------------------------------------------------------------
# TANGO BASE DIR
#------------------------------------------------------------------------------
TANGO_BASE = $(BASE_DIR)/tango

#------------------------------------------------------------------------------
# LOG4TANGO BASE DIR
#------------------------------------------------------------------------------
LOG4TANGO_BASE = $(TANGO_BASE)

#------------------------------------------------------------------------------
# MATLAB BASE DIR
#------------------------------------------------------------------------------
#MATLAB_BASE = /usr/local/matlabR14
MATLAB_BASE = /home/leclercq/dev/matlabR14sp1

#------------------------------------------------------------------------------
# INCLUDE DIRS
#------------------------------------------------------------------------------
INCLUDE_DIRS = -I . \
               -I $(OMNIORB_BASE)/include \
               -I $(TANGO_BASE)/include \
               -I $(MATLAB_BASE)/extern/include

#------------------------------------------------------------------------------
# LIB DIRS
#------------------------------------------------------------------------------
LIB_DIRS = -L $(OMNIORB_BASE)/lib \
           -L $(TANGO_BASE)/lib  \
           -L $(MATLAB_BASE)/bin/glnx86 \
           -L $(MATLAB_BASE)/extern/lib/glnx86

#------------------------------------------------------------------------------
# COMPILER/LINKER OPTIONS
#------------------------------------------------------------------------------
# COMMON
#------------------------------------------------------------------------------
MEX_NAME = tango_binding

#------------------------------------------------------------------------------
# GNU/LINUX (TESTED ON RH-LINUX-9 with GCC-3.2.2)
#------------------------------------------------------------------------------
CC = gcc
#------------------------------------------------------------------------------
CFLAGS = -W -Wall -Wno-unused -Wno-comment -Wno-sign-compare -D_REENTRANT -DMATLAB_MEX_FILE
LD = $(CC)
LDFLAGS = -fPIC -shared -o
MEX_EXT = mexglx
#------------------------------------------------------------------------------
#
# add -g and -D_DEBUG to the CFLAGS in case you want compile with debugging information
#
# CFLAGS += -g -D_DEBUG
#
#------------------------------------------------------------------------------
#
# add -DHAS_TANGO_5_OR_SUP to the CFLAGS in case your are compiling this binding 
# with a tango release >= 5.0.
# 
CFLAGS += -DHAS_TANGO_5_OR_SUP
#
#------------------------------------------------------------------------------
#
# add -DDISABLE_TANGO_GROUP_SUPPORT to the CFLAGS in case your
# are compiling this binding with a tango release <= 5.1. 
# 
# CFLAGS += -DDISABLE_TANGO_GROUP_SUPPORT
#
#------------------------------------------------------------------------------
#
# add -D_MATLAB_R12_ to the CFLAGS in case your are compiling this binding with 
# a Matlab release <= R12. 
# 
# CFLAGS += -D_MATLAB_R12_
#
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# OBJ FILES
#------------------------------------------------------------------------------
OBJS_SO = main.so.o \
	MexArgs.so.o \
	MexFile.so.o \
	MexUtils.so.o \
	GroupRepository.so.o \
	DevRepository.so.o \
	DataAdapter.so.o \
	TangoBinding.so.o \
	mexversion.so.o

#------------------------------------------------------------------------------
# RULE: all
#------------------------------------------------------------------------------
all: $(MEX_NAME).$(MEX_EXT) install

#------------------------------------------------------------------------------
# RULE: $(MEX_NAME).$(MEX_EXT)
#------------------------------------------------------------------------------
$(MEX_NAME).$(MEX_EXT): $(OBJS_SO)
	$(LD) $(LDFLAGS) $(MEX_NAME).$(MEX_EXT) $(OBJS_SO) $(LIB_DIRS) \
        -lmex -lmx -ltango -lomniORB4 -lomniDynamic4 -lCOS4 -lomnithread -llog4tango -lpthread

#------------------------------------------------------------------------------
# RULE: mexversion.so.o
#------------------------------------------------------------------------------
mexversion.so.o: $(MATLAB_BASE)/extern/src/mexversion.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(MATLAB_BASE)/extern/src/mexversion.c -c -o mexversion.so.o

#------------------------------------------------------------------------------
# RULE: main.so.o
#------------------------------------------------------------------------------
main.so.o: main.cpp
	 $(CC) $(CFLAGS) $(INCLUDE_DIRS) main.cpp -c -o main.so.o

#------------------------------------------------------------------------------
# RULE: MexArgs.so.o
#------------------------------------------------------------------------------
MexArgs.so.o: MexArgs.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) MexArgs.cpp -c -o MexArgs.so.o

#------------------------------------------------------------------------------
# RULE: MexFile.so.o
#------------------------------------------------------------------------------
MexFile.so.o: MexFile.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) MexFile.cpp -c -o MexFile.so.o

#------------------------------------------------------------------------------
# RULE: MexUtils.so.o
#------------------------------------------------------------------------------
MexUtils.so.o: MexUtils.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) MexUtils.cpp -c -o MexUtils.so.o

#------------------------------------------------------------------------------
# RULE: DevRepository.so.o
#------------------------------------------------------------------------------
DevRepository.so.o: DevRepository.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) DevRepository.cpp -c -o DevRepository.so.o

#------------------------------------------------------------------------------
# RULE: GroupRepository.so.o
#------------------------------------------------------------------------------
GroupRepository.so.o: GroupRepository.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) GroupRepository.cpp -c -o GroupRepository.so.o

#------------------------------------------------------------------------------
# RULE: DataAdapter.so.o
#------------------------------------------------------------------------------
DataAdapter.so.o: DataAdapter.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) DataAdapter.cpp -c -o DataAdapter.so.o

#------------------------------------------------------------------------------
# RULE: TangoBinding.so.o
#------------------------------------------------------------------------------
TangoBinding.so.o: TangoBinding.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) TangoBinding.cpp -c -o TangoBinding.so.o

#------------------------------------------------------------------------------
# RULE: clean
#------------------------------------------------------------------------------
clean:
	rm -f *.so.o $(MEX_NAME).$(MEX_EXT)

#------------------------------------------------------------------------------
# RULE: install
#------------------------------------------------------------------------------
install:
	cp -f ./$(MEX_NAME).$(MEX_EXT) ./mex-file
	rm -f ./$(MEX_NAME).$(MEX_EXT)








