/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/************************************/
#include "AMS5812.h"
#include "SPI.h"
#include "TFT_22_ILI9225.h"
#include <math.h>
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio


/************************************/
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly

/*************************************/
#define TFT_BRIGHTNESS 200
#define WHITE COLOR_WHITE
#define BLACK COLOR_BLACK
#define MAXAREA 4500
#define rectangle_length 150
#define rectangle_width  30
/******************************************/

float pressure = 0;
int Previous_Pressure = 0;
int Current_Pressure = 0;
int Previous_Area = 0;
int Current_Area = 0;
/******************************************/


/***************************************************************************************/
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);
AMS5812 dPress(Wire,0x10,AMS5812::AMS5812_1000_D);
void fill_area(int area,uint16_t colour);
/**************************************************************************************/
void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.setOrientation(1);
  tft.setBacklight(HIGH);
  tft.drawRectangle(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_RED);
 tft.drawRectangle(30, 70, 190, 110, COLOR_WHITE);
 tft.drawRectangle(29, 71, 191, 111, COLOR_WHITE);
 tft.setFont(Terminal12x16);
 tft.drawText(90, 120, "MPa", COLOR_WHITE);
 //fill_area(2250,COLOR_RED);
 // put your setup code here, to run once:
  while(!Serial){}
  // starting communication with the 
  // static pressure transducer
  if (dPress.begin() < 0) {
    Serial.println("Error communicating with sensor, check wiring and I2C address");
    while(1){}
  }
}


void loop() {
  
  dPress.readSensor();
  
  // displaying the data
  pressure = dPress.getPressure_Pa(); //Pressure in pascal
  if(pressure >= 1700000)
  {
    pressure=0;
  }
  
  float kPa = (pressure/1000); //pressure in Kilo  pascal
  float final_pressure = 0;
  int i = 200;
  
  
  while(i)
  {
    final_pressure = final_pressure+kPa;  
    i--;
    delay(1);
  }
  final_pressure = final_pressure/200;

  
  Serial.print("Pressure =\t");
  Serial.print(final_pressure,4);
  Serial.print("kPa");
  //Serial.print(Angle);
  Serial.print("\n");

Current_Area = map(final_pressure,0,100,0,4500);
//pressure display on screen

Current_Pressure = final_pressure;  
if((Current_Pressure-Previous_Pressure > 1)||(Previous_Pressure-Current_Pressure > 1))
{
  fill_area(Previous_Area,COLOR_BLACK);
  delay(10);
  if(Current_Area >= 0 && Current_Area < 900)
  {fill_area(Current_Area,COLOR_BLUE);
  }
  else if(Current_Area >=900 && Current_Area <3600)
  {
	  fill_area(Current_Area,COLOR_GREEN);
  }
  else if(Current_Area >= 3600 )
  {
	 fill_area(Current_Area,COLOR_RED); 
  }
 Previous_Pressure = Current_Pressure;
 Previous_Area = Current_Area;
}

 

}
void fill_area(int area,uint16_t colour)
{
	int length = area/rectangle_width;
	tft.fillRectangle(35, 75, 35+length, 105, colour);
	
}
