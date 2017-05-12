/*
 * sam.c
 *
 * Created: 4/21/2017 6:17:35 PM
 * Author : unbekannt
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h" 
#include "_main.h"
#include "_glcd.h"

#define BAUD 9600
#define MYBAUD F_CPU/16/BAUD-1



 int  DEF_MODE = 0;
 int  WIN_MODE = 1;
 int  COOL_MODE = 2;
 int  MAN_MODE  = 3;
 
 int ON = 1;
 int OFF = 0;

int targetTemp = 20;
int TempIn = 16; //default
int TempOut= 16; //default
int Cool_stat = 0;
int Win_stat = 0;

int mod;
char command;
char data[10];
char *s; 
int count=0;
int minTemp=16, menu_mode=0;

int main(void)
{
	mod=DEF_MODE;
	
	
	DDRB = 0xFF; //stepper
	
	DDRA = 0xFF; // GLCD
	DDRE = 0xFF; // GLCD
	
	DDRC =0xFF; // for  and Cooler for PINC0
	DDRF = 0;  //input INTERRUPT ADC
	
	DDRD=0;//button input
    
	lcd_init();
	lcd_clear();
     
    usart1_init();
//  button input
	
	EIMSK = (1<<INT0)|(1<<INT1)|(1<<INT2)|(1<<INT3);
	EICRA=0x02;
	sei();

//  temp input

   //char s[1];
   ADCSRA = 0x87;
   ADMUX =(1<<REFS1)|(1<<REFS0)|(1<<ADLAR);
//
menu_disp();
set_mode(mod);
set_targetTemp(targetTemp);

// logo_bluetooth(40,26);
 default_lcd_disp();
  while(1){
	 
//    _delay_ms(4000);

   ADMUX|=0b00000001;
   ADCSRA |= (1<<ADSC);
   while((ADCSRA&(1<<ADIF))==0);
   TempIn = ADCH;
   temp_disp(ADCH);
   ADMUX&=0b11111110;
//------------------------
   ADMUX|=0b00000010;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA&(1<<ADIF))==0);
	TempOut = ADCH;
    temp_disp2(ADCH);
	ADMUX&=0b11111101;
//-------------------------
	
    if(mod==DEF_MODE)
			defaultF();
	else if (mod ==WIN_MODE)
			window();
	else if (mod==COOL_MODE)
			cooler();
	else if (mod==MAN_MODE)
	        manual();
	delay(100);
     
	 data[0]=mod+'0';
	 data[1]=Cool_stat+'0';
	 data[2]=Win_stat+'0';
	 data[3]=targetTemp/10+'0';
	 data[4]=targetTemp%10+'0';
	 data[5]=TempIn/10+'0';
	 data[6]=TempIn%10+'0';
	 data[7]=TempOut/10+'0';
	 data[8]=TempOut%10+'0';
	 data[9]='\0';
	 
//	 Update(data);
	 
}

   while(1);


}

//temp 


void delay(int x){
for (int i=0;i<x*1000;i++);
}

void defaultF(){
	// inside is hotter then target
	int last_win_stat = Win_stat;
	int last_cool_stat= Cool_stat;
	int action_win=0;
	int action_cool = 0;
	if(targetTemp<TempIn){
		// inside is  hotter then outside
		if(TempOut<TempIn-1){
			Win_stat = ON;
			Cool_stat = OFF;
			
		}
		// inside is  equal or cooler then outside
		else if(TempOut>TempIn+1){
			Win_stat = OFF;
			Cool_stat = ON;
		}
	action_win=last_win_stat^Win_stat;
	action_cool = last_cool_stat^ Cool_stat;
	WinOpenClose(Win_stat,action_win);
	CoolOpenClose(Cool_stat,action_cool);
	
	}
	else {
		closeAll();
	}
}

void window(){
		int last_win_stat = Win_stat;
		int last_cool_stat= Cool_stat;
		int action_win=0;
		int action_cool = 0;
	Cool_stat = OFF;
	if(targetTemp<TempIn){
		// inside is  hotter then outside
		if(TempOut<TempIn-1){
			Win_stat = ON;
		}
		// inside is  equal or cooler then outside
		else if(TempOut>=TempIn+1) {
			Win_stat = OFF;
		}
	action_win=last_win_stat^Win_stat;
	action_cool = last_cool_stat^ Cool_stat;
	WinOpenClose(Win_stat,action_win);
	CoolOpenClose(Cool_stat,action_cool);

	}
	else {
		closeAll();
	}
}

void cooler(){
	int last_win_stat = Win_stat;
	int last_cool_stat= Cool_stat;
	int action_win=0;
	int action_cool = 0;
	Win_stat=OFF;
	if(targetTemp<TempIn){
		// inside is  hotter then outside
	//	if(TempOut<TempIn){
			Cool_stat = ON;
	//	}
		// inside is  equal or cooler then outside
	//	else if(TempIn<=TempOut){
	//		Cool_stat = OFF;
	//	}
	action_win = last_win_stat^Win_stat;
	action_cool = last_cool_stat^Cool_stat;
	WinOpenClose(Win_stat,action_win);
	CoolOpenClose(Cool_stat,action_cool);
	}

	else {closeAll();}
}

void manual(){
   int last_win_stat = Win_stat;
   int last_cool_stat= Cool_stat;
   int action_win=0;
   int action_cool = 0;
   
   	action_win = last_win_stat^Win_stat;
   	action_cool = last_cool_stat^Cool_stat;
   	WinOpenClose(Win_stat,action_win);
   	CoolOpenClose(Cool_stat,action_cool);
   
}

void closeAll(){
	
	int last_win_stat = Win_stat;
	int last_cool_stat= Cool_stat;
	int action_win=0;
	int action_cool = 0;
	Win_stat=OFF;
	Cool_stat=OFF;
	action_win = last_win_stat^Win_stat;
	action_cool = last_cool_stat^Cool_stat;
	
	WinOpenClose(Win_stat,action_win);
	
	CoolOpenClose(Cool_stat,action_cool);
}


ISR(INT0_vect){
if(count==0&&menu_mode==0){
    
   arrow_clr();
   if(getDirX()==20){
      setDirX(28);
      arrow(getDirX());
   }
   else {
       setDirX(20);
	   arrow(getDirX());
	   }
}

else if (count==1&&menu_mode==0){
   arrow_clr(getDirX());
   if(getDirX()==20){
	   setDirX(28);
	   arrow(getDirX());
   }
   else if(getDirX()==28){
	   setDirX(36);
	   arrow(getDirX());
    }	
    else {
	    setDirX(20);
	    arrow(getDirX());
    }  
   }
else if (count==1&&menu_mode==1){
	if (minTemp>16)
	{
		minTemp--;
		menu_disp1_edit(minTemp);
	}
	else{ minTemp =32;
	menu_disp1_edit(minTemp);
	}
 }
}

ISR(INT1_vect){
if(count==0&&menu_mode==0){
		menu_disp();
		arrow_clr(getDirX());
		if(getDirX()==20){
			setDirX(28);
			arrow(getDirX());
		}
		else {
			setDirX(20);
			arrow(getDirX());
		}
	}

	
else if (count==1&&menu_mode==0){
		arrow_clr(getDirX());
		if(getDirX()==20){
			setDirX(36);
			arrow(getDirX());
		}
		else if(getDirX()==28){
			setDirX(20);
			arrow(getDirX());
		}
		else {
			setDirX(28);
			arrow(getDirX());
		}
	}
	
else if (count==1&&menu_mode==1){
	if (minTemp<32)
	{
		minTemp++;
		menu_disp1_edit(minTemp);
	}
	else{ 
		minTemp=16;
	    menu_disp1_edit(minTemp);
	}
}
	
}

ISR(INT2_vect){
	
	if (count==0&&getDirX()==20){
      count++; 
	  menu_mode++;
	  menu_disp1(minTemp);
	  
	}
	
	else if(count==0&&getDirX()==28){
	count++;
	setDirX(20);
	menu_disp2();
	}
	else if (count==1&&getDirX()==20&&menu_mode==0)
	{
		//default mode chosen
		mod=DEF_MODE;
		set_mode(mod);
		//count--;
		//menu_disp();
	}
	else if (count==1&&getDirX()==28&&menu_mode==0)
	{
		//window mode chosen
		mod=WIN_MODE;
		set_mode(mod);
		//count--;
		//menu_disp();
	}else if (count==1&&getDirX()==36&&menu_mode==0)
	{
		//cooler mode chosen
		mod=COOL_MODE;
		set_mode(mod);
		//count--;
		//menu_disp();
	}
	else if (count==1&&menu_mode==1)
	{
		targetTemp=minTemp;
	  set_targetTemp(targetTemp);
	}
	
	else count=0;
	
}

ISR(INT3_vect){
	line_clear(16,0);
	menu_disp();
	count=0;
	setDirX(20);
	menu_mode=0;
}

/*
ISR(ADC_vect){
        
		temp_disp(ADCH);
		ADCSRA |= (1<<ADSC);
		_delay_ms(4000);
		
}
*/

