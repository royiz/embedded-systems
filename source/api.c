#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer

#include "../header/LCD.h"
#include <math.h>

#define PI 3.14159265

long int counterMax = 65535;
long int counterMin = 0;
int motor_deg;


//-----------------------------------------------------------------
//            Script mode - High level functions
//-----------------------------------------------------------------

void BlinkRGB(int x){
    unsigned int color = 0;
    int i = 0;
    for(i = 0; i < x; i++){
        for(color = 0; color < 8; color++){
            RgbPlayer(color);
            startTimerA0();
            enterLPM(lpm_mode);
        }
    }
    stopTimerA0();
}

void rlc_leds(int x){ //!!!
    unsigned char val = 1;
    int i = 0;
    for(i = 0; i < x; i++){
        val = 1;
        while(val < 16){
            print2HalfLEDs(val);
            val = val<<1;
            startTimerA0();
            enterLPM(lpm_mode);
        }
    }
    stopTimerA0();
}

void rrc_leds(int x){ //!!!
    unsigned char val = 8;
    int i = 0;
    for(i = 0; i < x; i++){
        val = 8;
        while(val){
            print2HalfLEDs(val);
            val = val>>1;
            startTimerA0();
            enterLPM(lpm_mode);
        }
    }
    stopTimerA0();
}

//-----------------------------------------------------------------
//            Motor functions
//-----------------------------------------------------------------
void stepper_deg(int p){
    int deg;
    float rotations;
    deg = (p - motor_deg);
    if (deg>= 360) deg = deg - 360;
    if (deg <   0) deg = deg + 360;

    if (deg >= 0 && deg <= 180){
        rotations = deg/Phi;
        turn_left_phi(floor(rotations));
    }
    else {
        rotations = (deg-180)/Phi;
        turn_right_phi(floor(rotations));
    }
    //send p to screen
    motor_deg=p;

}

void stepper_deg2(int p){
    int deg;
    float rotations;
    deg = abs(p - motor_deg);
    rotations = deg/Phi;
    if(p>motor_deg) {
        if (deg<180) turn_left_phi (floor(rotations));
        else         turn_right_phi(floor(rotations));
    }
    else{
        if (deg<180) turn_right_phi(floor(rotations));
        else         turn_left_phi (floor(rotations));
    }
    //send p to screen
    motor_deg=p;

}

void stepper_scan(int l, int r){
    stepper_deg2(l);
    stepper_deg2(r);
}

void turn_right_phi(int rotations){
    unsigned char val = 1;
    unsigned int counter = 0;
    while(counter < rotations){
        counter++;
        val = 1;
        while(val){
            print2Phases(val);
            val = val<<1;
            startTimerA1();
            enterLPM(lpm_mode);
        }
    }
    stopTimerA1();
//    state = state0;
}

void turn_left_phi(int rotations){
    unsigned char val = 8;
    unsigned int counter = 0;
    while(counter < rotations){
        counter++;
        val = 8;
        while(val){
            print2Phases(val);
            val = val>>1;
            startTimerA1();
            enterLPM(lpm_mode);
        }
    }
    stopTimerA1();
//    state = state0;
}

void calibration(void){
    unsigned char val = 8;
    unsigned int t = 0x0300;
    changePeriodTimeA1(t);
    counter_M = 0;
    UartRxEnable();
    if(start_calib == 0){
        enterLPM(lpm_mode);
    }
    while(start_calib){
        val = 8;
        counter_M++;
        while(val){
            print2Phases(val);
            val = val>>1;
            startTimerA1();
            UartRxEnable();
            enterLPM(lpm_mode);
        }
    }
    stopTimerA1();
    t = 0x0100;
    changePeriodTimeA1(t);
    Phi = 360.0/counter_M;
    motor_deg = 0;
//    UartTxEnable();
//    enterLPM(lpm_mode);
    end_calib = 1;
//    UartTxEnable();
//    enterLPM(lpm_mode);
    // power on TX
//    state = state0;
}

