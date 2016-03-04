/* Demo for
 * karaClass, Classes for  DUE project 
 * Copyright: Jean-Pierre Cocatrix jp@cocatrix.fr http://www.karawin.fr
 * 
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
// the library base
#include <karaScreen.h>
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
  void Touch(int xt, int yt);   
  void unTouch(int xt, int yt);
  int Color = ILI9341_DARKBLUE; 
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
  void ABts1_4(void);
  void ABts1_3(void);
  void ABts4_0(void);
  void ABts4_1(void);
  void ABts4_2(void);  
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

  Panels->Bts[0] = new TButtons(this,5);
  Panels->Bts[1] = new TButtons(this,3);
  Panels->Bts[2] = new TButtons(this,4);
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
  Panels->Bts[0]->Button[4]->Action = ABts1_4;
  Panels->Bts[0]->Button[3]->Action = ABts1_3;

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
  Panels->Bts[2]->Button[0]->Caption = PSTR("Bass -");
  Panels->Bts[2]->Button[1]->Caption = PSTR("Bass +");
  Panels->Bts[2]->Button[2]->Caption = PSTR("Treble -");
  Panels->Bts[2]->Button[3]->Caption = PSTR("Treble +");

  
  Panels->Bts[3]->btColor = ILI9341_LIGHTBLUE;
  Panels->Bts[3]->Button[2]->Caption = PSTR("Usb OTG");
  Panels->Bts[3]->Button[1]->Caption = PSTR("Sd card");
  Panels->Bts[3]->Button[0]->Caption = PSTR("Web Radio");
  Panels->Bts[3]->Button[0]->Action =  ABts4_0;
  Panels->Bts[3]->Button[1]->Action =  ABts4_1;
  Panels->Bts[3]->Button[2]->Action =  ABts4_2;
  
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
void TScreen::Touch(int xt, int yt)
{  
   Panels->Touch(xt,yt);
// unarm the trigger   
    ETouch.TrigOff();    
}
void TScreen::unTouch(int xt, int yt)
{ 
   Panels->unTouch(xt,yt);
// unarm the trigger   
   EunTouch.TrigOff();   
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

// Action on Panels->Bts[0] button 4 touch
void ABts1_4(void)
{
//    Serial.println(F("Action ABts1_4"));
// hide/show (toggle) 
  if (
    (myScreen.Panels->Bts[3]->isActive())||
    (myScreen.Panels->Bts[4]->isActive())  )
   {
      myScreen.Panels->Bts[3]->Hide();
      myScreen.Panels->Bts[4]->Hide();
//      Draw();   
   }
   if ((myScreen.Panels->Bts[1]->isActive()))
   {
      myScreen.Panels->Bts[1]->Hide();
      myScreen.Panels->Bts[2]->Show();
   }      
   else  
   if (myScreen.Panels->Bts[2]->isActive())
   {
     myScreen.Panels->Bts[2]->Hide();
//     Draw();   
   }
   else
   myScreen.Panels->Bts[1]->Show();  
 myScreen.Panels->Draw();    
}
// Action on Bts1 button 3
void ABts1_3(void)
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
// Action on Bts
void ABts4_0(void)
{
  myScreen.StatusBar->Status[0]->Caption("Web Radio");
}
void ABts4_1(void)
{
  myScreen.StatusBar->Status[0]->Caption("SD card");
}
void ABts4_2(void)
{
  myScreen.StatusBar->Status[0]->Caption("Usb OTG");
}


#endif
#else
  #error Oops! Trying to include  on another device?
#endif

