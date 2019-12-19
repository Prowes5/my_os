# 07_day

定义了一些关于显示的常量地址，把一些设置值保存到内存中。

`VRAM`地址设置为`0xa0000`,设置显存地址，`0xa0000-0xaffff`这个内存地址对应着相应的显示模式

之后的内容就是关于32位和C语言，这里代码暂时弄不明白，书中也没用介绍，看下`Makefile`的内容。由于这里整本书用的都是作者开发的一套工具，所以与平时的编译过程不大相同，主要大致熟悉一下编译链接这个流程。

	总结下来大致有：
	cc1 bootpack.c ---> bootpack.gas
	gas2nas bootpack.gas ---> bootpack.nas
	nask bootpack.nas ---> bootpack.obj
	obj2bim bootpack.obj ---> bootpack.bim
	bim2hrb bootpack.bim ---> bootpack.hrb

这一系列流程就从C代码变成了二进制文件，这个二进制文件是这个操作系统独有的文件格式。解释一下上述过程。

1. 首先cc1是一个C编译器，第一步就是讲C代码编译会汇编语言
2. 然而cc1编译器生成的是gas，nask不能讲其转换为机器语言，所以通过`gas2nas`转化为nas文件
3. 既然转换为nas文件，下一步就是这几天一直使用的通过nask转换为机器语言
4. 都知道C语言之后需要链接，那么接下来就是链接的过程
5. 最后一步`bim2hrb`就是转换和加工了，关于可执行文件格式的一些问题

`asmhead.nas`的代码就直接复制粘贴了，`Makefile`可以自己写下，顺便优化一下我的Makefile

算了，把注释翻译了看看能不能看懂代码吧