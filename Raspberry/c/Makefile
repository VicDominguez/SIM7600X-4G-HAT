CC=g++
DLIBS=-lbcm2835 -lrt -lpthread
OBJS= main.o arduPi.o sim7x00.o
sim7600:$(OBJS)
	$(CC) -Wall -o sim7600 $(OBJS) $(DLIBS)

%.o:%.c 
	$(CC) -c -o $@ $<

clean:
	rm *.o sim7600

