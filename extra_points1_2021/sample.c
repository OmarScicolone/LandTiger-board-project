#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "adc/adc.h"
#include "led/led.h"
#include "button/button.h"
#include "RIT/RIT.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

volatile int barray = 278;		//coordinate barra 
volatile int barrax1 = 90;		
volatile int barrax2 = 150;		

volatile int pallax = 230;			//coordinate palla 
volatile int pallay = 160;

volatile int angolo = 60;			//direzione palla iniziali
volatile int dirx = 0;
volatile int diry = 0;

volatile int score = 0;				//punteggio iniziale
volatile int record = 100;		//record iniziale

int main(void)
{
	int i, j;
	int x = 0;
	int y = 0;	
	
  SystemInit();  							//inizializzazioni...
	BUTTON_init();
  LCD_Initialization();
	LCD_Clear(Navy);
	init_timer(0, 0X1312d0);							
	init_RIT(0X4c4b40);
	ADC_init();

	/* GRAFICA INIZIALE */
	for (i=0; i<5; i++){
		LCD_DrawLine(x, y+i, x+240, y+i, Red);	//Cornice Superiore
	}
	for (i=0; i<5; i++){
		LCD_DrawLine(x+i, y, x+i, y+278, Red);	//Cornice Laterale sx
	}
	x = 235;
	for (i=0; i<5; i++){
		LCD_DrawLine(x+i, y, x+i, y+278, Red);	//Cornice Laterale dx
	}
	
	for(i=0; i<10 ; i++){
		LCD_DrawLine(barrax1, barray+i, barrax2, barray+i, Yellow);  //Disegno Barra 
	}
	
	for(i=0; i<5 ; i++){
			for(j=0; j<5 ; j++){
				LCD_SetPoint(pallax+i, pallay+j, Yellow);								//Disegno Palla
			}
	}
		
	GUI_Text(8, 160, (uint8_t *) "0", White, Navy);							//Disegno score iniziale
	GUI_Text(200, 8, (uint8_t *) "100", Red, Navy);							//Disegno record iniziale
	
	GUI_Text(56, 45, (uint8_t *) "WELCOME TO PONG", White, Navy);
	GUI_Text(7, 90, (uint8_t *) ">>Press KEY1 to start", White, Navy);
	GUI_Text(7, 110, (uint8_t *) ">>Press KEY2 to pause", White, Navy);
	
	enable_RIT();													//...per gestione pulsanti
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
  while (1)	
  {
		__ASM("wfi");
  }
}

