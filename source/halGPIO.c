#include  "../header/halGPIO.h"     // private library - HAL layer
#include "../header/LCD.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int KB;

unsigned int X = 500;
char x_str[4];
int flag4 = 0, digit = 2;
char Voltage_string[5];
const char string1[] = { "Hello World\r\n" };
unsigned int i = 0;
char start_calib = 0;
char end_calib = 0;
int counter_M = 0;
float Phi;
unsigned int stick_xy[2], prev_stick[2] = {0};
int low_x = 0, low_y = 0;
int high_x = 1023, high_y = 1023;
char seg_str[64];
int seg_idx=0;
char *Flash_ptr;
char segFlag = 0;
int seg_read=0;
int state2_index=0; //remove
int VY,VX;
int PB_flag=0;
int send_1=0;
int send_2=0;
int paint_flag=0;
int p =1;
int q=180;
int state8_value=0;



//--------------------------------------------------------------------
//             System Configuration
//--------------------------------------------------------------------
void sysConfig(void){
	GPIOconfig();
	TIMERconfig();
    ADCconfig();
    UARTconfig();
    Flashconfig();

}

//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int j;

	for(j=t; j>0; j--);
}


//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00)
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01)
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02)
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03)
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04)
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}

//*********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){
        //__bic_SR_register_on_exit(CPUOFF);

    delay(debounceVal);
//            selector of transition between states

    if(PBsArrIntPend & PB0){
      PB_flag=0; // ?
      PBsArrIntPend &= ~PB0;
        }

//---------------------------------------------------------------------
//            Exit from a given LPM
//---------------------------------------------------------------------
        switch(lpm_mode){
        case mode0:
         LPM0_EXIT; // must be called from ISR only
         break;

        case mode1:
         LPM1_EXIT; // must be called from ISR only
         break;

        case mode2:
         LPM2_EXIT; // must be called from ISR only
         break;

                case mode3:
         LPM3_EXIT; // must be called from ISR only
         break;

                case mode4:
         LPM4_EXIT; // must be called from ISR only
         break;
    }

}




//---------------------------------------------------------------------
//            UART Rx interrupt service routine
//---------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  stopTimerA0();
  disable_ADC10();
  RgbPlayer(0);
int b,a=3;
      if(state == state7){
          if (UCA0RXBUF!='~'){

              seg_idx++;
              write_SegC(UCA0RXBUF);
              if (Flash_master.files_count==1){
                  Flash_master.file_names[2]=2;
                  seg_idx++;
                  seg_idx--;
              }
          }
          else {

              UartTxDisable();
              UartRxDisable();
              Flash_master.files_count+=1;
              if (Flash_master.files_count==2)
              {

                  b=a;

              }



              if (Flash_master.files_count==1){
                  copy_C2D (0x1000);
                  Flash_master.file_sizes[0]=seg_idx;
                  seg_idx=0;
                  initialSegment();
                  UartRxEnable();


              }
              if (Flash_master.files_count==2){
                  Flash_master.file_sizes[1]=seg_idx;
                  copy_C2D ( 0x1040);

                  seg_idx=0;
                  initialSegment();
                  UartRxEnable();

              }
              if (Flash_master.files_count==3){
                  Flash_master.file_sizes[2]=seg_idx;
                  seg_idx=0;
                  Flash_master.file_ptrs[2]=(char*) 0x1080;
                  Flash_master.file_ptrs[1]=(char*) 0x1040;
                  Flash_master.file_ptrs[0]=(char*) 0x1000;

                  state=state0;
                  finishSegment();
                  UartRxEnable();
              }

          }
      }





      else if(state==state8){
          int value=UCA0RXBUF;
          if (value=='0' ||value=='1' ||value=='2' ){
              state8_value=UCA0RXBUF-48;
              sysConfig();
              // Motor
                MotorArrPortSel &= ~0x0F;            // GPIO capability
                MotorArrPortDir |= 0x0F;             // output dir

              readFlash();
              }


      }
      else if (state==state2){
          if (UCA0RXBUF=='0') // stay in state
          {
              paint_flag=0;
//              UartRxDisable();
//              UartTxEnable();

          }
          else if (UCA0RXBUF=='1') // leave state
          {
              state=state0;
          }
      }

      else {
          switch(UCA0RXBUF) {
          case '0':
              state = state0;
              break;

          case '1':
              state = state1;
              break;

          case '2':
              state = state2;
              break;

          case '3':
              state = state3;
              LPM0_EXIT;
              break;

          case '4':
              state = state4;
              LPM0_EXIT;
//              UartRxEnable();
              break;

          case '5': // start calibration
              if (state == state3 ||state == state4){
//                  UartTxDisable();
                  start_calib = 1;
                  LPM0_EXIT;
              }
              break;

          case '6': // stop calibration
              if (state == state3 ||state == state4 ){
                start_calib = 0;
                UartTxDisable();
//                LPM0_EXIT;
                if (state==state3) {
                    send_1=0;
                    send_2=0;
                    counter_M=0;
                    state=state0;
                }
                else if(state==state4 && send_1==0){
                    calculate_P_Q();
                    UartRxDisable();
                    UartTxEnable();
                    send_1=1;

                }
              }
              break;

          case '7':
                state = state7;
                break;
          case '8':
              state = state8;
              break;

          case '9':
              state = state0;
              break;

          default:
              state = state0;
                            break;
          }
      }



   switch(lpm_mode){
    case mode0:
     LPM0_EXIT; // must be called from ISR only
     break;

    case mode1:
     LPM1_EXIT; // must be called from ISR only
     break;

    case mode2:
     LPM2_EXIT; // must be called from ISR only
     break;

    case mode3:
     LPM3_EXIT; // must be called from ISR only
     break;

    case mode4:
     LPM4_EXIT; // must be called from ISR only
     break;
   }
}

