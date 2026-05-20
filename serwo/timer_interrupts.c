#include <LPC21xx.H>
#include "timer_interrupts.h"

#define mCounter_Enable 0x00000001
#define mCounter_Reset 0x00000002

#define mInterrupt_On_Mr0 0x00000001
#define mReset_On_Mr0 0x00000002
#define mMr0_Interrupt 0x00000001

#define VIC_Timer0_Channel_Nr 4
#define mIrq_Slot_Enable 0x00000020

void (*ptrTimer0InterruptFunction)(void);

__irq void Timer0IRQHandler(){ //potrzebne przy wywolaniu funkcji z interrupt
	T0IR=mMr0_Interrupt;
	ptrTimer0InterruptFunction();
	VICVectAddr=0x00;
}

void Timer0Interrupts_Init(unsigned int uiPeriod, void(*ptrInterruptFunction)(void)){
	VICIntEnable|=(0x1<<VIC_Timer0_Channel_Nr);
	VICVectCntl1=mIrq_Slot_Enable|VIC_Timer0_Channel_Nr;
	VICVectAddr1 = (unsigned long)Timer0IRQHandler;
	ptrTimer0InterruptFunction = ptrInterruptFunction;
	T0MR0=15*uiPeriod;
	T0MCR|=(mInterrupt_On_Mr0|mReset_On_Mr0);
	T0TCR|=mCounter_Enable;
}
