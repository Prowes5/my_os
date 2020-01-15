
#include "bootpack.h"
#include "graph.h"
#include "init_gdt.h"
#include "inter.h"
#include<stdio.h>

void io_hlt(void);
void write_memory(int addr, int value);
void io_sti(void);
void io_out8(int port, int addr);

void HariMain(void)
{
	//定义一个BOOTINFO结构体
	struct BOOTINFO *bootdata = (struct BOOTINFO *)0x0ff0;
	char tmp[50];
	char color_mouse[256];
	int mx, my;

	//初始化调色板
	init_colour();
	//初始化桌面
	init_desktop(bootdata->vram, bootdata->scrnx, bootdata->scrny);
	init_gdt_idt();
	//putchar(bootdata->vram, bootdata->scrnx, 0, 0, COL8_FFFFFF, A);

	//初始化PIC
	init_pic();
	io_sti();

	//输出字符到显示器上
	//putchar_asc(bootdata->vram, bootdata->scrnx, 8, 8, COL8_FFFFFF, "test");
	//putchar_asc(bootdata->vram, bootdata->scrnx, 8, 25, COL8_FFFFFF, "ABC");
	sprintf(tmp, "scanx = %d", bootdata->scrnx);
	putchar_asc(bootdata->vram, bootdata->scrnx, 8, 40, COL8_FFFFFF, tmp);

	mx = (bootdata->scrnx - 16) / 2;
	my = (bootdata->scrny - 28 - 16) / 2;

	//初始化鼠标指针
	init_mouse(color_mouse, COL8_0000FF);
	//鼠标指针为一个矩形块，输出矩形块
	putblock(bootdata->vram, bootdata->scrnx, 16, 16, mx, my, color_mouse, 16);

	//输入接收到的中断
	sprintf(tmp, "(%d, %d)", mx, my);
	putchar_asc(bootdata->vram, bootdata->scrnx, 0, 0, COL8_FFFFFF, tmp);

	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);

fin:
	io_hlt();
	goto fin;

}




