////////////////////////////////////////////////////////////////////////////////////////////
//  MQ7 - FUNCTION PROTOTYPES:                                                              //
//  Capteur de GAZ	(LPG - Butane)                                                         //       
//                                                                                         //       
//                                                                                         //         
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////
// Connection entre PIC 18F4550 & MQ6
/* 
						 +-------------------------------------
						 |
  RA0 -------------	-----+ A0  (4) (Analog Out)  Gas Senor MQ6		 
  RE1 ------------- -----+ DO  (3) (Digital OUT)
  GND -------------	-----+ GND (2)
  +5V -------------	-----+ VIN (1)
						 |
			 			 +-------------------------------------	
*/
#include <18F4550.h>

#device ADC=10		// adc en mode resolution 10 bits
#fuses HS,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,VREGEN
#use delay(clock=24M)
#use rs232(baud=115200, xmit=PIN_C6, rcv=PIN_C7)
//#use i2c(master, sda=PIN_B0, scl=PIN_B1)

//*************************************************************
// tentative de fusion 28/02

#define _1ms 	65535 - 6134		// Timer for 1ms 		18f4550 6134    16f887  5000
#define ventilo PIN_A1
#define Fan(x) output_bit(ventilo,x)


#define ON			1
#define OFF			0
#define Led 		PIN_A4 //
#define Led_on()	output_bit(PIN_A4,1)
#define Led_off()	output_bit(PIN_A4,0)
#define Button 		PIN_E2
#define Button_read() input( Button )
#define ledD 		PIN_A4
#define Led(x)		output_bit(ledD,x)
#define BL_			PIN_C0
#define BL(x)		output_bit(BL_,x)
#define tgl_led		output_toggle(ledD)

#define _gas_detect	PIN_E1
#define gas_detect(x)	output_bit(_gas_detect,x)

#define RL_VALUE 	10     		//define the load resistance on the board, in kilo ohms
#define RO_VALUE_CLEAN_AIR 11.3 //9.83 (Sensor resistance in clean air)/RO,
//*************************************************************
//#include <string.h>
//#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "LCD420_S3.c"


// tentative de fusion 28/02//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "DSM501A.h"


int1 flgB,flag_d = false;
int16 compteur = 0;
int16 compteur2=0;
int16 val_st,val_fin;
long long mes_tot = 0; 

// servo moteur 03/03/23!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#define pin_servo pin_D2

// LED_STICK !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#DEFINE DATA_PIN PIN_D7
#use FAST_IO(D)

//OLED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Initialize I2C
#include "SSD1306OLED.c"
#define SSD1306_RST    PIN_B2




//NETTOYAGE 10/03 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "Functions_MQ7.c"
#include "Functions_DSM501A.c"
#include "LED_STICK.c"




void servo(int1 state)
{
//printf("\n SERVO=%u\r\n",state);
byte x;
	for(x=0;x<10;x++)
	{
		output_bit(pin_servo,1);
		state ? delay_ms(1) : delay_ms(2);
		output_bit(pin_servo,0);
		delay_ms(15);
	}


}












//*************************************************************
void initialisation() {

// LED_STICK 10/03/23 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

output_drive(DATA_PIN);


	// tentative de fusion 28/02//!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	flgB = false;
	setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);		// 10,9 ms overflow
	set_timer1(_1ms);							// setting Timer 1 for 1ms overflow
	Disable_interrupts(INT_TIMER1);				// disable timer 1
	ext_int_edge(2, H_TO_L );					// setting edge start
	enable_interrupts(INT_EXT2);				// active extern INT
	enable_interrupts(GLOBAL);

////////
	
	//output_float(_gas_detect);
	setup_adc_ports(AN0_TO_AN3|VSS_VDD);
	//setup_adc_ports(sAN0|sAN1);
	setup_adc(ADC_CLOCK_INTERNAL);
	set_adc_channel(0);
	delay_us(10);
	lcd_init();
   	lcd_putc("\fTEST");
	lcd_putc("\nUMONS-2023");
	cursor(0);								// curseur OFF
	bl(ON);
	delay_ms(1000);	
	
	
	// tentative de fusion 28/02//!!!!!
	
	//wait_60s();
	Fan(ON);
	enable_interrupts(INT_TIMER1);	
	

}





//*************************************************************

