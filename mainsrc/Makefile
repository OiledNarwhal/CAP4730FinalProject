
# the name of the compiled binary
TARGET  := prog1_mesh

# list files to compile and link together
FILES   := main MySphere


#################################################################


# The following Makefile rules should work for Linux or Cygwin

CC		 := g++ -std=c++11
LD		 := g++ 
OBJSUFFIX	 := .o
LIBPREFIX	 := lib
STATIC_LIBSUFFIX := .a
CFLAGS 		 := -g
CFLAGS_PLATFORM  :=
LDFLAGS		 :=
FRAMEWORKS	 :=
LIBS		 := st png jpeg

ARCH=$(shell uname | sed -e 's/-.*//g')

ifeq ($(ARCH), CYGWIN_NT)

# if we're building in cygwin, we'll need to use the
# win32 versions of gl and glut

EXESUFFIX  := .exe
LIBS  	   += glut32 opengl32

else

ifeq ($(ARCH),Darwin)

# we're building on the mac

EXESUFFIX  :=
FRAMEWORKS += OpenGL GLUT AppKit IOKit

#
# Mac users need to point to the libjpeg and libpng directories
# 
# The default location of a fink install is given below.
# 
EXTRA_LIB_DIRS := /opt/local/lib
EXTRA_INC_DIRS  := /opt/local/include glew/include

else

# building on Linux

EXESUFFIX  :=
LIBS	   += glut GL GLU GLEW

#
# hack for myth machines.  Add /usr/lib as an explicit lib dir so
# it gets picked up instead of /usr/pubsw/lib.
# 

EXTRA_LIB_DIRS := /usr/lib64 /usr/lib glew/lib
EXTRA_INC_DIRS := glew/include

endif
endif


LIBST      :=  st
LIBST_ROOT :=  ../libst
LIBST_DIR  :=  $(LIBST_ROOT)/lib
LIBST_INC  :=  $(LIBST_ROOT)/include

TARGET     := $(addsuffix $(EXESUFFIX), $(TARGET))

INCDIRS    := . $(LIBST_INC) $(EXTRA_INC_DIRS)
LIBDIRS    := $(LIBST_DIR) $(EXTRA_LIB_DIRS)

CFLAGS     += $(addprefix -I, $(INCDIRS))
LDFLAGS    += $(addprefix -L, $(LIBDIRS))

LDLIBS	   := $(addprefix -l, $(LIBS))
LDFRAMEWORKS := $(addprefix -framework , $(FRAMEWORKS))

OBJS       :=  $(addsuffix $(OBJSUFFIX), $(FILES))


.SUFFIXES : .cpp $(OBJSUFFIX)

.PHONY : clean release all

all: $(TARGET)

$(LIBST_DIR)/libst.a:
	(cd $(LIBST_ROOT); $(MAKE))

$(TARGET): $(OBJS) $(LIBST_DIR)/libst.a
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS) $(LDFRAMEWORKS)


%.o : %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -rf *$(OBJSUFFIX) $(TARGET) *~ .#* #*
	(cd $(LIBST_ROOT); $(MAKE) clean)

release:
	@make --no-print-directory RELEASE=1


