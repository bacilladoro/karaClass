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
 * KaraClass is a partial class:
 * TScreen must be defined with your needs.
 * See examples folder
 * No needs to modify this .cpp
 */

#include "karaScreen.h"
void setTime(void); //action
void setDate(void); //action
extern TScreen  myScreen;
extern  UTouch myTouch;
// Forward declarations
void myTouchInt();
void StatInt();
void TimeInt();

//  ----------TLogo------------------------------
void TLogo::Background(uint16_t color)
{
  if (Height == 0) Height = Width; // assume square
  for (uint16_t i=0;i< Width*Height;i++)
    if (Image[i] == ILI9341_WHITE) Image[i] = color;
}
void TLogo::BackgroundWhite(uint16_t color)
{
  if (Height == 0) Height = Width; // assume square
  for (uint16_t i=0;i< Width*Height;i++)
    if (Image[i] == color) Image[i] = ILI9341_WHITE;
}
void TLogo::Draw(uint16_t x, uint16_t y)
{
   if (Height == 0) Height = Width; // assume square
   Parent->drawImage(Image, x, y, Width,Height ); 
}

//  ----------TButton-----------------------------------------------------
TButton::TButton(ILI9341_due* parent,uint16_t left, uint16_t top, uint16_t width, uint16_t height)
{
  Parent = parent;
  active = true; // show per default
  Left = left; Top = top; Width = width; Height = height;
  Caption = "";
}

void TButton::Draw(void)
{
 dbgprintln("TButton Draw");
 uint16_t color = Color;
 if (!active) return;
 Parent->setFont(Arial_bold_14);
 if (State&&CheckButton&&(LogoOn==NULL))
 {
      color = color&0xC618; // draw background more darker
 }
 Parent->drawRoundRect(Left+1,Top+PAD+2,Width,Height-(2*PAD),7,color&0xA514); //shadow of the button
 Parent->fillRoundRect(Left,Top+PAD,Width,Height-(2*PAD),7,color); // clear space under button
 Parent->drawRoundRect(Left,Top+PAD,Width,Height-(2*PAD),7,0); //Draw the conturn
 Parent->setTextArea(Left,Top+PAD,Width,Height-(2*PAD));
 Parent->setTextColor(ILI9341_NAVY,Color);

 dbgprint("TButton Draw2 Logo= ");
 dbgprint((int)Logo);dbgprint("  LogoOn= ");
 dbgprintln((int)LogoOn);

 if ((Logo != NULL) || (LogoOn != NULL))
 {
    // replace white with current color
    if (Logo != NULL) Logo->Background(color);
    if (LogoOn != NULL)  LogoOn->Background(color); // replace white with color in logoOn too

    if (State&&CheckButton)
    {
      if (LogoOn != NULL)
           LogoOn->Draw(Left+((Width - LogoOn->Width)/2) , Top+(2*PAD));
      else 
	  if (Logo != NULL)
	       Logo->Draw(Left+((Width - Logo->Width)/2) , Top+2*PAD);
    }
    else
	{
      if (Logo != NULL) Logo->Draw(Left+((Width - Logo->Width)/2) , Top+2*PAD);
    }
    if (Logo != NULL)    Logo->BackgroundWhite(color); //restore logo from color to white
    if (LogoOn != NULL)  LogoOn->BackgroundWhite(color);

    Parent->printAt(Caption,(Width -(Parent->getCharWidth('_')*(Caption.length())) )/2,Height-30);
 } else
 if (CheckButton) // a CheckButton without Logo
 {
 // draw the check box
    if (!State)
	{
	   if (Radio)
	     Parent->fillCircle(Left+(Width/2) , Top+(Height/3),Height/7,ILI9341_WHITESMOKE) ;
	   else
	     Parent->fillRect(Left+((Width - (Width/5))/2) , Top+(Height/4),Width/5,Height/5,ILI9341_WHITESMOKE) ;
       Parent->printAt(Caption,(Width -(Parent->getCharWidth('_')*(Caption.length())) )/2,Height-30);
	} else
	{
	   if (Radio)
	   Parent->fillCircle(Left+(Width/2) , Top+(Height/3),Height/7,ILI9341_RED) ;
	   else
	   Parent->fillRect(Left+((Width - (Width/5))/2) , Top+(Height/4),Width/5,Height/5,ILI9341_RED) ;
       Parent->printAt(Caption,(Width -(Parent->getCharWidth('_')*(Caption.length())) )/2,Height-30);
	}
	if (Radio)
	  Parent->drawCircle(Left+(Width/2) , Top+(Height/3) ,Height/7,ILI9341_BLACK);
    else
	  Parent->drawRect(Left+((Width - (Width/5))/2) , Top+(Height/4) ,Width/5,Height/5,ILI9341_BLACK);

 } else  // nothing special. draw text
 {
 Parent->printAt(Caption,(Width -(Parent->getCharWidth('_')*(Caption.length())) )/2,9*Height/32);
 }
}

void TButton::Touch()
{  dbgprintln("TButton Touch");
  if (!active) return;
  uint16_t idleColor = Color;
  Color = Color&0xA514;  // clear 343 less bits of the 565 color
  if (CheckButton)
  {
      State = !State; // toggle
  }
  Draw();
  Color = idleColor;
  if (onTouch != NULL) onTouch();
}
void TButton::unTouch(void)
{
  dbgprintln("TButton unTouch");
  Draw();
  if (onUnTouch != NULL) onUnTouch();
}
   
