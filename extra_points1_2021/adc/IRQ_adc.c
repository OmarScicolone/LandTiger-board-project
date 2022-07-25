#include "lpc17xx.h"
#include "adc.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h" 


extern int barrax1;
extern int barrax2;
extern int barray;

unsigned short AD_current;   
unsigned short AD_last;     // Ultimo valore convertito 

void ADC_IRQHandler(void) {
  	
  int i;
	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);			/* Valore convertito */
  
	if(AD_current != AD_last){
		for(i=0; i<10 ; i++){
			LCD_DrawLine(barrax1, barray+i, barrax2, barray+i, Navy);
		}
		
		barrax1 = (AD_current) * 180 / 0xFFF;
		barrax2 = barrax1+60;
		
		for(i=0; i<10 ; i++){
			LCD_DrawLine(barrax1, barray+i, barrax1+60, barray+i, Yellow);
		}
		
		AD_last = AD_current;
	}
}
