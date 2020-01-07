
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

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

void setsegment(struct GLOBAL_DESC_TABLE *gdt, unsigned int length, int base, int access);
void setgatedesc(struct GATE_DESC *idt, int offset, int selector, int access);
void init_gdt_idt();


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
