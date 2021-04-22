#include <REGX52.H>

extern void LcdInit();
extern void LcdWelcome();
extern void Lcd_Cursor(char row, char column);
extern void Lcd_DisplayCharacter(char a_char);
extern void Lcd_DisplayString(char row, char column, char * string);
extern void Lcd_WriteControl(unsigned char LcdCommand);
extern void Lcd_DisplayScreen (char *ptr);

unsigned char data Var1, Var2, Var3;

volatile unsigned char data licznik_wX = '0';
volatile unsigned char data licznik_wO = '0';

volatile unsigned char data player = 'x';

volatile unsigned char data ostatniWyg = ' ';
volatile unsigned char data koniecGry = '0';
volatile unsigned char data koniecGryOst='0';
volatile unsigned char data czyjaKolej[] = " TERAZ:_";
volatile unsigned char data remis[] = " REMIS! ";
char data remiss=0;
volatile unsigned char data wygralX[] = " Wyg: x!";
volatile unsigned char data wygralO[] = " Wyg: o!";
volatile unsigned char pole1 = ' ', pole2 = ' ', pole3 = ' ', pole4 = ' ', pole5 = ' ', pole6 = ' ', pole7 = ' ', pole8 = ' ', pole9 = ' ';

static unsigned char code wygX[] = 					"   KONIEC GRY!  "
  			                                    "   WYGRYWA: x!  "
                                            "----------------" 
                                            "KONRAD KOWALCZYK";

static unsigned char code wygO[] = 					"   KONIEC GRY!  "
  			                                    "   WYGRYWA: o!  "
                                            "----------------" 
                                            "KONRAD KOWALCZYK";

static unsigned char code poczatek[] =      " | |  WYGRANE   "
																		  	    " | |  x:  o:    "
                                            " | |            "
                                            "Konrad o/x 36334";

void wait(void) {
  unsigned char data x, y;
  for (x = 50; x > 0; x--)
    for (y = 50; y > 0; y--) {
      ;
    }
}

void zamieniacz(void) {
  if (player == 'x') {
    player = 'o';
  } else if (player == 'o') {
    player = 'x';
  }
}

void Send(unsigned char Value) {
  P3_4 = 1;
  TI = 0;
  SBUF = (Value);
  while (TI == 0) {
    ;
  }
  TI = 0;
  P3_4 = 0;
}

void sprawdzacz(void) {
  if ((pole1 == pole2 && pole2 == pole3 && pole1 != ' ') ||
      (pole4 == pole5 && pole5 == pole6 && pole4 != ' ') ||
      (pole7 == pole8 && pole8 == pole9 && pole7 != ' ') ||
      (pole1 == pole4 && pole4 == pole7 && pole7 != ' ') ||
      (pole2 == pole5 && pole5 == pole8 && pole2 != ' ') ||
      (pole3 == pole6 && pole6 == pole9 && pole3 != ' ') ||
      (pole1 == pole5 && pole5 == pole9 && pole1 != ' ') ||
      (pole3 == pole5 && pole5 == pole7 && pole3 != ' ')) {
      if (player == 'x') {
         ostatniWyg = 'o';
        if (licznik_wO != '9') {
          licznik_wO++;
        }
    } else {
        ostatniWyg = 'x';
        if (licznik_wX != '9') {
          licznik_wX++;
      }
    }
    koniecGry = '1';
    
  }
		if(koniecGry== '1' && remiss==0) {
			if(ostatniWyg=='x'){
				Lcd_DisplayString(3, 6, wygralX);
			}
			else if (ostatniWyg=='o'){
				Lcd_DisplayString(3, 6, wygralO);
			}
			else {
				Lcd_DisplayString(3, 6, remis);
			}
		}
		
}
void spr2(void){
	if (
			koniecGry == '0' &&
		  pole1 != ' ' &&
      pole2 != ' ' &&
      pole3 != ' ' &&
      pole4 != ' ' &&
      pole5 != ' ' &&
      pole6 != ' ' &&
      pole7 != ' ' &&
      pole8 != ' ' &&
      pole9 != ' ' ) {
      koniecGry = '1';
		  remiss=1;
			SBUF = '^';
      Lcd_DisplayString(3, 6, remis);
    }
	}

	
		
	void czyKONIEC(void){
		if(licznik_wX=='3'){
			Lcd_DisplayScreen(wygX);
			koniecGryOst='1';
		//	Send('#');
		}
		if(licznik_wO=='3'){
			Lcd_DisplayScreen(wygO);
			koniecGryOst='1';
		//	Send('*');
		}
			
	}


void Init(void) {
  P3_4 = 0; // ustaw sie na odbior danych z portu szeregowego
  
  SCON = 0x50; // inicjowanie portu szeregowego
  RCAP2H = TH2 = 0xFF; // predkosc transmisji 1200b/sec przy CLK=1.3824MHz 
  RCAP2L = TL2 = 0xDC; // predkosc transmisji 1200b/sec przy CLK=1.3824MHz 
  T2CON = 0x20; // Timer 2 wyznacza predkosc transmisji przez Serial
  RI = TI = 0; // gaszenie flag odbiornika i nadajnika
  ES = 1; // wlacz odbieranie danych przez IRQ
  TR2 = 1; // wlacz Timer 2
  EA = 1; // wlacz globalna maske przerwan
	IT0=1;
	EX0=1;
}

