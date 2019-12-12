PATH = ..\..\tolset\z_tools

hello.img: ipl.nas
	$(PATH)\nask.exe ipl.nas ipl.bin ipl.lst
	$(PATH)\edimg.exe imgin:$(PATH)\fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 imgout:hello.img

run:
	$(PATH)\make.exe -C $(PATH)\qemu

clean:
	del ipl.bin
	del ipl.lst

src:
	del ipl.bin
	del ipl.lst
	del hello.img

clean_img:
	del hello.img