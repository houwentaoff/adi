CC = gcc
LD = gcc 
TARGET = example
EXT = c

OBJ_DIR = ./obj
TALISE_INCL = ../devices/talise
TALISE_SRC = ../devices/talise
CFLAGS = -Wall -Werror -I. -I$(TALISE_INCL)

SRC = $(wildcard *.$(EXT))
OBJS = $(SRC:.$(EXT)=.o)

DEPS = $(TALISE_INCL)/libtalise.a

all: taliselib $(SRC) $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -o $@ $(OBJS) $(DEPS)

.$(EXT).o: talise  
	$(CC) $(CFLAGS) -o $@ -c $< 

taliselib:
	$(MAKE) -C $(TALISE_SRC)

clean:
	 rm *.o
	 rm example
	 $(MAKE) -C $(TALISE_SRC) clean