//-------TBigTime---------------------------
TBigTime::TBigTime(ILI9341_due* parent): Options(parent,2)
{
  dbgprintln("TBigTime");
  top = LHEIGHT;
  Parent = parent;
  Options.Button[0]->Caption = "Set Date";
  Options.Button[1]->Caption = "Set Time";
  Options.Button[1]->onUnTouch = setTime;
  Options.Button[0]->onUnTouch = setDate;
}

void TBigTime::Draw()
{
//  dbgprintln("TBigTime draw");
  char  ci[12] ;
  if (!active) return;
  if (!isDisplayed())
  {
    Parent->fillRect(0,top,WIDTH,HEIGHT-LHEIGHT,color) ;
    Parent->drawRect(0,top,WIDTH,HEIGHT-LHEIGHT,ILI9341_WHITE) ;
	displayed = true;
//	return;
  }
  DateTime dt0(rtc.now());

  Parent->setTextColor(ILI9341_WHITE);
  Parent->setFont(fixednums15x31);
  Parent->setTextArea(0, 0, WIDTH, HEIGHT);
  sprintf(ci,"%02d:%02d:%02d", dt0.hour(),dt0.minute(),dt0.second());
  Parent->fillRect  (WIDTH/2 -((fixednums15x31[2]*strlen(ci))/2 ),HEIGHT/3,fixednums15x31[2]*(strlen(ci)+1),fixednums15x31[3],color) ;
  Parent->printAt(ci,WIDTH/2 -((fixednums15x31[2]*strlen(ci))/2 ),HEIGHT/3);

  Parent->setFont(Arial_bold_14);
  sprintf(ci,"%02d/%02d/%04d", dt0.day(),dt0.month(),dt0.year()); 
  Parent->fillRect  (WIDTH/2 -((Arial_bold_14[2]*strlen(ci))/2 ),(HEIGHT/3) +50,Arial_bold_14[2]*(strlen(ci)+1),Arial_bold_14[3],color) ;
  Parent->printAt(ci,WIDTH/2 -((Arial_bold_14[2]*strlen(ci))/2 ),(HEIGHT/3) +50);
  Parent->setTextColor(ILI9341_GRAY);
  if (!Options.isActive()) Parent->printAt("Set: touch here",4*WIDTH/8 ,(HEIGHT-30));
}
void TBigTime::Touch(uint16_t xt, uint16_t yt)
{
  dbgprintln("TBigTime Touch");
  if ((Options.isActive()) && (yt >= Options.getTop()) && (yt <= Options.getTop()+BHEIGHT))
  {
	  Options.Touch(xt,yt);
	  return;
  }

// else do nothing  
}
void TBigTime::unTouch(uint16_t xt, uint16_t yt)
{
  dbgprintln("TBigTime unTouch");
  if ((Options.isActive()) && (yt >= Options.getTop()) && (yt <= Options.getTop()+BHEIGHT))
  {
	  Options.unTouch(xt,yt);
	  return;
  }
  if ((Options.isActive())&&(yt >=LHEIGHT ) && (yt <= HEIGHT-BHEIGHT))
  {
    Options.Hide();
    unDisplay();
    Draw();
	return;
   }
  if ((yt >= HEIGHT-BHEIGHT) && (yt <= HEIGHT))
  {
	  Options.Show();
	  Options.Draw();
	  return;
  }
  else Hide();
}
// ------TPKeyboard-------------------------
//

TKeyboard::TKeyboard(ILI9341_due* parent,uint8_t  nrky )
{
  dbgprintln("TKeyboard");
  uint16_t bwidth;
  uint16_t bheight = KHEIGHT/NRKBOARD;
  uint16_t pad = 2;
  Parent = parent;
  top = HEIGHT - KHEIGHT;
  topa = top -KCHEIGHT; // for the caption
  nrkey = nrky;
  bwidth = (WIDTH - ((nrkey+1)*pad))/nrkey;
  for (uint16_t j=0 ; j<NRKBOARD; j++)
  {
    for (uint16_t i=0 ; i<nrkey; i++)
    {
      Key[i][j] = new TButton(Parent,
                         pad+(bwidth*i)+(pad*i),
                         top+(j*bheight),
                         bwidth,
                         bheight );
      Key[i][j]->Color = btColor;           
    }
  }
   SetBase();
}
void TKeyboard::Start(String banner, uint8_t set= KBMAJ)
{
  dbgprintln("TKeyboard Start");
  state = false;
  Caption = "";
  Banner = banner;
  switch(set){
    case KBMAJ: Set1();break;
    case KBNUM: Set2();break;
    case KBMIN: Set3();break;
    case KBPHONE: Set4();break;
    default:break;
  }
}

void TKeyboard::SetMn()
{
  if (Key[0][0]->Caption == "Q") 
    Set3();
    else Set1();
}

