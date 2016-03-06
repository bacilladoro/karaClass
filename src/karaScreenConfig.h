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
  void ABts0_4(void);
  void ABts0_3(void);
  void ABts2_0(void);
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
  Panels->Panel->Line[1] = PSTR (String("Up to 12 lines"));
  Panels->Panel->Line[2] = PSTR (String(""));
  Panels->Panel->Line[3] = PSTR (String("This is a demo for karaClass library"));
  Panels->Panel->Line[4] = PSTR (String("Based on a web radio project"));
  Panels->Panel->Line[6] = PSTR (String("Touch the screen for buttons"));
  Panels->Panel->Line[5] = PSTR (String("------------------------------------------"));
  Panels->Panel->Line[7] = PSTR (String(""));
  Panels->Panel->Line[8] = PSTR (String("=====================+================+=========+"));

  Panels->Panel->Action = APanel;
  
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
  Panels->Bts[3]->Grouped(true); 
  
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
  Panels->Bts[0]->Button[4]->Action = ABts0_4;
  Panels->Bts[0]->Button[3]->Action = ABts0_3;

  Panels->Bts[0]->Button[2]->BiStable = true; // a toggle button
   
  Panels->Bts[1]->btColor = ILI9341_GREENYELLOW;
  Panels->Bts[1]->Button[0]->Caption = PSTR("Vol. -");
  Panels->Bts[1]->Button[1]->Caption = PSTR("Vol. +");
  Panels->Bts[1]->Button[2]->Caption = PSTR("Loudness");
  Panels->Bts[1]->Button[2]->BiStable = true; // a toggle button
  Panels->Bts[1]->Button[2]->Logo = new TLogo(this,wrledoff);
  Panels->Bts[1]->Button[2]->Logo->Width = wrledoffWidth;
  Panels->Bts[1]->Button[2]->Logo->Height = wrledoffHeight;
  Panels->Bts[1]->Button[2]->LogoOn = new TLogo(this,wrledon);
  Panels->Bts[1]->Button[2]->LogoOn->Width = wrledonWidth;
  Panels->Bts[1]->Button[2]->LogoOn->Height = wrledonHeight;
  
  Panels->Bts[2]->btColor = ILI9341_LIGHTSALMON;
  Panels->Bts[2]->Button[0]->Caption = PSTR("SSID");
  Panels->Bts[2]->Button[0]->Action = ABts2_0;
  Panels->Bts[2]->Button[1]->Caption = PSTR("Password");
  Panels->Bts[2]->Button[1]->Action = ABts2_0;
  Panels->Bts[2]->Button[2]->Caption = PSTR("Time");
  Panels->Bts[2]->Button[2]->Action = ABts2_2;
  
  Panels->Bts[3]->btColor = ILI9341_LIGHTBLUE;
  Panels->Bts[3]->Button[2]->Caption = PSTR("Usb OTG");
  Panels->Bts[3]->Button[1]->Caption = PSTR("Sd card");
  Panels->Bts[3]->Button[0]->Caption = PSTR("Web Radio");
  Panels->Bts[3]->Button[0]->Action =  ABts3_0;
  Panels->Bts[3]->Button[1]->Action =  ABts3_1;
  Panels->Bts[3]->Button[2]->Action =  ABts3_2;
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
extern SdFat SD;
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
  myScreen.StatusBar->Status[3]->Caption((char*)ci.c_str());
  myScreen.StatusBar->Status[3]->Modified = true;
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
    dbgprintln(F("Action ABts0_4"));
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
  myScreen.Println( "Received: "+ myScreen.Keyboard("Enter some text"));  //wait a sting from the keyboard and pruint16_t it
  myScreen.Panels->Draw();
}

extern RTC_Millis rtc;
void ABts2_2(void)
{ 
  DateTime dt0(rtc.now());
  uint8_t hour;
  uint8_t min;
  String newtime = myScreen.Keyboard( "Enter HH:MM",KBNUM); 
   myScreen.Println( "Waiting xx:xx ");
  myScreen.Println( "Received: "+ newtime);  //wait a sting from the keyboard and pruint16_t it
  myScreen.Panels->Draw();
  if (sscanf(newtime.c_str(),"%2d:%2d",&hour,&min) ==2)
  {
    if ((hour < 24)&&(min < 60)) 
    {
      rtc.adjust(DateTime(dt0.year(),dt0.month(),dt0.day(),hour,min,0));
    }
  }
  else myScreen.Println( "format Error");
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