//---------------------------------------------------------------------
//            UART Tx interrupt service routine
//---------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    while (!(IFG2&UCA0TXIFG));
    if( state == state4 && start_calib == 0 ){ //

            if(send_1 == 1 && send_2==0){
                UCA0TXBUF = p;
                send_2=1;
                }
            else if(send_1 == 1 && send_2==1){
                UCA0TXBUF = q;
                send_1=0; send_2=0;
                UartTxDisable();
                UartRxEnable();
                }
            }
    if (state==state1){
        ADCconfig();
        getStickValue();
        DelayMs(1000);

        if (stick_xy[0]>800&&stick_xy[0]<1000&&stick_xy[1]>800&&stick_xy[1]<1100)
            {
            stepper_deg2(315); //(x,y) (+,+)
            UCA0TXBUF=8;
            }
        else if (stick_xy[0]>800&&stick_xy[1]>350&&stick_xy[1]<700){
            stepper_deg2(270); //(x,y) (+,N)
            UCA0TXBUF=7;
        }
        else if (stick_xy[0]>800&&stick_xy[0]<1000&&stick_xy[1]<350){
            stepper_deg2(225); //(x,y) (+,-)
            UCA0TXBUF=6;
        }
        else if (stick_xy[0]>350&&stick_xy[0]<700&&stick_xy[1]<350){
            stepper_deg2(180); //(x,y) (N,-)
            UCA0TXBUF=5;
        }
        else if (stick_xy[0]<350&&stick_xy[1]<350){
            stepper_deg2(135); //(x,y) (-,-)
            UCA0TXBUF=4;
        }
        else if (stick_xy[0]<350&&stick_xy[1]>350&&stick_xy[1]<700){
            stepper_deg2(90); //(x,y) (-,N)
            UCA0TXBUF=3;
        }
        else if (stick_xy[0]<350&&stick_xy[1]>800&&stick_xy[1]<1000){
            stepper_deg2(15); //(x,y) (-,+)
            UCA0TXBUF=2;
        }
        else if (stick_xy[0]>350&&stick_xy[0]<700&&stick_xy[1]>800&&stick_xy[1]<1000){
            stepper_deg2(0); //(x,y) (N,+)
            UCA0TXBUF=1;
        }
        else UCA0TXBUF=0;
                                                                                                  // dont move //(x,y) (N,N)
        disable_ADC10();
        }



    else if (state==state2){
        ADCconfig();
        getStickValue();
        DelayMs(1000);
        if(PB_flag==1){
            PB_flag=0;
            UCA0TXBUF=9;
        }
        else if (stick_xy[0]>800&&stick_xy[0]<1000&&stick_xy[1]>800&&stick_xy[1]<1100) UCA0TXBUF=8; //(x,y) (+,+)
        else if (stick_xy[0]>800&&stick_xy[0]<1000&&stick_xy[1]>350&&stick_xy[1]<700)  UCA0TXBUF=7; //(x,y) (+,N)
        else if (stick_xy[0]>800&&stick_xy[0]<1000&&stick_xy[1]<350)                   UCA0TXBUF=6; //(x,y) (+,-)
        else if (stick_xy[0]>350&&stick_xy[0]<700&&stick_xy[1]<350)                    UCA0TXBUF=5; //(x,y) (N,-)
        else if (stick_xy[0]<350&&stick_xy[1]<350)                                     UCA0TXBUF=4; //(x,y) (-,-)
        else if (stick_xy[0]<350&&stick_xy[1]>350&&stick_xy[1]<700)                    UCA0TXBUF=3; //(x,y) (-,N)
        else if (stick_xy[0]<350&&stick_xy[1]>800&&stick_xy[1]<1000)                   UCA0TXBUF=2; //(x,y) (-,+)
        else if (stick_xy[0]>350&&stick_xy[0]<700&&stick_xy[1]>800&&stick_xy[1]<1000)  UCA0TXBUF=1; //(x,y) (N,+)
        else UCA0TXBUF=0;                                                                           // dont move //(x,y) (N,N)
        disable_ADC10();
        paint_flag++;
//        if (paint_flag=1000){
//            UCA0TXBUF=9; // check if we left the state
//            UartTxDisable();
//            UartRxEnable();
//        }

    }


      switch(lpm_mode){
            case mode0:
             LPM0_EXIT; // must be called from ISR only
             break;

            case mode1:
             LPM1_EXIT; // must be called from ISR only
             break;

            case mode2:
             LPM2_EXIT; // must be called from ISR only
             break;

             case mode3:
             LPM3_EXIT; // must be called from ISR only
             break;

             case mode4:
             LPM4_EXIT; // must be called from ISR only
             break;
          }
}



