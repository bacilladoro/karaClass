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



// Actions for TScreen
//void APanel(void);



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
  TEventScreen ETouch;  
  TEventScreen EunTouch; 
/////////////////////////////////////////////////////////  
// lines after must be adapted to your project
//Screen contains:

};

//Actions
extern TScreen myScreen;  
  void APanel(void);

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
 

///////////////////////////////////////////////////////////   
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
//   Panels->Touch(xt,yt);
// unarm the trigger   
    ETouch.TrigOff();    
}
void TScreen::unTouch(int xt, int yt)
{ 
//   Panels->unTouch(xt,yt);
// unarm the trigger   
   EunTouch.TrigOff();   
}

/*
 *   Action to be executed on touch button
 */
///////////////////////////////////////////////////////////   


#endif
#else
  #error Oops! Trying to include  on another device?
#endif

