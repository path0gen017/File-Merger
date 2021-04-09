CC 			= cc
CPLFLAGS 	= -std=c17 -O3 -c -Wall -Werror
BIN 		= merge
OBJS 		= main.o errorUtils.o
RM 			= rm

.PHONY: clean

$(BIN) : $(OBJS)
	$(CC) $(OBJS) -o $(BIN)

main.o :  main.c
	$(CC) $(CPLFLAGS) main.c -o main.o

errorUtils.o : errorUtils.c
	$(CC) $(CPLFLAGS) errorUtils.c -o errorUtils.o

clean:
	$(RM) $(OBJS) $(BIN)
