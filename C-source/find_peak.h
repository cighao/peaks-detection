/*
* 2016-5-30
* 采用 署名-非商业性使用-禁止演绎 3.0 进行许可 
*/

#ifndef	_FIND_PEAK
#define _FIND_PEAK

#include "stdio.h"
#include "stdlib.h" 

#define MAX_SIGNAL_LEN  100000 //输入信号的最大长度 
#define SCALE	0.4  //阈值比例系数 
#define NEIGHBOUR 35  //搜索邻域半径 
#define MAX_SUB_LEN 30 //中值滤波或者均值滤波时子序列最大长度 


//输入数据 
int input_signal(int sig[],FILE *inputfile);

//保存数据 
void output_signal(int sig[],int n,FILE *outputfile);

//均值滤波 
int mean_filter(int in[],int n,int out[],int k);

//中值滤波 
int median_filter(int in[],int n,int out[],int k);

//寻找波峰 
int find_peaks(int sig[],int n1,int peaks[] );

//寻找波谷 
int find_troughs(int sig[],int n1,int troughs[] );

//预处理，使数据做一个线性偏移 
void process(int sig[],int n);

#endif