ISR(USART1_RX_vect){
	
	command=UDR1;
	
if(command==41)
	usart1_string_transmit(data);
else
	rec_process(command);

}


void rec_process(char c){
	
	//temp
	if(c>65&&c<83)
	set_targetTemp(c-50);
	//cooler
	else if (c==33){
		Cool_stat=OFF;
	    CoolOpenClose(Cool_stat,1);
	}
	else if (c==34){
		Cool_stat=ON;
	    CoolOpenClose(Cool_stat,1);		
	}
	// window
	else if (c==35){
		Win_stat=OFF;
	WinOpenClose(Win_stat,1);
	}
	else if (c==36){
		Win_stat=ON;
	WinOpenClose(Win_stat,1);	
	}
	
	// mode
	else if (c==37){
		mod=DEF_MODE;
		
	}
	else if (c==38){
		mod = WIN_MODE;
		
	}
	else if (c==39){
		mod=COOL_MODE;
			}
	else if (c==40){
		mod=MAN_MODE;
	}
    
	//default_lcd_disp();
	set_mode(mod);

}


void WinOpenClose(int Win_stat, int action)
{
	if(action==1){
		if(Win_stat==0) //close
		for (int i=0; i<64;i++)
		{
			
			PORTB=0x06;
			_delay_ms(100);
			PORTB=0x03;
			_delay_ms(100);
			PORTB=0x09;
			_delay_ms(100);
			PORTB=0x0c;
			_delay_ms(100);
		}
		else if(Win_stat==1)//open
		for (int i=0; i<64;i++)
		{
			PORTB=0x06;
			_delay_ms(100);
			PORTB=0x0c;
			_delay_ms(100);
			PORTB=0x09;
			_delay_ms(100);
			PORTB=0x03;
			_delay_ms(100);
		}
	}
}

void CoolOpenClose(int Cool_stat,int action_cool){
	if(action_cool == 1){
		if(Cool_stat ==0){
			PORTC &=0xFE;
		}
		else if (Cool_stat ==1){
			PORTC |=0x01;
		}
	}
}

void usart1_init()
{
	UBRR1H = 0;
	UBRR1L = 96;
	UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1) | (UDRE1); //(1 << TXCIE1)
	UCSR1C = (1 << USBS1) | (1 << UCSZ11) | (1 << UCSZ10);
	sei();
}
void usart1_data_transmit(unsigned char data )
{
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
	_delay_ms(1);
}

void usart1_string_transmit(char *string)
{
	while(*string)
	{
		usart1_data_transmit(*string++);
	}
}
unsigned char usart1_data_receive( void )
{
	while (!(UCSR1A & (1 << RXC1)));
	return UDR1;
}

