//PPSW GITHUB
#include <LPC21xx.H>
#include "led.h"
#include "timer_interrupts.h"
#include "servo.h"

#define SERVODET_bm (1<<10)
#define OFFSET_VALUE 12

enum ServoState{CALLIB, OFFSET, IDLE, IN_PROGRESS};

struct Servo{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

volatile struct Servo sServo;

void DetectorInit(void){
	IO0DIR = IO0DIR&(~(SERVODET_bm));
}

enum DetectorState{ACTIVE, INACTIVE};

enum DetectorState eReadDetector(){
	if((IO0PIN&SERVODET_bm)==0){
		return ACTIVE;
	}
	
	else{
		return INACTIVE;
	}
}

void Automat(void){
	switch(sServo.eState){
		case CALLIB:
			if(eReadDetector()==INACTIVE){
				LedStepRight();
				sServo.eState = CALLIB;
			}
			
			else{
				sServo.uiCurrentPosition = 0;
				sServo.uiDesiredPosition = 0;
				sServo.eState = IDLE;
			}
			
			break;
			
		case OFFSET:
			if(sServo.uiCurrentPosition < sServo.uiDesiredPosition){
				LedStepRight();
				sServo.uiCurrentPosition++;
				sServo.eState = OFFSET;
			}
			
			else if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){
				LedStepLeft();
				sServo.uiCurrentPosition--;
				sServo.eState = OFFSET;
			}
			
			else{
				sServo.uiCurrentPosition = 0;
				sServo.uiDesiredPosition = 0;
				sServo.eState = IDLE;
			}
			
			break;
		
		case IDLE:
			if(sServo.uiCurrentPosition!=sServo.uiDesiredPosition){
				sServo.eState = IN_PROGRESS;
			}
			
			else{
				sServo.eState = IDLE;
			}
			break;
		
		case IN_PROGRESS:
			if(sServo.uiCurrentPosition<sServo.uiDesiredPosition){
				LedStepRight();
				sServo.uiCurrentPosition++;
				sServo.eState = IN_PROGRESS;
			}
			
			else if(sServo.uiCurrentPosition>sServo.uiDesiredPosition){
				LedStepLeft();
				sServo.uiCurrentPosition--;
				sServo.eState = IN_PROGRESS;
			}
			
			else{
				sServo.eState = IDLE;
			}
		
			break;
			
		default:	
			break;
	}
}

void ServoInit(unsigned int uiServoFrequency){
	sServo.eState = CALLIB;
	LedInit();
	Timer0Interrupts_Init((1000000/uiServoFrequency),&Automat);
	while(sServo.eState != IDLE){}
}

void ServoCallib(void){
	sServo.eState = CALLIB;
}

void ServoGoTo(unsigned int uiServoPosition){
	sServo.uiDesiredPosition = uiServoPosition;
	sServo.eState = IN_PROGRESS;
	while(sServo.eState != IDLE){}
}
