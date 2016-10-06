.PHONY: tp1

CC := g++
CPP_FILES := $(wildcard *.cpp)
H_FILES := $(wildcard *.h)
LD_FLAGS := -lsfml-graphics -lsfml-window -lsfml-system
#Make difference between x86 and arm
ifeq ($(shell uname -m), armv7l)
CC := $(CC)-6
endif
CC_FLAGS := --std=c++14 -pthread -O3
OUTNAME = agentTP1

all: clean tp1

clean:
	rm -f *.gch $(OUTNAME)

tp1:
	$(CC) $(CPP_FILES) $(H_FILES) -o $(OUTNAME) $(LD_FLAGS) $(CC_FLAGS)

win:
	rm -f $(OUTNAME).exe
	x86_64-w64-mingw32-c++ $(CPP_FILES) $(H_FILES) -o $(OUTNAME).exe $(LD_FLAGS) $(CC_FLAGS) -I./SFML-2.4.0/include/ -L./SFML-2.4.0/lib/