void ISR_INT0(void) interrupt 0 {
	
}

void ISR_Serial(void) interrupt 4 {
  if (RI == 1) {

    if (koniecGry == '0') {
      Lcd_DisplayString(3, 6, czyjaKolej);
      Lcd_Cursor(3, 13);
      if (player == 'x') {
        Lcd_DisplayCharacter('x');
      } else if (player == 'o') {
        Lcd_DisplayCharacter('o');
      }
      switch (SBUF) {
      case '1':
				if(pole1==' '){
        Lcd_Cursor(1, 1);
        pole1 = player;
        Lcd_DisplayCharacter(pole1);
        zamieniacz();
				}
        break;
      case '2':
				if(pole2==' '){
        Lcd_Cursor(1, 3);
        pole2 = player;
        Lcd_DisplayCharacter(pole2);
        zamieniacz();
				}
        break;
      case '3':
				if(pole3==' '){
        Lcd_Cursor(1, 5);
        pole3 = player;
        Lcd_DisplayCharacter(pole3);
        zamieniacz();
				}
        break;
      case '4':
				if(pole4==' '){
        Lcd_Cursor(2, 1);
        pole4 = player;
        Lcd_DisplayCharacter(pole4);
        zamieniacz();
				}
        break;
      case '5':
				if(pole5==' '){
        Lcd_Cursor(2, 3);
        pole5 = player;
        Lcd_DisplayCharacter(pole5);
        zamieniacz();
				}
        break;
      case '6':
				if(pole6==' '){
        Lcd_Cursor(2, 5);
        pole6 = player;
        Lcd_DisplayCharacter(pole6);
        zamieniacz();
				}
        break;
      case '7':
				if(pole7==' '){
        Lcd_Cursor(3, 1);
        pole7 = player;
        Lcd_DisplayCharacter(pole7);
        zamieniacz();
				}
        break;
      case '8':
				if(pole8==' '){
        Lcd_Cursor(3, 3);
        pole8 = player;
        Lcd_DisplayCharacter(pole8);
        zamieniacz();
				}
        break;
      case '9':
				if(pole9==' '){
        Lcd_Cursor(3, 5);
        pole9 = player;
        Lcd_DisplayCharacter(pole9);
        zamieniacz();
				}
        break;
      }
			sprawdzacz(); //czy wygrana
			spr2(); //czy remis
			czyKONIEC();
    }
		if(SBUF=='0'){
			licznik_wO='0';
			licznik_wX='0';
			player='x';
			Lcd_DisplayScreen(poczatek);
			koniecGryOst='0';
		}
		

    if (SBUF == '!') {
      //reset gry
      Lcd_Cursor(1, 1);
      Lcd_DisplayCharacter(' ');
      Lcd_Cursor(1, 3);
      Lcd_DisplayCharacter(' ');
      Lcd_Cursor(1, 5);
      Lcd_DisplayCharacter(' ');
      Lcd_Cursor(2, 1);
      Lcd_DisplayCharacter(' ');
      Lcd_Cursor(2, 3);
      Lcd_DisplayCharacter(' ');
      Lcd_Cursor(2, 5);
      Lcd_DisplayCharacter(' ');
      Lcd_Cursor(3, 1);
      Lcd_DisplayCharacter(' ');
      Lcd_Cursor(3, 3);
      Lcd_DisplayCharacter(' ');
      Lcd_Cursor(3, 5);
      Lcd_DisplayCharacter(' ');

      Lcd_DisplayString(3, 6, czyjaKolej);
      Lcd_Cursor(3, 13);

      if (ostatniWyg == 'x') {
        player = 'o';
        Lcd_DisplayCharacter('o');
      } else {
        player = 'x';
        Lcd_DisplayCharacter('x');
      }
      koniecGry = '0';
      pole1 = ' ', pole2 = ' ', pole3 = ' ', pole4 = ' ', pole5 = ' ', pole6 = ' ', pole7 = ' ', pole8 = ' ', pole9 = ' ';
			

      //dioda on/off przy resecie
      P3_7 = 0;
      wait();
      P3_7 = 1;
    }
    if(koniecGryOst == '0'){
    Lcd_Cursor(2, 9);
    Lcd_DisplayCharacter(licznik_wX);
    Lcd_Cursor(2, 13);
    Lcd_DisplayCharacter(licznik_wO);
		}
		
    Lcd_WriteControl(0x0C);
    RI = 0;
  }
}

void main(void) {
  
  LcdInit(); // Zainicjowanie rejestrów uP
  LcdWelcome(); // wyswietlenie powitania
  Init(); // Zainicjowanie systemu przerwan

  while (1) {
    ;
  }
}