CC=g++
DLIBS=-lbcm2835 -lrt -lpthread
OBJS= PhoneCall.o ../arduPi.o ../sim7x00.o
PhoneCall:$(OBJS) 
	$(CC) -Wall -o PhoneCall $(OBJS)  $(DLIBS) 

%.o:%.c
	$(CC) -c -o $@ $<

clean:
	rm -f *.o PhoneCall

