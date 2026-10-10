CC = cc
CFLAGS = -Wall -Wextra -Iinclude -std=c99 -MMD -MP
SRCS = src/entry.c src/options.c src/list.c src/utils.c src/sort.c src/format.c src/print.c src/main.c
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)
TARGET = my_ls

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

.PHONY: all clean
