#include<stdio.h>

void io_hlt(void);
void write_memory(int addr, int value);
void io_sti(void);
void io_out8(int port, int data);
void io_cli(void);
void io_store_eflags(int eflags);
int io_load_eflags(void);

void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

struct BOOTINFO
{
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

struct GLOBAL_DESC_TABLE {
	short length_low, base_low;
	char base_mid, access_right;
	char length_high, base_high;
};

struct GATE_DESC {
	short offest_low, selector;
	char dw_count, access_right;
	short offset_high;
};

void init_colour(void);
void set_colour(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void create_hollow_button(char *vram, int xsize, int x0, int y0, int length, int width, unsigned char c);
void create_raised_button(char *vram, int xsize, int x0, int y0, int length, int width, unsigned char c);
void init_desktop(char *vram, int xsize, int ysize);
void init_mouse(char *color_mouse, unsigned char c);

void putchar(char *vram, int xsize, int x, int y, char c, char *font);
void putchar_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void putblock(char *vram, int xsize, int bxsize, int bysize, int x0, int y0, char *buf, int pbxsize);

void setsegment(struct GLOBAL_DESC_TABLE *gdt, unsigned int length, int base, int access);
void setgatedesc(struct GATE_DESC *idt, int offset, int selector, int access);
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

//通过RGB像素点初始化颜色
void init_colour(void) {
	static unsigned char colors[16*3] = {
		0x00, 0x00, 0x00,		//黑色
		0xff, 0x00,	0x00,		//亮红
		0x00, 0xff, 0x00,		//亮绿
		0x00, 0x00, 0xff,		//亮蓝
		0xff, 0xff, 0x00,		//亮黄
		0xff, 0x00, 0xff,		//亮紫
		0x00, 0xff, 0xff,		//浅亮蓝
		0xff, 0xff, 0xff,		//白色
		0xc6, 0xc6, 0xc6,		//亮灰
		0x84, 0x00, 0x00,		//暗红
		0x00, 0x84, 0x00,		//暗绿
		0x84, 0x84, 0x00,		//暗黄
		0x00, 0x00, 0x84,		//暗蓝
		0x84, 0x00, 0x84,		//暗紫
		0x00, 0x84, 0x84,		//浅暗蓝
		0x84, 0x84, 0x84		//暗灰
	};
	set_colour(0, 15, colors);
}

void set_colour(int start, int end, unsigned char *rgb) {
	int i, eflags;
	eflags = io_load_eflags();
	io_cli();
	io_out8(0x03c8, start);
	for(i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);
}

/*
*1.vram:显存地址
*2.xsize:固定常数
*3.c:要显示的颜色
*4.x0,y0:矩形左上角坐标
*5.x1,y1:矩形右下角坐标
*/
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1) {
	int x, y;
	for(y = y0; y <= y1; y++) {
		for(x = x0; x <= x1; x++) {
			vram[y * xsize + x] = c;
		}
	}
}

/*
*创建一个凹陷效果的button
*1.xsize:分辨率常数
*2.x0,y0:button起始地址
*3.length,width：button长和宽
*4.c:颜色
*/

void create_hollow_button(char *vram, int xsize, int x0, int y0, int length, int width, unsigned char c) {
	boxfill8(vram, xsize, c, x0, y0, x0+length-2, y0+width-2);
	//上边缘
	boxfill8(vram, xsize, COL8_848484, x0, y0, x0+length-2, y0);
	//左边缘
	boxfill8(vram, xsize, COL8_848484, x0, y0, x0, y0+width-2);
	//下边缘
	boxfill8(vram, xsize, COL8_FFFFFF, x0, y0+width-2, x0+length-2, y0+width-2);
	//右边缘
	boxfill8(vram, xsize, COL8_FFFFFF, x0+length-2, y0, x0+length-2, y0+width-2);
}

/*
*创建一个凸出效果的button
*1.xsize:分辨率常数
*2.x0,y0:button起始地址
*3.length,width：button长和宽
*4.c:颜色
*/

void create_raised_button(char *vram, int xsize, int x0, int y0, int length, int width, unsigned char c) {
	boxfill8(vram, xsize, c, x0, y0, x0+length-2, y0+width-2);
	//上边缘
	boxfill8(vram, xsize, COL8_FFFFFF, x0, y0, x0+length-2, y0);
	//左边缘
	boxfill8(vram, xsize, COL8_FFFFFF, x0, y0, x0, y0+width-2);
	//下边缘
	boxfill8(vram, xsize, COL8_848484, x0, y0+width-2, x0+length-2, y0+width-2);
	//右边缘
	boxfill8(vram, xsize, COL8_848484, x0+length-2, y0, x0+length-2, y0+width-2);
}

void init_desktop(char *vram, int xsize, int ysize) {
	//主背景色
	boxfill8(vram, xsize, COL8_0000FF, 0, 0, xsize, ysize);

	//任务栏
	boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize-15, xsize, ysize);
	
