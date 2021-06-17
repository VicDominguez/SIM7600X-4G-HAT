CC=g++
DLIBS=-lbcm2835 -lrt -lpthread
OBJS= FTP.o ../arduPi.o ../sim7x00.o
SMS:$(OBJS) 
	$(CC) -Wall -o FTP $(OBJS)  $(DLIBS) 

%.o:%.c
	$(CC) -c -o $@ $<

clean:
	rm -f *.o FTP

