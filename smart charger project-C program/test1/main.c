#include <stdio.h>
#include "lib/ssd1306.h"
# include <avr/eeprom.h>
#include <stdlib.h>
#include <string.h>
uint8_t addr = SSD1306_ADDRESS;
#include <math.h>;
#include <avr/delay.h>


int um=1;
int dm=0;
int rm=0;

#define up (PIND & 0B00100000)
#define down (PIND & 0B00010000)
#define back (PIND & 0B00001000)
#define Enter (PIND & 0B00000100)


typedef struct chargePar
{
	char device[10];
	char vol[10];
	char cur[10];

} chargePar;

chargePar array[3];

uint16_t items;


void update_menu(int);
void detailed_menu(int);
void RTV_menu(int);

void run(int);
void temp_step(int);
void time_step(int);
void update_edit(int);
void temp_edit_value(int);
void timer_edit_value(int);
void updateDisplay(double fl);
void write_charPara(int k, chargePar chp);
void Read_charPara();






int main(void)
{
	
		//chargepar chp0;
		//chargepar chp1;
		//chargepar chp2;
		//strcpy(chp0.vol,"5");
		//strcpy(chp0.cur,"2");
		//strcpy(chp0.device,"m31");
		//eeprom_write_block(&chp0,(void*)(0*sizeof(chargepar)),sizeof(chargepar));
		//strcpy(chp1.vol,"5");
		//strcpy(chp1.cur,"3");
		//strcpy(chp1.device,"a20");
		//eeprom_write_block(&chp1,(void*)(1*sizeof(chargepar)),sizeof(chargepar));
		//strcpy(chp2.vol,"5");
		//strcpy(chp2.cur,"4");
		//strcpy(chp2.device,"a20");
		//eeprom_write_block(&chp2,(void*)(2*sizeof(chargepar)),sizeof(chargepar));
				
				
	
	Read_charPara(array);
	
	
	DDRD = 0x00;
	SSD1306_Init (addr);
	update_menu(um);
	SSD1306_DrawString (array[0].device);
	SSD1306_SetPosition (7, 2);
	SSD1306_DrawString (array[1].device);
	SSD1306_SetPosition (7, 3);
	SSD1306_DrawString (">>");
	SSD1306_DrawString (array[2].device);
	SSD1306_UpdateScreen (addr);
	
	while (1)
	{
		
		if(up){
			if(um>1){
				um--;
			}
			update_menu(um);
			_delay_ms(4000);
		}
		
		
		if(down){
			if(um<3){
				um++;
			}
			update_menu(um);
			_delay_ms(4000);
		}
		if(Enter){
			if((um>0)&&(um<4)){
				dm=um;
				detailed_menu(dm);
				um=5;
				_delay_ms(4000);
				
				
			
			}else if((dm>0)&&(dm<4)){
				rm=dm;
				RTV_menu(rm);
				_delay_ms(4000);
				dm=5;
				_delay_ms(4000);
			}else if((rm>0)&&(rm<4)){
				um=rm;
				update_menu(rm);
				_delay_ms(4000);
				rm=5;
				dm=0;
				
			
			}
		}
		if(back){
			if((dm<4)||(um<4)){
				um=1;
				update_menu(um);
				_delay_ms(4000);
			}
		}
	}

}


void update_menu(int m){

	switch(um){
		case 1:
		SSD1306_ClearScreen ();
		SSD1306_SetPosition (7, 1);
		SSD1306_DrawString (">>");
		SSD1306_DrawString (array[0].device);
		SSD1306_SetPosition (7, 2);
		SSD1306_DrawString (array[1].device);
		SSD1306_SetPosition (7, 3);
		SSD1306_DrawString (array[2].device);
		SSD1306_UpdateScreen (addr);
		//_delay_ms(500);
		break;
		
		case 2:
		SSD1306_ClearScreen ();
		SSD1306_SetPosition (7, 1);

		SSD1306_DrawString (array[0].device);
		SSD1306_SetPosition (7, 2);
				SSD1306_DrawString (">>");
		SSD1306_DrawString (array[1].device);
		SSD1306_SetPosition (7, 3);
		SSD1306_DrawString (array[2].device);
		SSD1306_UpdateScreen (addr);
		//_delay_ms(500);
		break;
		
		case 3:
		SSD1306_ClearScreen ();
		SSD1306_SetPosition (7, 1);

		SSD1306_DrawString (array[0].device);
		SSD1306_SetPosition (7, 2);
		SSD1306_DrawString (array[1].device);
		SSD1306_SetPosition (7, 3);
		SSD1306_DrawString (">>");
		SSD1306_DrawString (array[2].device);
		SSD1306_UpdateScreen (addr);
		//_delay_ms(500);
	}
	
}

