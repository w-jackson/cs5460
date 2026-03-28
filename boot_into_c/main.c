#include "console.h"
#include "types.h"
#include "mmu.h"

static inline void halt(void)
{
    asm volatile("hlt" : : );
}

static inline void write_cr3(uint64 val)
{
    asm volatile("movq %0,%%cr3" : : "r" (val));
}

int main(void)
{
    int i;
    int sum = 0;

    // Initialize the console
    uartinit();

    printk("Hello from C\n");

    // Initialize the page table here
    // ...

    // This test code touches 32 pages in the range 0 to 8MB
    for (i = 0; i < 64; i++) {
        uint64 addr = (uint64)i * 4096ULL * 32ULL;
        int *p = (int *)(uint64)addr;
        sum += *p;

        printk("page\n");
    }

    halt();

    return sum;
}