//*********************************************************************
//            TA0_A1 Interrupt vector
//*********************************************************************
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
    //TACCTL0 &= ~CCIFG;
    LPM0_EXIT;
//    return;
}

//*********************************************************************
//            TA1_A1 Interrupt vector
//*********************************************************************
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
{
    //TACCTL0 &= ~CCIFG;
    LPM0_EXIT;
//    return;
}


//*********************************************************************
//            ADC10 Interrupt Service Routine
//*********************************************************************
#pragma vector=ADC10_VECTOR
 __interrupt void ADC10_ISR(void){
     //Send to PC voltage values to calculate angle



 }

 //--------------------------------------------------------------------
 //              Print Byte to 4-bit Phases array
 //--------------------------------------------------------------------
void print2Phases(unsigned char ch){
    MotorArrPort &= ~0x0F;
    MotorArrPort |= ch;
}


//--------------------------------------------------------------------
//              Print Byte to 8-bit LEDs array
//--------------------------------------------------------------------
void print2LEDs(unsigned char ch){
    LEDsArrPort = ch;
}

//--------------------------------------------------------------------
//              Print Byte to 4-bit LEDs array
//--------------------------------------------------------------------
void print2HalfLEDs(unsigned char ch){
    LEDsArrPort &= ~0xF0;
    LEDsArrPort |= ch << 4; //Upper Half
}

//--------------------------------------------------------------------
//              Clear 8-bit LEDs array
//--------------------------------------------------------------------
void clrLEDs(void){
    LEDsArrPort &= ~0xF0 ;
    RGBsArrPort &= ~RGBmask;
}
//--------------------------------------------------------------------
//              Toggle 8-bit LEDs array
//--------------------------------------------------------------------
void toggleLEDs(char ch){
    LEDsArrPort ^= ch;
}
//--------------------------------------------------------------------
//              Set 8-bit LEDs array
//--------------------------------------------------------------------
void setLEDs(char ch){
    LEDsArrPort |= ch;
}
//--------------------------------------------------------------------
//              Read value of 4-bit SWs array
//--------------------------------------------------------------------
unsigned char readSWs(void){
//    unsigned char ch;
//
//    ch = PBsArrPort;
//    ch &= SWmask;     // mask the least 4-bit
//    return ch;
}
//---------------------------------------------------------------------
//             Increment / decrement LEDs shown value
//---------------------------------------------------------------------
void incLEDs(char val){
    LEDsArrPort += val;
}

