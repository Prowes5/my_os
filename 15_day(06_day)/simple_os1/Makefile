PATH = ..\..\tolset\z_tools
INCPATH = ..\..\tolset\z_tools\haribote\

NASK = $(PATH)\nask.exe
MASK = $(PATH)\make.exe
CC1 = $(PATH)\cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NAS = $(PATH)\gas2nask.exe -a
OBJ2BIM = $(PATH)\obj2bim.exe
BIM2HRB = $(PATH)\bim2hrb.exe
RULEFILE = $(PATH)\haribote\haribote.rul
EDIMG = $(PATH)\edimg.exe
MAKEFONT = $(PATH)\makefont.exe
BIN2OBJ = $(PATH)\bin2obj.exe


hello.img: ipl.nas simple.sys
	$(NASK) ipl.nas ipl.bin ipl.lst
	$(EDIMG) imgin:$(PATH)\fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:simple.sys to:@: \
		imgout:hello.img

simple.sys: asmhead.bin bootpack.hrb
	copy /b asmhead.bin+bootpack.hrb simple.sys

asmhead.bin: asmhead.nas
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

bootpack.hrb: bootpack.bim
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

bootpack.bim: bootpack.obj naskfunc.obj hankaku.obj graph.obj init_gdt.obj
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj hankaku.obj graph.obj init_gdt.obj

hankaku.obj: hankaku.bin
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

hankaku.bin: hankaku.txt
	$(MAKEFONT) hankaku.txt hankaku.bin

graph.obj: graph.nas
	$(NASK) graph.nas graph.obj graph.lst

graph.nas: graph.gas
	$(GAS2NAS) graph.gas graph.nas

graph.gas: graph.c
	$(CC1) -o graph.gas graph.c

init_gdt.obj: init_gdt.nas
	$(NASK) init_gdt.nas init_gdt.obj init_gdt.lst

init_gdt.nas: init_gdt.gas
	$(GAS2NAS) init_gdt.gas init_gdt.nas

init_gdt.gas: init_gdt.c
	$(CC1) -o init_gdt.gas init_gdt.c

bootpack.obj: bootpack.nas
	$(NASK) bootpack.nas bootpack.obj bootpack.lst

naskfunc.obj: naskfunc.nas
	$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

bootpack.nas: bootpack.gas
	$(GAS2NAS) bootpack.gas bootpack.nas

bootpack.gas: bootpack.c
	$(CC1) -o bootpack.gas bootpack.c



run:
	copy hello.img $(PATH)\qemu\fdimage0.bin
	$(PATH)\make.exe -C $(PATH)\qemu

clean:
	del *.bin
	del *.lst
	del *.sys
	del *.obj
	del *.gas
	del init_gdt.nas
	del graph.nas
	del bootpack.nas
	del bootpack.bim
	del bootpack.hrb
	del bootpack.map

src:
	make clean
	make clean_img

clean_img:
	del hello.img