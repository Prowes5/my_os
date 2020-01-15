
#include "inter.h"
#include "graph.h"
#include "bootpack.h"

void io_out8(int port, int addr);
void io_hlt();

//初始化PIC
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


//定义键盘中断
void inthandler21(int *esp) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putchar_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, " INT 21 (IRQ-1) : PS/2 keyboard");

    for(;;) {
        io_hlt();
    }
}


//定义鼠标中断
void inthandler2c(int *esp) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putchar_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, " INT 2C (IRQ-1) : PS/2 keyboard");
    
    for(;;) {
        io_hlt();
    }
}

void inthandler27(int *esp) {
    io_out8(PIC0_OCW2, 0x67);

}
