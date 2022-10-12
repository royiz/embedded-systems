#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer

#define song1_length 54
#define song2_length 47
#define song3_length 40

extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern void sysConfig(void);

extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();

extern __interrupt void USCI0RX_ISR(void);
extern __interrupt void USCI0RX_ISR(void);
extern __interrupt void TIMER0_A0_ISR (void);
extern __interrupt void TIMER1_A0_ISR (void);
extern __interrupt void ADC10_ISR(void);
//extern __interrupt void PBs_handler(void);
//extern __interrupt void KEY_PAD_handler (void);
//
//extern void DMAConfig(unsigned int *song, unsigned int *song_2d, unsigned int song_size);
extern unsigned int KB;
extern unsigned int X;
extern char start_calib;
extern char end_calib;
extern float Phi;
extern int counter_M;
extern unsigned int stick_xy[2], prev_stick[2];
extern int low_x, low_y, high_x, high_y;
extern char *Flash_ptr;
extern char segFlag;

extern void print2Phases(unsigned char ch);
extern void print2LEDs(unsigned char);
extern void print2HalfLEDs(unsigned char ch);
extern void clrLEDs(void);
extern void toggleLEDs(char);
extern void setLEDs(char);
extern unsigned char readSWs(void);
extern void incLEDs(char);
extern void RgbPlayer(unsigned int color);
extern void getDelay(unsigned int t);
extern void getStickValue(void);
extern void displayVoltage(unsigned int mem);
extern void HellowWorld(void);
extern void initialSegment(void);
extern void finishSegment(void);
extern void print_the_stick(void);
extern int paint_flag;
extern int p;
extern int q;

#endif







