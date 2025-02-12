all:
  gcc -Wall -Wextra -Werror -o md_koks.out LSP_PD2_agris_pudans.c

test:
  ./md_koks.out test.in test.out
  diff -s test.in test.out

  ./md_koks.out test-big.in test-big.out
  diff -s test-big.in test-big.out

gentest:
  dd if=/dev/urandom of=test.in count=2048
  dd if=/dev/urandom of=test-big.in count=2048 bs=100

clean:
  rm -f md_koks.out


.PHONY: all gentest test testbig clean
