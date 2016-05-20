#include<stc15.h>
#include<init.h>
#include<uart.h>
#include<string.h>
#include<DHT11.h>
bit GasFlag;
unsigned char Temp = 0, Wet = 0;
void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void UART_Action(unsigned char *dat, unsigned char len)
{	
	unsigned char str[6] = {0};
//UART_SendString(dat, len);
	if(len <= 1)
		return;

	if(len >= 3 && strncmp(dat, "S:G", 3) == 0)
	{
		UART_SendString("M:S.G:", 5);
		if(GasFlag)
			UART_SendString("Y\n", 2);
		else UART_SendString("N\n", 2);
	}
	else if(len >= 3 && strncmp(dat, "S:R", 3) == 0)
	{
		UART_SendString("M:S.R:", 5);
		if(P35)
			UART_SendString("Y\n", 2);
		else UART_SendString("N\n", 2);
	}
	else if(len >= 3 && strncmp(dat, "S:T", 3) == 0)
	{
		if(Wet == 0 || Temp == 0)
			return ;
		UART_SendString("M:S.T:", 5);
		str[0] = Temp / 10 + '0';
		str[1] = Temp % 10 + '0';
		str[2] = ',';
		str[3] = Wet / 10 + '0';
		str[4] = Wet % 10 + '0';
		str[5] = '\n';
		UART_SendString(str, 6);
	}
	else if(len >= 2 && strncmp(dat, "S:", 2) == 0)
	{
		//UART_SendString(dat, len);
		UART_SendString("N:S.SERR\n", 9);
	}
}
void main()
{
	unsigned char i;
	UART_T0_Init();
	P3M0 = 0x00; P3M1 = 0x00;
	Timer2Init();
	Interrput_Init();
	if(P33 == 0)
	{
		GasFlag = 1;
		P32 = 0;
	}
	else
	{
		GasFlag = 0;
		P32 = 1;
	}
	WDT_CONTR = 0x37;
	while(1)
	{
		WDT_CONTR |= 0x10;
		i++;
		if(i % 10 == 0)
			Temp = DHT11_Read(&Wet);
		Delay100ms();
		UART_Driver();
	}
}
void Interrupt_Timer2() interrupt 12
{
	UART_RxMonitor(1);
}
void Interrupt_X1() interrupt 2
{
	if(P33 == 0)
	{
		GasFlag = 1;
		P32 = 0;
	}
	else
	{
		GasFlag = 0;
		P32 = 1;
	}
}