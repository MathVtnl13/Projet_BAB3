#ifdef nbrLed
#else
	#define nbrLed 10
#endif
#define led_array nbrLed*3
#define led PIN_A5

volatile unsigned char LedData[led_array];
//////////////////////////////////////////////////
//     BIT BANG Driver for WS2812B RGB LEDs     //
//////////////////////////////////////////////////
#ifdef DATA_PIN
#else
	#define DATA_PIN PIN_D7
#endif
/*Creates the timing for the 1's and 0's.  Very dependent on hardware.
If oscillator speed changes, or fast_io is disabled, timing
will be altered.*/
/*
the delay_cycles in comment because the clock is 24MHz and not 48MHz
for WS2812, for WS2812B nothing change.
*/
#ifdef WS2812B
	 #define WS_ONE(pinPtr) {\
	   output_high(pinPtr); delay_cycles(1); delay_cycles(1); \
	   delay_cycles(1); delay_cycles(1); delay_cycles(1); delay_cycles(1); \
	   delay_cycles(1); delay_cycles(1); output_low(pinPtr);\
	 }
	 #define WS_ZERO(pinPtr) {\
	   output_high(pinPtr); delay_cycles(1); delay_cycles(1); \
	   output_low(pinPtr); delay_cycles(1); delay_cycles(1); \
	   delay_cycles(1); delay_cycles(1); delay_cycles(1);\
	 }
#else
	 #define WS_ONE(pinPtr) {\
	   output_high(pinPtr); delay_cycles(1); delay_cycles(1); \
	   /*delay_cycles(1); delay_cycles(1); delay_cycles(1); delay_cycles(1);*/ \
	   /*delay_cycles(1); delay_cycles(1);*/ output_low(pinPtr);\
	 }
	 #define WS_ZERO(pinPtr) {\
	   output_high(pinPtr); delay_cycles(1); /*delay_cycles(1);*/ \
	   output_low(pinPtr); /*delay_cycles(1); delay_cycles(1);*/ \
	   /*delay_cycles(1);/* delay_cycles(1); delay_cycles(1);*/\
	 }
#endif

/*Latches the data in to the devices after passing.  Call this
at the end of each update cycle.*/
#define WS_RESET(pinPtr) output_low(pinPtr); delay_us(100);

/*Low Level interface for sending data*/
void data_stream(unsigned int8 temp) {
   if(temp & 0x80) {WS_ONE(DATA_PIN);} else {WS_ZERO(DATA_PIN);}
   if(temp & 0x40) {WS_ONE(DATA_PIN);} else {WS_ZERO(DATA_PIN);}
   if(temp & 0x20) {WS_ONE(DATA_PIN);} else {WS_ZERO(DATA_PIN);}
   if(temp & 0x10) {WS_ONE(DATA_PIN);} else {WS_ZERO(DATA_PIN);}
   if(temp & 0x08) {WS_ONE(DATA_PIN);} else {WS_ZERO(DATA_PIN);}
   if(temp & 0x04) {WS_ONE(DATA_PIN);} else {WS_ZERO(DATA_PIN);}
   if(temp & 0x02) {WS_ONE(DATA_PIN);} else {WS_ZERO(DATA_PIN);}
   if(temp & 0x01) {WS_ONE(DATA_PIN);} else {WS_ZERO(DATA_PIN);}
}

/*WS2812 accepts colors in order of green-red-blue.
I do not, so these assign as red-green-blue which
is more comfortable to me.*/
void RGB_color(unsigned int8 red, unsigned int8 green, unsigned int8 blue) {
  data_stream(green);
   data_stream(red);
   data_stream(blue);
}

/*Single LED addressed.  Once per second it will be updated
to cycle through Blue --> Green --> Red, and repeat.*/
void prove_driver(void) {
   static unsigned int8 temp = 0;
   switch(temp) {
     case 0: RGB_color(0x00, 0x00, 0xFF); temp += 1; break;
     case 1: RGB_color(0x00, 0xFF, 0x00); temp += 1; break;
     case 2: RGB_color(0xFF, 0x00, 0x00); temp = 0;  break;
  }
  WS_RESET(DATA_PIN);
  delay_ms(1000);
}
/* Write the buffer to the stripled  */
void write_ws2812()
{
	unsigned char l,pt;
	for (l=0;l<nbrled;l++){
		pt=l*3;
		RGB_color(LedData[pt],LedData[pt+1],LedData[pt+2]);
	}	
	WS_RESET(DATA_PIN);
}
/* Clear stripled and the buffer */
void SetAllLedOff(){
	unsigned char l;

	for (l=0;l<nbrled;l++){
		RGB_color(0,0,0);
	}	
	WS_RESET(DATA_PIN);
	for (l=0;l<(nbrled*3);l++)
		LedData[l]=0;
}
/* Set on led on the stripled */
void SetRGB(unsigned char ledNumber, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char i = 3 * ledNumber;
    ledData[i] = red;
    i++;
    ledData[i] = green;
    i++;
    ledData[i] = blue;
	write_ws2812();
}