void TKeyboard::SetBase()
{
  for (uint16_t i=0 ; i<nrkey; i++)
    Key[i][NRKBOARD-1]->Caption = " ";  
  if (nrkey == NRKEY)
  {
	  Key[0][NRKBOARD-1]->Caption = "Mn"; // Majuscule/ Minuscule
	  Key[0][NRKBOARD-1]->Color &= 0xB618; //
	  Key[1][NRKBOARD-1]->Caption = "01"; // Numbers
	  Key[1][NRKBOARD-1]->Color &= 0xB618; //

  } else
  {
      Key[0][NRKBOARD-1]->Hide();
  }
  Key[nrkey-3][NRKBOARD-1]->Caption = "XX"; // Cancel
  Key[nrkey-1][NRKBOARD-1]->Caption = "OK"; // enter
  Key[nrkey-2][NRKBOARD-1]->Caption = "<-"; // erase
  Key[nrkey-1][NRKBOARD-1]->Color &=0xB618; //
  Key[nrkey-2][NRKBOARD-1]->Color &=0xB618; //
  Key[nrkey-3][NRKBOARD-1]->Color &=0xB618;
}

void TKeyboard::Set1()
{
    char table[] = "QWERTYUIOPASDFGHJKL;ZXCVBNM,./";  
//  char table[] = "AZERTYUIOPQSDFGHJKLMWXCVBN?./!";  
  for (uint16_t j=0 ; j<NRKBOARD-1; j++)
    for (uint16_t i=0 ; i<nrkey; i++)
      Key[i][j]->Caption = table[i+(nrkey*j)]; 
}
void TKeyboard::Set2()
{
  char table[] = "1234567890@#$%&-+()*\"':;!?{}/\\"; 
  for (uint16_t j=0 ; j<NRKBOARD-1; j++)
    for (uint16_t i=0 ; i<nrkey; i++)
      Key[i][j]->Caption = table[i+(nrkey*j)];
}
void TKeyboard::Set3()
{
    char table[] = "qwertyuiopasdfghjkl:zxcvbnm<>.";  
//  char table[] = "azertyuiopqsdfghjklmwxcvbn,;:!";  
  for (uint16_t j=0 ; j<NRKBOARD-1; j++)
    for (uint16_t i=0 ; i<nrkey; i++)
      Key[i][j]->Caption = table[i+(nrkey*j)];
}
void TKeyboard::Set4()
{
    char table[] = "1234567890:/";
    for (uint16_t j=0 ; j<NRKBOARD-1; j++)
    for (uint16_t i=0 ; i<nrkey; i++)
    Key[i][j]->Caption = table[i+(nrkey*j)];
}

void TKeyboard::setTop(uint16_t Top)
{
  top = Top;
  topa = top - -KCHEIGHT; // for the caption
  for (uint16_t j=0 ; j<NRKBOARD-1; j++)  
    for (uint16_t i=0 ; i<nrkey; i++)  
      Key[i][j]->Top = top; 
}

void TKeyboard::Draw(void)
{  dbgprintln("Keyboard draw");
  if (!active) return;
  displayed = true;
  Parent->fillRect(0,topa,WIDTH,KHEIGHT+KCHEIGHT,color) ;
  Parent->drawRect(0,topa,WIDTH,KHEIGHT+KCHEIGHT,ILI9341_WHITE) ;
  Parent->setFont(Arial_bold_14);
  Parent->setTextColor(ILI9341_WHITE); 
  Parent->setTextArea(KCHEIGHT,topa+2,WIDTH,KCHEIGHT/2); //Banner
  Parent->printAt(Banner,1, 4); 
  Parent->setTextArea(KCHEIGHT,topa+(KCHEIGHT/2)+3,WIDTH-2*KCHEIGHT,(KCHEIGHT/2)-6); //Caption
  Parent->fillRect(KCHEIGHT,topa+(KCHEIGHT/2)+3,WIDTH-2*KCHEIGHT,(KCHEIGHT/2)-6,ILI9341_WHITE) ; 
  Parent->setTextColor(ILI9341_BLACK); 
  Parent->printAt(Caption,4, 4); 
  Parent->setTextColor(ILI9341_NAVY);  
  for (uint16_t j=0 ; j<NRKBOARD; j++)  
    for (uint16_t i=0 ; i<nrkey; i++)  
      Key[i][j]->Draw();
}

// a button is touched. Find it and call its touch procedure.
void TKeyboard::Touch(uint16_t xt, uint16_t yt)
{
  if (!active) return;
  for (uint16_t j=0 ; j<NRKBOARD; j++)  
  {
    for (uint16_t i=0 ; i<nrkey; i++)  
    {
      if ((yt >= Key[i][j]->Top) && (yt <= Key[i][j]->Top + Key[i][j]->Height))
        if ((xt >= Key[i][j]->Left) && (xt <= Key[i][j]->Left + Key[i][j]->Width))
        {
		  Key[i][j]->Touch();
		  break;
        }
    }  
  }
 
}

