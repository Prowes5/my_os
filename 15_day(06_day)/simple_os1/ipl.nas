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
        mov ss,ax
        mov sp,0x7c00

        mov ax,0x0820            ;read disk
        mov es,ax
        mov cl,2                  ;设置起始读取1号扇区
        mov ch,0                  ;设置0柱面
        mov dh,0                  ;设置DH=0,DL=0(磁头号为0，驱动器为0)

readloop:
        mov si,0

retry:
        mov ax,0x0201             ;设置AH=2,AL=1(读扇区，一个扇区)
        mov dl,0                  ;DL=0,驱动器为0
        mov bx,0                  ;
        int 0x13                  ;中断读磁盘
        jnc next                  ;如果读取没有返回错误信息，则跳转到next
        add si,1                  ;允许错误循环次数
        cmp si,5                  ;
        jae error                  ;如果读取错误超过5次，跳转到error
        mov ah,0
        mov dl,0
        int 0x13
        jmp retry

next:
        mov ax,es                 ;
        add ax,0x20               ;
        mov es,ax                 ;es = es+0x20
        add cl,1                  ;扇区号+1
        cmp cl,18                 ;
        jbe readloop              ;扇区号<=18,跳转到readloop
        mov cl,1                  ;设置扇区号为1，读取其他磁头
        add dh,1                  ;dh加1,切换到1磁头
        cmp dh,2
        jb readloop               ;磁头号<2,跳转到readloop
        mov dh,0                  ;初始化dh为0
        add ch,1                  ;改变柱面号，切换柱面
        cmp ch,10
        jb readloop
  
start:
        mov [0x0ff0],ch
        jmp 0xc200                ;跳转到磁盘文件加载地址


error:
        mov si,msg

putloop:
        mov al,[si]                ;output error message
        cmp al,0
        je fin

        mov ax,0xe
        mov bx,0xf
        int 0x10
        add si,1
        jmp putloop

fin:
        hlt 
        jmp fin  

msg:

        DB      "load error"
        DB      0xa
        DB      0

        RESB	0x7dfe-$
        DB      0x55,0xaa
