#ifndef _UTIL_FILTER_H_
#define _UTIL_FILTER_H_
//滤波算法
 

//返回众数 scr：原数组  value 最大的数存储位置 
int  majority_fiter(int *scr,int *value,int len);
//取中位数
int  centre_fiter(int *scr,int *value,int len);
//平均滤波
int  average_filter(int *data,int *value,int length);
//中位值平均滤波法 detachLen:过滤长度
int centre_average_filter(int *scr,int *value,int len,int detachLen);


#endif //_AES_H_
