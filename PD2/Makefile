CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: LSP_PD2_agris_pudans

LSP_PD2_agris_pudans: LSP_PD2_agris_pudans.c
	$(CC) $(CFLAGS) -o $@ $<

test: all
	./test.sh

gentest: all
	dd if=/dev/urandom of=testinput bs=1M count=10

clean:
	rm -f LSP_PD2_agris_pudans testinput testoutput

.PHONY: all test gentest clean
