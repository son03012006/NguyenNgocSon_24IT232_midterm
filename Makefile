C = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -std=c99
SRCS = src/entry.c src/options.c src/list.c src/utils.c src/sort.c src/format.c src/print.c src/main.c
OBJS = $(SRCS:.c=.o)
TARGET = my_ls

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
