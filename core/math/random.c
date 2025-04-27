#include <stdlib.h>
#include <stdint.h>

#include "random.h"

int64_t random_int(int64_t min, int64_t max)
{
    int64_t val;
    if (_internal_get_random(&val) == -1)
        return (int64_t)-1;

    if (val < 0)
        val = -val; // or use labs(val) if needed

    return val % (max - min + 1) + min;
}

int _internal_get_random(int64_t *ptr)
{
    size_t ret = 0;

    asm volatile(
        "movq $318, %%rax\n\t"   // syscall: getrandom
        "movq %1, %%rdi\n\t"     // ptr
        "movq $8, %%rsi\n\t"     // 8 bytes
        "movq $1, %%rdx\n\t"     // flags=1 (GRND_NONBLOCK)
        "syscall\n\t"
        "movq %%rax, %0\n\t"
        : "=r"(ret)               // output
        : "r"(ptr)                // input
        : "rdi", "rsi", "rdx", "rax", "memory" // clobbered registers
    );

    return (int)ret;
}
