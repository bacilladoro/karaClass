
/* A demo of KaraClass library for arduino DUE board
 *  
 *  Copyright: Jean-Pierre Cocatrix jp@cocatrix.fr
 */

// library
#include <ILI9341_due_config.h>
#include <ILI9341_due.h>
#include <SPI.h>
#include <DueTimer.h>
#include <UTouch.h>
#include <RTClib.h>
#include <Wire.h>
#include <karaScreen.h>
#include "myArial14.h"
#include "karaScreenConfig.h"


#ifdef __arm__
extern "C" char* sbrk(int incr);
int FreeRam() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#else  // __arm__
extern char *__brkval;
extern char __bss_end;
/** Amount of free RAM
 * \return The number of free bytes.
 */
int FreeRam() {
  char top;
  return __brkval ? &top - __brkval : &top - &__bss_end;
}
#endif  // __arm
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

//class TScreen;
// Use hardware SPI 
// ILI9341_due tft(TFT_CS, TFT_DC, TFT_RST);
UTouch  myTouch(TFTT_CLK, TFTT_CS,TFTT_TDIN, TFTT_TDOUT, TFTT_IRQ);
TScreen  myScreen(TFT_CS, TFT_DC, TFT_RST);


char buffer[254]; // general purpose buffer.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println(F("Demo")); 
  rtc.begin(dt);  

///////////////////////////////////////
// MODIFY//////////////////////////////
// Pre display some Welcome messages.
  myScreen.Welcome(); // pre init myScreen to display the welcome
  myScreen.printAt("KaraClass demo by KaraWin", 30,90);
  myScreen.printAt("Free memory: ", 30,110);
  myScreen.println(String(FreeRam(),DEC).c_str());

    delay(2000); // Welcome show timer
  // Initialize myScreen
  myScreen.Begin();

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
 ; 
}
// Called every second. Put your code here
void TScreen::userSecond()
{
;
}



