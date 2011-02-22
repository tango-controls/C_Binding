#
# RcsID = " $Header$ ";
#
#*********************************************************************
#
# File:          Makefile 
#
# Project:       C client interface to Tango
#
# Description:   Makefile for the test sequence
#
# Author(s):     Jens Meyer
#
# Original:   	 November 2007
#
# $Log$
# Revision 1.2  2007/12/18 17:33:11  jensmeyer
# Added database access for properties and query requests.
#
# Revision 1.1.1.1  2007/12/05 15:05:04  jensmeyer
# Tango C language binding
#
#
# Copyright (c) 1997 by European Synchrotron Radiation Facility,
#                       Grenoble, France
#
#**********************************************************************


C_LIB_BASE     =  ../install/redhate4
#C_LIB_BASE     =  ../install/solaris9_gcc

#---------------------------------------------------------------------
# All include file and standard library pathes
#
#		make sure to get always the new include files 
#		under ../include
#

INCLDIRS =  	-I. \
					-I$(C_LIB_BASE)/include
					
CC      =       gcc
CPLUS+  =		 g++

#                       Libraries
LIBDIRS =       -L $(C_LIB_BASE)/lib
LFLAGS = 		 $(LIBDIRS) -g \
			   	-lc_tango \
		   	   -ldl -lpthread

CFLAGS = 		-g -Dlinux -c $(INCLDIRS)

NAME =       	-o


#
#---------------------------------------------------------------------
# All Files needed
#

#               object files
MENU_OBJS =		test_menu.o

WRITE_OBJS =	write_examples.o

#---------------------------------------------------------------------
# What has to be made
#
#		Names of executables in the home directory
MENU	 = 	 test_menu

WRITE  =		 write_examples

#---------------------------------------------------------------------
# build server and client
#

all:			$(MENU) $(WRITE)
#
$(MENU):  	$(MENU_OBJS)
			 $(CPLUS+) $(NAME) $(MENU) $(MENU_OBJS) $(LFLAGS)

test_menu.o:		test_menu.c
				$(CC) $(CFLAGS) test_menu.c

$(WRITE):  	$(WRITE_OBJS)
			 $(CPLUS+) $(NAME) $(WRITE) $(WRITE_OBJS) $(LFLAGS)

write_examples.o:		write_examples.c
				$(CC) $(CFLAGS) write_examples.c


clean:	
				-rm -f $(MENU_OBJS)
				-rm -f $(WRITE_OBJS) 


clobber:	clean
				-rm -f $(MENU) 
				-rm -f $(WRITE) 

