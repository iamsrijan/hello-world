include ../mkutils/vars.mk

export $(OS_NAME)
PROG_NAME := nav_cmd_proxy
INCL_DIR += -I .. 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(COPTS) $(INCL_DIR)

all:	$(OBJS)
	$(CC) -o $(PROG_NAME) *.o

clean:
	rm -f *.o *.so *.a $(PROG_NAME)

