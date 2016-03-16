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

#ifndef karaScreenConfig_h
#define karaScreenConfig_h

#include <RTClib.h>
#if defined(ARDUINO_ARCH_AVR)
    #include <avr/pgmspace.h>
#elif defined(ARDUINO_SAM_DUE)
    #define PROGMEM
#endif

//
///MODIFY/////////////////////////
// Images for the demo
#include "karawin.h"
//END MODIFY//////////////////////


// from main ino
extern  TScreen  myScreen;
extern  UTouch myTouch;
// Forward declarations
void myTouchInt();
void StatInt();
void TimeInt();
// forward
// Actions for TScreen
//MODIFY///////////////////////////////////////////////
// To be modified for your project
void APanel(void);


/////////////////////////////////////////////////////////
// must be modified depending of the screen configuration
// Complete screen configuration, init ans start
void TScreen::userBegin()
{

// Screen contained init:  
  Panels = new TPanels(this);
  Panels->Panel = new TPanel(this); 

  Panels->Panel->onTouch = APanel;
  
  Logo = new TLogo(this,karawin);
  Logo->Width = karawinWidth;

  /* don't forget to create the childs*/
  StatusBar = new TStBar(this);
 
  Panels->Slider = new TSlider(this,-50,50,0);


  // Keyboard and BigTime are dynamically allocated when needed
  //Panels->Keyboard = new TKeyboard(this);

}

////////////////////////////////////////////////////////////
/*
 *   Action to be executed on touch button
 *   Must be modified
 */
 ///////////////////////////////////////////////////////////   
// Action on panel: show/hide bts[0]
void  APanel(void)
{ 

myScreen.Panels->Draw(); 
}


#endif

