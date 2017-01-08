CC = gcc
CFLAGS = -Wall -Wextra -O2 -pipe $(INCLUDES)
INCLUDES = -Iinclude

AR = ar -crs
RM = rm -f

TARGET = libshm.a

.PHONY: all demo clean

all: $(TARGET)

$(TARGET): shm.o
	$(AR) $@ $^

%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) *.o
