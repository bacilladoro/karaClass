/*
 * Classes for  DUE project 
 * Copyright: Jean-Pierre Cocatrix jp@cocatrix.fr http://www.karawin.fr
 * 
 * based on and require
 * ILI9341_due.h - Arduino Due library for interfacing with ILI9341-based TFTs
 * Code: https://github.com/marekburiak/ILI9341_due
 * Documentation: http://marekburiak.github.io/ILI9341_due/
 * 
 * Require: 
 *  UTouch.h - Arduino/chipKit library support for Color TFT LCD Touch screens 
 *  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
 *
 * Basic functionality of this library are based on the demo-code provided by  
 *  ITead studio.
 * You can find the latest version of the library at 
 * http://www.RinkyDinkElectronics.com/
 * 
 * It is a partial class:
 * TScreen must be defined with your needs.
 * See examples folder
 * 
 */
#ifdef __arm__
#ifndef karaScreen_h
#define karaScreen_h

#if defined(ARDUINO_ARCH_AVR)
    #include <avr/pgmspace.h>
#elif defined(ARDUINO_SAM_DUE)
    #define PROGMEM
#endif

#include <ILI9341_due.h>
#include "fonts\allFonts.h"

// size of the screen
#define WIDTH 320
#define HEIGHT 240
// space between group of buttons
#define PAD 5
// Height of a group of buttons
#define BHEIGHT 68
// Position of buttons panel: POSBTS1: bottom  POSBTS2: middle  POSBTS3: top
#define POSBTS1 HEIGHT-BHEIGHT
#define POSBTS2 HEIGHT-(2*BHEIGHT)
#define POSBTS3 HEIGHT-(3*BHEIGHT)
// height of a status panel
#define SHEIGHT 22
// max size of the screen logo
#define LHEIGHT 32
#define LWIDTH  32
// max nuber of TButtons on TPanels
#define MAXBTS 5
// max number of buttons in a Tbuttons group
#define MAXBT 10
// default font for the status bar
#define TSTFONT Arial14 
// default space between line on TPANEL
#define LSPACE 3
// Nb Max of lines on TPANEL
#define MAXLINE 12 
// Nb max of status in a status bar
#define MAXSTATUS 4

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
  int xt;
  int yt;
  void TrigOn(int x, int y){xt = x;yt = y; trigger = true;}
  void TrigOff(void) {trigger = false;}   
  TEventScreen(){;}
  bool isArmed(){return trigger;}
};
//

class TLogo
{
  public:
  ILI9341_due* Parent;
  uint16_t Width = 0;
  uint16_t Height = 0;
  uint16_t *Image ;
  TLogo( ILI9341_due* parent, uint16_t *image) { Parent = parent, Image = image;}
  TLogo( ILI9341_due* parent, const  uint16_t *image){Parent = parent; Image = (uint16_t*)image; }
  void Background(int color);
  void BackgroundWhite(int color);
  void Draw( int x, int y);
};
//-------------------------------------------
// Base class of  objects to be displayed
class TBase
{
protected:
  ILI9341_due* Parent; 
  bool active = false;
  bool displayed = false;
public:
  virtual void Touch(int xt, int yt) {;}
  virtual void unTouch(int xt, int yt){;}
  virtual void Draw(void) = 0;
  bool isActive(void){ return active;} 
  bool isDisplayed (void){ return displayed;} 
  void Show(void){ active = true;}
  void Hide(void){ active = false;displayed = false;}       
};
//
//-------------------------------------------
// a Button 
class TButton: public TBase
{
public:
  TButton(ILI9341_due* parent,int left, int top, int width, int height);
  int Color = ILI9341_GOLD;
  String Caption;
  TLogo *Logo = NULL;
  TLogo *LogoOn = NULL;
  bool BiStable = false;
  bool State = false; // state for bistate
  void Draw(void);
  void (*Action)(void) = NULL; 
  void Touch(void);
  void unTouch(void) {Draw();}   
  int Left; int Top; int Width; int Height;
};


//-------------------------------------------
//A group of  nb button in a raw  at the bottom by default
// else call setTop
class TButtons: public TBase
{
private:
  int   nb;
  int top ;
  int eachwidth;
  int color = ILI9341_BLUE;
  bool grouped = false;
public:
  TButtons(ILI9341_due* parent,  int nbr);
  TButton* Button[MAXBT];
  int btColor = ILI9341_GOLD;
  String Caption;
  void setTop(int Top);
  int getTop(){return top;}
  void Touch(int xt, int yt);
  void unTouch(int xt, int yt); 
  void Draw(void);
  void Grouped(bool state);  
};
//-------------------------------------------
// Main Panel  of the screen
class TPanel: public TBase
{
private:
  short index = 0; // current index 
  int until = HEIGHT-LHEIGHT;
  int from =  LHEIGHT;  
public:
  TPanel(ILI9341_due* parent ){Parent = parent;Show();} // show is the default
  String Line[MAXLINE+2]; // array of String (text lines)
  int Color = ILI9341_DARKBLUE;
  void clearPanelLines();
  void clearPanel();
  void Println(char* str); // with scrolling
  void Println(String str);// with scrolling
  void Print(char* str); // with scrolling
  void Print(String str);// with scrolling
  void Draw(void);
  void Draw(int from,int until);
  void Until(short unt){until = unt; }
  void From(short fro){from= fro;} 
  void Touch(int xt, int yt){if (Action != 0) Action();} 
  void unTouch(int xt, int yt){;} 
  void (*Action)(void) = NULL; 
};

//-------------------------------------------
// A panel and Buttons container
// 
class TPanels: public TBase
{
public: 
  TPanels(ILI9341_due* parent);
  bool Active = true;
  TButtons* Bts[MAXBTS];
  TPanel* Panel;
  void Touch(int xt, int yt);
  void unTouch(int xt, int yt);
  void Draw();
//  void Refresh(){;} // some displays to update? 
};
//-------------------------------------------
// a status label to be drawn in a status bar
class TStatus
{
  private:
  String label="";  // print as "label: caption" at x left pixel
  String caption="";
  int at;
  int color = ILI9341_LIGHTSTEELBLUE;
 ILI9341_due* Parent; 
public:
  TStatus(ILI9341_due* parent,String lab,String capt, int x)  {Parent = parent; label = lab; caption = capt; at = x; }
  bool State = false;
  void Caption(char* cap){caption=cap;Modified = true;}
  void Label(char* cap){label=cap; Modified = true;}
  void Color(int col){ color = col;}
  void Draw();
  int At(){ return at;}
  void At(int here){ at = here;}
  int Width(); 
  bool Modified = false;
};

//-------------------------------------------
//The status bar at the top of the screen
// contains up to MAXSTATUS status
// show sd status, wifi status, time etc...
// always visible
class TStBar
{
private:
  int color = ILI9341_BLACK;
  int forecolor;
  void reDraw(int index);
  ILI9341_due* Parent;  
public:
  TStBar(ILI9341_due* parent); 
  TStatus *Status[MAXSTATUS];
  // some status to display
  void Draw();
  void Refresh(); // some displays to update?
};



#endif
#else
  #error Oops! Trying to include  on another device?
#endif
