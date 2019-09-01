ADIHALLIB = adihallib.a
CC = gcc
ARFLAGS = -rv
CFLAGS = -Wall -Werror -pedantic -I.
EXT = c

SRC = adi_hal.$(EXT)
OBJS = $(SRC:.$(EXT)=.o)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(ADIHALLIB): $(OBJS)
	ar $(ARFLAGS) $(ADIHALLIB) $(OBJS)

$(OBJS): ./adi_hal.h


clean:
	 rm *.o
	 rm *.a
