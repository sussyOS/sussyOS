#pragma once
#include "isr.h"

typedef void (*IRQHandler)(Registers* regs);

void i8259_IRQ_Initialize();
void i8259_IRQ_RegisterHandler(int irq, IRQHandler handler);