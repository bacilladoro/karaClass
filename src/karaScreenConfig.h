/* Demo for
 * KaraClass A basic user environment for arduino touch screen 
 * Copyright: Jean-Pierre Cocatrix jp@cocatrix.fr http://www.karawin.fr
 * 
 * https://github.com/karawin/karaClass
 * 
 * KaraClass is a partial class:
 * TScreen must be defined with your needs.
 * See examples folder
 * 
 * TO adapt to your project, only regions between MODIFY and ENDMODIFY comments have to be adapted.
 */

#ifdef __arm__
#ifndef karaScreenConfig_h
#define karaScreenConfig_h

#include <DueTimer.h>
#include <UTouch.h>

#if defined(ARDUINO_ARCH_AVR)
    #include <avr/pgmspace.h>
#elif defined(ARDUINO_SAM_DUE)
    #define PROGMEM
#endif
#include <RTClib.h>
//
// the library base
#include "karaScreen.h"
///MODIFY/////////////////////////
// Images for the demo
#include "karawin.h"
//END MODIFY//////////////////////

// Forward declarations
void myTouchInt();
void StatInt();
void TimeInt();


// TIMER
// timer interrupt for touch screen 100ms
#define TIMER1 100000
// for status bar refresh: 2 seconds
#define TIMER2 2000000
// clock display  1 sec
#define TIMER3 1000000

RTC_Millis rtc;
DateTime dt( F(__DATE__), F(__TIME__)); // init time with the compilation info

// forward
// Actions for TScreen
void APanel(void);
void ABts1_4(void);
void ABts1_3(void);
void ABts4_0(void);
void ABts4_1(void);
void ABts4_2(void);


///////////////////////////////////////////
//Actions on panel and button
  void APanel(void);
  void ABts0_4(void);
  void ABts0_3(void);
  void ABts2_0(void);
  void ABts2_1(void);
  void ABts2_2(void);
  void ABts3_0(void);
  void ABts3_1(void);
  void ABts3_2(void);   
//END MODIFY///////////////////////////////////////////////  

// Minimal init in order to display the welcome message
// not mandatory
void TScreen::Welcome()
{
  ILI9341_due::begin();  // for Print
// some default  
  welcome = true;
  setRotation(iliRotation270);  // landscape
  fillScreen(ILI9341_DARKBLUE); 
  setFontMode(gTextFontModeTransparent);
  setFont(Arial_bold_14);
  setTextColor(ILI9341_WHITE, ILI9341_NAVY);
}

