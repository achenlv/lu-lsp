CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: md_koks

md_koks: md_koks.c
	$(CC) $(CFLAGS) -o $@ $<
	# gcc -o md_koks.out md_koks.c

clean:
  rm -f md_koks.out


.PHONY: all test gentest clean