void TKeyboard::unTouch(uint16_t xt, uint16_t yt)
{
  if (!active) return;
  for (uint16_t j=0 ; j<NRKBOARD; j++)  
    for (uint16_t i=0 ; i<nrkey; i++)  
    {
      if ((yt >= Key[i][j]->Top) && (yt <= Key[i][j]->Top + Key[i][j]->Height))
        if ((xt >= Key[i][j]->Left) && (xt <= Key[i][j]->Left + Key[i][j]->Width))
        {
           Key[i][j]->unTouch();
           String caption = Key[i][j]->Caption;
           if (Key[i][j]->Caption == "Mn"){ SetMn();Draw();}
           else if (caption == "01"){ Set2();Draw();}
           else if (caption == "<-"){ Caption.remove(Caption.length()-1);}
           else if (caption == "OK"){ state = true;}
           else if (caption == "XX"){ Caption ="";state = true;}
           else Caption += caption;
           break;
        }    
    }
// redraw caption
  Parent->setTextArea(KCHEIGHT,topa+(KCHEIGHT/2)+1,WIDTH-KCHEIGHT,KCHEIGHT/2); //Caption
  Parent->fillRect(KCHEIGHT,topa+(KCHEIGHT/2)+1,WIDTH-2*KCHEIGHT,KCHEIGHT/2,ILI9341_WHITE) ;
  Parent->setFont(Arial_bold_14);
  Parent->setTextColor(ILI9341_BLACK);
  Parent->printAt(Caption,4, 4);
}

// ------TButtons--------------------------
  TButtons::TButtons(ILI9341_due* parent,  uint16_t nbr)
  {
	  uint16_t space,bwidth;
	  nb = nbr;
	  Parent = parent;
	  top = HEIGHT - BHEIGHT;
	  if (nb>MAXBT) nb=MAXBT;
	  space = nb+1;
	  bwidth = (WIDTH - (space*WPAD))/nb;
	  for (uint16_t i=0 ; i<nb; i++)
	  Button[i] = new TButton(Parent,WPAD+(bwidth*i)+(WPAD*i),
	  top,
	  bwidth,
	  BHEIGHT);
	  for (uint16_t i=nb ; i<MAXBT; i++)
	  Button[i] = NULL;
  } // nbr = number of buttons in the panel. MAXBT maxi

void TButtons::RadioBox(bool state)
{
  grouped = state;
  for (uint16_t i=0 ; i<nb; i++)  
  {
     if (Button[i] != NULL) 
	 {
	   Button[i]->CheckButton = grouped;
       Button[i]->Radio = true;
     }
  }
  if (state) Button[0]->State = true;
}
void TButtons::setTop(uint16_t Top)
{
  top = Top;
  for (uint16_t i = 0;i<nb;i++)
  { 
    if (Button[i] != NULL) Button[i]->Top = top;  
  }     
}

void TButtons::Draw(void)
{  dbgprint("Buttons draw top Height ");dbgprint(top); dbgprint(" ");dbgprintln(BHEIGHT);
  if (!active) return;
  displayed = true;
  Parent->fillRect(0,top,WIDTH,BHEIGHT,color) ;
  for (uint16_t i = 0;i<nb;i++)
  { 
    if (Button[i] != NULL)
    {
      Button[i]->Color = btColor;   
      Button[i]->Draw();
    }
  }
}

// a button is touched. Find it and call its touch procedure.
void TButtons::Touch(uint16_t xt, uint16_t yt)
{
  if (!active) return;
  for (uint16_t i = 0;i < nb; i++)
  {
    if (Button[i] != NULL)
    {
      if ((xt >= Button[i]->Left) && (xt <= Button[i]->Left+Button[i]->Width))
      {
	    if (Button[i]->Radio)  // clear all other radio button on this panel
        for (uint16_t j = 0;j < nb; j++)
          if (Button[j] != NULL)
          {
	         if((Button[j]->State)&&(Button[j]->Radio))
	         {
	     	    Button[j]->State = false ;
		        Button[j]->Draw();
	         }
          }
         Button[i]->Touch();
         break;      
      }
    }
  }    
}

void TButtons::unTouch(uint16_t xt, uint16_t yt)
{
  if (active&&displayed)
  {
   for (uint16_t i = 0;i < nb; i++)
     if (Button[i] != NULL)
     {
       if ((xt >= Button[i]->Left) && (xt <= Button[i]->Left+Button[i]->Width))
       {
          Button[i]->unTouch();
          break;      
       }
     }
  }
}

