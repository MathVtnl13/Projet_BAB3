/////////////////



#define WS2812B true
#define yellow 40
//#define nbrled 10

#include "WS2812_Driver.c" 




void barregraphe(byte value, byte intensity){
	
	byte i;
	
	if(intensity>10)intensity=10;
	if(value>100)value=100;
	value=value/nbrled;
	if(value==0){
		SetAllLedOff();
		return;
	}
	i=0;
	while(i<(nbrled+1)){
		if(i<=value){
			if(i<(nbrled*50/100)) SetRGB(i,0,25*intensity,0); // Setcolor(i,green);
			if((i>(nbrled*40/100))&&(i<(nbrled*80/100))) SetRGB(i,25*intensity,25*intensity,0); // Setcolor(i,yellow);
			if(i>(nbrled*70/100)) SetRGB(i,25*intensity,0,0); // Setcolor(i,red);
		}else{
			SetRGB(i,0,0,0);
			
		}
		i++;
	}
	
	
}

unsigned int16 map(unsigned int32 value, unsigned int32 in_min,unsigned int32 in_max,unsigned int32 out_min, unsigned int32 out_max){
	
	return(((int32)(value - in_min)*(int32)(out_max - out_min))/(in_max - in_min)) +out_min;
}