#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "my_functions_timer.h"
#include "../TouchPanel/TouchPanel.h"
#include "../adc/adc.h"
#include "../RIT/RIT.h"

extern volatile int pallax;
extern volatile int pallay;

extern volatile int angolo;
extern volatile int dirx;
extern volatile int diry;

extern volatile int barray;
extern volatile int barrax1;
extern volatile int barrax2;

extern volatile int score;
extern volatile int record;

static int rimbalzo = 0; 				/*0: prosegue; 1: rimbalzo muro; 2 rimbalzo barra*/

extern int started;
extern int lose;


uint16_t Sinusoide[45] =
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

/* GESTIONE TIMER 0 */ 
void TIMER0_IRQHandler (void)
{
	ADC_start_conversion(); /*Gestione barra*/
	
	/* Gestione Palla */
	disegnaPalla(pallax, pallay, 0);
	
	// YOU LOSE
	if( (pallay >= 320) || ((pallay+5 > 278) && ((pallax+5 <= 1) || (pallax >= 319)))
		){
		started = 0;
		lose = 1;
		disable_timer(0);
		GUI_Text(88, 45, (uint8_t *)"YOU LOSE" , White, Navy);
		GUI_Text(7, 90, (uint8_t *)">>Press INT0 to restart" , White, Navy);
	}
	
	 
	
	/* AGGIORNAMENTO DEL RECORD */
	if (score > record){
		record = score;
		aggiornaRecord(record);
	}
	
	/* CHECK POSIZIONE PALLA RISPETTO RECORD */
	if ((pallax >= 200 && pallax <= 232 && pallay >= 8 && pallay <= 24) || 
			(pallax + 5 >= 200 && pallax + 5 <= 232 && pallay >= 8 && pallay <= 24) ||
			(pallax >= 200 && pallax <= 232 && pallay + 5 >= 8 && pallay + 5 <= 24) ||
			(pallax + 5 >= 200 && pallax + 5 <= 232 && pallay + 5 >= 8 && pallay + 5 <= 24)){	
			disegnaScore_Record(record,1); //protezione record per evitare che la palla cancelli in numero
	}

	/* CHECK POSIZIONE PALLA RISPETTO SCORE */
	if ((pallax >= 8 && pallax <= 40 && pallay >= 160 && pallay <= 176) || 
			(pallax + 5 >= 8 && pallax + 5 <= 40 && pallay >= 160 && pallay <= 176) ||
			(pallax >= 8 && pallax <= 40 && pallay + 5 >= 160 && pallay + 5 <= 176) ||
			(pallax + 5 >= 8 && pallax + 5 <= 40 && pallay + 5 >= 160 && pallay + 5 <= 176)){
			disegnaScore_Record(score,0); //protezione score per evitare che la palla cancelli in numero
	}
	
	/* MOVIMENTO PALLA */
	if(dirx == 0){				//verso sx
		if(diry == 0){			//verso sx-basso
			switch(angolo){
				case 45:
					/* rimbalzo con barra */
					if(pallay+5 == barray || pallay+5 == barray - 1){
						if(pallax+5 >= barrax1 && pallax < barrax1+60){
							rimbalzo = 2;
							//aggiornamento punteggio
							score = aggiornaScore(score);
		
							diry = 1;
							if (pallax + 2 >= barrax1 + 20 && pallax + 3 <= barrax1 + 40 ){
								angolo = 60;
								pallax -= 1;
								pallay -= 2;
							}
							else {
								angolo = 30;
								pallax -= 2;
								pallay -= 1;
							}
							disegnaPalla(pallax, pallay, 1);
						}
						else{
						pallax -= 1;
						pallay += 1;
						disegnaPalla(pallax, pallay, 1);
						}
					}
					else{
						pallax -= 1;
						pallay += 1;
						disegnaPalla(pallax, pallay, 1);
					}
				break;
				case 60:
					/* rimbalzo con barra */
					if(pallay+5 == barray || pallay+5 == barray - 1){
						if(pallax+5 >= barrax1 && pallax < barrax1+60){
							rimbalzo = 2;
							//aggiornamento punteggio
							score = aggiornaScore(score);
							
							diry = 1;
							if (pallax + 2 >= barrax1 + 20 && pallax + 3 <= barrax1 + 40){
								angolo = 60;
								pallax -= 1;
								pallay -= 2;
							}
							else {
								angolo = 30;
								pallax -= 2;
								pallay -= 1;
							}
							disegnaPalla(pallax, pallay, 1);
						}
						else{
						pallax -= 1;
						pallay += 2;
						disegnaPalla(pallax, pallay, 1);
						}
					}
					/* rimbalzo parete sinistra */
					else if(pallax <= 6 && pallay+5 < 278){
						rimbalzo = 1;
						dirx = 1;
						pallax += 1;
						pallay += 2;
						disegnaPalla(pallax, pallay, 1);
					}
					/* procede normalmente */
					else{
						pallax -= 1;
						pallay += 2;
						disegnaPalla(pallax, pallay, 1);
					}
				break;
				case 30:
					/* rimbalzo con barra */
					if(pallay+5 == barray || pallay+5 == barray - 1){
						if(pallax+5 >= barrax1 && pallax < barrax1+60){
							
							rimbalzo = 2;
							
							//aggiornamento punteggio
							score = aggiornaScore(score);
							
							diry = 1;
							if (pallax + 2 >= barrax1 + 20 && pallax + 3 <= barrax1 + 40){
								angolo = 60;
								pallax -= 1;
								pallay -= 2;
							}
							else {
								angolo = 30;
								pallax -= 2;
								pallay -= 1;
							}
							disegnaPalla(pallax, pallay, 1);
						}
						else{
						pallax -= 2;
						pallay += 1;
						disegnaPalla(pallax, pallay, 1);
						}
					}
					/* rimbalzo parete sinistra */
					else if(pallax <= 6 && pallay+5 < 278){
						rimbalzo = 1;
						dirx = 1;
						pallax += 2;
						pallay += 1;
						disegnaPalla(pallax, pallay, 1);
					}
					/* procede normalmente */
					else{
						pallax -= 2;
						pallay += 1;
						disegnaPalla(pallax, pallay, 1);
					}
				break;
			}
		}
		else{								//verso sx-alto
			switch(angolo){
				case 30:
					//controllo rimbalzi con la parete sinistra
					if(pallax <= 6){
						rimbalzo = 1;
						dirx = 1;
						pallax += 2;
						pallay -= 1;
						disegnaPalla(pallax, pallay, 1);
					}
					//controllo rimbalzo tetto
					else if(pallay <= 6){
						rimbalzo = 1;
						diry = 0;
						pallax -= 2;
						pallay += 1;
						disegnaPalla(pallax, pallay, 1);
					}
					//prosegue normalmente
					else{
						pallax -= 2;
						pallay -= 1;
						disegnaPalla(pallax, pallay, 1);
					}
				break;
				
				case 60:
					//controllo rimbalzi con la parete sinistra
					if(pallax <= 6){
						rimbalzo = 1;
						dirx = 1;
						pallax += 1;
						pallay -= 2;
						disegnaPalla(pallax, pallay, 1);
					}
					//controllo rimbalzo tetto
					else if(pallay <= 6){
						rimbalzo = 1;
						diry = 0;
						pallax -= 1;
						pallay += 2;
						disegnaPalla(pallax, pallay, 1);
					}
					//prosegue normalmente
					else{
						pallax -= 1;
						pallay -= 2;
						disegnaPalla(pallax, pallay, 1);
					}
				break;
			}
		}
	}
	else{									//verso dx
		if(diry == 0){			//verso dx-basso
			switch(angolo){
				case 30:
					/* rimbalzo con barra */
					if(pallay+5 == barray || pallay+5 == barray - 1){
						if(pallax+5 >= barrax1 && pallax < barrax1+60){
							rimbalzo = 2;
							//aggiornamento punteggio
							score = aggiornaScore(score);
							
							diry = 1;
							if (pallax + 2 >= barrax1 + 20 && pallax + 3 <= barrax1 + 40){
								angolo = 60;
								pallax += 1;
								pallay -= 2;
							}
							else {
								angolo = 30;
								pallax += 2;
								pallay -= 1;
							}
							disegnaPalla(pallax, pallay, 1);
						}
						else{
							pallax += 2;
							pallay += 1;
							disegnaPalla(pallax, pallay, 1);
						}
					}
					/* rimbalzo parete destra */
					else if(pallax >= 229 && pallay+5 < 278){
						rimbalzo = 1;
						dirx = 0;
						pallax -= 2;
						pallay += 1;
						disegnaPalla(pallax, pallay, 1);
					}
					/* procede normalmente */
					else{
						pallax += 2;
						pallay += 1;
						disegnaPalla(pallax, pallay, 1);
					}
				break;
				case 60:
					/* rimbalzo con barra */
					if(pallay+5 == barray || pallay+5 == barray - 1){
						if(pallax+5 >= barrax1 && pallax < barrax1+60){
							rimbalzo = 2;
							//aggiornamento punteggio
							score = aggiornaScore(score);
							
							diry = 1;
							if (pallax + 2 >= barrax1 + 20 && pallax + 3 <= barrax1 + 40){
								angolo = 60;
								pallax += 1;
								pallay -= 2;
							}
							else {
								angolo = 30;
								pallax += 2;
								pallay -= 1;
							}
							disegnaPalla(pallax, pallay, 1);
						}
						pallax += 1;
						pallay += 2;
						disegnaPalla(pallax, pallay, 1);
					}
					/* rimbalzo parete destra */
					else if(pallax >= 229 && pallay+5 < 278){
						rimbalzo = 1;
						dirx = 0;
						pallax -= 1;
						pallay += 2;
						disegnaPalla(pallax, pallay, 1);
					}
					/* procede normalmente */
					else{
						pallax += 1;
						pallay += 2;
						disegnaPalla(pallax, pallay, 1);
					}
				break;
			}
		}
		else{								//verso dx-alto
			switch(angolo){
				case 30:
					//controllo rimbalzi con la parete destra
					if(pallax >= 229){
						rimbalzo = 1;
						dirx = 0;
						pallax -= 2;
						pallay -= 1;
						disegnaPalla(pallax, pallay, 1);
					}
					//controllo rimbalzo tetto
					else if(pallay <= 6){
						rimbalzo = 1;
						diry = 0;
						pallax += 2;
						pallay += 1;
						disegnaPalla(pallax, pallay, 1);
					}
					//prosegue normalmente
					else{
						pallax += 2;
						pallay -= 1;
						disegnaPalla(pallax, pallay, 1);
					}
				break;
				
				case 60:
					//controllo rimbalzi con la parete destra
					if(pallax >= 229){
						rimbalzo = 1;
						dirx = 0;
						pallax -= 1;
						pallay -= 2;
						disegnaPalla(pallax, pallay, 1);
					}
					//controllo rimbalzo tetto
					else if(pallay <= 6){
						rimbalzo = 1;
						diry = 0;
						pallax += 1;
						pallay += 2;
						disegnaPalla(pallax, pallay, 1);
					}
					//prosegue normalmente
					else{
						pallax += 1;
						pallay -= 2;
						disegnaPalla(pallax, pallay, 1);
					}
				break;
		}
	}
}
	//Gestione Sonoro
	if(rimbalzo != 0) {
		if(rimbalzo == 1)
			init_timer(1, 1890);				//timer 1 per la gestione del suono
		else
			init_timer(1, 1125);
		enable_timer(1);
		rimbalzo = 0;
	}
	
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
	}

/* GESTIONE TIMER 1 */ 
void TIMER1_IRQHandler (void)
{
	static int index = 0;
	/* DAC management */	
	LPC_DAC->DACR = Sinusoide[index]<<6;
	index++;
	if(index==47){ 
		disable_timer(1);
		reset_timer(1);
		index = 0;
	}
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}
