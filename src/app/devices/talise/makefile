TALLIB = libtalise.a
CC = gcc
ADIHALSRC =  ../adi_hal
ARFLAGS = -rv
CFLAGS = -Wall -Werror -I. -I$(ADIHALSRC)

EXT = c

SRC = $(wildcard *.$(EXT))
OBJS = $(SRC:.$(EXT)=.o)

all: adihallib $(SRC) $(TALLIB)

$(TALLIB): $(OBJS)
	ar $(ARFLAGS) $(TALLIB) $(OBJS) $(ADIHALSRC)/adi_hal.o

.$(EXT).o: adihallib
	$(CC) $(CFLAGS) -o $@ -c $<

adihallib:
	$(MAKE) -C $(ADIHALSRC)

clean:
	rm *.o
	rm *.a
	$(MAKE) -C $(ADIHALSRC) clean
