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
