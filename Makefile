ProgramName=playWAV
CCFLAGS= -g -Wall -pedantic
CC=gcc
LDFLAGS= `sdl2-config --cflags --libs`  #linker flags defined for later usage
src=$(wildcard *.c) #get all files .cpp
obj=$(patsubst %.c,%.o,$(src)) #change *.cpp strings to *.o

all: $(ProgramName)

%.o:%.c
	$(CC) -c $(CCFLAGS) $< -o $@
$(ProgramName): $(obj)
	$(CC) $^ -o $@ $(LDFLAGS) 
clean:
	-rm -f *.o $(ProgramName)
run: 
	./$(ProgramName)
