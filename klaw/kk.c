#include <REGX52.H>
#define Led_ON  P0_0=0    
#define Led_OFF P0_0=1 
volatile unsigned char data okres;
volatile unsigned char data ind;
volatile unsigned char bdata znak;
unsigned char bdata znak;
sbit znak_3=znak^3;
sbit znak_2=znak^2;
sbit znak_1=znak^1;
bit Nowy;

/*
E7 EB ED | 1 2 3
D7 DB DD | 4 5 6
B7 BB BD | 7 8 9
77 7B 7D | X 0 #
*/

void wait(void)
{
	unsigned char data x,y;
	for(x=50;x>0;x--)
		for(y=50;y>0;y--){;}
}

unsigned char data ind=0;

unsigned char Szukaj(unsigned char Value)
{ 
  unsigned char code Keys[] =  {0x7B,0xE7,0xEB,0xED,0xD7,0xDB, //tablica
                                 0xDD,0xB7,0xBB,0xBD,0x77,0x7D};//scankodów
  unsigned char code ASCII[] = {'0','1','2','3','4','5',       //tablica
                                 '6','7','8','9','*','#'};      //znaków ASCII
                              
  unsigned char data ReturnValue = 0xFF;  // wartosc zwraca gdy blad
  unsigned char data numer=0;             // indeks poszukiwanego skankodu
    
	while((Keys[numer]!=Value)&(numer<0x0C)){numer++;} // poszukiwanie skankodu
  if(numer<0x0C)                                    // gdy znaleziono skankod                         
	{
   ReturnValue=ASCII[numer];
  }
  return ReturnValue;                     // zwroc wartosc ASCII od znaku
}

void Send(unsigned char Value)
{
	P3_4=1;									
		TI=0;									
			SBUF=Szukaj(Value);	
			while(TI==0){;}			
		TI=0;								
	P3_4=0;									
}

void wykonaj(unsigned char Value)
{
	Send(Value);	
	Led_ON;
	wait();
	Led_OFF;
	Nowy=1;					
}

void Init(void)
{
	P3_4=0;					
	okres=0x00;			
	ind=0x00;				
	Nowy=0;					
	SCON=0x40;			
	RCAP2H=TH2=0xFF;
	RCAP2L=TL2=0xDC;
	T2CON=0x10;			
	TMOD=0x20;			
	TH0=okres;			
	TL0=okres;
	ET1=1;  				
	EA=1;						
	TR2=1;					
	TR1=1;					
}
void ISR_Serial(void) interrupt 4
 {
   if(RI==1) //odebrano znak
    {
      if(SBUF=='0'){Led_ON;}
			wait();
      Led_OFF;
      RI=0;
    }
 }

void skan(void) interrupt 3
{
	unsigned char code Tab[]={0xEF,0x7F,0xDF,0xBF};
	
	if(SBUF=='^'){
		Nowy=1;
	}
	else if(SBUF=='!'){
		Nowy=0;
	}

	TR1=0;
		P2=Tab[ind];
		znak=P2;
		if((znak_3 & znak_2 & znak_1)==0x00)
			{
				wykonaj(znak);
			}
			else
				{
					Nowy=0;
					if(ind < 0x03)
						{
							ind++;
						}
						else{ind=0;}
				}
	TH1=TL1=okres;													
	TR1=1;
}

void main(void)
{
	Init();
	while(1){;}
}