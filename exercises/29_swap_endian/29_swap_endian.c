#include <stdint.h>
#include <stdio.h>

uint32_t swap_endian(uint32_t num) {
    // TODO: 在这里添加你的代码
    return num >> 24 | (num >> 8) & 0x00ff00 | (num << 8) & 0xff0000 | num << 24;
}

int main(int argc, char* argv[]) {
    uint32_t num = 0x78563412;
    uint32_t swapped = swap_endian(num);
    printf("0x%08x -> 0x%08x\n", num, swapped);
    return 0;
}