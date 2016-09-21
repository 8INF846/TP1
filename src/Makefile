CC := g++
CPP_FILES := main.cpp MapReal.cpp Environnement.cpp Vacuum.cpp SuckWithLevelStrategy.cpp
H_FILES := Map.h MapReal.h Environnement.h Pos.h Vacuum.h Strategy.h SuckWithLevelStrategy.h
LD_FLAGS :=
#Make difference between x86 and arm
ifeq ($(shell uname -m), armv7l)
CC := $(CC)-6
endif
CC_FLAGS := --std=c++14 -Wall -pthread -O3
OUTNAME = agentTP1

all: clean tp1

clean:
	rm -f *.gch $(OUTNAME)

tp1:
	$(CC) $(CPP_FILES) $(H_FILES) -o $(OUTNAME) $(LD_FLAGS) $(CC_FLAGS)
