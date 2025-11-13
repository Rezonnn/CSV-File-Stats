CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = csv_stats

all: $(TARGET)

$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o -lm

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(TARGET) main.o

run: $(TARGET)
	./$(TARGET) sample.csv 2
