
global i8259_outb
i8259_outb:
    [bits 32]
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

global i8259_inb
i8259_inb:
    [bits 32]
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret

global i8259_Panic
i8259_Panic:
    cli
    hlt

global i8259_EnableInterrupts
i8259_EnableInterrupts:
    sti
    ret

global i8259_DisableInterrupts
i8259_DisableInterrupts:
    cli
    ret

global crash_me
crash_me:
    ; div by 0
    ; mov ecx, 0x1337
    ; mov eax, 0
    ; div eax
    int 0x80
    ret