# Autogenerated by mfbuild-perl

CFLAGS	=	-Wall -g
BUILD	=	client
OBJS	=	client.o str2int.o state_machine.o

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) $(OBJS) core a.out .nfs*

client:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o client $(LDLIBS)

client.o:	client.c state_machine.h
str2int.o:	str2int.c str2int.h
state_machine.o:	state_machine.c state_machine.h str2int.h
