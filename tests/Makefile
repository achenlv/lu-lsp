# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Ilib/cli -Ilib/algorithms  -Ilib/parsers
LDFLAGS = 

DEBUG_FLAGS = -g


# Find all source files recursively
SRCS = test_memory_allocation.c $(wildcard lib/cli/*.c) $(wildcard lib/algorithms/*.c)  $(wildcard lib/parsers/*.c)
OBJS = $(SRCS:.c=.o)

# Output binary
TARGET = test_memory_allocation 

# Build the target
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

# Rule to compile source files into object files
# Adjust output path to match object location
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run debug build
run-debug: debug
	./$(TARGET)

test: 
	./$(TARGET) -c chunks.txt -s sizes.txt -a best_fit

next_fit:
	./$(TARGET) -c x1 -s x2 -a next_fit  > next_fit.txt

first_fit:
	./$(TARGET) -c x1 -s x2 -a first_fit  > first_fit.txt


# Phony targets to avoid conflicts with file names
.PHONY: clean run
