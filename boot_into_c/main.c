#include "console.h"
#include "types.h"
#include "mmu.h"

static uint64 pml4[NPML4ENTRIES] __attribute__((aligned(PGSIZE)));
static uint64 pml3[NPDPTENTRIES] __attribute__((aligned(PGSIZE)));
static uint64 pml2[NPDENTRIES] __attribute__((aligned(PGSIZE)));
static uint64 pml1[4][NPTENTRIES] __attribute__((aligned(PGSIZE)));

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
    int i, j;
    int sum = 0;

    // Initialize the console
    uartinit();

    printk("Hello from C\n");

    // Initialize the page table here
    for (i = 0; i < 4; i++) {
        pml2[i] = (uint64)pml1[i] | PTE_P | PTE_W;

        for (j = 0; j < 512; j++) {
            pml1[i][j] = ((i * 512 +j) * 4096) | PTE_P | PTE_W;
        }
    }
    pml3[0] = (uint64)pml2 | PTE_P | PTE_W;
    pml4[0] = (uint64)pml3 | PTE_P | PTE_W;


    write_cr3((uint64)pml4);

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
