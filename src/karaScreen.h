/*
 * KaraClass A basic user environment for arduino touch screen 
 * Copyright: Jean-Pierre Cocatrix jp@cocatrix.fr http://www.karawin.fr
 * Code: https://github.com/karawin/karaClass
 * 
 * based on and require
 * ILI9341_due.h - Arduino Due library for interfacing with ILI9341-based TFTs
 * Code: https://github.com/marekburiak/ILI9341_due
 * Documentation: http://marekburiak.github.io/ILI9341_due/
 * 
 * Require: 
 *  UTouch.h - Arduino/chipKit library support for Color TFT LCD Touch screens 
 *  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
 * You can find the latest version of the library at 
 * http://www.RinkyDinkElectronics.com/
 * 
 * It is a partial class:
 * TScreen must be defined with your needs ouside.
 * See examples folder
 *  No needs to modify this .h
 */
#ifdef __arm__
#ifndef karaScreen_h
#define karaScreen_h

#if defined(ARDUINO_ARCH_AVR)
    #include <avr/pgmspace.h>
#elif defined(ARDUINO_SAM_DUE)
    #define PROGMEM
#endif
#include <stdio.h>
#include <RTClib.h>

// some trace if DEBUG = 1 else 0
#define DEBUG 1
#if DEBUG
#define dbgprint Serial.print
#define dbgprintln Serial.println
#else
#define dbgprint(...) /**/  
#define dbgprintln(...) /**/  
#endif

extern RTC_Millis rtc;
#include <ILI9341_due_config.h>
#include <ILI9341_due.h>
#include "fonts\Arial_bold_14.h"
// added a space with width = 1
#include "myArial14.h"
#include "fonts\fixednums15x31.h"

#define KARACLASS_VERSION 1.02

// size of the screen
#define WIDTH 320
#define HEIGHT 240
// max size of the screen logo
#define LHEIGHT 32
#define LWIDTH  32
// space between group of buttons (height)
#define PAD 5
// space between buttons in a TButtons
#define WPAD 5
// Height of a group of buttons TButtons
#define BHEIGHT 68
// nb Key in a raw of keyboard max
#define NRKEY 10
// nb of raw in keyboard max
#define NRKBOARD 4
// Default keyboard to Majuscule 'upper case', minuscule 'lower case' or numerical
#define KBMAJ 1
#define KBMIN 3
#define KBNUM 2
#define KBPHONE 4
//Height of a keyboard
#define KHEIGHT 160
//Height of the Keyboard Banner Caption
#define KCHEIGHT (HEIGHT-KHEIGHT-LHEIGHT)

// Position of buttons panel: POSBTS1: bottom  POSBTS2: middle  POSBTS3: top
#define POSBTS1 HEIGHT-BHEIGHT
#define POSBTS2 HEIGHT-(2*BHEIGHT)
#define POSBTS3 HEIGHT-(3*BHEIGHT)
// height of a status panel
#define SHEIGHT 22
// max nuber of TButtons on TPanels
#define MAXBTS 5
// max number of buttons in a Tbuttons group
#define MAXBT 10
// default font for the status bar
#define TSTFONT myArial14 
// default space between line on TPANEL
#define LSPACE 3
// Nb Max of lines on TPANEL
#define MAXLINE 12 
// Nb max of status in a status bar
#define MAXSTATUS 4
// Delay before screensaver (second)
#define TSCREENSAVER 60
//-------------------------------------------
//An event class to execute some action asked from interrupt.
// An interrupt must be as short as possible so one arms the event to do some action in the main loop.
// See TScreen in the example.
//-------------------------------------------
class TEventScreen
{
protected:
bool trigger;
public:
  uint16_t xt;
  uint16_t yt;
  void TrigOn(void){trigger = true;}
  void TrigOn(uint16_t x, uint16_t y){xt = x;yt = y; trigger = true;}
  void TrigOff(void) {trigger = false;}   
  TEventScreen(){;}
  bool isArmed(){return trigger;}
};
//
//An image for the logo or button
class TLogo
{
  public:
  ILI9341_due* Parent;
  uint16_t Width = 0;
  uint16_t Height = 0;
  uint16_t *Image ;
  TLogo( ILI9341_due* parent, uint16_t *image) { Parent = parent, Image = image;}
  TLogo( ILI9341_due* parent, const  uint16_t *image){Parent = parent; Image = (uint16_t*)image; }
  void Background(uint16_t color); //if not in eprom, change the white color of the logo to color, for button use
  void BackgroundWhite(uint16_t color); //revert to white the previously changed logo
  void Draw( uint16_t x, uint16_t y);  // Draw it at x,y
};
//-------------------------------------------
// Base class of  objects to be displayed
// base data and method common to childs.
class TBase
{
protected:
  ILI9341_due* Parent; // the TScreen itself
  bool active = false; // is or can be displayed
  bool displayed = false; // is on screen. changed to true in Draw
public:
  virtual void Touch(uint16_t xt, uint16_t yt) {;}
  virtual void unTouch(uint16_t xt, uint16_t yt){;}
  virtual void Draw(void)  = 0;
  bool isActive(void) const { return active;} 
  bool isDisplayed (void) const { return displayed;} 
  bool unDisplay(void)  { displayed = false;}
  void Show(void){ active = true;}
  void Hide(void){ active = false;displayed = false;} 
  void reDraw(){Hide();Show();Draw();}      
};
//
//-------------------------------------------
// a Button panel to be owned in a TButtons panel
class TButton: public TBase
{
public:
  TButton(ILI9341_due* parent,uint16_t left, uint16_t top, uint16_t width, uint16_t height);
  uint16_t Color = 0; // background color
  TLogo* Logo = NULL;     // a logo if needed
  TLogo* LogoOn = NULL;   // a logo for bistable button in state = true
  bool CheckButton = false;  // if true, the button is a bistable
  bool Radio = false;     // true if this button belongs to a radio panel ie draw it in circle
  bool State = false;     // state for bistate
  void Draw(void);
  void (*onTouch)(void) = NULL; // the external action to be called when pressed.
  void (*onUnTouch)(void) = NULL; // the external action to be called when released.
  void Touch(void);
  void unTouch(void) ;
  uint16_t Left; uint16_t Top; uint16_t Width; uint16_t Height;  // position on screen (from constructor)
  String Caption ;  // The caption on the button
};

