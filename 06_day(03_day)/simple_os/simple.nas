
        ORG 0xc200

        ;mov ax,0
        ;mov es,ax
        ;mov ds,ax
        ;mov ss,ax
        mov ah,0x00             ;设置显示模式
        mov al,0x13             ;设置显示模式为256色
        int 0x10
fin:

        hlt
        jmp fin