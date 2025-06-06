#include <stdint.h>
#include "stdio.h"
#include "memory.h"

extern uint8_t __bss_start;
extern uint8_t __end;

void* g_data = (void*)0x20000;

void __attribute((section(".entry"))) cstart_(uint16_t bootDrive)
{
    memset(&__bss_start, 0,&__end - &__bss_start);

    clrscr();

    printf("Hello, world from kernel!\n");

end:
    for (;;);
}