void SetColor(unsigned char ledNumber, unsigned char color)
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    if (color <= 85)
    {
        red = 3*(85-color);
        green = 3*color;
        blue = 0;
    }
    else if (color <=170)
    {
        red = 0;
        green = 3*(170-color);
        blue = 3*(color-85);
    }
    else
    {
        red = 3*(color-170);
        green = 0;
        blue = 3*(255-color);
    }

    unsigned char i = 3 * ledNumber;
    ledData[i] = red;
    i++;
    ledData[i] = green;
    i++;
    ledData[i] = blue;
	write_ws2812();
}

void SetAllRGB(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char idx = 0;

    for (int i=0; i<nbrLed; i++)
    {
        ledData[idx] = red;
        idx++;
        ledData[idx] = green;
        idx++;
        ledData[idx] = blue;
        idx++;
    }
	write_ws2812();
}


// Write a color wheel value (hue) to all LEDs.
// colour is between 0 and 255.
// 0 = Red,
// 85 = Green
// 170 = Blue
// back to 255 = red again
void SetAllColor(unsigned char color)
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    if (color <= 85)
    {
        red = 3*(85-color);
        green = 3*color;
        blue = 0;
    }
    else if (color <=170)
    {
        red = 0;
        green = 3*(170-color);
        blue = 3*(color-85);
    }
    else
    {
        red = 3*(color-170);
        green = 0;
        blue = 3*(255-color);
    }

    unsigned char idx = 0;

    for (int i=0; i<nbrLed; i++)
    {
        ledData[idx] = green;
        idx++;
        ledData[idx] = red;
        idx++;
        ledData[idx] = blue;
        idx++;
    }
	write_ws2812();
} 

//Makes a 24bit color from 3 8bit color variables (RGB)
int32 Strip_Color(int8 r, int8 g, int8 b){
   return ((int32)r << 16) | ((int32)g <<  8) | b;
} 

int32 Wheel(byte WheelPos) {
   WheelPos = 255 - WheelPos;
   if(WheelPos < 85){
      return Strip_Color(255 - (WheelPos * 3), 0, WheelPos * 3);
   }
   else if(WheelPos < 170){
      WheelPos -= 85;
      return Strip_Color(0, (WheelPos * 3), 255 - WheelPos * 3);
   }
   else{
      WheelPos -= 170;
      return Strip_Color(WheelPos * 3, 255 - (WheelPos * 3), 0);
   }
}

void Rainbow(int8 wait) {
   unsigned int16 i,j;
   unsigned int32 val;
   unsigned char idx = 0;

   for(j=0;j<256;j++) {
      for(i=0;i<nbrLed;i++) {
         //LedData(i, Wheel((i+j) & 255));

		val=Wheel(((i*256/nbrled)+j) & 255);
        ledData[idx] = val>>16;
        idx++;
        ledData[idx] = val>>8;
        idx++;
        ledData[idx] = val;
        idx++;
      }
   write_ws2812();
   delay_ms(wait);
   }
}

void RainbowCycle(int8 wait) {
   int16 i,j;
   unsigned int32 val;
   unsigned char idx = 0;

   for(j=0;j<256*5; j++) { // 5 cycles of all colors on wheel
      for(i=0;i<nbrLed;i++) {
        //LedData(i, Wheel(((i * 256 / STRIP_SIZE) + j) & 255));
		val=Wheel(((i*256 / nbrLed) + j) & 255);
        ledData[idx] = val>>16;
        idx++;
        ledData[idx] = val>>8;
        idx++;
        ledData[idx] = val;
        idx++;
    }
   write_ws2812();
   delay_ms(wait);
   }
} 

void SetBrightness(byte value){






}