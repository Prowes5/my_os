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
