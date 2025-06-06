#pragma once
#include <stdint.h>

void __attribute__((cdecl)) i8259_outb(uint16_t port, uint8_t value);
uint8_t __attribute__((cdecl)) i8259_inb(uint16_t port);
uint8_t __attribute__((cdecl)) i8259_EnableInterrupts();
uint8_t __attribute__((cdecl)) i8259_DisableInterrupts();

void i8259_iowait();
void __attribute__((cdecl)) i8259_Panic();