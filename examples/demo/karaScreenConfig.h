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
#ifdef DS3231
extern  RTC_DS3231 rtc;
#else
extern  RTC_Millis rtc;
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
  void ABts0_0(void);
  void ABts0_3(void);
  void ABts0_4(void);
  void ABts1_3(void);
  void ABts1_4(void);
  void ABts2_0(void);
  void ABts2_1(void);
  void ABts2_2(void);
  void ABts3_0(void);
  void ABts3_1(void);
  void ABts3_2(void);
  void ABts4_0(void);
  void ABts4_1(void);
  void ABts4_2(void);

/////////////////////////////////////////////////////////
// must be modified depending of the screen configuration
// Complete screen configuration, init ans start
void TScreen::userBegin()
{

// Screen contained init:  
  Panels = new TPanels(this);
  Panels->Panel = new TPanel(this); 
  Panels->Panel->Line[0] = PSTR (String("First line"));
  Panels->Panel->Line[1] = PSTR (String("Up to 12 lines"));
  Panels->Panel->Line[2] = PSTR (String(""));
  Panels->Panel->Line[3] = PSTR (String("This is a demo for karaClass library"));
  Panels->Panel->Line[4] = PSTR (String("Show components"));
  Panels->Panel->Line[6] = PSTR (String("Touch the screen for buttons"));
  Panels->Panel->Line[5] = PSTR (String("------------------------------------------"));

  Panels->Panel->onTouch = APanel;
  
  Logo = new TLogo(this,karawin);
  Logo->Width = karawinWidth;

  /* don't forget to create the childs*/
  StatusBar = new TStBar(this);
  StatusBar->Status[0]= new TStatus(this,"Stat1: ","OK",10);
  StatusBar->Status[1]= new TStatus(this,"","Status ",90); 
  StatusBar->Status[2]= new TStatus(this,"","DEMO",240);
  StatusBar->Status[3]= new TStatus(this," ","0",160);

  Panels->Slider = new TSlider(this,-254,0,-50);
  Panels->Slider->Caption = "Slider for test";
  Panels->Slider->setTop(POSBTS2);

  // Keyboard and BigTime are dynamically allocated when needed
  //Panels->Keyboard = new TKeyboard(this);

  Panels->Bts[0] = new TButtons(this,5);
  Panels->Bts[1] = new TButtons(this,3);
  Panels->Bts[2] = new TButtons(this,3);
  Panels->Bts[3] = new TButtons(this,3);
  // position
  Panels->Bts[1]->setTop(POSBTS2);
  Panels->Bts[1]->RadioBox(true); // a radiobox panel of buttons
  Panels->Bts[2]->setTop(POSBTS3);
  Panels->Bts[3]->setTop(POSBTS2);
  Panels->Bts[3]->RadioBox(true); // a radiobox panel of buttons
  
  Panels->Bts[0]->Button[0]->Caption = PSTR("Slider");
  Panels->Bts[0]->Button[1]->Caption = PSTR("Logo");
  Panels->Bts[0]->Button[2]->Caption = PSTR("Toggle");
  Panels->Bts[0]->Button[3]->Caption = PSTR("Radio");
  Panels->Bts[0]->Button[4]->Caption = PSTR("Option");
  //images
  Panels->Bts[0]->Button[1]->Logo = new TLogo(this,wrnextm);
  Panels->Bts[0]->Button[1]->Logo->Width = wrnextmWidth;
  Panels->Bts[0]->Button[2]->CheckButton = true; // a toggle button
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
  Panels->Bts[0]->Button[0]->onTouch = ABts0_0;

   
  Panels->Bts[1]->btColor = ILI9341_GREENYELLOW;
  Panels->Bts[1]->Button[0]->Caption = PSTR("radio");
  Panels->Bts[1]->Button[1]->Caption = PSTR("radio");
  Panels->Bts[1]->Button[2]->Caption = PSTR("toggle");
  Panels->Bts[1]->Button[2]->Radio = false; // 
  Panels->Bts[1]->Button[2]->CheckButton = true; // a toggle button
  
  Panels->Bts[2]->btColor = ILI9341_LIGHTSALMON;
  Panels->Bts[2]->Button[0]->Caption = PSTR("Kbd upper");
  Panels->Bts[2]->Button[0]->onTouch = ABts2_0;
  Panels->Bts[2]->Button[1]->Caption = PSTR("Kbd lower");
  Panels->Bts[2]->Button[1]->onTouch = ABts2_1;
  Panels->Bts[2]->Button[2]->Caption = PSTR("Date Time");
  Panels->Bts[2]->Button[2]->onUnTouch = ABts2_2;
  
  Panels->Bts[3]->btColor = ILI9341_LIGHTBLUE;
  Panels->Bts[3]->Button[2]->Caption = PSTR("At a time");
  Panels->Bts[3]->Button[1]->Caption = PSTR("Only one");
  Panels->Bts[3]->Button[0]->Caption = PSTR("Radio btn");
}

void TScreen::userStatus()
{
	//  dbgprintln("userStatus");
	bool stori = myScreen.StatusBar->Status[2]->State;
	bool st = !stori;
	myScreen.StatusBar->Status[2]->State = st;
	myScreen.StatusBar->Status[0]->Caption((char*)(st?"OK":"NOK"));
	myScreen.StatusBar->Status[0]->Modified = true;
	if (st != stori)
	myScreen.StatusBar->Status[2]->Modified = true;

	StatusBar->Refresh();
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
	myScreen.Panels->Slider->Hide();

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
      myScreen.Panels->Bts[2]->Show();
   }      
   else  
   if (myScreen.Panels->Bts[2]->isActive())
   {
     myScreen.Panels->Bts[2]->Hide();
     myScreen.Panels->Bts[1]->Hide();   
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
	myScreen.Panels->Slider->Hide();  
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

// Action on Bts0 button 0
void ABts0_0(void)
{
  if (myScreen.Panels->Slider->isActive())
    myScreen.Panels->Slider->Hide();
  else{
    myScreen.Panels->Slider->Show();
  }
  myScreen.Panels->Draw();
}

void ABts2_0(void)
{  
  myScreen.Panels->Bts[2]->Hide();
   myScreen.Panels->Slider->Hide();
  myScreen.clearCurrentLine();
  myScreen.Println( "Received: "+ myScreen.Keyboard("Enter some text"));  //wait a sting from the keyboard and pruint16_t it
  myScreen.Panels->Draw();
}
void ABts2_1(void)
{
	myScreen.Panels->Bts[2]->Hide();
	 myScreen.Panels->Slider->Hide();
	myScreen.clearCurrentLine();
	myScreen.Println( "Received: "+ myScreen.Keyboard("Enter some text",KBMIN));  //wait a sting from the keyboard and pruint16_t it
	myScreen.Panels->Draw();
}


void ABts2_2(void)
{
  myScreen.Panels->Slider->Hide(); 
  myScreen.Panels->StartBigTime();
}


#endif

