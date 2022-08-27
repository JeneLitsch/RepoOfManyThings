as jit/test.asm -o jit/test.bin --64
ld -Ttext 200000 --oformat binary -o jit/test.raw jit/test.bin