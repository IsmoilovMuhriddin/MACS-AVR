/*
 * max_lcd.h
 *
 * Created: 5/3/2017 12:26:30 AM
 *  Author: Mashrabbek Akbarov
 */ 


#ifndef MAX_LCD_H_
#define MAX_LCD_H_
char *title[];
char *main_menu[2];

char *temp_menu[3];
char *mode_menu[3];
char *mode_type[3];

int dirx,diry;
void setDirX(int);
int	getDirX();
void setDirY(int);
int getDirY();
void menu_disp2();

void menu_disp1(int t);

void menu_disp1_edit(int t);

void menu_disp();

void default_lcd_disp();

void setTitle(int t);

void set_targetTemp(int t);

void set_mode(int i);

void temp_disp(int temp);

void mode_clear();

void arrow(int x);

void arrow_clr(int x);

void printTemp();

void byte_clear(int x,int y);

void title_clear();

void line_clear(int x, int y);

void triangle_up(int x,int y);

void triangle_down(int x,int y);
void triangle_right(int x,int y);

void triangle_right_clear(int x,int y);

void triangle_left(int x,int y);

void home_pic(int x, int y);


void temp_disp2(int temp);

void logo_bluetooth(int x, int y);




#endif /* MAX_LCD_H_ */