// ------TSlider-------------
void TSlider::Draw(void)
{
  dbgprint("TSlide draw top Height ");dbgprint(top); dbgprint(" ");dbgprintln(BHEIGHT);
  if (!active) return;
  displayed = true;
  Parent->fillRect(0,top,WIDTH,BHEIGHT,color) ;
  Parent->drawRect(0,top,WIDTH,BHEIGHT,ILI9341_WHITE) ;
  Parent->fillRoundRect((3*WIDTH/20)-2,top+(4*BHEIGHT/10),(14*WIDTH/20)+6,(5*BHEIGHT/10),4*BHEIGHT/20,btColor) ;
  Parent->drawRoundRect((3*WIDTH/20)-2,top+(4*BHEIGHT/10),(14*WIDTH/20)+6,(5*BHEIGHT/10),4*BHEIGHT/20,ILI9341_BLACK) ;
  Parent->setFont(Arial_bold_14);
  Parent->setTextColor(ILI9341_WHITE);
  Parent->setTextArea(3*WIDTH/20- (4*ARIAL_BOLD_14_WIDTH),top+(5*BHEIGHT/10)+PAD,WIDTH-PAD,ARIAL_BOLD_14_HEIGHT); //min max
  Parent->printAt((String)mini,0,0);
  Parent->printAt((String)maxi,(14*WIDTH/20)+(5*ARIAL_BOLD_14_WIDTH) , 0);
  Parent->setTextArea(3*WIDTH/20,top+(3*BHEIGHT/20),14*WIDTH/20,ARIAL_BOLD_14_HEIGHT); //Caption
  Parent->printAt(Caption,(Caption.length()*ARIAL_BOLD_14_WIDTH)/3,  0);

  Update(value);
}
void TSlider::Update(int val)
{
	dbgprint("TSlide clear top Height ");dbgprint(top); dbgprint(" ");dbgprintln(BHEIGHT);
	if (!active) return;
	if (!displayed) Draw();
	else
	{
	  String sval(val );
	  step = (swidth *(val-mini))/ (maxi-mini);
	  Parent->fillCircle((3*WIDTH/20)+(4*BHEIGHT/20) + step ,top+(4*BHEIGHT/10)+(5*BHEIGHT/20),rad+2 , btColor);
      Parent->fillRect((3*WIDTH/20)+(4*BHEIGHT/20) + step  - (ARIAL_BOLD_14_WIDTH* sval.length())/2 + ARIAL_BOLD_14_WIDTH/4 ,
	     top+(4*BHEIGHT/10)+(5*BHEIGHT/20)-ARIAL_BOLD_14_HEIGHT/2 ,2*rad ,ARIAL_BOLD_14_HEIGHT,btColor);
	  sval =value ;
      Parent->setFont(Arial_bold_14);
      Parent->setTextColor(ILI9341_WHITE);
	  step = (swidth *(value-mini))/ (maxi-mini);
	  Parent->fillCircle((3*WIDTH/20)+(4*BHEIGHT/20) + step ,top+(4*BHEIGHT/10)+(5*BHEIGHT/20),rad+2 ,0xA514);
	  Parent->fillCircle((3*WIDTH/20)+(4*BHEIGHT/20) + step ,top+(4*BHEIGHT/10)+(5*BHEIGHT/20),rad ,  ILI9341_BLACK);
      Parent->setTextArea((3*WIDTH/20)+(4*BHEIGHT/20) + step  - (ARIAL_BOLD_14_WIDTH* sval.length())/2 + ARIAL_BOLD_14_WIDTH/4 ,
	     top+(4*BHEIGHT/10)+(5*BHEIGHT/20)-ARIAL_BOLD_14_HEIGHT/2 ,2*rad ,ARIAL_BOLD_14_HEIGHT);
      Parent->print(sval);
	}
}
/*  int rad = 4*BHEIGHT/20;
  int swidth = (14*WIDTH/20) - (8*BHEIGHT/20);
  int step = (swidth *(value+mini))/ (maxi-mini);*/

void TSlider::Slide(uint16_t xt, uint16_t yt)
{
  dbgprint("TSlide Slide  XT= ");dbgprintln(xt);
  xt -= (3*WIDTH/20)+(4*BHEIGHT/20); //relative to slider now
  if ((int16_t)xt<-1) xt = 0;
  dbgprint("TSlide Slide  XT= ");dbgprintln(xt);
  int val = value;
  value = (xt*(maxi-mini)+ (swidth*mini))/swidth;
/*  dbgprint("TSlide Slide  value= ");dbgprintln(value);
  dbgprint("TSlide Slide  swidth= ");dbgprintln(swidth);
  dbgprint("TSlide Slide  maxi-mini= ");dbgprintln(maxi-mini);*/
  (value <= mini)? value = mini:value;
  (value >= maxi)? value = maxi:value;
  Update(val);
}
// ------TPanel------------
void TPanel::clearPanelLines()
{
  dbgprintln("TPanel clearPanelLines");
  index = 0;
  for (uint16_t i = 0;i<MAXLINE;i++)
  {
    Line[i] = "";
  }
  Draw();
}


void TPanel::Print(String str)
{
  dbgprintln("TPanel Print");
  if (index >= MAXLINE) // scroll?
  {
    for (uint16_t i =1; i<MAXLINE; i++)
      Line[i-1] = Line[i];
    index--;
    Line[index] ="";   
  }
  Line[index] = Line[index]+ str;
//  Draw();  
}
void TPanel::Println(String str)
{
  dbgprintln("TPanel Println");
  if (index >= MAXLINE) // scroll?
  {
    for (uint16_t i =1; i<MAXLINE; i++)
      Line[i-1] = Line[i];
    index--; 
    Line[index] =""; 
  }
  Line[index] += str;
  index++;
  Draw();  
}
void TPanel::Print(char* str){ Print((String) str);}

void TPanel::Println(char* str){ Println((String) str);}

void TPanel::clearPanel()
{
  Parent->fillRect(0,LHEIGHT,WIDTH,until,Color) ;
}
void TPanel::Draw(uint16_t from,uint16_t luntil)
{  
  dbgprint("TPanel draw from until "); dbgprint (from);  dbgprint ("  "); dbgprintln(luntil);
  if (!active) return;
  until = luntil;
  uint16_t hi =Parent->getFontHeight();
  Parent->setFont(myArial14);
  Parent->fillRect(0,from,WIDTH,until-LHEIGHT,Color) ;
  Parent->setTextArea(10, LHEIGHT, WIDTH-20, until-LHEIGHT);
  Parent->setTextColor(ILI9341_WHITE,Color);
  for (uint16_t i = 0;i<MAXLINE;i++)
  {
    //clipping
    if (hi > until-LHEIGHT-Parent->getFontHeight()) break;
    Parent->printAt(Line[i],10,hi);
    hi += Parent->getFontHeight()+LSPACE;
  }
}

