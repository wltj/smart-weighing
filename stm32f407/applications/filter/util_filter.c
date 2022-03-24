//滤波算法
#include <string.h>
#include <stdlib.h>
#include "util_filter.h"
#include <rtthread.h>


void printfun(int *puf, int len)
{
    int i;

    for(i = 0; i < len; i++)
    {
        printf("%d ", puf[i]);
    }
    printf("\n");
}
//冒泡，从大到小排序 
static int  sort(int *scr,int *des,int length)
{
    int max;
    memset(des,0,length);
    memcpy(des,scr,length*sizeof(int));	
    for(int i=0;i<length;i++)
    {
        for(int j=i;j<length;j++)
        {
            if(des[i] < des[j])
            {
                max = des[j];
                des[j] = des[i];
                des[i] = max;
            }
        }
    }
    return 0;
} 
////返回众数 scr：原数组  value 最大的数存储位置 
//static int  num_select(int *p,int *value,int len)
//{
//    int *buffer[2];
//    int j=0,count=0;
//    int max;
//    int length = len*sizeof(int);
//    buffer[0] = (int *)malloc(length+1); //for number
//    if(buffer[0]==NULL)
//    {
//      return -1;
//    }
//    buffer[1] = (int *)malloc(length); //for number count
//    if(buffer[1]==NULL)
//    {
//      return -1;
//    }
//    memset(buffer[0],-1,length+1);
//    memset(buffer[1],-1,length);
//      
//    for(int i=0,j=0,count=0; i<len;)
//    {
//        if(p[i] == p[1+i])
//        {
//            buffer[0][j] = p[i];
//            count++;
//            buffer[1][j] = count;
//        }
//        else
//        {
//            count = 0;
//            if(buffer[0][j] != -1)
//            j++;	
//        }
//        i++;
//    } 
//    count = 0;
//    max= buffer[1][0];
//    for(int i=0;i<j;i++)
//    { 
//        if(buffer[1][i] > max)
//        {
//          max = buffer[1][i];
//          count = i;
//        }
//    } 
//    for(int i=0;i<=j;i++)
//    {
//        if(max == buffer[1][i])
//        {
//          *value= buffer[0][i];
//          free((void *)buffer[0]);
//  	      free((void *)buffer[1]); 
//          return 0;
//        } 
//    }

//    free((void *)buffer[0]);
//  	free((void *)buffer[1]); 
//    return -1;
//}

/*在有序数组中查找出现最多的元素*/
int find_max_seq(int *a, int n, int *p, int *index)
{
    //int i, elem;
    int i;
    int count = 1, count1 = 1;
    int flag = 1;    /*数组不存在相同的元素标志*/
    *index = 0;
    
    for(i = 0; i < n-1; ) {
        while(a[i]==a[i+1]) {
            count++;
            i++;
            flag = 0;  /*数组存在相同的元素标志*/
        }
        if(count > count1) {
            count1 = count;
            //elem = a[i];
            *index = 0;
            p[*index] = a[i];
            (*index)++;
        }
        else if(count == count1) {
            p[*index] = a[i];
            (*index)++;
        }
        count = 1;
        i++;
    }
    if(flag) {
        *index = 0;
        for(i = 0; i < n; i++) {
             p[*index] = a[i];
       (*index)++;
        }
    }
    //return elem;
    return 0;
}


//返回众数 scr：原数组  value 最大的数存储位置 
//int  majority_fiter(int *scr,int *value,int len)
//{
//    int *buffer=NULL;
//    int length = len*sizeof(int);
//    buffer = (int *)malloc(length); //for numbe
//    if(buffer==NULL)
//    {
//      return -1;
//    }
//    if(len==0)
//    {
//        return -1;
//    }
//    memset(buffer,0,length);
//    sort(scr,buffer, len);//排序
//    int code = num_select(buffer,value,len);//返回众数
//    free((void *)buffer);
//    return code;
//}
//返回众数 scr：原数组  value 最大的数存储位置 
int  majority_fiter_2(int *scr,int len,int *desvalueBuf,int *outCnt)
{
    int *buffer=NULL;
    char *out_buff_p =(char *) desvalueBuf;
    int i;

    int length = len*sizeof(int);
    buffer = (int *)malloc(length); //for numbe
    if(buffer==NULL)
    {
      return -1;
    }
    if(len==0)
    {
        return -1;
    }
    memset(buffer,0,length);
    sort(scr,buffer, len);//排序
  //  printfun(buffer, len);

    find_max_seq(buffer, len, (int *)out_buff_p, outCnt);

    free((void *)buffer);
}

//取中位数
int  centre_fiter(int *scr,int *value,int len)
{
    int *buffer=NULL;
    int length = len*sizeof(int);
    buffer = (int *)malloc(length); //for numbe
    if(buffer==NULL)
    {
      return -1;
    }
    if(len==0)
    {
        return -1;
    }
    memset(buffer,0,length);
    sort(scr,buffer, len);
    *value  = buffer[len/2];
    free((void *)buffer);
    return 0;
}

//平均滤波
int  average_filter(int *data,int *value,int length)
{
    uint32_t add=0;
    int i;
    if(length==0)
    {
        return -1;
    }
    for(i=0;i<length;i++)
    {
        add += data[i];
    }
    *value=add/length; 
    return 0;
}


//中位值平均滤波法 detachLen:过滤长度
int centre_average_filter(int *scr,int *value,int len,int detachLen)
{
    int *buffer=NULL;
    int length = len*sizeof(int);
    buffer = (int *)malloc(length); //for numbe
    if(buffer==NULL)
    {
        return -1;
    }
    if(len==0)
    {
        return -1;
    }
    memset(buffer,0,length);
    sort(scr,buffer, len);
    int filter_sum = 0;
    if(len<=2*detachLen)//不足数量时，先用平均滤波
    {
        for(int i=0;i<len;i++)
        {
            filter_sum += buffer[i];
        }
        *value=filter_sum/len; 
    }
    else 
    {
        // 去除最大最小极值后求平均
        for(int i = detachLen; i < len - detachLen; i++)
        {
            filter_sum += buffer[i];
        }
        *value=filter_sum / (len - 2*detachLen);
    }
    free((void *)buffer);
    return 0;
}



