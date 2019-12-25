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

bootpack.bim: bootpack.obj naskfunc.obj
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj

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
	del ipl.bin
	del ipl.lst

src:
	del *.bin
	del *.lst
	del hello.img
	del *.sys
	del *.obj
	del bootpack.gas
	del bootpack.nas
	del bootpack.bim
	del bootpack.hrb
	del bootpack.map

clean_img:
	del hello.img