void TPanel::Draw()
{  dbgprintln("TPanel draw");
  if (!active) return;
//  Parent->fillRect(0,LHEIGHT,WIDTH,until,Color) ;
  Draw(from,until);  
}

//  --------TPanels -------------------------
TPanels::TPanels(ILI9341_due* parent)
{
    Parent = parent;
    Show(); // default to show 
    for (uint16_t i = 0; i < MAXBTS; i++)
      Bts[i] = NULL;
} 
void TPanels::StartBigTime()
{
dbgprintln("startBigTime");
  if (BigTime==NULL) 
  {
    BigTime = new TBigTime(Parent);
    BigTime->Show();
//  Draw(); 
  } 
}
void TPanels::StopBigTime()
{
dbgprintln("stopBigTime");
  if (BigTime!=NULL)
  { 
    BigTime->Hide();
	delete BigTime ;
	BigTime = NULL;
  }
  Draw();
}
void TPanels::StartKeyboard(String banner, uint8_t set= KBMAJ)
{  
  dbgprintln("TPanels  startKeyboard");
// create Keyboard
  if (set != KBPHONE)
    Keyboard = new TKeyboard(Parent);
  else 
    Keyboard = new TKeyboard(Parent,4);
// invalidate other child
/*  for (uint16_t i = 0; i< MAXBTS; i++)
    if (Bts[i]!= NULL)
      Bts[i]->Hide();*/
  Keyboard->Start(banner,set);
  Keyboard->Show();
  Keyboard->Draw();
}

String TPanels::GetKeyboard()
{   
  dbgprintln("TPanels GetKeyboard");
String Caption = Keyboard->Caption;
   delete  Keyboard;
   Keyboard = NULL;
  Draw();
  return Caption;
}

void TPanels::Touch(uint16_t xt, uint16_t yt)
{
  uint16_t i;
  dbgprint("TPanels Touch yt= ");dbgprintln(yt);

  if (BigTime!= NULL)
    if ((BigTime->isActive()) && (yt >= LHEIGHT) && (yt <= HEIGHT)) //modal: the all screen
    {
	  BigTime->Touch(xt,yt);
	  return;
    }

  if (Keyboard!= NULL)
      if ((Keyboard->isActive()) && (yt >= LHEIGHT) && (yt <= HEIGHT)) //modal 
      { 
        Keyboard->Touch(xt,yt);
        return;
      }  
  if (Slider!= NULL)
    if ((Slider->isActive()) && (yt >= Slider->getTop()) && (yt <= Slider->getTop()+BHEIGHT))
    {
      Slider->Touch(xt,yt);
      return;
    }

  for (i = 0; i< MAXBTS; i++)
    if (Bts[i]!= NULL)
      if ((Bts[i]->isActive()) && (yt >= Bts[i]->getTop()) && (yt <= Bts[i]->getTop()+BHEIGHT)) 
      { 
        Bts[i]->Touch(xt,yt);
        return;
      }  
  if ((Panel->isActive()))
    Panel->Touch(xt,yt);     
}

void TPanels::unTouch(uint16_t xt, uint16_t yt)
{
  uint16_t i;
  dbgprintln("TPanels unTouch");
  if (BigTime!= NULL)
  if ((BigTime->isActive()) && (yt >= LHEIGHT) && (yt <= HEIGHT))
  {
	  BigTime->unTouch(xt,yt);
	  if (!BigTime->isActive() ) StopBigTime();
	  return;
  }

  if (Keyboard!= NULL)
      if ((Keyboard->isActive()) && (yt >= LHEIGHT)  && (yt <= HEIGHT))
      { 
        Keyboard->unTouch(xt,yt);
        return;
      }  
	  
  if (Slider!= NULL)
  if ((Slider->isActive()) && (yt >= Slider->getTop()) && (yt <= Slider->getTop()+BHEIGHT))
  {
	  Slider->unTouch(xt,yt);
	  return;
  }
	    
  for (i = 0; i< MAXBTS; i++)
    if (Bts[i]!= NULL)
      if ((Bts[i]->isActive()) && (yt >= Bts[i]->getTop()) && (yt <= Bts[i]->getTop()+BHEIGHT)) 
      { 
        Bts[i]->unTouch(xt,yt);
        return;
      }  
  if ((Panel->isActive()))
    Panel->unTouch(xt,yt);     
}

void TPanels::Slide(uint16_t xt, uint16_t yt)
{
  uint16_t i;
    if (Slider!= NULL)
    if ((Slider->isActive()) && (yt >= Slider->getTop()) && (yt <= Slider->getTop()+BHEIGHT))
    {
	    Slider->Slide(xt,yt);
	    return;
    }

  for (i = 0; i< MAXBTS; i++)
  if (Bts[i]!= NULL)
  if ((Bts[i]->isActive()) && (yt >= Bts[i]->getTop()) && (yt <= Bts[i]->getTop()+BHEIGHT))
  {
	  Bts[i]->Slide(xt,yt);
	  return;
  }
}

