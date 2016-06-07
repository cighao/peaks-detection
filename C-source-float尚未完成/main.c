/*
* 2016-5-30
* 采用 署名-非商业性使用-禁止演绎 3.0 进行许可 
*/

#include "find_peak.h"
#include "find_peak.c"

int sig[MAX_SIGNAL_LEN];  //原始数据 
float filter_sig[MAX_SIGNAL_LEN]; //滤波后的数据 
int peaks[MAX_SIGNAL_LEN];   //波峰位置数据 
int troughs[MAX_SIGNAL_LEN];   // 波谷位置 

int main(){
	char inputfile_name[30] = "test-data/y02.dat";
	char outputfile_name[30] = "test-data/out.dat";
	
	int i,n1,n2,n3,n4;
	
	FILE *input_file = fopen(inputfile_name,"r");
	FILE *output_file = fopen(outputfile_name,"w");
	if(input_file == NULL){
		fprintf(stderr,"输入文件不存在\n");
		exit(1);
	}
	
	n1 = input_signal(sig,input_file);  //输入 
	
	n2 = mean_filter(sig,n1,filter_sig,6);  //均值滤波 
	
	output_signal(filter_sig,n2,output_file) ;  //保存滤波后的数据 
	printf("滤波后的数据已经保存在文件 %s 中\n",outputfile_name);
	
	n3 = find_peaks(filter_sig,n2,peaks);  //波峰 
	n4 = find_troughs(filter_sig,n2,troughs);   //波谷 

	
	printf("\n滤波后波峰位置：\n");
	for(i=0;i<n3;i++)
		printf("%d ",peaks[i]+1);  // 下标应该从1开始 
	printf("\n");
	printf("\n滤波后波谷位置：\n");
	for(i=0;i<n4;i++)
		printf("%d ",troughs[i]+1);
	printf("\n");
}
