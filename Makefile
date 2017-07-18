#####################################
#
#  Standard Makefile for BlitzKrieg
#     designed by marcello@cellosoft.com
#
#####################################

NAME=firestarter

# Compiler
CXX=g++

# Mingw location
MINGW=c:/mingw/

# Compiler options
WIN32=1
DEBUGMODE=1

### Warn mode
WFLAGS = -Wall -W -Werror -Wno-unused -fmessage-length=0


CFLAGS = $(WFLAGS) -MMD -DXP_WIN -D_WINDOWS -DWIN32 -D_MINGW


ifdef	DEBUGMODE
	CFLAGS	+= -DDEBUGMODE=$(DEBUGMODE) -DDEBUG=1 -g
	DEBUG_DIR=_debug
else
	CFLAGS	+= $(OFLAGS)
	DEBUG_DIR=
endif

### Libraries

LIBDIR  = 
LIBS = -laldmb -ldumb -lldpng -lpng -lz -lalleg

ifdef WIN32
	RM = rm -rf
	PATH_SEP = "\\"
	EXE = .exe
else
	RM = rm -rf
	PATH_SEP = /
	EXE = 
endif

### Get OBJS
include Makefile.lst


.PHONY: all test clean

first: test

all: $(NAME)$(DEBUG_DIR)$(EXE) editor$(EXE)

test: $(NAME)$(DEBUG_DIR)$(EXE)
	$(NAME)$(DEBUG_DIR)$(EXE)

src/Data.h: data/fonts.dat
	c:\allegro\tools\dat -h $@ -p DATA_FONTS_ $^

$(NAME)$(DEBUG_DIR)$(EXE): $(OBJS)
	$(CXX) $(OBJS) -o $@ -mwindows $(LIBDIR) $(LIBS)
	
editor$(EXE): $(EDITOR_OBJS)
	$(CXX) $(EDITOR_OBJS) -o $@ $(LIBDIR) $(LIBS)

-include $(addprefix objs$(DEBUG_DIR)/,$(addsuffix .d,$(notdir $(MODULES) $(JS_MODULES))))


objs$(DEBUG_DIR)/%.o: src/%.cpp src/%.h
	@echo _
	$(CXX) -c $< -o $@ $(CFLAGS)
	
	
clean:
	$(RM) $(NAME)$(EXE)
	$(RM) objs$(DEBUG_DIR)/*.d
	$(RM) objs$(DEBUG_DIR)/*.o
