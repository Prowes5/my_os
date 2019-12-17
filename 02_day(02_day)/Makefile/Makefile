hello.img: ipl.nas
	..\..\tolset\z_tools\nask.exe ipl.nas ipl.bin ipl.lst
	..\..\tolset\z_tools\edimg.exe imgin:..\..\tolset\z_tools\fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 imgout:hello.img

run:
	copy hello.img ..\..\tolset\z_tools\fdimage0.bin
	..\..\tolset\z_tools\make.exe -C ..\..\tolset\z_tools\qemu

clean:
	del ipl.bin
	del ipl.lst

src:
	del ipl.bin
	del ipl.lst
	del hello.img

clean_img:
	del hello.img