//---------------------------------------------------------------------
//                              RGB
//---------------------------------------------------------------------
void RgbPlayer(unsigned int color) {
    RGBsArrPort &= ~RGBmask;
    RGBsArrPort |= color<<5; //!!!
}

//---------------------------------------------------------------------
//             get delay value from UART
//---------------------------------------------------------------------
void getDelay(unsigned int t){ //!!! to add d
    flag4 = 1;
//    enterLPM(lpm_mode);
    X = atoi(x_str);
    X += 200;
    t = X<<6;
    TACCR0 = t;
    flag4 = 0;
    digit = 2;
//    UartRxEnable();
//    state = state0;
}

//---------------------------------------------------------------------
//             Joystick functions
//---------------------------------------------------------------------
void getStickValue(void){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC; //start sample
        ADC10SA = (unsigned int)stick_xy; //transfer the sampling to the array
}



void displayVoltage(unsigned int sample){

}

void HellowWorld(void){
    i = 0;
    IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
    UCA0TXBUF = string1[i++];
}

/////////////////////////////////////////////////////////////////
//                  flash
/////////////////////////////////////////////////////////////////
void write_SegC(char value)
{
//  char *Flash_ptr;                          // Flash pointer
  *Flash_ptr++ = value;                   // Write value to flash
}

void initialSegment(void){
    Flash_ptr = (char *)0x1080;              // Initialize Flash pointer
    FCTL1 = FWKEY + ERASE;                    // Set Erase bit
    FCTL3 = FWKEY;                            // Clear Lock bit
    *Flash_ptr = 0;                           // Dummy write to erase Flash segment
    FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
}

void finishSegment(void){
    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

void copy_C2D ( int newAddress)
{
  char *Flash_ptrC;                         // Segment C pointer
  char *Flash_ptrD;                         // Segment D pointer
  unsigned int i;

  Flash_ptrC = (char *) 0x1080;             // Initialize Flash segment C pointer
  Flash_ptrD = (char *) newAddress;         // Initialize Flash segment D pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *Flash_ptrD = 0;                          // Dummy write to erase Flash segment D
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i=0; i<64; i++)
  {
    *Flash_ptrD++ = *Flash_ptrC++;          // copy value segment C to segment D
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}
void readFlash( void){
    int i=0;
    int parmeter;
    char* head =Flash_master.file_ptrs[state8_value];
    int loop_size=Flash_master.file_sizes[state8_value];
    while (i<loop_size)  {
        parmeter = readSegment(head);
        head=head+parmeter+1;
        i=i+parmeter+1;
    }
}
int readSegment(char* head ){
    // head is value in hex to the head of the seg
    char head_value= *head;
    int parmeter;
    switch (head_value) {
    case '\x01' :
        *head++;
        head_value=(char) *head;
        parmeter= head_value;
        BlinkRGB(parmeter);
        return 1;
        break;

    case '\x02' :
        *head++;
        head_value=(char) *head;
        parmeter= head_value;
        rlc_leds(parmeter);
        return 1;
        break;

    case '\x03':
        *head++;
        head_value=(char) *head;
        parmeter= head_value;
        rrc_leds(parmeter);
        return 1;
        break;


    case '\x04' :
        *head++;
        head_value=(char) *head;
        parmeter= head_value;
        getDelay(parmeter);
        return 1;
        break;

    case '\x05':
       clrLEDs();
        return 0;
        break;


    case '\x06':
        *head++;
        head_value=(char) *head;
        parmeter= head_value;
        stepper_deg2(parmeter);
        return 1;
        break;


    case '\x07':
        *head++;
        head_value=(char) *head;
        int x= head_value;
        *head++;
        head_value=(char) *head;
        int r=head_value;
        stepper_scan(x,r);
        return 2;
        break;
    case '\x08':
        state=state0;

        return 0;
        break;

    default: // and case 0x08
            state=state0;
            return 0;
            break;
        }

}

void ser_output(char *str){
    while(*str != 0){
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }

}

void print_the_stick(void){
    char vt_chara6[5];
    char vt_chara7[5];
    ltoa(stick_xy[1],vt_chara6); ltoa(stick_xy[0],vt_chara7);
    state2_index++;

    ser_output(vt_chara6);  ser_output(vt_chara7);


}

void   calculate_P_Q(void){
    int temp;
    int i=100;
    temp= counter_M;
    p=0;
    q=0;
    while (i<temp){
        temp=temp-i;
        p++;
    }
    q=temp;
}

