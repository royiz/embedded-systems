#include  "../header/bsp.h"    // private library - BSP layer
#include "../header/LCD.h"
//global varible:
int counter=0;

struct Flash_struct Flash_master;
//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------

void GPIOconfig(void){
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
  // UART
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;



  P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1DIR |= RXLED + TXLED;
  P1OUT &= 0x00;

  // p1.0 - PB JOYSTICK
  P1SEL &= ~0x01;
  P1DIR &= ~0x01;
  P1IES &= ~0x01;

  PBsArrIntEdgeSel |=  0x01;   // pull-up mode    ///IES: falling=1, rising=0
  PBsArrIntEn       |=  0x01;   // Enable intetapts    // enable only p1.0
  PBsArrIntPend     &= ~0xFF;   // clear pending interrupts
  // LEDs 8-bit Array Port configuration
    LEDsArrPortSel &= ~0xF0;            // GPIO capability
    LEDsArrPortDir |= 0xF0;             // output dir
    LEDsArrPort = 0x00;            // clear all LEDs

  // RGB - updated!
  RGBsArrPortSel &= ~0xE0;            // GPIO capability
  RGBsArrPortDir |= 0xE0;             // output dir
  RGBsArrPort &= ~0xE0;             // clear all RGBs
  
  // Motor
    MotorArrPortSel &= ~0x0F;            // GPIO capability
    MotorArrPortDir |= 0x0F;             // output dir
    MotorArrPort &= ~0x0F;             // clear all MOTOROUTs

    // Flash
    FCTL2 = FWKEY + FSSEL0 + FN1;

//  // PushButtons Setup
//    PBsArrPortSel &= ~0x0F;
//    PBsArrPortDir &= ~0x0F;
//    PBsArrIntEdgeSel |= 0x03;          // pull-up mode
//    PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode
//    PBsArrIntEn |= 0x0F;
//    PBsArrIntPend &= ~0x0F;            // clear pending interrupts

  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers configuration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  //TIMER A CONFIG
  TACTL = TASSEL_2 + ID_3;           // SMCLK, divide by 8
  TACCR0 = 0xB0A3;                            // 0.5s - 0xFFFF SMCLK-- 0xB0A3 -- ACLK 0x2C28

  TA1CTL = TASSEL_2 + ID_3;           // SMCLK, divide by 8
  TA1CCR0 = 0x0100;
} 

//------------------------------------------------------------------------------------- 
//            LCD configuration 
//-------------------------------------------------------------------------------------
void LCDconfig(void){
    lcd_init();    //This function configures LCD
    lcd_clear();
    lcd_home();  
}              

//-------------------------------------------------------------------------------------
//            ADC configuration
//-------------------------------------------------------------------------------------
void ADCconfig(void){
    ADC10CTL1 = INCH_4 + ADC10DIV_0 + CONSEQ_1 + SHS_0; //input chnnel(2)/clock divider/ repeat single-channel + ACLK
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON;// ADC10IE--Vcc-Vss/sample and hold time (8)/multiple sample and convartion valid for repeat mode/ adc10 on
    ADC10CTL0 &= ~ADC10IFG;                         // clear interrupts
    ADC10AE0 = 0x18;                                // P1.3 = y + P1.4 = x
    ADC10DTC1 = 2;                                  //number of transfers in each block
}

//-------------------------------------------------------------------------------------
//            UART configuration
//-------------------------------------------------------------------------------------
void UARTconfig(void){
    UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
    UCA0BR0 = 104;                           // Baud rate = UCA0BR0 + UCA0BR1 * 256
    UCA0BR1 = 0x00;                           //
    UCA0MCTL = UCBRS0;                        // Modulation?
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine** (software reset)
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}


//-------------------------------------------------------------------------------------
//            Flash configuration
//-------------------------------------------------------------------------------------
void Flashconfig(void){
    Flash_master.files_count=0;
}

//-------------------------------------------------------------------------------------
//            Timer A
//-------------------------------------------------------------------------------------
void startTimerA0(void){
    TACTL |= MC_3 + TACLR;
    TACCTL0 &= ~CCIFG;
    TACCTL0 |= CCIE;
}

void stopTimerA0(void){
    TACTL &= ~MC_3;         // turn off TA
    TACCTL0 &= ~CCIE;      // disable interrupt
}

void startTimerA1(void){
    TA1CTL |= MC_3 + TACLR;
    TA1CCTL0 &= ~CCIFG;
    TA1CCTL0 |= CCIE;
}

void stopTimerA1(void){
    TA1CTL &= ~MC_3;         // turn off TA
    TA1CCTL0 &= ~CCIE;      // disable interrupt
    counter=counter+1;
    if (counter==300){
        int a=0;
    }
}

void changePeriodTimeA1(unsigned int t){
    TA1CCR0 = t;
}

//------------------------------------------------------------------------------
//            ADC10 start
//------------------------------------------------------------------------------
void ADC10enable(void){
    ADC10CTL0 |= ADC10ON + ADC10IE + ENC; // DIGITAL OUTPUT: activate ADC + enable interrupts + enable conversion
    ADC10CTL0 |= ADC10SC;                 // start conversion
}
//------------------------------------------------------------------------------
//            ADC10 disable
//------------------------------------------------------------------------------
void disable_ADC10(void){
    ADC10CTL0 &= ~ADC10ON;                          // de-activate
    ADC10CTL0 &= ~ADC10IE;                          // disable interrupts
    ADC10CTL0 &= ~ENC;                              // disable conversion
    ADC10CTL0 &= ~ADC10SC;                          // stop conversion
}

//------------------------------------------------------------------------------
//            UART Rx Enable Interrupt
//------------------------------------------------------------------------------
void UartRxEnable(void){
    IE2 |= UCA0RXIE;
}

//------------------------------------------------------------------------------
//            UART Rx Disable Interrupt
//------------------------------------------------------------------------------
void UartRxDisable(void){
    IE2 &= ~UCA0RXIE;
}

//------------------------------------------------------------------------------
//            UART Tx Enable Interrupt
//------------------------------------------------------------------------------
void UartTxEnable(void){
    IE2 |= UCA0TXIE;
}

//------------------------------------------------------------------------------
//            UART Tx Disable Interrupt
//------------------------------------------------------------------------------
void UartTxDisable(void){
    IE2 &= ~UCA0TXIE;
}


//---------------------------------------------------------------------
//            ADC enable
//---------------------------------------------------------------------
void ADCenable(void){
//        ADC10SA = (unsigned int)ADC_samples;
//        ADC10CTL0 |= ADC10ON + ADC10IE + ENC; // DIGITAL OUTPUT: activate ADC + enable interrupts + enable conversion
//        ADC10CTL0 |= ADC10SC;


}

