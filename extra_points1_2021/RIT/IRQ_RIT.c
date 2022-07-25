#include "lpc17xx.h"
#include "RIT.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h"
#include "../timer/my_functions_timer.h"

int down_INT0=0;							//variabili per gestire bouncing bottoni
int down_KEY1=0;
int down_KEY2=0;

extern int pallax;
extern int pallay;
extern int barrax1;

extern int score;
extern int record;

extern int angolo;

int paused = 0;
int started = 0;
int lose = 0;

void RIT_IRQHandler (void)
{	
	int i, j;
	
		/* INT0 */
		if(down_INT0!=0){ 
			if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){				// --> INT0 premuto 
				down_INT0++;		
				switch(down_INT0){
					case 2:
						if(lose == 1){
						lose = 0;
						disable_timer(0);
						paused = 0;
					
						/* Reset stato iniziale */
						pallax = 230;
						pallay = 160;
						angolo = 45;
						
						LCD_Clear(Navy);
						init_timer(0, 0x1312d0); 								// inizializzazione del timer 0 per gestione movimento palla/barra
						ADC_init();
							
						// cornice superiore
						for(i=0; i<5; i++)
							LCD_DrawLine(0,i,240,i,Red);
							
						// cornice vert SX
						for(i=0; i<5; i++)
							LCD_DrawLine(i,0,i,278,Red);
							
						// cornice vert DX
						for(i=0; i<5; i++)
							LCD_DrawLine(235+i,0,235+i,278,Red);
							
						// disegno barra iniziale
						for(i=0; i<60; i++){
							for(j=0; j<10; j++)
							LCD_SetPoint(barrax1+i, 278+j, Yellow);
						}
							
						// disegno pallina
						for(i=0; i<5; i++){
							for(j=0; j<5; j++)
							LCD_SetPoint(pallax+i, pallay+j, Yellow);
						}
						
						// disegna score
						disegnaScore_Record(score, 0);					//funzione che ridisegna losì score/record per 
																										//proteggerlo dal passaggio della palla
						// disegna record 
						disegnaScore_Record(record, 1);
					}
					break;
					default:
						break;
				}
			}
			else {																		// --> INT0 rilasciato
				down_INT0=0;			
				NVIC_EnableIRQ(EINT0_IRQn);						
				LPC_PINCON->PINSEL4    |= (1 << 20);   
			}
		}
		/* KEY1 */
		if(down_KEY1!=0){ 
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){		// --> KEY1 premuto
				down_KEY1++;				
				switch(down_KEY1){
					case 2:
						if(lose != 1){
						if(paused != 1){
							started = 1;
							for(i=0;i<84;i++){ 										//per cancellare scritte iniziali
								LCD_DrawLine(6, 44+i, 234, 44+i, Navy);
							}
							enable_timer(0);											// abilitazione del timer 0 per gestione movimento
						}
					}
					break;
					default:
						break;
				}
			}
			else {																	// --> KEY1 premuto
				down_KEY1=0;			
				NVIC_EnableIRQ(EINT1_IRQn);							
				LPC_PINCON->PINSEL4    |= (1 << 22);    
			}
		}
		/* KEY2 */
		if(down_KEY2!=0){ 
			if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	// --> KEY2 premuto
				down_KEY2++;				
				switch(down_KEY2){
					case 2:
					if(started != 0){	
						if (paused == 0){
								paused = 1;
								disable_timer(0);																					//gestione pausa
								GUI_Text(96, 45, (uint8_t *) "PAUSED", White, Navy);
								GUI_Text(7, 90, (uint8_t *) ">>Press KEY2 to resume", White, Navy);
							}
						else{
								paused = 0;
								for(i=0; i<61; i++)
									LCD_DrawLine(6,45+i,185,45+i,Navy);
								enable_timer(0);
						}
					}	
						break;
					default:
						break;
				}
			}
			else {																		// --> KEY2 rilasciato
				down_KEY2=0;			
				NVIC_EnableIRQ(EINT2_IRQn);							 
				LPC_PINCON->PINSEL4    |= (1 << 24);     
			}
		}
	
		
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}
