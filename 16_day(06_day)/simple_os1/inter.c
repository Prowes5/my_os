

void io_out8(int port, int addr);


#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

void init_pic() {
    io_out8(PIC0_IMR, 0xff);//禁止所有中断
    io_out8(PIC1_IMR, 0xff);//禁止所有中断

    io_out8(PIC0_ICW1, 0x11);//边沿触发模式
    io_out8(PIC0_ICW2, 0x20);//IRQ0-7接收INT20-27
    io_out8(PIC0_ICW3, 1 << 2);//PIC1由IRQ2连接
    io_out8(PIC0_ICW4, 0x01);//无缓冲区模式

    io_out8(PIC1_ICW1, 0x11);//边沿触发模式
    io_out8(PIC1_ICW2, 0x28);//IRQ8-15接收INT0x28-0x2f
    io_out8(PIC1_ICW3, 2);//PIC1由IRQ2连接
    io_out8(PIC1_ICW4, 0x01);//无缓冲区模式

    io_out8(PIC0_IMR, 0xfb);//除PIC1,2号引脚之外全部禁止
    io_out8(PIC1_IMR, 0xff);//禁止所有中断
}