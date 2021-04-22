#include <REGX52.H>

/*
dp g f e d c b a
0	 0 1 1 1 1 1 1 | 0
0  0 0 0 0 1 1 0 | 1 
0  1 0 1 1 0 1 1 | 2
0  1 0 0 1 1 1 1 | 3
0  1 1 0 0 1 1 0 | 4
0  1 1 0 1 1 0 1 | 5
0  1 1 1 1 1 0 1 | 6
0  0 0 0 0 1 1 1 | 7
0  1 1 1 1 1 1 1 | 8
0  1 1 0 1 1 1 1 | 9
*/

unsigned char xdata LCD1 _at_ 0xFE00;
unsigned char xdata LCD2 _at_ 0xFD00;
unsigned char code Koder[] = {0x3F,0x06,0x5B,0x4F,0x66,
                              0x6D,0x7D,0x07,0x7F,0x6F};
volatile unsigned char data ind1,ind2;
volatile unsigned char data Dot1,Dot2;	
volatile unsigned char data okres;																	

void Send(unsigned char Value)
{
	P3_4=1;									
		TI=0;									
			SBUF=(Value);	
			while(TI==0){;}			
		TI=0;								
	P3_4=0;									
}
															
void ISR_INT0(void) interrupt 0
{
	if(ind2<9)
	{
		Send('!');
		ind2++;
	}
	else
	{
		ind2=0;
		ind1++;
		if(ind1>9){ind1=0;}
	}
}

void ISR_Serial(void) interrupt 4
{
	if(SBUF=='0'){
		ind1=0;
		ind2=0;}
  if(RI==1)
  {
    RI=0;
  }
}

void skan(void) interrupt 3
{
	TR1=0;
		LCD1=(Dot1 | Koder[ind1]); //01010101
		LCD2=(Dot2 | Koder[ind2]); //10101010
	TH1=TL1=okres;													
	TR1=1;
}

void Init(void)
{														
  P3_4=0;          // ustaw sie na odbior danych z portu szeregowego
	ind1=0;
	ind2=1;
	Dot1=Dot2=0;
	okres=0x00;
	IT0=1;
	EX0=1;
  SCON=0x50;       // inicjowanie portu szeregowego
  RCAP2H=TH2=0xFF; // predkosc transmisji 1200b/sec przy CLK=1.3824MHz 
  RCAP2L=TL2=0xDC; // predkosc transmisji 1200b/sec przy CLK=1.3824MHz 
  T2CON=0x30;      // Timer 2 wyznacza predkosc transmisji przez Serial
  RI=TI=0;   			 // gaszenie flag odbiornika i nadajnika
  ES=1;            // wlacz odbieranie danych przez IRQ
	TMOD=0x20;			
	TH0=okres;			
	TL0=okres;
	ET1=1;  												
	TR1=1;		
  TR2=1;           // wlacz Timer 2
  EA=1;            // wlacz globalna maske przerwan
}

void main(void)
{
Init();
for(;;){;}
}