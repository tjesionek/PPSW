#include <LPC21xx.H>

#include "keyboard.h"
#include "servo.h"

int main(){
	DetectorInit();
	KeyboardInit();
	ServoInit(50);
	ServoGoTo(24);
	ServoGoTo(12);
	
	while(1){
		if(eKeyboardRead()==BUTTON_0){
			//sServo.eState = CALLIB;
			ServoCallib();
		}
		
		else if(eKeyboardRead()==BUTTON_1){
			//sServo.uiDesiredPosition = 12;
			ServoGoTo(12);
		}
		
		else if(eKeyboardRead()==BUTTON_2){
			//sServo.uiDesiredPosition = 24;
			ServoGoTo(24);
		}
		
		else if(eKeyboardRead()==BUTTON_3){
			//sServo.uiDesiredPosition = 36;
			ServoGoTo(36);
		}
	}
}
