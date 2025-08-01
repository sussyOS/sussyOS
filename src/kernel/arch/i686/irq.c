#include "irq.h"
#include "pic.h"
#include "i8259.h"
#include "io.h"
#include <stddef.h>
#include <util/arrays.h>
#include "stdio.h"

#define PIC_REMAP_OFFSET        0x20

IRQHandler g_IRQHandlers[16];
static const PICDriver* g_Driver = NULL;

void i8259_IRQ_Handler(Registers* regs)
{
    int irq = regs->interrupt - PIC_REMAP_OFFSET;
    
    if (g_IRQHandlers[irq] != NULL)
    {
        // handle IRQ
        g_IRQHandlers[irq](regs);
    }
    else
    {
        printf("Unhandled IRQ %d...\n", irq);
    }

    // send EOI
    g_Driver->SendEndOfInterrupt(irq);
}

void i8259_IRQ_Initialize()
{
    const PICDriver* drivers[] = {
        i8259_GetDriver(),
    };

    for (int i = 0; i < SIZE(drivers); i++) {
        if (drivers[i]->Probe()) {
            g_Driver = drivers[i];
        }
    }

    if (g_Driver == NULL) {
        printf("Warning: No PIC found!\n");
        return;
    }

    printf("Found %s PIC.\n", g_Driver->Name);
    g_Driver->Initialize(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);

    // register ISR handlers for each of the 16 irq lines
    for (int i = 0; i < 16; i++)
        i8259_ISR_RegisterHandler(PIC_REMAP_OFFSET + i, i8259_IRQ_Handler);

    // enable interrupts
    i8259_EnableInterrupts();

    // g_Driver->Unmask(0);
    // g_Driver->Unmask(1);
}

void i8259_IRQ_RegisterHandler(int irq, IRQHandler handler)
{
    g_IRQHandlers[irq] = handler;
}