void io_hlt(void);
void write_memory(int addr, int value);

void HariMain(void)
{
	int i;
	char *p;
	for(i = 0xa0000; i <= 0xaffff; i++) {
		//write_memory(i, 0xf & i);
		p  = (char *)i;				//(char *)进行类型转换，防止编译器报警告
		*p = i & 0xf;
	}

fin:
	io_hlt();
	goto fin;

}
