;filename: naskfunc

[FORMAT "WCOFF"]                ;制作目标文件的格式
[BITS 32]                       ;制作32位模式用的机器语言
[INSTRSET "i486p"]              ;说明为486指令
[FILE "naskfunc.nas"]           ;源文件名信息

        GLOBAL      _io_hlt,_write_memory     ;程序中的函数名声明

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