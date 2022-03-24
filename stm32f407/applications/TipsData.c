#include "TipsData.h"
#include "led_com.h"

////���򼽽����ɼ��ڻ�����������³ԥ�����������崨ǭ����¸������¸۰�̨
const unsigned short ProvinceAbbreviationCode[]={
	0x4EAC,0x6D25,0x5180,0x664B,0x8499,0x8FBD,0x5409,0x9ED1,0x6CAA,0x82CF,
	0x6D59,0x7696,0x95FD,0x8D63,0x9C81,0x8C6B,0x9102,0x6E58,0x7CA4,0x6842,
	0x743C,0x6E1D,0x5DDD,0x9ED4,0x6EC7,0x85CF,0x9655,0x7518,0x9752,0x5B81,
	0x65B0,0x6E2F,0x6FB3,0x53F0
};
 
////

//��ȡʡ����ProvinceAbbreviationCode�е�λ�� ����0xffʧ��
unsigned char getProvinceIndex(unsigned char * data)
{
	 unsigned char i=0;
	 unsigned short hexData=0; 
	 hexData=charToShortHex(data);
	 for(i=0;i<sizeof(ProvinceAbbreviationCode);i++)
	{
		if(ProvinceAbbreviationCode[i]==hexData)
		{
			return i;
		}
	}
	return 0xff;
} 	
unsigned char getLetterIndex(unsigned char * data)
{
	unsigned char i=0;
	 unsigned short hexData=0; 
	 hexData=charToShortHex(data);
	 if((hexData>='A')&&(hexData<='Z'))
	 {
			return hexData-'A';
	 }
	 	 if((hexData>='a')&&(hexData<='z'))
	 {
			return hexData-'a';
	 }
	 return 0xff;
}
unsigned char getNumberIndex(unsigned char * data)
{
	unsigned char i=0;
	 unsigned short hexData=0; 
	 hexData=charToShortHex(data);
	 if((hexData>='0')&&(hexData<='9'))
	 {
			return hexData-'0';
	 } 
	 return 0xff;
}


unsigned short charToShortHex(unsigned char * chars)
{
	unsigned short hexData=0;
	 unsigned char temp=0;
	 temp=Char_To_Hex(chars);
	 hexData=temp<<8;
	 temp=Char_To_Hex(chars+2);	
	 hexData|=temp;
	 return hexData;
}
char Char_To_Hex(unsigned char * chars)
{
    unsigned char h = 0;
    unsigned char l = 0;
    
    unsigned char t = chars[0];
    if(t >= '0' && t <= '9')
    {
        h = t - '0';
    }
    else if(t >= 'A' && t <= 'F')
    {
        h = t - 'A' + 10;
    }
    else if(t >= 'a' && t <= 'f')
    {
        h = t - 'a' + 10;
    }
    
    t = chars[1];
    if(t >= '0' && t <= '9')
    {
        l = t - '0';
    }
    else if(t >= 'A' && t <= 'F')
    {
        l = t - 'A' + 10;
    }
    else if(t >= 'a' && t <= 'f')
    {
        l = t - 'a' + 10;
    }
    unsigned char ret = (h << 4) + l;
    return ret;
}

void String_To_Unicode_To_Hex(unsigned char *str ,unsigned char *data_hex,unsigned short len)
{
    unsigned int i = 0;
    for(;i<len;i++)
    {
        data_hex[i] = Char_To_Hex(&str[i*2]);
    }
    
    for(i=0;i<len;i+=2)
    {
        unsigned char x = 0;
        x = data_hex[i];
        data_hex[i] = data_hex[i+1];
        data_hex[i+1] = x;
    }
}

void Unicode_8_To_Hex(unsigned char *data_unicode,unsigned char *data_hex,unsigned int len)
{
    unsigned int i = 0;
    for(;i<len;i+=2)
    {
        data_hex[i] = data_unicode[i+1];
        data_hex[i+1] = data_unicode[i];
    }
        
}
void Unicode_16_To_Hex(unsigned short *data_unicode,unsigned char *data_hex,unsigned int len)
{
    unsigned int i = 0;
    for(;i<len;i+=2)
    {
        data_hex[i] = data_unicode[i/2];
        data_hex[i+1] = data_unicode[i/2]>>8;
    }
        
}
//0  ��ʾ: ����δ��λ�����ƶ�
//1  ��ʾ������δ��λ��������/���ⷽ���ƶ�
//2  ��ʾ������δ��λ��������/���ⷽ���ƶ�����1��ͬ��ʱ����ʾ����/����������෴(�ڵ���������)
//3  ��ʾ������δ��λ���볯��/���ƶ�
//4  ��ʾ������δ��λ���볯��/���ƶ�����3��ͬ��ʱ����ʾ�����������෴(�ڵ���������)
//5  ��ʾ������δ��λ���볯��/���ƶ�
//6  ��ʾ������δ��λ���볯��/���ƶ�����5��ͬ��ʱ����ʾ�ϱ��������෴(�ڵ���������) 
void getLocationErrorStr( char *buff,unsigned long len,unsigned char type,unsigned char status)
{
		rt_memset(buff,0,len);
		switch(type)
		{
			case 0:
				rt_memcpy(buff,TD_MOVE,rt_strlen(TD_MOVE));
				break;
			//-------------------------------------------
			case 1:
				if(status==1)
				{
					rt_sprintf(buff,TD_MOVE1);
				}
				else
				{
					rt_sprintf(buff,TD_MOVE2);
				}
				break;
			case 2:
				if(status==1)
				{
					rt_sprintf(buff,TD_MOVE2);
				}
				else
				{
					rt_sprintf(buff,TD_MOVE1);
				}
				break;
					//-------------------------------------------
			case 3:
				if(status==1)
				{
					rt_sprintf(buff,TD_MOVE3);
				}
				else
				{
					rt_sprintf(buff,TD_MOVE4);
				}
				break;
			case 4:
				if(status==1)
				{
					rt_sprintf(buff,TD_MOVE4);
				}
				else
				{
					rt_sprintf(buff,TD_MOVE3);
				}
				break;
						//-------------------------------------------
			case 5:
				if(status==1)
				{
					rt_sprintf(buff,TD_MOVE5);
				}
				else
				{
					rt_sprintf(buff,TD_MOVE6);
				}
				break;
			case 6:
				if(status==1)
				{
					rt_sprintf(buff,TD_MOVE6);
				}
				else
				{
					rt_sprintf(buff,TD_MOVE5);
				}
				break;
		}
}