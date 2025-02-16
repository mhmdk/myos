#!/bin/python


asmfile=open("src/interrupt_stubs.asm",'w')

asmfile.write(";this file is automatically generated by generate_interrupt_stubs.py\n\n")
asmfile.write("""extern set_interrupt_handler
extern handle_interrupt
data_segment_selector equ 2<<3
global setup_all_interrupts
global trap_return
section .text
setup_all_interrupts:
""")
for i in range(0,256):
    asmfile.write("call setup_interrupt_{0}\n".format(i))

asmfile.write("ret\n\n")




for i in range(0,256):
    asmfile.write("""setup_interrupt_{0}:
push interrupt_stub_{0}
push {0}
call set_interrupt_handler
add esp, 8
ret

""".format(i))

for i in range(0,256):
    asmfile.write("interrupt_stub_{0}:\n".format(i))
    if not (i == 8 or (i >= 10 and i <= 14) or i == 17):
        asmfile.write( "; not an exception, we set the error code manually to 0\n")
        asmfile.write( "push 0\n")
    asmfile.write("""push {0}
jmp interrupt_stubs_common

""".format(i))


asmfile.write("""interrupt_stubs_common:
push ds
push es
push fs
push gs

pushad

mov ax,data_segment_selector
mov es, ax
mov ds, ax
push esp ; trap_frame* parameter
cld  ; C code following the sysV ABI requires DF to be clear on function entry
call handle_interrupt
add esp, 4

trap_return:
popad

pop gs
pop fs
pop es
pop ds

;skip trap_number and err
add esp, 8

iret
""")
        
    
    