//-----------------------------------------------------------------
//            Joystick functions
//-----------------------------------------------------------------
void manualControl(void){
           ADCconfig();
           getStickValue();
           DelayMs(1000);

           if (stick_xy[0]>800&&stick_xy[0]<1000&&stick_xy[1]>800&&stick_xy[1]<1100) stepper_deg2(315); //(x,y) (+,+)
           else if (stick_xy[0]>800&&stick_xy[1]>350&&stick_xy[1]<700) stepper_deg2(270); //(x,y) (+,N)
           else if (stick_xy[0]>800&&stick_xy[0]<1000&&stick_xy[1]<350)stepper_deg2(225); //(x,y) (+,-)
           else if (stick_xy[0]>350&&stick_xy[0]<700&&stick_xy[1]<350) stepper_deg2(180); //(x,y) (N,-)
           else if (stick_xy[0]<350&&stick_xy[1]<350)                  stepper_deg2(135); //(x,y) (-,-)
           else if (stick_xy[0]<350&&stick_xy[1]>350&&stick_xy[1]<700) stepper_deg2(90); //(x,y) (-,N)
           else if (stick_xy[0]<350&&stick_xy[1]>800&&stick_xy[1]<1000)stepper_deg2(15); //(x,y) (-,+)
           else if (stick_xy[0]>350&&stick_xy[0]<700&&stick_xy[1]>800&&stick_xy[1]<1000)stepper_deg2(0); //(x,y) (N,+)
                                                                                      // dont move //(x,y) (N,N)
           disable_ADC10();

//        getStickValue();
//
//
//        if (stick_xy[0] >400&&stick_xy[0]<500&&stick_xy[1]>900) {
//            stepper_deg(0); //400<x<500 900<y deg=0
//        }
//
//        if (stick_xy[0]>900&&stick_xy[0]<1100&&stick_xy[1]>900&&stick_xy[1]<1100){
//            stepper_deg(315); //
//        }
//        if (stick_xy[0]>900&&stick_xy[0]<1100&&stick_xy[1]>3500&&stick_xy[1]<550){
//            stepper_deg(270); //
//        }
//        if (stick_xy[0]>900&&stick_xy[0]<1100&&stick_xy[1]>=0&&stick_xy[1]<150){
//            stepper_deg(225); //
//        }
//        if (stick_xy[0]>350&&stick_xy[0]<550&&stick_xy[1]>=0&&stick_xy[1]<150){
//            stepper_deg(180); //
//        }
//        if (stick_xy[0]>=0&&stick_xy[0]<150&&stick_xy[1]>=0&&stick_xy[1]<150) {
//            stepper_deg(135); //
//        }
//        if (stick_xy[0]>=0&&stick_xy[0]<150&&stick_xy[1]>350&&stick_xy[1]<550) {
//            stepper_deg(90); //
//        }
//        if (stick_xy[0]>=0&&stick_xy[0]<150&&stick_xy[1]>900&&stick_xy[1]<1150) {
//            stepper_deg(45); //
//        }



}

//-----------------------------------------------------------------
//            Script Mode functions
//-----------------------------------------------------------------
void operateCmd(char command[]){
    int x;
    int r;
    char hex[2];
    char cmd;
    cmd = command[1];
    switch(cmd){
    case '1':
        hex[0] = command[2];
        hex[1] = command[3];
        x = hex2int(hex);
        BlinkRGB(x);
        break;

    case '2':
        hex[0] = command[2];
        hex[1] = command[3];
        x = hex2int(hex);
        rlc_leds(x);
        break;

    case '3':
        hex[0] = command[2];
        hex[1] = command[3];
        x = hex2int(hex);
        rrc_leds(x);
        break;

    case '4':
        hex[0] = command[2];
        hex[1] = command[3];
        x = hex2int(hex);
        getDelay(0); //!!!
        break;

    case '5':
        clrLEDs();
        break;

    case '6':
        hex[0] = command[2];
        hex[1] = command[3];
        x = hex2int(hex);
        stepper_deg(x);
        break;

    case '7':
        hex[0] = command[2];
        hex[1] = command[3];
        x = hex2int(hex);
        hex[0] = command[4];
        hex[1] = command[5];
        r = hex2int(hex);
        stepper_scan(x,r);
        break;

    case '8':
        enterLPM(lpm_mode);
        break;

    default:
        enterLPM(lpm_mode);
        break;
    }
}

int hex2int(char *hex) { //!! to fix
    int val = 0;
    int i = 0;
    for(i = 0; i < 2; i++) {
        // get current character then increment
        int byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

void samplePoten(void){
    ADC10enable();
    enterLPM(lpm_mode);
}


