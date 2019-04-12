CC=g++
CFLAGS= -c -std=c++11 -Wall -I.
DEPS = KV_converter.h

OBJS = main.o KV_converter.o

%.o: %.cpp $(DEPS)
	$(CC) -o $@ $(CFLAGS) -std=c++11 $< 

converter : $(OBJS)
	$(CC) -o $@ $(OBJS)
	rm *.o

.PHONY: clean

clean :
	rm $(OBJS)