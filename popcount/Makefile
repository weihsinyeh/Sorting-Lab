cc = gcc
CFLAFS = -01 -g -Wall

OBJS := popcount.o
all: main 
main: $(OBJS)
		$(CC) $(LDFLAGS) -o $@ $^ -lm

%.o: %.c 
		$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $< 

clean:
		rm -f $(OBJS)
		rm -f *.dSYM
		rm -f main 
		rm -f popcount.o
		rm -f .*.o.d

plot: output.txt
		gnuplot scripts/runtime.gp
