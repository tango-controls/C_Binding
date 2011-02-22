#
#		Makefile to generate the Tango library
#

include Make.rules


ifdef _solaris
ifdef gcc
CC = c++
AR = ar rv
AR_SL = $(CC) -fPIC -shared -ldl
VERS_OPT = -Wl,-h,
SL_EXT = so
else
CC = CC
AR = CC -xar -o 
AR_SL = $(CC) -mt -G -lCstd -ldl
VERS_OPT = -h
SL_EXT = so
endif
endif


ifdef linux
CC = c++
AR = ar rv
AR_SL = $(CC) -fPIC -shared -ldl 
VERS_OPT = -Wl,-soname,
SL_EXT = so
#
DOC_CONFIG = ../doxygen/Doxyfile
GEN_DOC = $(DOXYGEN) $(DOC_CONFIG)
endif


CLIENT_SRC = .

OBJS_DIR = 		objs/$(BIN_DIR)
OBJS_DIR_SL = 	objs_sl/$(BIN_DIR)


INCLUDE_DIRS = -I$(TANGO_BASE)/include \
					-I$(OMNI_BASE)/include \
	       		-I$(LOG4TANGO_BASE)/include \
	       		-I$(CLIENT_SRC) 

LIB_DIRS     = -L $(TANGO_HOME)/lib \
					-L$(OMNI_BASE)/lib \
	       		-L$(LOG4TANGO_BASE)/lib
	       
# -----------------------------------------------------------------
#
#		The compiler flags
#
#------------------------------------------------------------------

ifdef _solaris
ifdef gcc
CXXFLAGS = -O2 -D_REENTRANT $(INCLUDE_DIRS)
CXXFLAGS_SL = $(CXXFLAGS) -fPIC
else
CXXFLAGS = 	-mt -D_POSIX_PTHREAD_SEMANTICS \
	   		-xregs=no%appl $(INCLUDE_DIRS)
CXXFLAGS_SL = $(CXXFLAGS) -KPIC
endif

LFLAGS =  $(LIB_DIRS)  		\
				-ltango			\
				-llog4tango		\
				-lomniORB4 		\
				-lomniDynamic4	\
				-lomnithread	\
				-lCOS4			\
				-lpthread		\
				-lposix4 -lsocket -lnsl
endif


ifdef linux
CXXFLAGS = -g -D_REENTRANT $(INCLUDE_DIRS)
#CXXFLAGS =  -D_REENTRANT -D_TANGO_LIB $(INCLUDE_DIRS)
CXXFLAGS_SL = $(CXXFLAGS) -fPIC

LFLAGS =  $(LIB_DIRS)  		\
				-ltango			\
				-llog4tango		\
				-lomniORB4 		\
				-lomniDynamic4	\
				-lomnithread	\
				-lCOS4			\
				-ldl -lpthread
endif

#-------------------------------------------------------------------

LIBNAME = libc_tango
AR_EXT = a

		
CLIENT_OBJS = 	$(OBJS_DIR)/c_tango_proxy.o \
		$(OBJS_DIR)/c_tango_command.o \
		$(OBJS_DIR)/c_tango_attribute.o 
	  
CLIENT_OBJS_SL = $(OBJS_DIR_SL)/c_tango_proxy.so.o \
		$(OBJS_DIR_SL)/c_tango_command.so.o \
		$(OBJS_DIR_SL)/c_tango_attribute.so.o

CLIENT_INCLUDE =	c_tango.h

#
# Rule for archive libary
#
	
.SUFFIXES:	.o .c
.c.o:
	$(CC) $(CXXFLAGS) -c $<

#
# Rule for shared library
#

.SUFFIXES:	.so.o .c
.c.so.o:
	$(CC) $(CXXFLAGS_SL) -c $< -o $*.so.o

#
# Rule for API files
#		
$(OBJS_DIR)/%.o: $(CLIENT_SRC)/%.c
	@./cr_dir $(OBJS_DIR)
	$(CC) $(CXXFLAGS) -c $< -o $(OBJS_DIR)/$*.o
	
$(OBJS_DIR_SL)/%.so.o: $(CLIENT_SRC)/%.c
	@./cr_dir $(OBJS_DIR_SL)
	$(CC) $(CXXFLAGS_SL) -c $< -o $(OBJS_DIR_SL)/$*.so.o
					
#-----------------------------------------------------------------


all:	$(LIBNAME).$(AR_EXT) $(LIBNAME).$(SL_EXT) install_include install_link

#
#	The archive libs
#

$(LIBNAME).$(AR_EXT): 	$(CLIENT_OBJS)
	@./cr_dir $(INSTALL_BASE)/lib
	$(AR) $(INSTALL_BASE)/lib/$(LIBNAME).$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).$(AR_EXT) \
	$(CLIENT_OBJS)

#
#	The shared libs
#

