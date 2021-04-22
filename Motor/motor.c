#include <REGX52.H>

#define En1 P2_3
#define En2 P2_4
#define Start_Value 0x00 
#define Stop_Value  0xFF

volatile unsigned char data Suwak;
volatile unsigned char data Nastawa;
volatile unsigned char data Licznik;
/*
void Send(unsigned char Value)
 {
  P3_4=1;               // wejscie w nadawanie -> nadawanie danych
   TI=0;                // ustaw (zeruj) flage Transmin Interrupt
    SBUF=Value;         // wyjscie z nadawania ustawia flage TI na 1
    while(TI==0){;}     // czekaj az znak nie zostanie wyslany
   TI=0;                // kasuj (zeruj) flage Transmin Interrupt
  P3_4=0;               // wyjscie z nadawania -> odbieranie danych 
 }
*/
void ISR_Timer1(void) interrupt 3
 {
   Suwak++;
   if(Suwak<Nastawa)
    {
      if(Nastawa==Start_Value){En1=En2=0;} else {En1=En2=1;}
    }
   else
    {
      if(Nastawa==Stop_Value){En1=En2=1;} else {En1=En2=0;}
    }
   if(Suwak==Stop_Value){Suwak=0x00;}
 }

void ISR_Serial(void) interrupt 4
 {
   if(RI==1)
    {
      if(SBUF=='#')
       {
         if(Nastawa<Stop_Value) {Nastawa=Nastawa+25;}
       }
      if(SBUF=='*')
       {
         //Nastawa=0x00;
         P2_1 = ~P2_1;
         P2_2 = ~P2_2;
				 
       } 
      if(SBUF=='!' || SBUF=='0'){Nastawa=0x00;} 			// zatrzymanie silnika
      RI=0;
    }
 }
 
void ISR_INT1(void)  interrupt 2
 {
   ;
 }

void Init(void)
 {
   P3_4=0; 
   Suwak=Start_Value;
   Nastawa=0x00;
   Licznik=0x00;
   P2=0xC3;         // 1100 0011
   SCON=0x50;       // inicjowanie portu szeregowego
   RCAP2H=TH2=0xFF; // predkosc transmisji 1200b/sec przy CLK=1.3824MHz 
   RCAP2L=TL2=0xDC; // predkosc transmisji 1200b/sec przy CLK=1.3824MHz 
   T2CON=0x30;      // Timer 2 wyznacza predkosc transmisji przez Serial
   RI=TI=0;         // gaszenie flag odbiornika i nadajnika
   ES=1;            // wlacz odbieranie danych przez IRQ
   TR2=1;           // wlacz Timer 2
   TCON=0x00;
   TMOD=0x20;
   TH1=TL1=0x80;
   ET1=1;
   IT1=1;
   EX1=1;
   EA=1;
   TR1=1;
 }

void main(void)
 {
   Init();
   for(;;){;}
 }