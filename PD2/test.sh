#!/bin/bash

# Create test input if it doesn't exist
if [ ! -f testinput ]; then
    dd if=/dev/urandom of=testinput bs=1M count=10
fi

# Test normal copy
./LSP_PD2_agris_pudans testinput testoutput
if [ $? -ne 0 ]; then
    echo "Test failed: Copy operation returned error"
    exit 1
fi

# Verify files are identical
if ! cmp testinput testoutput >/dev/null 2>&1; then
    echo "Test failed: Files are different"
    exit 1
fi

echo "All tests passed"
exit 0
