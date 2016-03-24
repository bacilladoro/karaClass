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
extern  SFEMP3Shield myPlayer;
// Forward declarations
void myTouchInt();
void StatInt();
void TimeInt();

// forward
// Actions for TScreen
void APanel(void);
void ABts1_4(void);
void ABts1_3(void);
void ABts4_0(void);
void ABts4_1(void);
void ABts4_2(void);

// Global
short playfile =0;
///////////////////////////////////////////
//Actions on panel and button
  void APanel(void);
  void ABts0_4(void);
  void ABts0_3(void);
  void ABts0_2(void);
  void ABts0_0(void);
  void ABts0_1(void);
  void ABts1_0(void);
  void ABts1_1(void);
  void ABts1_2(void);
  void ABts2_0(void);
  void ABts2_1(void);
  void ABts2_2(void);
  void ABts2_3(void);
  void ABts3_0(void);
  void ABts3_0(void);
  void ABts3_1(void);
  void ABts3_2(void);   
//END MODIFY///////////////////////////////////////////////  

// Minimal init in order to display the welcome message
// not mandatory

/////////////////////////////////////////////////////////
// must be modified depending of the screen configuration
// Complete screen configuration, init ans start
void TScreen::userBegin()
{
  dbgprintln("TScreen userBegin"); delay(100);
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
  Panels->Slider = new TSlider(this,-50,50,0);
  Panels->Slider->Caption = "Slider for test";
  Panels->Slider->setTop(POSBTS3);


  Panels->Bts[0] = new TButtons(this,5);
  Panels->Bts[1] = new TButtons(this,4);
  Panels->Bts[2] = new TButtons(this,4);
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
  Panels->Bts[0]->Button[3]->Caption = PSTR("Stop");
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
  Panels->Bts[0]->Button[3]->Logo = new TLogo(this,wrstopm);
  Panels->Bts[0]->Button[3]->Logo->Width = wrstopmWidth;
  Panels->Bts[0]->Button[4]->Logo = new TLogo(this,wrsettingsm);
  Panels->Bts[0]->Button[4]->Logo->Width = wrsettingsmWidth; 
  // actions
  Panels->Bts[0]->Button[4]->onUnTouch = ABts0_4;
  Panels->Bts[0]->Button[3]->onUnTouch = ABts0_3;
  Panels->Bts[0]->Button[2]->onUnTouch = ABts0_2;
  Panels->Bts[0]->Button[1]->onTouch = ABts0_1;
  Panels->Bts[0]->Button[0]->onTouch = ABts0_0;
  Panels->Bts[0]->Button[2]->CheckButton = true; // a toggle button
   
  Panels->Bts[1]->btColor = ILI9341_GREENYELLOW;
  Panels->Bts[1]->Button[0]->Caption = PSTR("Volume");
  Panels->Bts[1]->Button[1]->Caption = PSTR("Bass");
  Panels->Bts[1]->Button[2]->Caption = PSTR("Treble");
  Panels->Bts[1]->Button[3]->Caption = PSTR("Autoplay");
  Panels->Bts[1]->Button[3]->CheckButton = true; // a toggle button
  Panels->Bts[1]->Button[0]->onTouch = ABts1_0;
  Panels->Bts[1]->Button[1]->onTouch = ABts1_1;
  Panels->Bts[1]->Button[2]->onTouch = ABts1_2;
  
  Panels->Bts[2]->btColor = ILI9341_LIGHTSALMON;
  Panels->Bts[2]->Button[0]->Caption = PSTR("SSID");
  Panels->Bts[2]->Button[0]->onTouch = ABts2_0;
  Panels->Bts[2]->Button[1]->Caption = PSTR("Password");
  Panels->Bts[2]->Button[1]->onTouch = ABts2_1;
  Panels->Bts[2]->Button[2]->Caption = PSTR("Time");
  Panels->Bts[2]->Button[2]->onUnTouch = ABts2_2;
  Panels->Bts[2]->Button[3]->Caption = PSTR("Source");
  Panels->Bts[2]->Button[3]->Logo = new TLogo(this,wrsourcem);
  Panels->Bts[2]->Button[3]->Logo->Width = wrsourcemWidth;
  Panels->Bts[2]->Button[3]->onUnTouch = ABts2_3;
  
  Panels->Bts[3]->btColor = ILI9341_LIGHTBLUE;
  Panels->Bts[3]->Button[2]->Caption = PSTR("Usb OTG");
  Panels->Bts[3]->Button[1]->Caption = PSTR("Sd card");
  Panels->Bts[3]->Button[0]->Caption = PSTR("Web Radio");
  Panels->Bts[3]->Button[0]->onTouch =  ABts3_0;
  Panels->Bts[3]->Button[1]->onTouch =  ABts3_1;
  Panels->Bts[3]->Button[2]->onTouch =  ABts3_2;
  dbgprintln("TScreen userBegin2");delay(100);
}


