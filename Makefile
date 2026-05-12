CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = mybox
SRCS = main.c ls.c cp.c wc.c
OBJS = $(SRCS:.c=.o)
LINKS = ls cp wc

.PHONY: all clean install test_all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c mybox.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS) $(LINKS)

install: $(TARGET)
	$(foreach link,$(LINKS),ln -sf $(TARGET) $(link) &&) true

test_all: all
	bash testcases/test_bbtop.sh
	bash testcases/test_bbfscheck.sh
	bash testcases/test_bbnetmon.sh
