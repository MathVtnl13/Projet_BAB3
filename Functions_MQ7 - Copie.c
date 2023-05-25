/////////////////



//*************************************************************
void wait_30s() 
{
  // wait 60s for DSM501 to warm up
  lcd_putc("\fWait 60s Warmup!\n  Sec");
  for (int i = 1; i <= 60; i++)
  {
    delay_ms(1000); // 1s
    lcd_gotoxy(1,2);
    printf(lcd_putc,"%u",i);
  }
  lcd_gotoxy(1,2);
  lcd_putc("Ready!    ");
  delay_ms(1000);	
}


void wait_60s() 
{
  // wait 60s for DSM501 to warm up
  lcd_putc("\fWait 60s Warmup!\n  Sec");
  for (int i = 1; i <= 60; i++)
  {
    delay_ms(1000); // 1s
    lcd_gotoxy(1,2);
    printf(lcd_putc,"%u",i);
  }
  lcd_gotoxy(1,2);
  lcd_putc("Ready!    ");
  delay_ms(1000);	
}



//*************************************************************
float gas_plot_log_scale(float rs_ro_ratio)
{
    
	float x=0.056*rs_ro_ratio;
	float ppm=pow(x,-1.43);
	return ppm;
	
	//return pow(10,(((log(rs_ro_ratio)-curve[1])/curve[2]) + curve[0]));  
}

//float test_pow() {
//	float i = 0.1;    
//	for(;i<2;i=i+0.1) {
//		float x = 1.1;    
//		for(;x<2;x=x+0.1) {
//			float ppm=pow(i,x);
//			printf("[i x pow] = (%f %f %f)\r\n",i, x, ppm);
//		}
//	}
//}


//*************************************************************
float calculate_resistance(int16 adc_channel)
{                                                         // sensor and load resistor forms a voltage divider. so using analog value and load value 

  return ( ((float)RL_VALUE*(1023-adc_channel)/adc_channel));     // we will find sensor resistor.
}



//*************************************************************
float read_mq()
{
  int count;
  float rs=0;
  for (count=0;count<5;count++) {                                 // take multiple readings and average it.
    rs += calculate_resistance(Read_adc());   // rs changes according to gas concentration.
    Delay_ms(50);
  } 
  rs = rs/5;
  return rs;  
}



//*************************************************************
/*
 * Sensor Related Functions
 */
float SensorCalibration(){
int count;                                   // This function assumes that sensor is in clean air.
float val=0;

	for (count=0;count<50;count++) {                   //take multiple samples and calculate the average value   
    	val += calculate_resistance(Read_adc());
    	delay_ms(500);
  	}
  	val = val/50;                  
  	val = val/RO_VALUE_CLEAN_AIR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 
		                                                 //according to the chart in the datasheet 
  return val; 
}

//*************************************************************