// from main ino
extern TScreen  myScreen;
extern ESP8266 myWifi;
extern  UTouch myTouch;
/////////////////////////////////////////////////////////
// must be modified depending of the screen configuration
// Complete screen configuration, init ans start
void TScreen::Begin()
{
  // Initialize the touch screen  
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  if (!welcome) Welcome();

//MODIFY///////////////////////////////////////////////   
// To be modified for your project  
// Screen contained init:  
  Panels = new TPanels(this);
  Panels->Panel = new TPanel(this); 
  Panels->Panel->Line[0] = PSTR (String("First line"));
  Panels->Panel->Line[1] = PSTR (String("Up to 12 lines"));
  Panels->Panel->Line[2] = PSTR (String(""));
  Panels->Panel->Line[3] = PSTR (String("This is a demo for karaClass library"));
  Panels->Panel->Line[4] = PSTR (String("Based on a web radio project"));
  Panels->Panel->Line[6] = PSTR (String("Touch the screen for buttons"));
  Panels->Panel->Line[5] = PSTR (String("------------------------------------------"));

  Panels->Panel->onTouch = APanel;
  
  Logo = new TLogo(this,karawin);
  Logo->Width = karawinWidth;

  /* don't forget to create the childs*/
  StatusBar = new TStBar(this);
  StatusBar->Status[0]= new TStatus(this,"Src:  ","Web",10);
  StatusBar->Status[1]= new TStatus(this,"","WIFI",240); 
  StatusBar->Status[2]= new TStatus(this,"","SD",210);
  StatusBar->Status[3]= new TStatus(this," ","0",135);

  // Keyboard is dynamically allocated when needed
  //Panels->Keyboard = new TKeyboard(this);

  Panels->Bts[0] = new TButtons(this,5);
  Panels->Bts[1] = new TButtons(this,3);
  Panels->Bts[2] = new TButtons(this,3);
  Panels->Bts[3] = new TButtons(this,3);
  Panels->Bts[4] = new TButtons(this,4);
  // position
  Panels->Bts[1]->setTop(POSBTS2);
  Panels->Bts[2]->setTop(POSBTS3);
  Panels->Bts[3]->setTop(POSBTS2);
  Panels->Bts[3]->RadioBox(true); // a radiobox panel of buttons
  
  Panels->Bts[0]->Button[0]->Caption = PSTR("Prev");
  Panels->Bts[0]->Button[1]->Caption = PSTR("Next");
  Panels->Bts[0]->Button[2]->Caption = PSTR("Play");
  Panels->Bts[0]->Button[3]->Caption = PSTR("Source");
  Panels->Bts[0]->Button[4]->Caption = PSTR("Option");
  //images
  Panels->Bts[0]->Button[0]->Logo = new TLogo(this,wrbackm);
  Panels->Bts[0]->Button[0]->Logo->Width = wrbackmWidth;
  Panels->Bts[0]->Button[1]->Logo = new TLogo(this,wrnextm);
  Panels->Bts[0]->Button[1]->Logo->Width = wrnextmWidth;
  Panels->Bts[0]->Button[2]->Logo = new TLogo(this,wrplaym);
  Panels->Bts[0]->Button[2]->Logo->Width = wrplaymWidth;
  Panels->Bts[0]->Button[2]->LogoOn = new TLogo(this,wrpausem);
  Panels->Bts[0]->Button[2]->LogoOn->Width = wrpausemWidth;
  Panels->Bts[0]->Button[3]->Logo = new TLogo(this,wrsourcem);
  Panels->Bts[0]->Button[3]->Logo->Width = wrsourcemWidth;
  Panels->Bts[0]->Button[4]->Logo = new TLogo(this,wrsettingsm);
  Panels->Bts[0]->Button[4]->Logo->Width = wrsettingsmWidth; 
  // actions
  Panels->Bts[0]->Button[4]->onTouch = ABts0_4;
  Panels->Bts[0]->Button[3]->onTouch = ABts0_3;

  Panels->Bts[0]->Button[2]->CheckButton = true; // a toggle button
   
  Panels->Bts[1]->btColor = ILI9341_GREENYELLOW;
  Panels->Bts[1]->Button[0]->Caption = PSTR("Vol. -");
  Panels->Bts[1]->Button[1]->Caption = PSTR("Vol. +");
  Panels->Bts[1]->Button[2]->Caption = PSTR("Loudness");
  Panels->Bts[1]->Button[2]->CheckButton = true; // a toggle button
  
  Panels->Bts[2]->btColor = ILI9341_LIGHTSALMON;
  Panels->Bts[2]->Button[0]->Caption = PSTR("SSID");
  Panels->Bts[2]->Button[0]->onTouch = ABts2_0;
  Panels->Bts[2]->Button[1]->Caption = PSTR("Password");
  Panels->Bts[2]->Button[1]->onTouch = ABts2_1;
  Panels->Bts[2]->Button[2]->Caption = PSTR("Time");
  Panels->Bts[2]->Button[2]->onUnTouch = ABts2_2;
  
  Panels->Bts[3]->btColor = ILI9341_LIGHTBLUE;
  Panels->Bts[3]->Button[2]->Caption = PSTR("Usb OTG");
  Panels->Bts[3]->Button[1]->Caption = PSTR("Sd card");
  Panels->Bts[3]->Button[0]->Caption = PSTR("Web Radio");
  Panels->Bts[3]->Button[0]->onTouch =  ABts3_0;
  Panels->Bts[3]->Button[1]->onTouch =  ABts3_1;
  Panels->Bts[3]->Button[2]->onTouch =  ABts3_2;
////END MODIFY//////////////////////////////////////////////
// do not modify after  
//Activate and display screen
// starts the interrupts  
  DueTimer::getAvailable().attachInterrupt(myTouchInt).start(TIMER1);  // for touch / untouch
  DueTimer::getAvailable().attachInterrupt(StatInt).start(TIMER2);     // for status bar computing
  DueTimer::getAvailable().attachInterrupt(TimeInt).start(TIMER3);;    // for user task and time task

  Show();
  Draw();   
}

