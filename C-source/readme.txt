
2016-5-30
采用 署名-非商业性使用-禁止演绎 3.0 进行许可 

----------------------------------------------------

此程序已在 windows 下进行测试运行，运行正常。结果符合预期值。
如需要运行在 linux 系统下，只需做较小调整即可。


输入文件说明：
	（2）输入的数据类型必须为整数，文件末尾不能有空格或者空行之类的多余字符。
	（1）两个数据之间用空格或者空行隔开

程序重要参数说明：
	SCALE：	阈值比例系数，当信号各个波峰的幅度相差较大时可以适当加大这个参数
	NEIGHBOUR： 波峰搜索的半径，如果信号周期较大时，应该增加该参数，较小时应该减小该参数。

为了扩大数据之间的差异，对所有输入数据都做了一个线性偏移