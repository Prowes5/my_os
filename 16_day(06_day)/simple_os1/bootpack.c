#include<stdio.h>

void io_hlt(void);
void write_memory(int addr, int value);
void io_sti(void);


struct BOOTINFO
{
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

void init_colour(void);
void init_desktop(char *vram, int xsize, int ysize);
void putchar_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse(char *color_mouse, unsigned char c);
void putblock(char *vram, int xsize, int bxsize, int bysize, int x0, int y0, char *buf, int pbxsize);

void init_gdt_idt();

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_0000FF		3
#define COL8_FFFF00		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define	COL8_848484		15

void init_pic();

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

	init_pic();

	putchar_asc(bootdata->vram, bootdata->scrnx, 8, 8, COL8_FFFFFF, "test");
	putchar_asc(bootdata->vram, bootdata->scrnx, 8, 25, COL8_FFFFFF, "ABC");
	sprintf(tmp, "scanx = %d", bootdata->scrnx);
	putchar_asc(bootdata->vram, bootdata->scrnx, 8, 40, COL8_FFFFFF, tmp);

	mx = (bootdata->scrnx - 16) / 2;
	my = (bootdata->scrny - 28 - 16) / 2;

	init_mouse(color_mouse, COL8_0000FF);
	putblock(bootdata->vram, bootdata->scrnx, 16, 16, mx, my, color_mouse, 16);

fin:
	io_hlt();
	goto fin;

}