// The screen motor
void TScreen::Task()
{
   if (ETouch.isArmed())  // touch trigged in interrupt. Process it here in the main loop
     Touch(ETouch.xt,ETouch.yt);
   if (EunTouch.isArmed())
     unTouch(EunTouch.xt,EunTouch.yt);
   if (ESecond.isArmed())
     doSecond(DateTime (rtc.now()));;
   if (EStatus.isArmed())
     doStatus();
   
     
// call the user processing
   userTask();  
}

// allocate a keyboard and wait for the returned String if any.
// Non blocking for the user point of view.
String TScreen::Keyboard(String banner, uint8_t set = KBMAJ )
{
  Panels->StartKeyboard(banner,set);
  while (!Panels->isKeyboard()) Task();
  return Panels->GetKeyboard();  
}

/////////////////////////////////////////////////////
//  ---------------------TScreen--------------------------
// may be modified depending of the screen configuration
//-------------------------------------------
//the screen made with one Status bar, a logo,  5 buttons array max and a panel of text.
///MODIFY/////////////////////////////////////////////////    
void TScreen::Draw()
{
  Logo->Draw( 288, 0 );
  if (!active) return;
  StatusBar->Draw();
  Panels->Draw();
}
////END MODIFY////////////////////////////////////
//
// touch screen detected
void TScreen::Touch(uint16_t xt, uint16_t yt)
{  
// unarm the trigger   
    ETouch.TrigOff(); 
	screensaver = 0; // activity so reset screensaver   
   Panels->Touch(xt,yt);
}
void TScreen::unTouch(uint16_t xt, uint16_t yt)
{ 
// unarm the trigger   
   EunTouch.TrigOff();   
   Panels->unTouch(xt,yt);
}
// called every second. Add your own processing
void TScreen::doSecond(DateTime dtime)
{
  ESecond.TrigOff();   
  if (Panels->BigTime != NULL) 
  {
    if (Panels->BigTime->isActive())
      Panels->BigTime->Draw();
  }
  
//  dbgprint("screensaver: ");dbgprintln(screensaver);
  if (screensaver == TSCREENSAVER)
  {
     screensaver++;
	 dbgprintln("screensaver go");
       Panels->StartBigTime();
  } else   
  if (screensaver != TSCREENSAVER+1) screensaver++;
  userSecond(); // call the user part
}
void TScreen::doStatus()
{
//  dbgprintln("doStatus");
  EStatus.TrigOff();
  bool stori = myScreen.StatusBar->Status[2]->State;
  bool st = SD.exists("webradio.ini");
  myScreen.StatusBar->Status[2]->State = st;
  if (st != stori)
    myScreen.StatusBar->Status[2]->Modified = true;

/*  stori = myScreen.StatusBar->Status[1]->State;
  st =myScreen.StatusBar->Status[1]->State = myWifi.Test();
  if (st != stori)
    myScreen.StatusBar->Status[1]->Modified = true;*/

  StatusBar->Refresh();
}
///MODIFY/////////////////////////////////////////


////END MODIFY////////////////////////////////////


