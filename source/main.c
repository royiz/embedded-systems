#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
#include "../header/LCD.h"

#include <stdio.h>

enum FSMstate state;
enum SYSmode lpm_mode;
char command[7] = {"075AC8"};

void main(void){
  
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  Phi = 0.7;
  motor_deg = 0;
  sysConfig();
  while(1){
	switch(state){
	    case state0: ////Idle state
	        UARTconfig();
            enterLPM(lpm_mode);   ///sleep mode
            break;
		 
	    case state1: // stick control motor
//            UartRxDisable();
//            UartTxEnable();
        manualControl();

		break;
			 
	    case state2:    // painter


	            UartRxDisable();
	            UartTxEnable();

		break;  
		     
	    case state3:    // calibrtion1
	        calibration();
		break;   
                
        case state4:   // calibrtion2
            calculate_P_Q();
            calibration();
		break;

        case state5:
            enable_interrupts();
        break;

        case state6:
            lcd_clear();
            counterMax = 65535;
            counterMin = 0;
        break;

        case state7:            // write to flash
            if(segFlag == 0){
                FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
                initialSegment();
                segFlag = 1;
            }


        break;
        case state8:        // exute flash
            UartRxEnable();


            break;
      
	}
  }
}
  
  
  
  
  
  