	//任务栏阴影
	boxfill8(vram, xsize, COL8_FFFFFF, 0, ysize-15, xsize, ysize-15);
	
	//左下角按钮
	create_raised_button(vram, xsize, 0, ysize-13, 30, 13, COL8_C6C6C6);

	//通知栏
	create_hollow_button(vram, xsize, xsize-40, ysize-13, 40, 13, COL8_C6C6C6);
}

/*
*1.vram: 显卡地址
*2.xsize: 分辨率常数
*3.x,y: 起始位置
*4.c: 颜色
*5.font: 字母像素数组
*/
void putchar(char *vram, int xsize, int x, int y, char c, char *font) {
	int i;
	char d;
	for(i = 0; i < 16; i++) {
		d = font[i];
		if ((d & 0x80) != 0)
			vram[(y + i) * xsize + x + 0] = c;
		if ((d & 0x40) != 0)
			vram[(y + i) * xsize + x + 1] = c;
		if ((d & 0x20) != 0)
			vram[(y + i) * xsize + x + 2] = c;
		if ((d & 0x10) != 0)
			vram[(y + i) * xsize + x + 3] = c;
		if ((d & 0x08) != 0)
			vram[(y + i) * xsize + x + 4] = c;
		if ((d & 0x04) != 0)
			vram[(y + i) * xsize + x + 5] = c;
		if ((d & 0x02) != 0)
			vram[(y + i) * xsize + x + 6] = c;
		if ((d & 0x01) != 0)
			vram[(y + i) * xsize + x + 7] = c;
	}
}
/*
*封装输出字符串函数
*1.vram: 显卡地址
*2.xsize: 分辨率常数
*3.x,y: 起始位置
*4.c: 颜色
*5.font: 字符串
*/
void putchar_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s) {
	extern char hankaku[4096];
	for(; *s != 0; s++) {
		putchar(vram, xsize, x, y, c, hankaku + *s * 16);
		x += 8;
	}
}

void init_mouse(char *color_mouse, unsigned char c) {
	static char mouse[16][16] = {
		"**..............",
		"*O*.............",
		"*OO*............",
		"*OOO*...........",
		"*OOOO*..........",
		"*OOOOO*.........",
		"*OOOOOO*........",
		"*OOOOO*O*.......",
		"*O*OOO*.*.......",
		"**.*OO*.........",
		"*...***.........",
		"................",
		"................",
		"................",
		"................",
		"................"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if(mouse[y][x] == '*')
				color_mouse[y * 16 + x] = COL8_000000;
			else if(mouse[y][x] == 'O')
				color_mouse[y * 16 + x] = COL8_FFFFFF;
			else if(mouse[y][x] == '.') 
				color_mouse[y * 16 + x] = c;
		}
	}
}

/*
*1.vram: 显存地址
*2.xsize: 分辨率
*3.bxsize：块长度
*4.bysize：块宽度
*5.x0：块初始位置x坐标
*6.y0：块初始位置y坐标
*7.buf：颜色数组
*8.pbxsize：每一行元素的个数
*/

void putblock(char *vram, int xsize, int bxsize, int bysize, int x0, int y0, char *buf, int pbxsize) {
	int x, y;
	for (y = 0; y < bysize; y++) {
		for (x = 0; x < bxsize; x++) {
			vram[(y0 + y) * xsize + (x0 + x)] = buf[y * pbxsize + x];
		}
	}
}

void init_gdt_idt() {
	struct GLOBAL_DESC_TABLE *gdt = (struct GLOBAL_DESC_TABLE *) 0x270000;
	struct GATE_DESC *idt = (struct GATE_DESC *) 0x26f800;
	int i;
	
	for (i = 0; i < 8192; i++) {
		setsegment(gdt + i, 0, 0, 0);
	}
	setsegment(gdt + 1, 0xffffffff, 0x0, 0x4092);
	setsegment(gdt + 2, 0x7ffff, 0x280000, 0x409a);
	load_gdtr(0xffff, 0x270000);

	for (i = 0; i < 256; i++) {
		setgatedesc(idt + i, 0, 0, 0);
	}
	load_idtr(0x7ff, 0x26f800);
}

void setsegment(struct GLOBAL_DESC_TABLE *gdt, unsigned int length, int base, int access) {
	if (length > 0xfffff) {
		access |= 0x8000;
		length /= 0x1000;
	}
	gdt->length_low = length & 0xffff;
	gdt->base_low = base & 0xffff;
	gdt->base_mid = (base >> 16) & 0xff;
	gdt->access_right = access & 0xff;
	gdt->length_high = ((access >> 16) & 0xf) | ((access >> 8) & 0xf0);
	gdt->base_high = (access >> 24) & 0xff;
}

void setgatedesc(struct GATE_DESC *idt, int offset, int selector, int access) {
	idt->offest_low = offset & 0xffff;
	idt->selector = selector;
	idt->dw_count = (access >> 8) & 0xff;
	idt->access_right = access & 0xff;
	idt->offset_high = (offset >> 16) & 0xffff;
}

