#include "hal.h"
#include <arch/i686/gdt.h>
#include <arch/i686/idt.h>

void HAL_Initialize()
{
    i8259_GDT_Initialize();
    i8259_IDT_Initialize();
}