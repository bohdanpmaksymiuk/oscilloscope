#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

const int microseconds_per_sample=9500;
int h=0, w=0;

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  h = tft.width();
  w = tft.height(); 
  
  pinMode(A8,INPUT);  // Input 1
  pinMode(A9,INPUT);  
  pinMode(A10,INPUT); // Input 2
  
  pinMode(A11,OUTPUT);  // Ground
  pinMode(A12,OUTPUT);  // Ground
  digitalWrite(A11,0);
  digitalWrite(A12,0);
}

int x=1; int t0=0; int t1=0; int count=0; unsigned long nextT = 0; 

unsigned long sum = 0;  double pin =0;  double val =0;  int lastval=0;
unsigned long sum1 = 0; double pin1 =0; double val1 =0; int lastval1=0;

void loop(void) {

    // DRAW SECOND MARK AND VOLTAGE
    t1 = micros()/1000000;
    if (t1>t0)
    {
      t0=t1;
      tft.drawFastHLine(0,w-(x-1), w, ILI9341_DARKGREY);
      tft.setRotation(3);
      //Serial.println(t1);
      tft.setTextSize(1);
      tft.setTextColor(ILI9341_GREEN); 
      
      tft.setCursor((x-40),h-20);
      tft.print((pin/1024)*5); // print voltage.
      tft.print(" V");

      tft.setTextColor(ILI9341_YELLOW); 
      
      tft.setCursor((x-40),h-10);
      tft.print((pin1/1024)*5); // print voltage.
      tft.print(" V");
      tft.setRotation(0);
    }

    // GATHER DATA
    if (micros()<= nextT)
    {
      sum = sum   + analogRead(A8); 
      sum1 = sum1 + analogRead(A10);
      
      count = count +1;
      
    } else {
    // DRAW DATA POINTS
      pin   = sum/(count);  val   = (pin/1024)*h;
      pin1  = sum1/(count); val1  = (pin1/1024)*h;
      
      //Serial.println(val);

      // Draw DATA
      tft.drawLine(h-lastval  ,w-x,(h-val)  ,w-x+1, ILI9341_GREEN);
      tft.drawLine(h-lastval1 ,w-x,(h-val1) ,w-x+1, ILI9341_YELLOW);

      // DRAW NON DATA ELEMENTS
      tft.drawFastHLine(0, w-x, w, ILI9341_BLACK);
      tft.drawFastHLine(0, w-x-1, w, ILI9341_RED);

      tft.drawPixel((h/5)*1,w-x+1, ILI9341_DARKGREY);
      tft.drawPixel((h/5)*2,w-x+1, ILI9341_DARKGREY);
      tft.drawPixel((h/5)*3,w-x+1, ILI9341_DARKGREY);
      tft.drawPixel((h/5)*4,w-x+1, ILI9341_DARKGREY);
      tft.drawPixel((h/5)*5,w-x+1, ILI9341_DARKGREY);

      // get ready for the next time around
      x=x+1;
      if (x > w) { x = 1; } // handle wrapping

      // setup data for next sample
      count = 0;
      lastval   = val;  sum   = 0; 
      lastval1  = val1; sum1  = 0;
      
      nextT = micros()+microseconds_per_sample; 
    }
  
}


