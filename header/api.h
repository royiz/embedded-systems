#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern void BlinkRGB(int x);
extern void rlc_leds(int x);
extern void rrc_leds(int x);
extern void stepper_deg(int p);
extern void stepper_deg2(int p);
extern void stepper_scan(int l, int r);
extern void turn_right_phi(int rotations);
extern void turn_left_phi(int rotations);
extern void manualControl(void);
extern void operateCmd(char opc[]);
extern int hex2int(char *hex);
extern void samplePoten(void);

extern long int counterMax, counterMin;
extern int motor_deg;

#endif







