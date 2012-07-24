
#------------------------------------------------

CC 	= g++
CFLAGS 	= -Wall -D_REENTRANT -D_GNU_SOURCE -ggdb -O2
LFLAGS 	= -lstdc++ -ggdb

#------------------------------------------------

%.o : %.c	
	$(CC) $(CFLAGS) -c $^ -o $@
%.o : %.cpp	
	$(CC) $(CFLAGS) -c $^ -o $@

#------------------------------------------------

OBJS 	= keyworddict.o 

all : test

test : $(OBJS) test.o
	$(CC) $(LFLAGS) $^ -o $@
	
clean :

	rm -rf *.o
	rm -rf core.*
	rm -rf test
	