void TPanels::Draw()
{ 
  dbgprint(PSTR("TPanels Draw unti= "));dbgprintln(HEIGHT);
  uint16_t i;
  uint16_t unti= HEIGHT;
  if (!active) return;

  if (BigTime != NULL)
  {
//	  if (BigTime->isActive())
	  {
		  if (!BigTime->isDisplayed()) BigTime->Draw();
		  return;
	  }
  }
  for (i = 0; i< MAXBTS; i++)
    if (Bts[i]!= NULL)
    {  
      dbgprint(PSTR("Bts "));dbgprint(i); dbgprint(PSTR("Active ")); dbgprintln(Bts[i]->isActive()?"1 ":"0 ");
      if (Bts[i]->isActive())
      {
        if (!Bts[i]->isDisplayed()){ Bts[i]->Draw(); }
		else { Bts[i]->reDraw();}
        if (unti > Bts[i]->getTop())
          unti = Bts[i]->getTop();
		  dbgprint(PSTR("TPanels Draw unti= "));dbgprintln(unti);
      }
    }
  if (Keyboard != NULL)
  {
     if (Keyboard->isActive())
      {
        if (!Keyboard->isDisplayed()) Keyboard->Draw(); 
        if (unti > Keyboard->getTop())
          unti =Keyboard->getTop();
      }       
  }
    if (Slider!= NULL)
    {
	    if (Slider->isActive())
	    {
		    if (!Slider->isDisplayed()){ Slider->Draw(); }
		    else { Slider->reDraw();}
		    if (unti > Slider->getTop())
		    unti = Slider->getTop();
	    }
    }
  Panel->Draw(LHEIGHT,unti);    
}
//  -------TStatus--------------------------
void TStatus::Draw()
{
//  dbgprintln("TStatus Draw");
  Modified = false;
  Parent->setTextColor(color);
  Parent->printAt(label,at,2); 
  if (label =="") //only a caption with color following state
  {
    if (State)
       Parent->setTextColor(ILI9341_FORESTGREEN) ; 
    else Parent->setTextColor(ILI9341_RED);  
  }
  Parent->print(caption);
}
uint16_t TStatus::Width() 
{ 
/*  dbgprintln(label+caption);
  dbgprint("nb char :"); dbgprintln(label.length()+caption.length());
  dbgprint("Width :"); dbgprintln((label.length()+caption.length())*Parent->getCharWidth('_'));
  dbgprint("Width _:"); dbgprintln(Parent->getCharWidth('_'));
*/
  return ((label.length()+caption.length())*Parent->getCharWidth('_')); 
} 
 
// --------TStBar--------------------------------
TStBar::TStBar(ILI9341_due* parent)
{
    Parent = parent; 
    for (uint16_t i = 0; i < MAXSTATUS; i++)
      Status[i] = NULL;
}  
// Draw all status if modified
void TStBar::Refresh()
{
  for (uint16_t i = 0; i<MAXSTATUS; i++)
  if (Status[i] != NULL)
    if (Status[i]->Modified)
      reDraw(i);
}
// Draw all 
void TStBar::Draw()
{
  Parent->fillRect(0,0,WIDTH-32,SHEIGHT,color) ; 
  Parent->setFont(TSTFONT);
  Parent->setTextArea(0, 0, WIDTH-LWIDTH, SHEIGHT);
  for (uint16_t i = 0; i<MAXSTATUS;i++)
  {
    if (Status[i] != NULL) Status[i]->Draw();
  }  
}
// Draw one status child
void TStBar::reDraw(uint16_t inde)
{
  Parent->fillRect(Status[inde]->At(),0,(Status[inde]->Width()),SHEIGHT,color) ;
  Parent->setFont(TSTFONT);
  Parent->setTextArea(0, 0, WIDTH-LWIDTH, SHEIGHT); 
  if ((inde <MAXSTATUS)&&(Status[inde] != NULL)) Status[inde]->Draw();
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
	screensaver = 0; // activity so reset screensaver
	Panels->Touch(xt,yt);
}
void TScreen::unTouch(uint16_t xt, uint16_t yt)
{
	// unarm the trigger
	EunTouch.TrigOff();
	screensaver = 0; // activity so reset screensaver
	Panels->unTouch(xt,yt);
}
void TScreen::Slide(uint16_t xt, uint16_t yt)
{
	// unarm the trigger
	ESlide.TrigOff();
	screensaver = 0; // activity so reset screensaver
	Panels->Slide(xt,yt);
}
// called every second. Add your own processing
void TScreen::doSecond(DateTime dtime)
{
	ESecond.TrigOff();
	if (Panels->BigTime != NULL)
	{
		if (Panels->BigTime->isActive())
		Panels->BigTime->Draw();
	}
	
	//  dbgprint("screensaver: ");dbgprintln(screensaver);
	if (screensaver == TSCREENSAVER)
	{
		screensaver++;
		dbgprintln("screensaver go");
		Panels->StartBigTime();
	} else
	if (screensaver != TSCREENSAVER+1) screensaver++;
	userSecond(); // call the user part
}

// now defined in karaScreenConfig.h
void TScreen::doStatus()
{
	//  dbgprintln("doStatus");
	EStatus.TrigOff();
    userStatus();
}

// Minimal init in order to display the welcome message
// not mandatory
void TScreen::Welcome()
{
	ILI9341_due::begin();  // for Print
	// some default
	welcome = true;
	setRotation(iliRotation270);  // landscape
	fillScreen(ILI9341_DARKBLUE);
	setFontMode(gTextFontModeTransparent);
	setFont(Arial_bold_14);
	setTextColor(ILI9341_WHITE, ILI9341_NAVY);
}

