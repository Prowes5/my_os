;filename: naskfunc

[FORMAT "WCOFF"]                ;制作目标文件的格式
[BITS 32]                       ;制作32位模式用的机器语言
[INSTRSET "i486p"]              ;说明为486指令
[FILE "naskfunc.nas"]           ;源文件名信息

        GLOBAL      _io_hlt,_write_memory,_io_sti,_io_stihlt     ;程序中的函数名声明
        GLOBAL      _io_in8,_io_in16,_io_in32,_io_out8
        GLOBAL      _io_out16,_io_out32,_io_load_eflags
        GLOBAL      _io_store_eflags,_io_cli,_load_gdtr,_load_idtr

;函数具体定义

[SECTION .text]

;void io_hlt(void)
_io_hlt:

        hlt
        ret

;write_memory(int addr, int value)
_write_memory:
        mov     ecx, [esp+4]
        mov     al, [esp+8]
        mov     [ecx], al
        ret

;void io_cli(void);
_io_cli:
        cli
        ret

;void io_sti(void);
_io_sti:
        sti
        ret

;void io_stihlt(void);
_io_stihlt:
        sti
        hlt
        ret

;int io_in8(int port);
_io_in8:
        mov     edx,[esp+4]
        mov     eax,0
        in      al,dx
        ret

;int io_in16(int port);
_io_in16:
        mov     edx,[esp+4]
        mov     eax,0
        in      ax,dx
        ret

;int io_in32(int port);
_io_in32:
        mov     edx,[esp+4]
        mov     eax,0
        in      eax,dx
        ret

;void io_out8(int port, int data);
_io_out8:
        mov     edx,[esp+4]
        mov     al,[esp+8]
        out     dx,al
        ret

;void io_out16(int port, int data);
_io_out16:
        mov     edx,[esp+4]
        mov     al,[esp+8]
        out     dx,ax
        ret

;void io_out32(int port, int data);
_io_out32:
        mov     edx,[esp+4]
        mov     eax,[esp+8]
        out     dx,eax
        ret

;int io_load_eflags(void);
_io_load_eflags:
        pushfd
        pop     eax
        ret

;void io_store_eflags(int eflags);
_io_store_eflags:
        mov     eax,[esp+4]
        push    eax
        popfd   
        ret

;void load_gdtr(int limit, int addr);
_load_gdtr:
        mov     ax, [esp+4]
        mov     [esp+6], ax
        lgdt    [esp+6]
        ret

;void load_idtr(int limit, int addr);
_load_idtr:
        mov     ax, [esp+4]
        mov     [esp+6], ax
        lidt    [esp+6]
        ret