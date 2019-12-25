void io_hlt(void);
void write_memory(int addr, int value);

void HariMain(void)
{
	int i;
	for(i = 0xa0000; i <= 0xaffff; i++) {
		write_memory(i, 0xf & i);
	}

fin:
	io_hlt();
	goto fin;

}
