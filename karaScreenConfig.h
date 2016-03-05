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
 */

#ifdef __arm__
#ifndef karaScreenConfig_h
#define karaScreenConfig_h

#if defined(ARDUINO_ARCH_AVR)
    #include <avr/pgmspace.h>
#elif defined(ARDUINO_SAM_DUE)
    #define PROGMEM
#endif
#include <RTClib.h>
// the library base
#include "karaScreen.h"
// Images for the demo
#include "karawin.h"

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
  void Begin(void);  // init me
  void Welcome(void);// light init
  void Task(void); // things to do in the main loop. Compute touch, refresh childs etc...
  void Draw(void);  
  void Touch(uint16_t xt, uint16_t yt);   
  void unTouch(uint16_t xt, uint16_t yt);
  uint16_t Color = ILI9341_DARKBLUE; 
  String Keyboard(String banner, uint8_t set);
  void Println(char* str)  {Panels->Panel->Println(str);} // with scrolling
  void Println(String str) {Panels->Panel->Println(str);}// with scrolling
  void Print(char* str)  {Panels->Panel->Print(str);} // with scrolling
  void Print(String str) {Panels->Panel->Print(str);}// with scrolling
  TEventScreen ETouch;  
  TEventScreen EunTouch; 
/////////////////////////////////////////////////////////  
// lines after must be adapted to your project
//Screen contains:
  TLogo *Logo = NULL;
  TPanels *Panels ; 
  TStBar*  StatusBar;
};

//Actions
extern TScreen myScreen;  
  void APanel(void);
  void ABts0_4(void);

///////////////////////////////////////////////////////////   
  
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
// must be modified depending of the screen configuration
void TScreen::Begin()
{
  Welcome();

///////////////////////////////////////////////////////////   
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
  StatusBar->Status[0]= new TStatus(this,"Src:  ","Web",10);
 

  // Keyboard is dynamically allocated when needed
  //Panels->Keyboard = new TKeyboard(this);

  Panels->Bts[0] = new TButtons(this,5);

  // position
//  Panels->Bts[1]->setTop(POSBTS2);
//  Panels->Bts[2]->setTop(POSBTS3);
//  Panels->Bts[3]->setTop(POSBTS2);
//  Panels->Bts[3]->Grouped(true); 
  
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
//////////////////////////////////////////////////////////////////////////
   
//Activate and displscreen
  Show();
  Draw();   
}

void TScreen::Task()
{
   StatusBar->Refresh();
   if (ETouch.isArmed())
   {
     Touch(ETouch.xt,ETouch.yt);
   }
   if (EunTouch.isArmed())
   {
     unTouch(EunTouch.xt,EunTouch.yt);
   }  
}

String TScreen::Keyboard(String banner, uint8_t set = KBMAJ )
{
  myScreen.Panels->StartKeyboard(banner,set);
  while (!myScreen.Panels->isKeyboard()) myScreen.Task();
  return myScreen.Panels->GetKeyboard();  
}
//  ---------------------TScreen--------------------------
// must be modified depending of the screen configuration
//-------------------------------------------
//the screen made with one Status bar, a logo,  5 buttons array max and a panel of text.
///////////////////////////////////////////////////////////    
void TScreen::Draw()
{
  Logo->Draw( 288, 0 );
  if (!active) return;
  StatusBar->Draw();
  Panels->Draw();
}

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

/*
 *   Action to be executed on touch button
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