$(LIBNAME).$(SL_EXT):	$(CLIENT_OBJS_SL)
	@./cr_dir $(INSTALL_BASE)/lib
	$(AR_SL) -o $(INSTALL_BASE)/lib/$(LIBNAME).$(SL_EXT).$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS) \
	$(VERS_OPT)$(LIBNAME).$(SL_EXT).$(MAJOR_VERS) \
	$(CLIENT_OBJS_SL) $(LFLAGS)

install_link:
	d=`pwd`
	cd $(INSTALL_BASE)/lib; \
	rm $(LIBNAME).$(AR_EXT); ln -s $(LIBNAME).$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).$(AR_EXT) $(LIBNAME).$(AR_EXT); \
	rm $(LIBNAME).$(SL_EXT); ln -s $(LIBNAME).$(SL_EXT).$(MAJOR_VERS) $(LIBNAME).$(SL_EXT); \
	rm $(LIBNAME).$(SL_EXT).$(MAJOR_VERS); ln -s $(LIBNAME).$(SL_EXT).$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS) $(LIBNAME).$(SL_EXT).$(MAJOR_VERS); \
	cd $d
	
install_include:
	@./cr_dir $(INSTALL_BASE)/include
	cp $(CLIENT_INCLUDE) $(INSTALL_BASE)/include; cd ..

install_win32:
	@./cr_dir $(INSTALL_BASE_WIN32)/include
	cp $(CLIENT_INCLUDE) $(INSTALL_BASE_WIN32)/include; cd ..	
	@./cr_dir $(INSTALL_BASE_WIN32)_dll/include
	cp $(CLIENT_INCLUDE) $(INSTALL_BASE_WIN32)_dll/include; cd ..
	
	@./cr_dir $(INSTALL_BASE_WIN32)/lib
	cp win32/winnt_lib/tango_static/lib/c_tangod.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).lib $(INSTALL_BASE_WIN32)/lib
	cp win32/winnt_lib/tango_static/lib/c_tangod.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).pdb $(INSTALL_BASE_WIN32)/lib
	cp win32/winnt_lib/tango_static/lib/c_tango.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).lib $(INSTALL_BASE_WIN32)/lib

	@./cr_dir $(INSTALL_BASE_WIN32)_dll/lib
	cp win32/winnt_lib/tango_dll/lib/c_tangod.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).lib $(INSTALL_BASE_WIN32)_dll/lib
	cp win32/winnt_lib/tango_dll/lib/c_tangod.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).pdb $(INSTALL_BASE_WIN32)_dll/lib
	cp win32/winnt_lib/tango_dll/lib/c_tango$(MAJOR_VERS)$(MINOR_VERS)$(PATCH_VERS)d.dll $(INSTALL_BASE_WIN32)_dll/lib
	cp win32/winnt_lib/tango_dll/lib/c_tango.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).lib $(INSTALL_BASE_WIN32)_dll/lib
	cp win32/winnt_lib/tango_dll/lib/c_tango$(MAJOR_VERS)$(MINOR_VERS)$(PATCH_VERS).dll $(INSTALL_BASE_WIN32)_dll/lib

	d=`pwd`
	cd $(INSTALL_BASE_WIN32)/lib; \
	rm tangod.lib; ln -s c_tangod.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).lib c_tangod.lib; \
	rm tango.lib; ln -s c_tango.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).lib c_tango.lib; \

	cd $(INSTALL_BASE_WIN32)_dll/lib; \
	rm tangod.lib; ln -s c_tangod.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).lib c_tangod.lib; \
	rm tango.lib; ln -s c_tango.$(MAJOR_VERS).$(MINOR_VERS).$(PATCH_VERS).lib c_tango.lib;	\
	cd $d

	
doc:
	cd server; $(GEN_DOC); cd ..

clean:
	rm -f *.o core
	rm -f $(OBJS_DIR_SL)/*.o
	rm -f $(OBJS_DIR)/*.o
		
clean_all:
	rm -f *.o
	rm -f test/*.html
	rm -f test/solaris/*.html
	rm -f test/suse72/*.html
	rm -f test/*.log
	rm -f test/solaris/*.log
	rm -f test/suse72/*log
	rm -f winnt_lib/tango_static/Debug/*.obj
	rm -f winnt_lib/tango_static/Debug/*.lib
	rm -f winnt_lib/tango_static/Release/*.obj
	rm -f winnt_lib/tango_static/Release/*.lib
	rm -f winnt_lib/tango_dll/Debug/*.obj
	rm -f winnt_lib/tango_dll/Debug/*.lib
	rm -f winnt_lib/tango_dll/Debug/*.dll
	rm -f winnt_lib/tango_dll/Debug/*.exp
	rm -f winnt_lib/tango_dll/Debug/*.map
	rm -f winnt_lib/tango_dll/Release/*.obj
	rm -f winnt_lib/tango_dll/Release/*.lib
	rm -f winnt_lib/tango_dll/Release/*.dll
	rm -f winnt_lib/tango_dll/Release/*.exp
	rm -f winnt_lib/tango_dll/Release/*.map
	
