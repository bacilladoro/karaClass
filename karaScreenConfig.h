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
 * Pattern for new project
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
// for status bar refresh: 2 secondes
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


class TScreen :public ILI9341_due ,public TBase
{
public:
  TScreen(uint8_t cs, uint8_t dc, uint8_t rst = 255):ILI9341_due(cs,  dc, rst ){;}
  void Begin(void);                             // init me please
  void Welcome(void);                           // light init for welcome message
  void Task(void);                              // things to do in the main loop. Compute touch, refresh childs etc...
  void userTask(void);                          // things to do in the main loop. User part defined in main ino
  void Draw(void);                              // Draw the all screen 
  void Touch(uint16_t xt, uint16_t yt);         //compute a touch event  from interrupt 
  void unTouch(uint16_t xt, uint16_t yt);       //compute an untouch event
  uint16_t Color = ILI9341_DARKBLUE;            // default screen background color
  String Keyboard(String banner, uint8_t set);  // call a non blocking keyboard (Task() called inside)
  void Println(char* str)  {Panels->Panel->Println(str);}  // with scrolling
  void Println(String str) {Panels->Panel->Println(str);}  // with scrolling
  void Print(char* str)  {Panels->Panel->Print(str);}      // with scrolling
  void Print(String str) {Panels->Panel->Print(str);}      // with scrolling
  TEventScreen ETouch;  // internal event to relay interrupt touch detection
  TEventScreen EunTouch;// '' 
  
/////MODIFY/////////////////////////////////////////////  
// lines after must be adapted to your project
//Screen contains:
  TLogo *Logo = NULL;
  TPanels *Panels ; 
  TStBar*  StatusBar;
};
///////////////////////////////////////////
//Actions on panel and button
  void APanel(void);
   
//END MODIFY///////////////////////////////////////////////  

// Minimal init in order to display the welcome message
// not mandatory
void TScreen::Welcome()
{
  ILI9341_due::begin();  // for Print
// some default  
  setRotation(iliRotation270);  // landscape
  fillScreen(ILI9341_DARKBLUE); 
  setFontMode(gTextFontModeTransparent);
  setFont(Arial_bold_14);
  setTextColor(ILI9341_WHITE, ILI9341_NAVY);
}

// from main ino
extern TScreen  myScreen;
extern  UTouch myTouch;
/////////////////////////////////////////////////////////
// must be modified depending of the screen configuration
// Complete screen configuration, init ans start
void TScreen::Begin()
{
  // Initialize the touch screen  
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  Welcome();

//MODIFY///////////////////////////////////////////////   
// To be modified for your project  
// Screen contained init:  
  Panels = new TPanels(this);
  Panels->Panel = new TPanel(this); 
  Panels->Panel->Line[0] = PSTR (String("First line"));

  Panels->Panel->Action = APanel;
  
  Logo = new TLogo(this,karawin);
  Logo->Width = karawinWidth;

  /* don't forget to create the childs*/
  StatusBar = new TStBar(this);
//  StatusBar->Status[0]= new TStatus(this,"Src:  ","Web",10);

  // Keyboard is dynamically allocated when needed
  //Panels->Keyboard = new TKeyboard(this);

//  Panels->Bts[0] = new TButtons(this,5);
  // position
//  Panels->Bts[0]->setTop(POSBTS1);
  
//  Panels->Bts[0]->Button[0]->Caption = PSTR("Prev");
//images
//  Panels->Bts[0]->Button[0]->Logo = new TLogo(this,wrbackm);
  // actions
//  Panels->Bts[0]->Button[4]->Action = ABts0_4;

//  Panels->Bts[0]->Button[2]->BiStable = true; // a toggle button
   
////END MODIFY//////////////////////////////////////////////
// do not modify after  
//Activate and displayscreen
// starts the interrupts  
  DueTimer::getAvailable().attachInterrupt(myTouchInt).start(TIMER1);
  DueTimer::getAvailable().attachInterrupt(StatInt).start(TIMER2);
  DueTimer::getAvailable().attachInterrupt(TimeInt).start(TIMER3);;

  Show();
  Draw();   
}

// The screen motor
void TScreen::Task()
{
   StatusBar->Refresh();
   if (ETouch.isArmed())
     Touch(ETouch.xt,ETouch.yt);
   if (EunTouch.isArmed())
     unTouch(EunTouch.xt,EunTouch.yt);
     
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
   Panels->Touch(xt,yt);
}
void TScreen::unTouch(uint16_t xt, uint16_t yt)
{ 
// unarm the trigger   
   EunTouch.TrigOff();   
   Panels->unTouch(xt,yt);
}


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
void StatInt()
{
   bool st = SD.exists("webradio.ini");
  myScreen.StatusBar->Status[2]->State = st;
  myScreen.StatusBar->Status[2]->Modified = true;
}
void TimeInt()
{  
String ci ;
DateTime dt0(rtc.now());
  ci = dt0.hour() ;
  ci = ci  +":"+ dt0.minute() + ":" + dt0.second();
/*  myScreen.StatusBar->Status[3]->Caption((char*)ci.c_str());
  myScreen.StatusBar->Status[3]->Modified = true;*/
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


#endif
#else
  #error Oops! Trying to include  on another device?
#endif