//-------------------------------------------
//A keyboard panel of nrkey*NRKBOARD keys , a banner and a String to show the typing. Owned by TPanels

class TKeyboard: public TBase
{
protected:
  uint16_t topa ; // top for all panel
  uint16_t top;   // top of the keyboard key
  bool state = false;
  uint8_t  nrkey;
  uint16_t color = ILI9341_BLUE;
public:
  TKeyboard(ILI9341_due* parent,uint8_t  nrky= NRKEY  );
  String Caption = ""; 
  String Banner = "";
  TButton* Key[NRKEY][NRKBOARD];
  uint16_t btColor = ILI9341_GOLD;  // default color
  void setTop(uint16_t Top);  // may be for a wider screen ;-)
  uint16_t getTop() const {return topa;}
  void Touch(uint16_t xt, uint16_t yt);
  void unTouch(uint16_t xt, uint16_t yt);
  void Draw(void);
  void Start(String banner, uint8_t set ); // Init the keyboard with banner and the set if any 
  bool isAvailable() const {return state;}  // A  String is ready
  void SetMn();
  void SetBase(); 
  void Set1();   
  void Set2();   
  void Set3();   
  void Set4();
};

//-------------------------------------------
//A panel group of  nb button in a raw ) at the bottom by default
// else call setTop. Owned by TPanels
class TButtons: public TBase
{
private:
  uint16_t   nb;
  uint16_t top ;
  uint16_t color = ILI9341_BLUE;
  bool grouped = false;
public:
  TButtons(ILI9341_due* parent,  uint16_t nbr);
  TButton* Button[MAXBT];
  uint16_t btColor = ILI9341_GOLD;  // default button  background color 
//  String Caption;  // not used for now
  void setTop(uint16_t Top);
  uint16_t getTop(){return top;}
  void Touch(uint16_t xt, uint16_t yt);
  void unTouch(uint16_t xt, uint16_t yt); 
  void Draw(void);
  void RadioBox(bool state);  // if true, all buttons are bistable but only one is in state trus
};
//-------------------------------------------
// Main Panel  of the screen. Owned by TPanels
class TPanel: public TBase
{
private:
  short index = 0; // current index 
  uint16_t until = HEIGHT-LHEIGHT;
  uint16_t from =  LHEIGHT;  
public:
  TPanel(ILI9341_due* parent ){Parent = parent;Show();} // show is the default
  String Line[MAXLINE+2]; // array of String (text lines)
  uint16_t Color = ILI9341_DARKBLUE;
  void clearPanelLines();
  void clearPanel();
  void clearCurrentLine(){ Line[index] = "";}
  void Println(char* str); // with scrolling
  void Println(String str);// with scrolling
  void Print(char* str); // with scrolling
  void Print(String str);// with scrolling
  void Draw(void);
  void Draw(uint16_t from,uint16_t until);
  void Until(short unt){until = unt; }  // to display until the top of buttons panel if any
  void From(short fro){from= fro;}      // to display from the bottom of  buttons panel if any
  void Touch(uint16_t xt, uint16_t yt){if (onTouch != 0) onTouch();} 
  void unTouch(uint16_t xt, uint16_t yt){;} 
  void (*onTouch)(void) = NULL; // external action on touch
};