void TScreen::userStatus()
{
//  dbgprintln("userStatus");

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

void ABts0_0(void) // previous
{
  if (myScreen.Panels->Bts[3]->Button[1]->State)  //sd
  {
      playfile--;if (playfile == 0) playfile = 1;
	  if (myScreen.Panels->Bts[0]->Button[2]->State)  //playing
	  {
		  myScreen.Panels->Panel->clearPanelLines();
		  myPlayer.stopTrack();
		  ABts0_2();
	  }	 else  ABts0_2();
  }  
}
void ABts0_1(void) // next
{
char trackName[] = "track001.mp3";
	if (myScreen.Panels->Bts[3]->Button[1]->State)
	{
		playfile++;
		 dbgprintln (playfile);
		sprintf(trackName, "track%03d.fla", playfile);
		dbgprintln (trackName);
		if(!SD.exists(trackName))
		{
		  sprintf(trackName, "track%03d.mp3",playfile);
		  dbgprintln ("fla not found");
		  dbgprintln (trackName);
		} 

		if (!SD.exists(trackName))
		{ 
		  playfile=1;
		  dbgprint (trackName);dbgprintln (" not found");
		}
		if (myPlayer.isPlaying()!=0)
		{
			myScreen.Panels->Panel->clearPanelLines();
			myPlayer.stopTrack();
			myScreen.Panels->Bts[0]->Button[2]->State = false;
			ABts0_2();
		} else	ABts0_2();	
	}
}
void ABts0_2(void)
{
char trackName[] = "track001.mp3";
 char title[30] ={0,0}; // buffer to contain the extract the Title from the current filehandles
 char artist[30]={0,0}; // buffer to contain the extract the artist name from the current filehandles
 char album[30]={0,0}; // buffer to contain the extract the album name from the current filehandles
 uint8_t result; //result code from some function as to be tested at later time.

  if (myScreen.Panels->Bts[3]->Button[1]->State)
  {
    if ((!myScreen.Panels->Bts[0]->Button[2]->State) && (myPlayer.isPlaying()==1))
	{
	  myPlayer.pauseDataStream();
	  dbgprint("Pause ");
	}
	else
	{
	  result = myPlayer.isPlaying();
      dbgprint(F("Status play1 "));dbgprintln(String(result));
	  if (result == 2)
	  {
	    myPlayer.resumeDataStream();
	    dbgprint("Resume ");
	  }
	  else
	  {
		  if (result == 1) myPlayer.stopTrack();
	      sprintf(trackName, "track%03d.mp3", playfile);
	      dbgprint(F("Status "));dbgprintln(String(result));
	      if (playfile == 0) playfile = 1;
          result = myPlayer.playTrack(playfile);
    	  dbgprint(F("playTrack Result= "));dbgprintln(String(result));
	      myPlayer.trackTitle((char*)&title);
	      myPlayer.trackArtist((char*)&artist);
	      result = myPlayer.isPlaying();
	      dbgprint(F("Status "));dbgprintln(String(result));
	      dbgprintln(F(title));
	      dbgprintln(F(artist));
	      myScreen.Panels->Panel->clearPanelLines();
	      myScreen.Println(trackName);
	      myScreen.Println(title);
	      myScreen.Println(artist);
	  }
	}
  }

}
void ABts0_3(void) //stop
{
  if (myScreen.Panels->Bts[3]->Button[1]->State)
  {
	  if (myScreen.Panels->Bts[0]->Button[2]->State)
	  {
		  myScreen.Panels->Bts[0]->Button[2]->State = FALSE;
		  myScreen.Panels->Bts[0]->Draw();
	  }
	  myScreen.Panels->Panel->clearPanelLines();
	  myPlayer.stopTrack();
  }
}


void sliderHide()
{
     myScreen.Panels->Slider->Hide();
     if (myScreen.Panels->Bts[2]->isDisplayed() ) myScreen.Panels->Bts[2]->Draw();
     else myScreen.Panels->Panel->reDraw();
}
void sliderShow()
{
	  myScreen.Panels->Slider->Show();
	  myScreen.Panels->Slider->Draw();
}
void sliderVolume()
{
	union twobyte mp3_vol; // create key_command existing variable that can be both word and double byte of left and right.
	mp3_vol.word = myPlayer.getVolume(); // returns a double uint8_t of Left and Right packed into int16_t
	// assume equal balance and use byte[1] for math
	mp3_vol.byte[1] = abs(myScreen.Panels->Slider->getValue());

	if(mp3_vol.byte[1] >= 254) { // range check
		mp3_vol.byte[1] = 254;
		} 
	myPlayer.setVolume(mp3_vol.byte[1], mp3_vol.byte[1]); // commit new volume
}
void ABts1_0(void) // volume
{
union twobyte mp3_vol;
   mp3_vol.word = myPlayer.getVolume();
  myScreen.Panels->Slider->Caption = "Volume";
  myScreen.Panels->Slider->setMini(-150);myScreen.Panels->Slider->setMaxi(0);myScreen.Panels->Slider->setCenter(-mp3_vol.byte[1]);
  myScreen.Panels->Slider->onSlide = sliderVolume;
  myScreen.Panels->Slider->onunTouch = sliderHide; //Hide
  if (myScreen.Panels->Slider->isActive())
  {
    sliderHide();
  }
  else{
	sliderShow();
  }
 // myScreen.Panels->Draw();
}

void sliderBass()
{
uint16_t BassAmplitude; 
BassAmplitude = myPlayer.getBassAmplitude(); 
 myPlayer.setBassAmplitude( myScreen.Panels->Slider->getValue());
}
void ABts1_1(void) // bass
{
  myScreen.Panels->Slider->Caption = "Bass Amplitude";
  myScreen.Panels->Slider->setMini(0);myScreen.Panels->Slider->setMaxi(15);myScreen.Panels->Slider->setCenter(myPlayer.getBassAmplitude());
  myScreen.Panels->Slider->onSlide = sliderBass;
  myScreen.Panels->Slider->onunTouch = sliderHide; //Hide
  myPlayer.setBassFrequency(200);
  if (myScreen.Panels->Slider->isActive())
  {
    sliderHide();
  }
  else{
	  sliderShow();
  }
}
void sliderTreble()
{
	uint8_t TrebleAmplitude; 
	TrebleAmplitude = myPlayer.getTrebleAmplitude(); //
	myPlayer.setTrebleAmplitude( (uint8_t)myScreen.Panels->Slider->getValue());
}
void ABts1_2(void) // Treble
{
	myScreen.Panels->Slider->Caption = "Treble Amplitude";
	myScreen.Panels->Slider->setMini(-8);myScreen.Panels->Slider->setMaxi(7);myScreen.Panels->Slider->setCenter(myPlayer.getTrebleAmplitude());
	myScreen.Panels->Slider->onSlide = sliderTreble;
	myScreen.Panels->Slider->onunTouch = sliderHide; //Hide
	myPlayer.setTrebleFrequency(2000);
	if (myScreen.Panels->Slider->isActive())
	{
      sliderHide();
	}
	else{
	  sliderShow();
	}
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
	  myScreen.Panels->Slider->Hide();  
   }
   if (myScreen.Panels->Bts[1]->isActive())
   {
      myScreen.Panels->Bts[1]->Hide();
      myScreen.Panels->Bts[2]->Hide();
	  myScreen.Panels->Slider->Hide();
   }      
   else  
   {
     myScreen.Panels->Bts[2]->Show();
     myScreen.Panels->Bts[1]->Show();
//     Draw();   
   }

 myScreen.Panels->Draw();    
}
// Action on Bts2 button 3
void ABts2_3(void)
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

#ifdef DS3231
extern  RTC_DS3231 rtc;
#else
extern  RTC_Millis rtc;
#endif

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


