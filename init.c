#include<stc15.h>
void Interrput_Init()
{	
	IT1 = 0;                    //����INT1���ж����� (1:���½��� 0:�����غ��½���)
    EX1 = 1;                    //ʹ��INT1�ж�
	EA = 1;
}
void Timer2Init(void)		//1����@11.0592MHz
{
	AUXR &= 0xFB;		//��ʱ��ʱ��12Tģʽ
	T2L = 0x66;		//���ö�ʱ��ֵ
	T2H = 0xFC;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	IE2 |= 0x04;
	EA = 1;
}

