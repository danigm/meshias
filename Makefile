GCC=gcc
OUT=daemon
CFLAGS=-g -Wall
LDFLAGS=-Llibnetfilter_queue -lnetfilter_queue -lnfnetlink

all:
	cd libnetfilter_queue; make; cd ..
	$(GCC) -o $(OUT) nfqnl_test.c $(CFLAGS) $(LDFLAGS)

clean:
	cd libnetfilter_queue; make clean; cd ..
	rm $(OUT) nfqnl_test.o