//-------------------------------------------
//A big time panel  , Owned by TPanels
class TBigTime: public TBase 
{
	private:
	uint16_t color = 0X1863;
	uint16_t top;   // top of the panel
	public:
	TButtons Options ;
	TBigTime(ILI9341_due* parent);
	void Touch(uint16_t xt, uint16_t yt);
	void unTouch(uint16_t xt, uint16_t yt);
	void Draw(void);
	void Hide(void) {TBase::Hide(); Options.Hide();}
};
//-------------------------------------------
// A TKeyboard, Tpanel and TButtons container
// 
class TPanels: public TBase
{
public: 
  TPanels(ILI9341_due* parent);
  bool Active = true;
  TButtons* Bts[MAXBTS]; // the set of buttons panels
  TKeyboard* Keyboard = NULL;  // created when needed
  TBigTime* BigTime = NULL; //created when needed
  TPanel* Panel = 0; 
  void Touch(uint16_t xt, uint16_t yt);
  void unTouch(uint16_t xt, uint16_t yt);
  void Draw();
  void StartBigTime();
  void StopBigTime();
  void StartKeyboard(String banner, uint8_t set );// create and show keyboard 
  String GetKeyboard(); // return caption and destroy keyboard
  bool isKeyboard(void){return Keyboard->isAvailable();}// a caption ready  
};

//-------------------------------------------
// a status label to be drawn in a status bar
class TStatus
{
  private:
  String label="";  // printed_t as "label: caption" at x left pixel
  String caption="";
  uint16_t at;  // position
  uint16_t color = ILI9341_LIGHTSTEELBLUE;
 ILI9341_due* Parent; 
public:
  TStatus(ILI9341_due* parent,String lab,String capt, uint16_t x):Parent( parent), label(lab), caption(capt) , at(x){}
  bool State = false;  // if true, the color is changed in green else it is red
  void Caption(char* cap){caption=cap;Modified = true;}
  void Label(char* cap){label=cap; Modified = true;}
  void Color(uint16_t col){ color = col;}
  void Draw();
  uint16_t At(){ return at;}
  void At(uint16_t here){ at = here;}
  uint16_t Width(); 
  bool Modified = false; // true if label or caption is changed externally (interrupt...)
};

//-------------------------------------------
//The status bar at the top of the screen
// contains up to MAXSTATUS status
// show sd status, wifi status, time etc...
// always visible
class TStBar
{
private:
  uint16_t color = ILI9341_BLACK;
  uint16_t forecolor;
  void reDraw(uint16_t index);
  ILI9341_due* Parent;  
public:
  TStBar(ILI9341_due* parent); 
  TStatus *Status[MAXSTATUS];
  // some status to display
  void Draw();
  void Refresh(); // some display to update if modified?
};

class TScreen :public ILI9341_due ,public TBase
{
	private:
	 int screensaver = 0;
	bool welcome = false;
	void Touch(uint16_t xt, uint16_t yt);         //compute a touch event  from interrupt
	void unTouch(uint16_t xt, uint16_t yt);       //compute an untouch event
	void doSecond(DateTime dtime);
	void doStatus();
	public:
	TScreen(uint8_t cs, uint8_t dc, uint8_t rst = 255):ILI9341_due(cs,  dc, rst ){;}
	void Begin(void);                             // init me please
	void Welcome(void);                           // light init for welcome message
	void Task(void);                              // things to do in the main loop. Compute touch, refresh childes etc...
	void userTask(void);                          // things to do in the main loop. User part defined in main ino
	void userSecond(void);
	void Draw(void);                              // Draw the all screen
	uint16_t Color = ILI9341_DARKBLUE;            // default screen background color
	String Keyboard(String banner, uint8_t set);  // call a non blocking keyboard (Task() called inside)
	void clearCurrentLine() {Panels->Panel->clearCurrentLine();}
	void Println(char* str)  {Panels->Panel->Println(str);}  // with scrolling
	void Println(String str) {Panels->Panel->Println(str);}  // with scrolling
	void Print(char* str)  {Panels->Panel->Print(str);}      // with scrolling
	void Print(String str) {Panels->Panel->Print(str);}      // with scrolling
	TEventScreen ETouch;  // internal event to relay interrupt touch detection
	TEventScreen EunTouch;// ''
	TEventScreen ESecond;  // internal event to do some action in a per second timer
	TEventScreen EStatus;  // internal event to compute status
	
	/////MODIFY/////////////////////////////////////////////
	// lines after may be adapted to your project
	//Screen contains:
	TLogo *Logo = NULL;
	TPanels *Panels ;
	TStBar*  StatusBar;
};


#endif
#else
  #error Oops! Trying to include  on another device?
#endif
