

/* A web radio receiver on wifi for arduino DUE board
 *  
 *  Copyright: Jean-Pierre Cocatrix jp@cocatrix.fr
 */

// library
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
#include "karaScreenConfig.h"
#include "karaScreen.h"

///////////////////////////////////
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
////////////////////////////////////

//class TScreen;
// Use hardware SPI 
//ILI9341_due tft(TFT_CS, TFT_DC, TFT_RST);
UTouch  myTouch(TFTT_CLK, TFTT_CS,TFTT_TDIN, TFTT_TDOUT, TFTT_IRQ);
TScreen  myScreen(TFT_CS, TFT_DC, TFT_RST);
SdFat SD;
SdFile bmpFile; // set filesystem

char buffer[254]; // general purpose buffer.
String SSID;
String SSIDPASSWORD;
 ESP8266 myWifi;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println(F("Web Radio")); 
  rtc.begin(dt);  
  static bool stwifi = myWifi.SoftReset();
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
  // put your main code here, to run repeatedly:
  while (true) { 
     myScreen.Task();// mandatory. The engine
  }
}
////MODIFY///////////////////////////////////////////////
// Called in the TScreen::Task(). Put your  code here
void TScreen::userTask()
{
 ; 
}
// Called every second. Put your code here
void TScreen::userSecond()
{
;

}