void detailed_menu(int dm){
	switch(dm){
		case 1:
		SSD1306_ClearScreen ();
		SSD1306_SetPosition (7, 1);
		SSD1306_DrawString (">>Confirm");
		SSD1306_SetPosition (7, 2);
		SSD1306_DrawString ("Voltage level -> ");
		SSD1306_DrawString (array[0].vol);
		SSD1306_DrawString (" V ");
		SSD1306_SetPosition (7, 3);
		SSD1306_DrawString ("Current level -> ");
		SSD1306_DrawString (array[0].cur);
		SSD1306_DrawString (" A ");
		SSD1306_UpdateScreen (addr);
		//_delay_ms(500);
		break;
		
		case 2:
		SSD1306_ClearScreen ();
		SSD1306_SetPosition (7, 1);
		SSD1306_DrawString (">>Confirm");
		SSD1306_SetPosition (7, 2);
		SSD1306_DrawString ("Voltage level ->  ");
		SSD1306_DrawString (array[1].vol);
		SSD1306_DrawString (" V ");
		SSD1306_SetPosition (7, 3);
		SSD1306_DrawString ("Current level ->  ");
		SSD1306_DrawString (array[1].cur);
		SSD1306_DrawString (" A ");
		SSD1306_UpdateScreen (addr);
		//_delay_ms(500);
		break;
		
		case 3:
		SSD1306_ClearScreen ();
		SSD1306_SetPosition (7, 1);
		SSD1306_DrawString (">>Confirm");
		SSD1306_SetPosition (7, 2);
		SSD1306_DrawString ("Voltage level -> ");
		SSD1306_DrawString (array[2].vol);
		SSD1306_DrawString (" V ");
		SSD1306_SetPosition (7, 3);
		SSD1306_DrawString ("Current level -> ");
		SSD1306_DrawString (array[2].cur);
		SSD1306_DrawString (" A ");
		SSD1306_UpdateScreen (addr);
		//_delay_ms(500);
		break;

	}
}

void RTV_menu(int rm){
	switch(rm){
		case 1:
		
		SSD1306_ClearScreen ();
		SSD1306_SetPosition (7, 1);
		SSD1306_DrawString (">>Main Menu");
		SSD1306_SetPosition (7, 2);
		SSD1306_DrawString ("RLT Vol. -> 5.1V ");
		
		SSD1306_SetPosition (7, 3);
		SSD1306_DrawString ("RLT Cur. -> 2.3A ");
		SSD1306_UpdateScreen (addr);
		//_delay_ms(500);
		break;
		
		case 2:
		SSD1306_ClearScreen ();
		SSD1306_SetPosition (7, 1);
		SSD1306_DrawString (">>Main Menu");
		SSD1306_SetPosition (7, 2);
		SSD1306_DrawString ("RLT Vol. -> 5.0V ");
		SSD1306_SetPosition (7, 3);
		SSD1306_DrawString ("RLT Cur. -> 2.0A ");
		SSD1306_UpdateScreen (addr);
		//_delay_ms(500);
		break;
		
		case 3:
		SSD1306_ClearScreen ();
		SSD1306_SetPosition (7, 1);
		SSD1306_DrawString (">>Main Menu");
		SSD1306_SetPosition (7, 2);
		SSD1306_DrawString ("RLT Vol. -> 3.0V ");
		SSD1306_SetPosition (7, 3);
		SSD1306_DrawString ("RLT Cur. -> 0.96A ");
		SSD1306_UpdateScreen (addr);
		//_delay_ms(500);
		break;

	}
	
}

void updateDisplay(double fl){
	int a ;
	int b;
	int c;
	int demil100;
	int decimal10;
	int point;
	float origin = fl;
	a= origin*100;
	b= origin*10;
	demil100 = a%10;
	decimal10=b%10;
	point = origin;

	char fster[30]={0};
	sprintf(fster,"%i.%i%i A",point,decimal10,demil100);
	SSD1306_ClearScreen ();
	SSD1306_DrawLine (0, MAX_X, 4, 4);
	SSD1306_SetPosition (7, 1);
	// draw string
	SSD1306_DrawString ("CURRENT  -  ");
	SSD1306_DrawString (fster);
	SSD1306_DrawLine (0, MAX_X, 18, 18);
	SSD1306_SetPosition (40, 3);
	SSD1306_UpdateScreen (addr);
}


void write_charPara(int k, chargePar chp)
{
	items = eeprom_read_word((uint16_t*)1000);
	int currentAddress = items*sizeof(chargePar);
	eeprom_write_block(&chp,(void*)(k*sizeof(chargePar)),sizeof(chargePar));
	items++;
	eeprom_update_word((uint16_t*)1000,items);
	
};

void Read_charPara()
{

	for(int i =0;i<3;i++ ){
		int current = i*sizeof(chargePar);
		eeprom_read_block(&array[i],(void*)current,sizeof(chargePar));
		_delay_ms(1000);
	}


};