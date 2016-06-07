/*
* 2016-5-30
* 采用 署名-非商业性使用-禁止演绎 3.0 进行许可 
*/

#include "find_peak.h"

static int get_max(int sig[],int n);
static int get_min(int sig[],int n);
static int is_exist(int a[],int n,int key);
static int find_maximums(int sig[],int n,int maximums[]);
static int get_median(int a[],int n);

/*
* 从inputfile中获取信号序列，返回值是读取信号的数量 
* sig:读取到的信号序列 
*/
int input_signal(int sig[],FILE *inputfile){
	int num=0;
	while(!feof(inputfile)){
		int tmp;
		fscanf(inputfile,"%d",&tmp);
		sig[num++] =  tmp;
		if(num >= MAX_SIGNAL_LEN)
			break;
	}
	return num;
} 

/*
* 将信号输出到文件中
* sig:输出的序列，n:序列长度，outputfile:输出文件 
*/
void output_signal(int sig[],int n,FILE *outputfile){
	int i;
	for(i=0;i<n;i++){
		fprintf(outputfile,"%d",sig[i]);
		if(i<n-1)
			fprintf(outputfile," ");
	}		
}

/*
* 寻找波峰 
* sig：输入序列，n1：序列长度 ，peaks[]:保存波峰的位置 
* 返回值是波峰的数量 
*/
int find_peaks(int sig[],int n1,int peaks[] ){
	int maximums[MAX_SIGNAL_LEN];
	int n2 = find_maximums(sig,n1,maximums);
	int max = get_max(sig,n1);
	int min = get_min(sig,n1);
	double threshold = (max - min)*SCALE;
	int i,j,num=0;
	int found = 0;
	for(i=0;i<n2;i++){
		int index = maximums[i];
		if(max - sig[index] <= threshold)
			found = 1;
		else
			continue;
		for(j=index-NEIGHBOUR;j<=index+NEIGHBOUR;j++){
			if(j<0 || j>=n1)
				continue;
			if(sig[index] < sig[j])
				found = 0;
			if(sig[index] == sig[j] && is_exist(peaks,num,j))
				found = 0;
		}
		if(found == 1)
			peaks[num++] = index;
	}
	return num;
}

/*
* 寻找波谷 
* sig：输入序列，n1：序列长度, troughs[]:保存波谷的位置 
* 返回值是波谷的数量 
* 算法：原始序列的相反数的波峰就是原始序列的波谷 
*/
int find_troughs(int sig[],int n1,int troughs[] ){
	int tmp[MAX_SIGNAL_LEN];
	int i;
	for(i=0;i<n1;i++)
		tmp[i] = sig[i] * -1;
	return find_peaks(tmp,n1,troughs); 
} 

/*
* 均值滤波
* in:输入序列，n:输入序列长度
* out:滤波后序列，k:求均值的子序列长度 
* 返回值是滤波后序列长度 
*/ 
int mean_filter(int in[],int n,int out[],int k){
	if(k > n || k > MAX_SUB_LEN){
		fprintf(stderr,"均值滤波出错，请检查参数\n");
		exit(1);
	}
	int i,num = 0; 
	for(i=0;i<n-k+1;i++){
		int sum = 0;
		int tmp;
		for(tmp=i;tmp<=i+k-1;tmp++)
			sum = sum + in[tmp];
		out[num++] = sum/k;
	}
	return num;
}

/*
* 中值滤波
* in:输入序列，n:输入序列长度
* out:滤波后序列，k:求中值的子序列长度 
* 返回值是滤波后序列长度 
*/ 
int median_filter(int in[],int n,int out[],int k){
	if(k > n || k > MAX_SUB_LEN){
		fprintf(stderr,"中值滤波出错，请检查参数\n");
		exit(1);
	}
	int i,num = 0;
	for(i=0;i<n-k+1;i++){
		int j,index=0;
		int tmp[MAX_SUB_LEN];
		for(j=i;j<=i+k-1;j++){
			tmp[index++] = in[j];
		}
		out[num++] = get_median(tmp,index);
	}
	return num;
}

/*
static int find_maximums(int sig[],int n,int maximums_index[]){
	int i,num=0;
	if(n<=2){
		return num;
	}
	int diff[MAX_SIGNAL_LEN]; //差分
	int tmp=0;
	for(i=1;i<n;i++){
		if(sig[i] - sig[i-1] > 0)
			diff[tmp++] = 1;
		else if(sig[i] - sig[i-1] == 0)
			diff[tmp++] = 0;
		else
			diff[tmp++] = -1;		 
	}
	for(i=1;i<tmp;i++){ //第二次差分求极大值 
		if(diff[i] - diff[i-1] < 0)
			maximums_index[num++] = i; 
	}
	return num;	
}*/

/*
* 寻找sig[]中的极大值点的序号 
* 结果保存在 maximums中，返回的是极值点数量 
*/
static int find_maximums(int sig[],int n,int maximums[]){
	int i,j,num=0;
	for(i=1;i<n-1;i++){
		if(sig[i] <= sig[i-1])   // sig[i-1] >= sig[i] 
			continue;
		if(sig[i] < sig[i+1])   //  sig[i-1] < sig[i] < sig[i+1]
			continue;
		if(sig[i] > sig[i+1]){  //  sig[i-1] < sig[i] > sig[i+1]  
			maximums[num++] = i;
			continue;
		}
		j = i+2;               // sig[i-1] < sig[i] == sig[i+1]
		while(j<n && sig[j] == sig[i]) 
			j++;
		if(j == n){
			break;
		}
		if(sig[i] > sig[j])
			maximums[num++] = i;
		i = j-1;		
	}
	return num;
}

/*
* 预处理，使数据做一个线性偏移 
*/
void process(int sig[],int n){
	int i;
	int max = get_max(sig,n);
	int min = get_min(sig,n);
	if(max - min > 1000)
		return ;
	for(i=0;i<n;i++){
		sig[i] = (sig[i] - min)*1000+200;
	}
} 

/*
* 求序列的最大值 
*/
static int get_max(int sig[],int n){
	int i,max = 0x80000000;
	for(i=0;i<n;i++){
		if(sig[i] > max)
			max = sig[i];
	}
	return max;
} 

/*
* 求序列的最小值 
*/
static int get_min(int sig[],int n){
	int i,min = 0x7FFFFFFF;
	for(i=0;i<n;i++){
		if(sig[i] < min)
			min = sig[i];
	}
	return min;
} 

/*
* 序列中是否存在某个数 
*/
static int is_exist(int a[],int n,int key){
	int i;
	for(i=0;i<n;i++){
		if(a[i] == key)
			return 1;
	}
	return 0;
}

/*
* 求一个序列的中位值,当n为偶数时取中间两个数的平均值 
*/
static int get_median(int a[],int n){
	int i,j,tmp;
	for(i=0;i<n;i++){  //排序 
		for(j=i+1;j<n;j++){
			if(a[i] < a[j]){
				tmp = a[i];
				a[i] = a[j];
				a[j] = a[i];
			}
		}
	}
	if(n%2==1)
		return a[(n-1)/2];
	return (a[n/2] + a[n/2-1])/2;
} 
