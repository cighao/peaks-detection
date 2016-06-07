/*
* 2016-5-30
* 采用 署名-非商业性使用-禁止演绎 3.0 进行许可
*/

#include "find_peak.h"
#include "math.h"

#define IS_EQUAL(a,b) (fabs((a)-(b))<0.0000001)

static float get_max(float sig[],int n);
static float get_min(float sig[],int n);
static int is_exist(int a[],int n,int key);
static int find_maximums(float sig[],int n,int maximums[]);
static float get_median(float a[],int n);
static int compare(float a,float b);


/*
* 从inputfile中获取信号序列，返回值是读取信号的数量 
* sig:读取到的信号序列 
*/
int input_signal(int sig[],FILE *inputfile){
	int i=0;
	while(!feof(inputfile)){
		int tmp;
		fscanf(inputfile,"%d",&tmp);
		sig[i++] =  tmp;
		if(i>= MAX_SIGNAL_LEN)
			break;
	}
	return i;
} 

/*
* 将信号输出到文件中
* sig:输出的序列，n:序列长度，outputfile:输出文件 
*/
void output_signal(float sig[],int n,FILE *outputfile){
	int i;
	for(i=0;i<n;i++){
		fprintf(outputfile,"%.4f",sig[i]);
		if(i<n-1)
			fprintf(outputfile," ");
	}		
}

/*
* 寻找波峰 
* sig：输入序列，n1：序列长度 ，peaks[]:保存波峰的位置 
* 返回值是波峰的数量 
*/
int find_peaks(float sig[],int n1,int peaks[] ){
	int maximums[MAX_SIGNAL_LEN];
	int n2 = find_maximums(sig,n1,maximums);
	float max = get_max(sig,n1);
	float min = get_min(sig,n1);
	float threshold = (max - min)*SCALE;
	int i,j,num=0;
	int found = 0;
	printf("%f %f\n",max,min);
	for(i=0;i<n2;i++){
		int index = maximums[i];
		printf("%d\n",index+1);
		if(max - sig[index] <= threshold)
			found = 1;
		else
			continue;
		for(j=index-NEIGHBOUR;j<index+NEIGHBOUR;j++){
			if(j<0 || j>=n1)
				continue;
			if(compare(sig[index] , sig[j]) == -1)
				found = 0;
			if(compare(sig[index] , sig[j]) == 0 && is_exist(peaks,num,j))
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
int find_troughs(float sig[],int n1,int troughs[] ){
	float tmp[MAX_SIGNAL_LEN];
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
int mean_filter(int in[],int n,float out[],int k){
	if(k > n || k > MAX_SUB_LEN){
		fprintf(stderr,"均值滤波出错，请检查参数\n");
		exit(1);
	}
	int i,num = 0; 
	for(i=0;i<n-k+1;i++){
		float sum = 0.0;
		int tmp;
		for(tmp=i;tmp<=i+k-1;tmp++)
			sum = sum + in[tmp];
		out[num++] = sum/(k*1.0);
	}
	return num;
}

/*
* 中值滤波
* in:输入序列，n:输入序列长度
* out:滤波后序列，k:求中值的子序列长度 
* 返回值是滤波后序列长度 
*/ 
int median_filter(int in[],int n,float out[],int k){
	if(k > n || k > MAX_SUB_LEN){
		fprintf(stderr,"中值滤波出错，请检查参数\n");
		exit(1);
	}
	int i,num = 0;
	for(i=0;i<n-k+1;i++){
		int j,index=0;
		float tmp[MAX_SUB_LEN];
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
static int find_maximums(float sig[],int n,int maximums[]){
	int i,j,num=0;
	for(i=1;i<n-1;i++){
		if(compare(sig[i],sig[i-1]) <= 0)   // sig[i-1] >= sig[i] 
			continue;
		if(compare(sig[i] , sig[i+1]) < 0 )   //  sig[i-1] < sig[i] < sig[i+1]
			continue;
		if(compare(sig[i] , sig[i+1]) > 0){  //  sig[i-1] < sig[i] > sig[i+1]  
			maximums[num++] = i;
			continue;
		}
		j = i+2;               // sig[i-1] < sig[i] == sig[i+1]
		while(j<n && compare(sig[i],sig[j])!=0) 
			j++;
		if(j == n){
			break;
		}
		if(compare(sig[i],sig[j])>0)
			maximums[num++] = i;
		i = j-1;		
	}
	return num;
}

/*
* 求序列的最大值 
*/
static float get_max(float sig[],int n){
	int i;
	float max = -10000000.0;
	for(i=0;i<n;i++){
		if(sig[i] > max)
			max = sig[i];
	}
	return max;
} 

/*
* 求序列的最小值 
*/
static float get_min(float sig[],int n){
	int i;
	float min = 10000000.0;
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
static float get_median(float a[],int n){
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
	return (a[n/2] + a[n/2-1])/(2*1.0);
}

/*
* 两个浮点数的比较 
*/
static int compare(float a,float b){
	if(a - b > 0.000001)
		return 1;
	if(a - b < -0.000001)
		return -1;
	return 0;
} 
