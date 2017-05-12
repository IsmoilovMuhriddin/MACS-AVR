/*
 * max_lcd.c
 *
 * Created: 5/3/2017 12:26:53 AM
 *  Author: Mashrabbek Akbarov
 */ 
#include "avr/io.h"
#include "_main.h"
#include "_glcd.h"
#include "max_lcd.h"
char *title[]={"Main Function","Set Target","Set Mode"};
char *main_menu[2]={"1.temperature","2.mode"};

char *temp_menu[3]={""};
char *mode_menu[3]={"1.Default","2.Window","3.Cooler"};

char * mode_t[4] = {"deflt","windw","coolr","man"};
int dirx=20, diry=4;


void setDirX(int x){dirx =x;}
int	getDirX(){return dirx;}
void setDirY(int y){diry =y;}
int getDirY(){return diry;}
	
void menu_disp2(){
	
	setTitle(2);
	
	line_clear(16,0);
	lcd_string(2,1,mode_menu[0]);
	lcd_string(3,1,mode_menu[1]);
	lcd_string(4,1,mode_menu[2]);
	arrow(dirx);
	
}

void menu_disp1(int t){
	char *temp[1];

	setTitle(1);
	
	line_clear(16,0);
	triangle_up(20,11);
	triangle_down(34,11);
	itoa(t,temp,10);
	lcd_string(3,2,temp);
	GLCD_Circle(25,31,1);
	lcd_string(3,5,"C");
}

void menu_disp1_edit(int t){
	char *temp[1];
	itoa(t,temp,10);
	lcd_string(3,2,temp);
}

void menu_disp(){
	
	setTitle(0);
	dirx=20;
	lcd_string(2,1,main_menu[0]);
	lcd_string(3,1,main_menu[1]);
	arrow(dirx);
	
}

void default_lcd_disp(){
	
	GLCD_Line(0,88,54,88);// vertical line
	GLCD_Line(9,0,9,127);// menu line
	GLCD_Line(54,0,54,127); // bottom line
	
	
	GLCD_Circle(1,108,1);
	lcd_string(0,18,"C");
	home_pic(0,122);
	
	//menu
	//1 menu_disp();
	
	//mode
	lcd_string(2,15,"mode:");
	// 2 set_mode(mod);
	//mode
	
	//target
	// 3set_targetTemp(targetTemp);
	//target
	
	// down menu
	triangle_up(55,5);
	triangle_down(63,5);
	triangle_left(59,80);
	lcd_string(7,1,"select|OK|");
	lcd_string(7,14,"back");
	//
	
}

void setTitle(int t){
	title_clear();
	lcd_string(0,0,title[t]);
}

void set_targetTemp(int t){
	char *s[1];
	itoa(t,s,10);
	lcd_string(5,15,s);
	GLCD_Circle(41,108,1);
	lcd_string(5,18,"C");
}

void set_mode(int i){
	mode_clear();
	lcd_string(3,15,mode_t[i]);
}

void temp_disp(int temp){
	
	//if(temp<10)
	char str[3];
	itoa(temp,str,10);
	
	//byte_clear(0,92);
	//byte_clear(0,100);

	if(temp<100){
		if(temp>=10)
		lcd_string(0,15,str);
		else
		lcd_string(0,16,str);
	}
	else lcd_string(0,15,"X");
}

void mode_clear(){
	int i,j;
	for (i=0; i <8; i++)
	{
		for (j=0; j<37; j++)
		{
			GLCD_Dot_Out(24+i,90+j);
		}
	}
}

void arrow(int x){
	triangle_right(x,diry);
}

void arrow_clr(int x){
	triangle_right_clear(x,diry);
}

void printTemp(){


	
}

void byte_clear(int x,int y){
	int i,j;
	for (i=0; i <8; i++)
	{
		for (j=0; j<7; j++)
		{
			GLCD_Dot_Out(x+i,y+j);
		}
	}
}

void title_clear(){
	int i,j;
	for (i=0; i <8; i++)
	{
		for (j=0; j<87; j++)
		{
			GLCD_Dot_Out(i,j);
		}
	}
}

void line_clear(int x, int y){
	int i,j;
	for (i=0; i <24; i++)
	{
		for (j=0; j<87; j++)
		{
			GLCD_Dot_Out(x+i,y+j);
		}
	}
}

void triangle_up(int x,int y){
	for (int i=0; i<4;i++){
		for(int s=y-i; s<=y+i; s++)
		GLCD_Dot(x+i,s);
	}
}

void triangle_down(int x,int y){
	for (int i=0; i<4;i++){
		for(int s=y-i; s<=y+i; s++)
		GLCD_Dot(x-i,s);
	}
}
void triangle_right(int x,int y){
	for (int i=0; i<4;i++){
		for(int s=x-i; s<=x+i; s++)
		GLCD_Dot(s,y-i);
	}
}

void triangle_right_clear(int x,int y){
	for (int i=0; i<4;i++){
		for(int s=x-i; s<=x+i; s++)
		GLCD_Dot_Out(s,y-i);
	}
}

void triangle_left(int x,int y){
	for (int i=0; i<4;i++){
		for(int s=x-i; s<=x+i; s++)
		GLCD_Dot(s,y+i);
	}
}

void home_pic(int x, int y){
	for (int i=0; i<4;i++){
		for(int s=y-i; s<=y+i; s++)
		GLCD_Dot(x+i,s);
		
		for (int k=y-2;k<y+3;k++)
		GLCD_Dot(x+3+i,k);
	}
	
}


void temp_disp2(int temp){
	
	//if(temp<10)
	char str[3];
	itoa(temp,str,10);
	
	//byte_clear(0,92);
	//byte_clear(0,100);

	if(temp<100){
		if(temp>=10){
		lcd_string(5,0,str);
		lcd_string(5,2,"'C out");
		}
		else{
		lcd_string(5,1,str);
		lcd_string(5,2,"'C out");}
	}
	else lcd_string(5,0,"X");
	
}

void logo_bluetooth(int x, int y){
	GLCD_Line(x,y,x+7,y);
	GLCD_Line(x+2,y-2,x+8,y+2);
	GLCD_Line(x+8,y-2,x+2,y+2);
	GLCD_Line(x,y,x+2,y+2);
	GLCD_Line(x+8,y,x+6,y+2);
}