///////////////////////////////////////////////////////
uint16_t xt,yt;
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
extern SdFat SD;
extern  ESP8266 myWifi;
void StatInt()
{
  myScreen.EStatus.TrigOn();
}
void TimeInt()
{  
char  ci[10] ;
DateTime dt0(rtc.now());
  myScreen.ESecond.TrigOn();
  sprintf(ci,"%02d:%02d:%02d", dt0.hour(),dt0.minute(),dt0.second());
  myScreen.StatusBar->Status[3]->Caption(ci);
}
///MODIFY///////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/*
 *   Action to be executed on touch button
 *   Must be modified
 */
 ///////////////////////////////////////////////////////////   
// Action on panel: show/hide bts[0]
void  APanel(void)
{ 
  if ( myScreen.Panels->Bts[0]->isActive())
  {
    myScreen.Panels->Bts[0]->Hide();
    myScreen.Panels->Bts[1]->Hide();
    myScreen.Panels->Bts[2]->Hide();
    myScreen.Panels->Bts[3]->Hide();
    myScreen.Panels->Bts[4]->Hide();     
  }
  else 
  {  
    myScreen.Panels->Bts[0]->Show(); 
  } 
myScreen.Panels->Draw(); 
}

// Action on Panels->Bts[0] button 4 touch
void ABts0_4(void)
{
    dbgprintln(F("onTouch ABts0_4"));
// hide/show (toggle) 
  if (
    (myScreen.Panels->Bts[3]->isActive())||
    (myScreen.Panels->Bts[4]->isActive())  )
   {
      myScreen.Panels->Bts[3]->Hide();
      myScreen.Panels->Bts[4]->Hide();
//      Draw();   
   }
   if ((myScreen.Panels->Bts[1]->isActive())&&(!myScreen.Panels->Bts[2]->isActive()))
   {
//      myScreen.Panels->Bts[1]->Hide();
      myScreen.Panels->Bts[2]->Show();
   }      
   else  
   if (myScreen.Panels->Bts[2]->isActive())
   {
     myScreen.Panels->Bts[2]->Hide();
     myScreen.Panels->Bts[1]->Hide();
//     Draw();   
   }
   else
   myScreen.Panels->Bts[1]->Show();  
 myScreen.Panels->Draw();    
}
// Action on Bts0 button 3
void ABts0_3(void)
{
    myScreen.Panels->Bts[1]->Hide();
    myScreen.Panels->Bts[2]->Hide();
    myScreen.Panels->Bts[4]->Hide();    
   if (myScreen.Panels->Bts[3]->isActive())
   {
     myScreen.Panels->Bts[3]->Hide();    
   }  
   else
   {
     myScreen.Panels->Bts[3]->Show();

   }
 myScreen.Panels->Draw();    
}

void ABts2_0(void)
{  
  myScreen.Panels->Bts[2]->Hide();
  myScreen.clearCurrentLine();
  myScreen.Println( "Received: "+ myScreen.Keyboard("Enter some text for SSID"));  //wait a sting from the keyboard and pruint16_t it
  myScreen.Panels->Draw();
}
void ABts2_1(void)
{
	myScreen.Panels->Bts[2]->Hide();
	myScreen.clearCurrentLine();
	myScreen.Println( "Received: "+ myScreen.Keyboard("Enter some text for Password"));  //wait a sting from the keyboard and pruint16_t it
	myScreen.Panels->Draw();
}

extern RTC_Millis rtc;
void ABts2_2(void)
{ 
  myScreen.Panels->StartBigTime();
}
// Action on Bts
void ABts3_0(void)
{
  myScreen.StatusBar->Status[0]->Caption("Web Radio");
}
void ABts3_1(void)
{
  myScreen.StatusBar->Status[0]->Caption("SD card");
}
void ABts3_2(void)
{
  myScreen.StatusBar->Status[0]->Caption("Usb OTG");
}


#endif
#else
  #error Oops! Trying to include  on another device?
#endif

