

/* A web radio receiver on wifi for arduino DUE board
 *  
 *  Copyright: Jean-Pierre Cocatrix jp@cocatrix.fr
 */

// library
#include <RTClib.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <IniFile.h>
#include <DueTimer.h>
//#include <ESP8266.h>
#include <SPI.h>
#include <UTouch.h>
#include "karaScreenConfig.h"

// hardware config:
// For the TFT panel and associated touch panel, these are the default.
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10
#define TFTT_CLK 30
#define TFTT_CS 28
// touch screen
#define TFTT_TDIN 26
#define TFTT_TDOUT 24
#define TFTT_IRQ 22
// cs for the sd card reader (other pins to miso, mosi and clk of spi bus)
const uint8_t SD_CS = 11;
#define SD_SPI_SPEED SPI_HALF_SPEED  // SD card SPI speed, try SPI_FULL_SPEED
//
// TIMER
// timer interrupt for touch screen 100ms
#define tfttimer Timer1 
#define TIMER1 100000
// for status bar refresh: 2 secondes
#define statustimer Timer2 
#define TIMER2 2000000
// clock 1 sec
#define timetimer Timer3 
#define TIMER3 1000000

class TScreen;
// Use hardware SPI 
//ILI9341_due tft(TFT_CS, TFT_DC, TFT_RST);
UTouch  myTouch(TFTT_CLK, TFTT_CS,TFTT_TDIN, TFTT_TDOUT, TFTT_IRQ);
//ESP8266 myWifi;
TScreen  myScreen(TFT_CS, TFT_DC, TFT_RST);

SdFat SD;
SdFile bmpFile; // set filesystem

char buffer[254]; // general purpose buffer.
String SSID;
String SSIDPASSWORD;
RTC_Millis rtc;
DateTime dt( F(__DATE__), F(__TIME__));


int xt,yt;
// handler routine for timer1 to read the touch screen
void myTouchInt()
{
    static bool inTouch = false;
    if ((!inTouch)&&(myTouch.dataAvailable()))
    {
      inTouch = true;
      myTouch.read();
      xt = myTouch.getX();
      yt = myTouch.getY();
      myScreen.ETouch.TrigOn(xt,yt);
    } 
    if ((inTouch)&&(!myTouch.dataAvailable()))
    {
      inTouch = false;
      myScreen.EunTouch.TrigOn(xt,yt);
    } 
}
// compute the status bar
void StatInt()
{
   bool st = SD.exists("webradio.ini");
  myScreen.StatusBar->Status[2]->State = st;
/*  if (!st)
{ SD.begin(SD_CS, SD_SPI_SPEED);}*/
  myScreen.StatusBar->Status[2]->Modified = true;
}
void TimeInt()
{  
String ci ;
DateTime dt0(rtc.now());
  ci = dt0.hour() ;
  ci = ci  +":"+ dt0.minute() + ":" + dt0.second();
  myScreen.StatusBar->Status[3]->Caption((char*)ci.c_str());
  myScreen.StatusBar->Status[3]->Modified = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println(F("Web Radio")); 
  
//  myWifi.SoftReset();
  rtc.begin(dt);
// Pre display some Welcome messages.
  myScreen.Welcome(); // pre init myScreen to display the welcome
  myScreen.printAt("Web Radio by KaraWin", 30,90);
  myScreen.printAt("Free memory: ", 30,110);
  myScreen.println(String(FreeRam(),DEC).c_str());
  myScreen.printAt("WIFI version: ",30,130);
//  myScreen.println(myWifi.GetVersion());

// Check the sd card to read the external init configuration  
  if (!SD.begin(SD_CS, SD_SPI_SPEED)){
    Serial.println(PSTR(" SD failed!"));}
  else
  {
    IniFile ini("webradio.ini");
    if (!ini.open()) {
    Serial.println("Ini file webradio.ini does not exist");
    } else
    {
      Serial.println("Ini file exists");
      ini.getValue("NETWORK","SSID",buffer,sizeof(buffer));
      SSID = buffer; Serial.println(SSID);
      ini.getValue("NETWORK","PASSWORD",buffer,sizeof(buffer));
      SSIDPASSWORD = buffer; Serial.println(SSIDPASSWORD);
      ini.close();
    }
  }
//    myWifi.SetWIFIMode(3);
//    myWifi.SetJoinAP(SSID,SSIDPASSWORD);
  delay(1000);

  
// Initialize the touch screen  
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  
  // Initialize myScreen
  myScreen.Begin();
  delay(1000);  
  myScreen.Panels->Panel->clearPanelLines();  
  
// starts the interrupts  
  Timer1.attachInterrupt(myTouchInt);
  Timer1.start(TIMER1); 
  Timer2.attachInterrupt(StatInt);
  Timer2.start(TIMER2);
  Timer3.attachInterrupt(TimeInt);
  Timer3.start(TIMER3);  

}
int cnt = 0;
String string;
char charac[10];
void loop() {
  // put your main code here, to run repeatedly:
while (true) { 
   myScreen.Task();// mandatory

   if (cnt < 20)
   {
     string = cnt++;
     strcpy(charac,string.c_str());
     delay(400); 
     myScreen.Println("string "+ string+ " this is the "+string+" eme line");
     myScreen.Println(charac);
     myScreen.Print("Touch the screen for buttons");
     myScreen.Println(" and features");
   }
   if (cnt++ == 20)
     myScreen.Println("1234567890123456789012345678901234567890123456789");
}
}




