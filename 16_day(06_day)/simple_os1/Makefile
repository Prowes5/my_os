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

bootpack.bim: bootpack.obj naskfunc.obj hankaku.obj graph.obj init_gdt.obj inter.obj
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj hankaku.obj graph.obj init_gdt.obj inter.obj

hankaku.obj: hankaku.bin
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

hankaku.bin: hankaku.txt
	$(MAKEFONT) hankaku.txt hankaku.bin

%.obj: %.nas
	$(NASK) $*.nas $*.obj $*.lst

%.nas: %.gas
	$(GAS2NAS) $*.gas $*.nas

%.gas: %.c
	$(CC1) -o $*.gas $*.c

naskfunc.obj: naskfunc.nas
	$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

run:
	copy hello.img $(PATH)\qemu\fdimage0.bin
	$(PATH)\make.exe -C $(PATH)\qemu

clean:
	del *.bin
	del *.lst
	del *.sys
	del *.obj
	del bootpack.bim
	del bootpack.hrb
	del bootpack.map

src:
	make clean
	make clean_img

clean_img:
	del hello.img