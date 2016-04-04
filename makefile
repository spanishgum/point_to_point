##########################################################
#
# Best Makefile Ever
#  Adam Stallard
#
##########################################################

define HEADER

   ---------------------------------------------------------
   |                                                       |
   |              Program XYZ - Version 0.0.0              |
   |                                                       |
   |                    Adam Stallard                      |
   |                                                       |
   ---------------------------------------------------------

   . . . building components . . .

endef

define FOOTER

  . . . components ready!! enjoy :)

endef

export HEADER FOOTER

##########################################################


CC = g++ -std=c++11
CCFLAGS = -Wall -pedantic
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
EXE := main


##########################################################


.SUFFIXES :
.PHONY : fresh clean cls hdr ftr


##########################################################

all : $(EXE)

$(EXE) : $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

%.o : %.cpp
	$(CC) $(CCFLAGS) -c $<


##########################################################


fresh : clean cls hdr all ftr

clean :
	-@rm $(OBJS) $(EXE) 2>/dev/null || true

cls :
	@clear

hdr :
	@$(info $(HEADER))

ftr :
	@$(info $(FOOTER))


##########################################################