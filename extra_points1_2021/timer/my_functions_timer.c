#include "../GLCD/GLCD.h"
#include <stdio.h>


void disegnaPalla(int pallax, int pallay, int mode){ //0 cancella, 1 scrivi
	int i, j;
	
	for(i=0; i<5 ; i++){
			for(j=0; j<5 ; j++){
				LCD_SetPoint(pallax+i, pallay+j, mode ? Yellow : Navy);
			}
	}
}

int aggiornaScore(int score){
	char str[sizeof(int) * 4 + 1];
	int i,j;
	
	for(i=0; i<32; i++){
   for(j=0; j<16; j++)
    LCD_SetPoint(8+i, 160+j, Navy);
  }
	
	if (score < 100)
		score += 5;
	else
		score +=10;
	
	sprintf(str, "%d", score);
	
	GUI_Text(8, 160, (uint8_t *) str, White, Navy);
	
	return score;
}

void disegnaScore_Record(int num, int mode){     //0 punteggio, 1 record
	char str[sizeof(int) * 4 + 1];
	
	sprintf(str, "%d", num);
	
	if (mode == 0)
		GUI_Text(8, 160, (uint8_t *) str, White, Navy);
	else
		GUI_Text(200, 8, (uint8_t *) str, Red, Navy);
}

void aggiornaRecord(int record){
	int i, j, x = 200, y = 8;
	char str[sizeof(int) * 4 + 1];
	
	for(i=0; i<16 ; i++){
			for(j=0; j<32 ; j++){
				LCD_SetPoint(x+j,y+i, Navy);
			}
	}
	
	sprintf(str, "%d", record);
	GUI_Text(200, 8, (uint8_t *) str, Red, Navy);
	
}
