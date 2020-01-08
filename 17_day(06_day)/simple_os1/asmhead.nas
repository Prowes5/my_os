;tab=4

BOTPAK  EQU     0x00280000              ;bookpack的装载地址
DSKCAC  EQU     0x00100000              ;磁盘缓存的地址
DSKCAC0 EQU     0x00008000              ;实模式磁盘缓存地址（这里不就是MBR之后的那个扇区？）


CYLS    EQU     0x0ff0                  ;
LEDS    EQU     0x0ff1
VMODE   EQU     0x0ff2                  ;设置显示模式
SCRNX   EQU     0x0ff4                  ;分辨率X
SCRNY   EQU     0x0ff6                  ;分辨率y
VRAM    EQU     0x0ff8                  ;图像缓冲区

        ORG     0xc200                  ;

;设置一下显卡模式

        mov     ax,0
        mov     es,ax
        mov     ds,ax
        mov     ss,ax
        mov     al,0x13                 ;设置显示模式为256色
        mov     ah,0x00                 ;设置显示模式
        int     0x10
        mov     byte [VMODE],8          ;
        mov     word [SCRNX],320
        mov     word [SCRNY],200
        mov     dword [VRAM],0x000a0000

;获取键盘LED状态

        mov     ah,0x02                 ;AH=00h 获取键盘的状态
        int     0x16
        mov     [LEDS],al

;(这里暂时还不懂啥意思)

        mov     al,0xff
        out     0x21,al
        nop
        out     0xa1,al

        cli                             ;cli指令为禁止所有中断发生，只有内核态可用

;（没懂）

        call    waitkbdout
        mov     al,0xd1
        out     0x64,al
        call    waitkbdout
        mov     al,0xdf
        out     0x60,al
        call    waitkbdout


;切换到保护模式

[INSTRSET "i486p"]

        LGDT    [GDTR0]
        mov     eax,cr0
        and     eax,0x7fffffff
        or      eax,0x00000001
        mov     cr0,eax
        jmp     pipelineflush

pipelineflush:

        mov     ax,1*8
        mov     ds,ax
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        mov     ss,ax

;
        mov     esi,bootpack
        mov     edi,BOTPAK
        mov     ecx,512*1024/4
        call    memcpy

;
        mov     esi,0x7c00
        mov     edi,DSKCAC
        mov     ecx,512/4
        call    memcpy
        
;

        mov     esi,DSKCAC0+512
        mov     edi,DSKCAC+512
        mov     ecx,0
        mov     cl,byte [CYLS]
        imul    ecx,512*18*2/4
        sub     ecx,512/4
        call    memcpy

;启动bootpack

        mov     ebx,BOTPAK
        mov     ecx,[ebx+16]
        add     ecx,3
        shr     ecx,2
        jz      skip
        mov     esi,[ebx+20]
        add     esi,ebx
        mov     edi,[ebx+12]
        call    memcpy

skip:

        mov     esp,[ebx+12]
        jmp     dword 2*8:0x0000001b

waitkbdout:

        in      al,0x64
        and     al,0x02
        jnz     waitkbdout
        ret



memcpy:

        mov     eax,[esi]
        add     esi,4
        mov     [edi],eax
        add     edi,4
        sub     ecx,1
        jnz     memcpy
        ret

        ALIGNB  16

GDT0:

        RESB    8
        DW      0xffff,0x0000,0x9200,0x00cf
        DW      0xffff,0x0000,0x9a28,0x0047

        DW      0

GDTR0:

        DW      8*3-1
        DD      GDT0

        ALIGNB  16
bootpack: