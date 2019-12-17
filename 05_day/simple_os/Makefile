PATH = ..\..\tolset\z_tools
NASK = $(PATH)\nask.exe

hello.img: ipl.nas simple.sys
	$(NASK) ipl.nas ipl.bin ipl.lst
	$(PATH)\edimg.exe imgin:$(PATH)\fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:simple.sys to:@: \
		imgout:hello.img

simple.sys: simple.nas
	$(NASK) simple.nas simple.sys simple.lst

run:
	$(PATH)\make.exe -C $(PATH)\qemu

clean:
	del ipl.bin
	del ipl.lst

src:
	del ipl.bin
	del ipl.lst
	del hello.img
	del simple.sys
	del simple.lst

clean_img:
	del hello.img