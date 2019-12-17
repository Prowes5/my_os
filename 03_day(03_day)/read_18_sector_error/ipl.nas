; Author: prowes5
; TAB=4

        ORG     0x7c00			

        JMP     entry
        DB      0x90
        DB      "HARIBOTE"		
        DW      512				 
        DB      1				 
        DW      1				 
        DB      2				 
        DW      224				 
        DW      2880			 
        DB      0xf0			 
        DW      9				 
        DW      18				 
        DW      2				 
        DD      0				 
        DD      2880			 
        DB      0,0,0x29		 
        DD      0xffffffff		 
        DB      "HARIBOTEOS "	 
        DB      "FAT12   "		 
        RESB    18				 

entry:
        mov ax,0
        mov ds,ax                ;init segment registers
        mov sp,0x7c00
        mov ss,ax

        mov ax,0x0820            ;read disk
        mov es,ax
        mov cl,1                  ;设置起始读取2号扇区

readloop:
        mov si,0

retry:
        mov ax,0x0201             ;设置AH=2,AL=1(读扇区，一个扇区)
        mov ch,0                  ;设置0柱面
        mov dx,0                  ;设置DH=0,DL=0(磁头号为0，驱动器为0)
        mov bx,0                  ;
        add si,1                  ;允许错误循环次数
        int 0x13                  ;中断读磁盘
        jnc next                  ;如果读取没有返回错误信息，则跳转到next
        cmp si,5                  ;
        jz error                  ;如果读取错误超过5次，跳转到error
        jmp retry

fin:
        hlt
        jmp fin

error:
        mov si,msg

putloop:
        mov al,[si]                ;output error message
        cmp al,0
        jz fin

        mov ax,0xe
        mov bx,0xf
        int 0x10
        mov si,1
        jmp putloop

next:
        mov ax,es
        add ax,0x20
        mov es,ax
        add cl,1
        cmp cl,18
        jbe readloop 


msg:

        DB      "load error"
        DB      0xa
        DB      0

        RESB	0x7dfe-$
        DB      0x55,0xaa