void main(){

    SSD1306_ClearDisplay();

	byte level;
char buffer[15];
	
	float PPM=0;
	
	int1 stateservo;
    float Ro=0;
	initialisation();
	//float R0fixe=10;
	//wait_30s();
	lcd_gotoxy(1,2);
	lcd_putc("Calibrating....");
	Ro = SensorCalibration();
	//Ro=3;
    printf("Ro=%f\r\n",Ro);
	lcd_gotoxy(1,2);
	lcd_putc("Done!          ");
	//delay_ms(500);
	//lcd_gotoxy(1,2);
	//printf(lcd_putc,"%f KOhms",Ro);
    delay_ms(1500); 
    //gas_detect(0);

	//int CURSOR=0;
	//barregraphe(20,1); 
	while(true){
		//test_pow();

     
	      if (compteur2 > 1000) {						// if 10 sec passed
			
          float rs = read_mq();
			printf("rs=%f\r\n",rs);
         	float ratio = rs/Ro;
            printf("ration=%f\r\n",ratio);
            
        	//printf(lcd_putc,"%f ",gas_plot_log_scale(ratio));
            printf("PPM=%f\r\n",gas_plot_log_scale(ratio));
            PPM=gas_plot_log_scale(ratio);

     

	        SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	        SSD1306_DrawText(2, 6, (char*)"PPM: ", 1);
	
	        sprintf(buffer,"%12f",PPM);
	        SSD1306_DrawText(2,16 , buffer, 1);
	        SSD1306_Display();
			
			
			
			level=map(PPM,0,11,0,100);
			printf("    LEVEL2: %d\r\n",level);

            
			barregraphe(level,4);
			
			
			compteur2=0;
		} 
	  
	  
                
				
	    



			
			

         if (compteur > 31000) {						// if 30 sec passed
			Fan(OFF);											// Mes_tot en uS -> x1000
			float ration = mes_tot /(sampletime_ms /100); 	// Integer percentage 0=>100
			concentration = 1.1*pow(ration,3)-3.8*pow(ration,2)+520*ration+0.62;
			printf("\n Mesure total: %Lu\r\n",mes_tot);
			printf(" \n    Ratio: %f\r\n",ration);
			printf(" \n    Concentration: %f \r\n",concentration);
			
			mes_tot = 0;
			compteur = 0;
            flag_d = false;
			
			


            SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	        SSD1306_DrawText(2, 30, (char*)"Concentration: ", 1);
          	



            if (concentration < 1000){
				lcd_gotoxy(11,1);
				lcd_putc("CLEAN   ");
			SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	        SSD1306_DrawText(2, 40, (char*)"CLEAN ", 1);
				
			}
			if (concentration > 1000 && concentration < 10000) {
				lcd_gotoxy(11,1);
				lcd_putc("GOOD   ");
			SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	        SSD1306_DrawText(2, 40, (char*)"GOOD ", 1);
				
			}	
			if (concentration > 10000 && concentration < 20000) {
				lcd_gotoxy(11,1);
				lcd_putc("ACCEP   ");
				SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	        SSD1306_DrawText(2, 40, (char*)"ACCEPT", 1);
								
			}
			if (concentration > 20000 && concentration < 50000) {
				lcd_gotoxy(11,1);
				lcd_putc("HEAVY   ");
				SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	        SSD1306_DrawText(2, 40, (char*)"HEAVY", 1);
                

					
			}
			if (concentration > 50000 ) {
				lcd_gotoxy(11,1);
				lcd_putc("HAZARD  ");
				SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	        SSD1306_DrawText(2, 40, (char*)"HAZARD", 1);
			}

           SSD1306_Display();
		   Fan(ON);
				
		}
		
		
	
			lcd_gotoxy(1,1);
	    printf(lcd_putc,"MT:%Lu   ",mes_tot);
         	
	
			
			
		    
			



           if((concentration>20000)||(PPM>3)){

            stateservo=1;
            servo(stateservo);


           }else{

            stateservo=0;
            servo(stateservo);}

		 
			
            

     







//            if( !Button_read() ){
//				Led_on();
//				unsigned int c = CURSOR;
//				delay_ms(2000);
//				CURSOR=(CURSOR+1)%2;
//				printf( "CURSOR(%d)=%d\r\n", c, CURSOR );
//				Led_off();            
//			}
//
//			switch(CURSOR){
//				
//				case 0:// Particules
//				
//			    lcd_gotoxy(1,1);
//	            printf(lcd_putc,"MT:%Lu   ",mes_tot);
//	
//	
//				lcd_gotoxy(1,2);
//				printf(lcd_putc,"Cc:%f p.  ",concentration);
//				
//				
//				if (concentration < 1000){
//				lcd_gotoxy(11,1);
//				lcd_putc("CLEAN   ");
//				
//			}
//			if (concentration > 1000 && concentration < 10000) {
//				lcd_gotoxy(11,1);
//				lcd_putc("GOOD   ");
//				
//			}	
//			if (concentration > 10000 && concentration < 20000) {
//				lcd_gotoxy(11,1);
//				lcd_putc("ACCEP   ");
//								
//			}
//			if (concentration > 20000 && concentration < 50000) {
//				lcd_gotoxy(11,1);
//				lcd_putc("HEAVY   ");
//                
//
//					
//			}
//			if (concentration > 50000 ) {
//				lcd_gotoxy(11,1);
//				lcd_putc("HAZARD  ");
//               
//			}
//				
//				break;
//				
//				case 1:
//				lcd_gotoxy(1,1);
//            	lcd_putc("Gas is present  ");
//		    	lcd_gotoxy(1,2);
//            
//                printf(lcd_putc,"PPM:%f p.  ",PPM);
//				printf("PPM=%f\r\n",PPM);
//				break;
//				
//				
//				
//		      }
			
	 

	
	}
}