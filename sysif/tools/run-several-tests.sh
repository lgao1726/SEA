echo does jumps to dest:
./run-test.sh ../test/kmain-yieldto.c ../test/sys-yieldto-jumps-to-dest.gdb | grep 'test OK'

echo does preserves locals:
./run-test.sh ../test/kmain-yieldto.c ../test/sys-yieldto-preserves-locals.gdb | grep 'test OK'

echo does preserves status register:
./run-test.sh ../test/kmain-yieldto.c ../test/sys-yieldto-preserves-status-register.gdb | grep 'test OK'
