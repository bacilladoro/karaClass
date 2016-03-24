


/* A web radio receiver on wifi for arduino DUE board
 *  
 *  Copyright: Jean-Pierre Cocatrix jp@cocatrix.fr
 */

// library
//#include <FileIO.h>
#include <Ethernet.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <IniFile.h>
#include <ILI9341_due_config.h>
#include <ILI9341_due.h>
#include <SPI.h>
#include <DueTimer.h>
#include <UTouch.h>
#include <Wire.h>
#include <RTClib.h>
#include "myArial14.h"

#include <ESP8266.h>
#include <SFEMP3Shield.h>
#include <karaScreen.h>
#include "karaScreenConfig.h"


///////////////////////////////////
// hardware config:
// For the TFT panel and associated touch panel, these are the default.
#define TFT_RST  10
#define TFT_DC   11
#define TFT_CS   12
#define TFTT_CLK 30
#define TFTT_CS 28
// touch screen
#define TFTT_TDIN 26
#define TFTT_TDOUT 24
#define TFTT_IRQ 22

// cs for the sd card reader (other pins to miso, mosi and clk of spi bus)
const uint8_t SD_CS =  9;
#define USE_MULTI_BLOCK_IO 0
// Use total of 13 512 byte buffers.
const uint8_t BUFFER_BLOCK_COUNT = 12;
// Dimension for queues of 512 byte SD blocks.
const uint8_t QUEUE_DIM = 16;  // Must be a power of two!

#define SD_SPI_SPEED SPI_FULL_SPEED  // SD card SPI speed, try SPI_FULL_SPEED
////////////////////////////////////

//class TScreen;
// Use hardware SPI 

SFEMP3Shield myPlayer;

UTouch  myTouch(TFTT_CLK, TFTT_CS,TFTT_TDIN, TFTT_TDOUT, TFTT_IRQ);
TScreen  myScreen(TFT_CS, TFT_DC, TFT_RST);

SdFat SD;
//SdFat SD;
SdFile bmpFile; // set filesystem

char buffer[254]; // general purpose buffer.
String SSID;
String SSIDPASSWORD;
 ESP8266 myWifi;
extern bool trigInt ;

void(* resetFunc) (void) = 0; //declare reset function @ address 0


void setup() {
  uint8_t result; //result code from some function as to be tested at later time.
  pinMode(13  , OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println(F("Web Radio"));
#ifdef DS3231 
  rtc.begin();
#else
  rtc.begin(dt);
#endif
  static bool stwifi = myWifi.SoftReset();
  pinMode(3, INPUT); // PIR INPUT
  //Initialize the MP3 Player Shield
  if (!SD.begin(SD_CS, SD_SPI_SPEED)){
  Serial.println(PSTR(" SD failed!"));}
  else Serial.println(PSTR(" SD Ok!"));
  if(!SD.chdir("/")) SD.errorHalt("sd.chdir");

  Serial.print(F("F_CPU = "));
  Serial.println(F_CPU);
  Serial.print(F("Free RAM = ")); // available in Version 1.0 F() bases the string to into Flash, to use less SRAM.
  Serial.println(FreeRam(), DEC);  // FreeRam() is provided by SdFatUtil.h
 
  result = myPlayer.begin();
  //check result, see readme for error codes.
  if(result != 0) {
	  Serial.print(F("Error code: "));
	  Serial.print(result);
	  Serial.println(F(" when trying to start MP3 player"));
	  if( result == 6 ) {
		    Serial.println(F("Warning: patch file not found, skipping.")); // can be removed for space, if needed.
		    Serial.println(F("Use the \"d\" command to verify SdCard can be read")); // can be removed for space, if needed.
	  }
  }


///////////////////////////////////////
// MODIFY//////////////////////////////
// Pre display some Welcome messages.
  myScreen.Welcome(); // pre init myScreen to display the welcome
  myScreen.printAt("Web Radio by KaraWin", 30,90);
  myScreen.printAt("Free memory: ", 30,110);
  myScreen.println(String(FreeRam(),DEC).c_str());
  if (stwifi)
  {
    myScreen.printAt("WIFI version: ",30,130);
    myScreen.println(myWifi.GetVersion());
  }
// Check the sd card to read the external init configuration  
  if (!SD.begin(SD_CS, SD_SPI_SPEED)){
    Serial.println(PSTR(" SD failed!"));
	resetFunc();  //call reset}
	}
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
    delay(2000); // Welcome show timer
  // Initialize myScreen
  myScreen.Begin();
    
    if (stwifi)
	{
      stwifi = myWifi.SetWIFIMode(3);
      stwifi = myWifi.SetJoinAP(SSID,SSIDPASSWORD);
    }

/// END MODIFY


}

uint16_t cnt = 0;
String string;
char charac[10];
void loop() {
     myScreen.Task();// mandatory. The engine
}
////MODIFY///////////////////////////////////////////////
// Called in the TScreen::Task(). Put your  code here
void TScreen::userTask()
{

//myPlayer.available();
 
}
// Called every 100 msecond. Put your code here
void TScreen::user100msecond()
{
}

// Called every second. Put your code here
void TScreen::userSecond()
{

 if (digitalRead(3) == HIGH) 
 {
   if ((myScreen.Panels->BigTime != NULL) && (screensaver == TSCREENSAVER+1))
      myScreen.Panels->StopBigTime();
   else   screensaver = 0;
 }
//dbgprintln(screensaver);
 uint8_t result; //
 digitalWrite(13, !digitalRead(13));
  if (myScreen.Panels->Bts[3]->Button[1]->State)//src=sd
  {
	  result = myPlayer.isPlaying();
      if ((result==0)  && myScreen.Panels->Bts[0]->Button[2]->State && myScreen.Panels->Bts[1]->Button[3]->State )
      {
        ABts0_1();// play next
//		dbgprint(F("userScond next "));dbgprintln(String(result));
      } else
	  {
	    if (!result && myScreen.Panels->Bts[0]->Button[2]->State)
	    {
	       myScreen.Panels->Bts[0]->Button[2]->State = false;
           myScreen.Panels->Draw();
		}
	  }
	 
  }
  
}



