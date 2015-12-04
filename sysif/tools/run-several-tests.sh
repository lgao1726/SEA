echo does jumps to dest:
./run-test.sh ../test/kmain-yield.c ../test/round-robin-distinct-stacks.gdb | grep 'test OK'

echo does preserves locals:
./run-test.sh ../test/kmain-yield.c ../test/round-robin-fairness.gdb | grep 'test OK'

