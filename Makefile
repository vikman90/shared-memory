CC = gcc
CFLAGS = -Wall -Wextra -O2 -pipe $(INCLUDES)
INCLUDES = -Iinclude
LIBS = -L. -lshm

AR = ar -crs
RM = rm -f

TARGET = libshm.a
DEMO = shmdemo

.PHONY: all demo clean

all: $(TARGET)

$(TARGET): shm.o
	$(AR) $@ $^

%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

demo: demo.o $(TARGET)
	$(CC) $(LFLAGS) -o $@ $< $(LIBS)

clean:
	$(RM) $(TARGET) $(DEMO) *.o
