#include <LPC21xx.H>
#include "timer.h"

#define mCounter_Enable (1<<0)
#define mCounter_Reset (1<<1)
#define mReset_On_Mr0 (1<<1)
#define mInterupt_On_Mr0 (1<<0)
#define mInterupt_Flag_Mr0 (1<<0)

void InitTimer0(void){
	T0TCR = mCounter_Enable;
	T0TCR|=1<<mCounter_Enable;
}

void WaitOnTimer0(unsigned int uiTime){
	T0TCR|=mCounter_Reset;
	T0TCR&=(~mCounter_Reset);
	
	while(T0TC<((uiTime)*15)){} 
}

void InitTimer0Match0(unsigned int uiDelayTime){
	T0MR0=uiDelayTime*15;
	T0MCR|=(mReset_On_Mr0|mInterupt_On_Mr0);
	T0TCR|=mCounter_Reset;
	T0TCR&=~mCounter_Reset;
	T0TCR|=mCounter_Enable;
}

void WaitOnTimer0Match0(){
	while((T0TCR&mInterupt_Flag_Mr0)==0){}
	T0IR=mInterupt_Flag_Mr0;
}

