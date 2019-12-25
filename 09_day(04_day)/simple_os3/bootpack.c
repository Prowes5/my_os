void io_hlt(void);
void write_memory(int addr, int value);
void io_sti(void);
void io_out8(int port, int data);
void io_cli(void);
void io_store_eflags(int eflags);
int io_load_eflags(void);

void init_colour(void);
void set_colour(int start, int end, unsigned char *rgb);


void HariMain(void)
{
	int i;
	char *p = (char *)0xa0000;
	for(i = 0; i <= 0xffff; i++) {
		//write_memory(i, 0xf & i);
		//p  = (char *)i;				//(char *)进行类型转换，防止编译器报警告
		p[i] = i & 0xf;
	}

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
		0x00, 0x00, 0x00,		//亮蓝
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