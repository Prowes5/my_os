
#include "init_gdt.h"
#include "bootpack.h"

void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);


//初始化gdt全局描述表和idt中断表
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

	//设定idt
	setgatedesc(idt + 0x21, (int)asm_inthandler21, 2 << 3, AR_INTGATE32);//键盘的中断
	setgatedesc(idt + 0x27, (int)asm_inthandler27, 2 << 3, AR_INTGATE32);
	setgatedesc(idt + 0x2c, (int)asm_inthandler2c, 2 << 3, AR_INTGATE32);//鼠标的中断
}

/*
*设置各个段到GDT中
*1. gdt: 全局描述表
*2. length: 段的大小
*3. base: 段的基地址
*4. access: 段的一些属性
*/
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


/*
*设置idt表
*1. idt: 对应的idt项
*2. offset: 中断发生时需要调用的函数
*3. selector: 表示处理函数属于哪一段（注：低三位不能用）
*4. access: IDT的属性
*/
void setgatedesc(struct GATE_DESC *idt, int offset, int selector, int access) {
	idt->offest_low = offset & 0xffff;
	idt->selector = selector;
	idt->dw_count = (access >> 8) & 0xff;
	idt->access_right = access & 0xff;
	idt->offset_high = (offset >> 16) & 0xffff;
}