void TScreen::Begin()
{
	// Initialize the touch screen
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);
	if (!welcome) Welcome();
	// do not modify after
	//Activate and display screen
	userBegin();
	// starts the interrupts
	DueTimer::getAvailable().attachInterrupt(myTouchInt).start(TIMER1);  // for touch / untouch
	DueTimer::getAvailable().attachInterrupt(StatInt).start(TIMER2);     // for status bar computing
	DueTimer::getAvailable().attachInterrupt(TimeInt).start(TIMER3);;    // for user task and time task
	Show();
	Draw();
}
// The screen motor
void TScreen::Task()
{
	if (ETouch.isArmed())  // touch trigged in interrupt. Process it here in the main loop
	  Touch(ETouch.xt,ETouch.yt);
	if (EunTouch.isArmed())
	  unTouch(EunTouch.xt,EunTouch.yt);
	if (ESlide.isArmed())
	  Slide(ESlide.xt,ESlide.yt);
	if (ESecond.isArmed())
	  doSecond(DateTime (rtc.now()));;
	if (EStatus.isArmed())
	  doStatus();
	
	
	// call the user processing
	userTask();
}

// allocate a keyboard and wait for the returned String if any.
// Non blocking for the user point of view.
String TScreen::Keyboard(String banner, uint8_t set  )
{
	Panels->StartKeyboard(banner,set);
	while (!Panels->isKeyboard()) Task();
	return Panels->GetKeyboard();
}

///////////////////////////////////////////////////////
static uint16_t xt,yt,ixt,iyt;

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
		ixt = xt; iyt = yt;
		myScreen.ETouch.TrigOn(xt,yt);
	}
	if ((inTouch)&&(myTouch.dataAvailable()))
	{
		myTouch.read();
		ixt = myTouch.getX();
		iyt = myTouch.getY();
		if ((ixt != xt)||(iyt != yt))
		myScreen.ESlide.TrigOn(ixt,iyt);
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
	myScreen.EStatus.TrigOn();
}
void TimeInt()
{
	char  ci[10] ;
	DateTime dt0(rtc.now());
	myScreen.ESecond.TrigOn();
	sprintf(ci,"%02d:%02d:%02d", dt0.hour(),dt0.minute(),dt0.second());
	myScreen.StatusBar->Status[3]->Caption(ci);
}

void setTime(void)
{ 
  DateTime dt0(rtc.now());
  
  int hour;
  int min;
  int sec =0;
  int res;
  myScreen.Panels->BigTime->Hide();
  String newtime = myScreen.Keyboard( PSTR("Enter HH:MM[:SS]"),KBPHONE); 
  myScreen.Panels->BigTime->Show();
/*  dbgprint(PSTR("Current time : Year: "));dbgprint(dt0.year());
  dbgprint(PSTR(" Month: "));dbgprint(dt0.month());
  dbgprint(PSTR(" Day: "));dbgprint(dt0.day());
  dbgprint(PSTR(" Hour: "));dbgprint(dt0.hour());
  dbgprint(PSTR(" Minute: "));dbgprint(dt0.minute());
  dbgprint(PSTR(" Second: "));dbgprintln(dt0.second());*/
  res = sscanf(newtime.c_str(),"%2u:%2u:%2u",&hour,&min,&sec) ;
  if ((hour < 24)&&(min < 60)&&(sec<60))
  {
    if ((res == 3) || (res == 2))
    {
	  dbgprintln(PSTR("time 3: ")+newtime);
      rtc.adjust(DateTime(dt0.year(),dt0.month(),dt0.day(),hour,min,sec));
    }
  }
}

void setDate(void)
{
	DateTime dt0(rtc.now());
	int year;
	int month;
	int day;
	int res;
	myScreen.Panels->BigTime->Hide();
	String newtime = myScreen.Keyboard( PSTR("Enter DD/MM[/YY]"),KBPHONE);
	myScreen.Panels->BigTime->Show();
/*  dbgprint("Current time : Year: ");dbgprint(dt0.year());
  dbgprint(" Month: ");dbgprint(dt0.month());
  dbgprint(" Day: ");dbgprint(dt0.day());
  dbgprint(" Hour: ");dbgprint(dt0.hour());
  dbgprint(" Minute: ");dbgprint(dt0.minute());
  dbgprint(" Second: ");dbgprintln(dt0.second());*/
  res = sscanf(newtime.c_str(),"%2u/%2u/%2u",&day,&month,&year);
	if ((month < 13)&&(day<32))
	{
		if (res ==3)
		{
		    year +=2000;
			dbgprint(PSTR("date 3: ")+newtime);dbgprint(PSTR("   Year "));dbgprint(year);dbgprint(PSTR("   Month "));dbgprint(month);dbgprint(PSTR("   Day "));dbgprintln(day);
			rtc.adjust(DateTime(year,month,day,dt0.hour(),dt0.minute(),dt0.second()));
		}
		else if (res == 2)
		{
			dbgprint(PSTR("date 4: ")+newtime);dbgprint(PSTR("   Year "));dbgprint(dt0.year());dbgprint(PSTR("   Month "));dbgprint(month);dbgprint(PSTR("   Day "));dbgprintln(day);
			rtc.adjust(DateTime(dt0.year(),month,day,dt0.hour(),dt0.minute(),dt0.second()));
		}
	}
}