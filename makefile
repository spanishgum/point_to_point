##########################################################
#
# Best Makefile Ever
#  Adam Stallard
#
##########################################################

define HEADER

   ---------------------------------------------------------
   |                                                       |
   |                Point to Point Delivery                |
   |              Source Location Optimization             |
   |                     Version 1.0.0                     |
   |                                                       |
   |                    Adam Stallard                      |
   |                   Eric Serbousek                      |
   |                    Travis Chung			   |
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
CCFLAGS = -Wall -pedantic -pthread
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
EXE := main
LEMON := -lemon -I lemon_lib/include -L lemon_lib/lib

##########################################################


.SUFFIXES :
.PHONY : fresh clean cls hdr ftr semi_clean


##########################################################

all : fresh

$(EXE) : $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

%.o : %.cpp
	$(CC) $(CCFLAGS) $(LEMON) -c $<


##########################################################


fresh : clean cls hdr $(EXE) ftr semi_clean

clean :
	-@rm $(OBJS) $(EXE) 2>/dev/null || true

cls :
	@clear

hdr :
	@$(info $(HEADER))

ftr :
	@$(info $(FOOTER))

semi_clean :
	-@rm $(OBJS) 2>/dev/null || true

	
##########################################################
