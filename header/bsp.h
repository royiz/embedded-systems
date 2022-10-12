#ifndef _bsp_H_
#define _bsp_H_


#include  <msp430g2553.h>          // MSP430x2xx
//#include  <msp430xG46x.h>           // MSP430x4xx -->MSP430FG4618

#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1

#define   debounceVal      250
#define   LEDs_SHOW_RATE   0xD9E0  ////0xFFFF  // 62.5 msec
#define   Duty_Cycle_up     0x000E
#define   Duty_Cycle_down   0x0001

// LEDs abstraction
#define LEDsArrPort        P2OUT
#define LEDsArrPortDir     P2DIR
#define LEDsArrPortSel     P2SEL
#define LEDsMask           0xF0

// RGB - updated!
#define RGBsArrPort         P1OUT
#define RGBsArrPortDir      P1DIR
#define RGBsArrPortSel      P1SEL
#define RGBmask             0xE0

// Motor
#define MotorArrPort        P2OUT
#define MotorArrPortSel     P2SEL
#define MotorArrPortDir     P2DIR


//// PushButtons abstraction
#define PBsArrPort          P1IN
#define PBsArrIntPend      P1IFG
#define PBsArrIntEn         P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL
#define PBsArrPortDir      P1DIR
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08

// potentiometer p1.3

extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void LCDconfig(void);
extern void ADCconfig(void);
extern void UARTconfig(void);
extern void Flashconfig(void);

extern void startTimerA0(void);
extern void stopTimerA0(void);
extern void startTimerA1(void);
extern void stopTimerA1(void);
extern void changePeriodTimeA1(unsigned int t);
extern void disable_ADC10(void);
extern void ADC10enable(void);
extern void UartRxEnable(void);
extern void UartRxDisable(void);
extern void UartTxEnable(void);
extern void UartTxDisable(void);
extern void ADCenable(void);

struct Flash_struct {
    int files_count;    //number of files
    int file_names[3];
    char *file_ptrs[3];  //pointers to begining of files
    int file_sizes[3];  //file sizes
};

extern struct Flash_struct Flash_master;



#endif



