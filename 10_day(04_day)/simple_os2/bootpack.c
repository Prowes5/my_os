void io_hlt(void);
void write_memory(int addr, int value);
void io_sti(void);
void io_out8(int port, int data);
void io_cli(void);
void io_store_eflags(int eflags);
int io_load_eflags(void);

void init_colour(void);
void set_colour(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

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
	char *p = (char *)0xa0000;

	int xsize = 320;
	int ysize = 200;
	init_colour();

	//主背景色
	boxfill8(p, xsize, COL8_0000FF, 0, 0, xsize, ysize);

	//任务栏
	boxfill8(p, xsize, COL8_C6C6C6, 0, ysize-15, xsize, ysize);
	//任务栏阴影

	boxfill8(p, xsize, COL8_FFFFFF, 0, ysize-15, xsize, ysize-15);

	//boxfill8(p, xsize, COL8_000000, 0, ysize-15+1, xsize, ysize-15+1);


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
