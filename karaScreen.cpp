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
 * Basic functionality of this library are based on the demo-code provided by  
 *  ITead studio.
 * You can find the latest version of the library at 
 * http://www.RinkyDinkElectronics.com/
 * 
 * KaraClass is a partial class:
 * TScreen must be defined with your needs.
 * See examples folder
 * 
 */

#include "karaScreen.h"

//  ----------TLogo------------------------------
void TLogo::Background(int color)
{
  if (Height == 0) Height = Width; // assume square
  for (int i=0;i< Width*Height;i++)
    if (Image[i] == ILI9341_WHITE) Image[i] = color;
}
void TLogo::BackgroundWhite(int color)
{
  if (Height == 0) Height = Width; // assume square
  for (int i=0;i< Width*Height;i++)
    if (Image[i] == color) Image[i] = ILI9341_WHITE;
}
void TLogo::Draw(int x, int y)
{
   if (Height == 0) Height = Width; // assume square
   Parent->drawImage(Image, x, y, Width,Height ); 
}

//  ----------TButton-----------------------------------------------------
TButton::TButton(ILI9341_due* parent,int left, int top, int width, int height)
{
  Parent = parent;
  active = true; // show per default
  Left = left; Top = top; Width = width; Height = height;
}
void TButton::Draw(void)
{
 int color = Color;
 if (!active) return;
 Parent->setFont(Arial_bold_14);
 if (State&&BiStable&&(LogoOn==NULL))
 {
      color = color&0xC618;
      Parent->fillRoundRect(Left,Top+PAD,Width,Height-(2*PAD),7,color);
//      color = ILI9341_RED;
 }
 else Parent->fillRoundRect(Left,Top+PAD,Width,Height-(2*PAD),7,color);

 Parent->drawRoundRect(Left,Top+PAD,Width,Height-(2*PAD),7,0);
 Parent->setTextArea(Left,Top+PAD,Width,Height-(2*PAD));
 Parent->setTextColor(ILI9341_NAVY,Color);
 if (Logo != 0) 
 {
    // replace white with current color
    Logo->Background(color);
    if (LogoOn != NULL)  LogoOn->Background(color);
    if (State&&BiStable)
    {
      if (LogoOn != NULL)
           LogoOn->Draw(Left+((Width - LogoOn->Width)/2) , Top+(2*PAD));
      else Logo->Draw(Left+((Width - Logo->Width)/2) , Top+2*PAD);
    }
    else
      Logo->Draw(Left+((Width - Logo->Width)/2) , Top+3*PAD);
      
    Logo->BackgroundWhite(color);
    if (LogoOn != NULL)  LogoOn->Background(color);

    Parent->printAt(Caption,(Width -(Parent->getCharWidth('_')*(Caption.length())) )/2,Height-30);
 } else
 {
 Parent->printAt(Caption,(Width -(Parent->getCharWidth('_')*(Caption.length())) )/2,Height/3);
 }
}

void TButton::Touch()
{
  int idleColor = Color;
  Color = Color&0xC618;  // clear 343 less bits of the 565 color
  if (BiStable)
  {
      State = !State; // toggle
  }
  Draw();
  Color = idleColor;
  if (Action != NULL) Action();
}

// ------TButtons--------------------------
TButtons::TButtons(ILI9341_due* parent,  int nbr)
{
  int space,bwidth;
  nb = nbr;
  Parent = parent;
  top = HEIGHT - BHEIGHT;
  if (nb>MAXBT) nb=MAXBT;
  space = nb+1;
  bwidth = (WIDTH - (space*PAD))/nb;
  for (int i=0 ; i<nb; i++)  
  {
    Button[i] = new TButton(Parent,PAD+(bwidth*i)+(PAD*i),
                         top,
                         bwidth,
                         BHEIGHT);                   
  }                      
}
void TButtons::Grouped(bool state)
{
  grouped = state;
  for (int i=0 ; i<nb; i++)  
  {
     Button[i]->BiStable = grouped;
  }
}
void TButtons::setTop(int Top)
{
  top = Top;
  for (int i = 0;i<nb;i++)
  { 
    Button[i]->Top = top;  
  }     
}

void TButtons::Draw(void)
{
  if (!active) return;
  displayed = true;
  Parent->fillRect(0,top,WIDTH,BHEIGHT,color) ;
  for (int i = 0;i<nb;i++)
  { 
    Button[i]->Color = btColor;   
    Button[i]->Draw();
  }
}

// a button is touched. Find it and call its touch procedure.
void TButtons::Touch(int xt, int yt)
{
  if (active)
  {
    if (grouped)
    {
      for (int i = 0;i < nb; i++)
      if(Button[i]->State)
      {
        Button[i]->State = false ;
        Button[i]->Draw();
      }  
    }
    for (int i = 0;i < nb; i++)
    {
      if ((xt >= Button[i]->Left) && (xt <= Button[i]->Left+Button[i]->Width))
      {
         Button[i]->Touch();
         break;      
      }
    }    
  }
}

void TButtons::unTouch(int xt, int yt)
{
  if (active)
  {
   for (int i = 0;i < nb; i++)
   if ((xt >= Button[i]->Left) && (xt <= Button[i]->Left+Button[i]->Width))
   {
      Button[i]->unTouch();
      break;      
   }
  }
}

// ------TPanel------------
void TPanel::clearPanelLines()
{
  index = 0;
  for (int i = 0;i<MAXLINE;i++)
  {
    Line[i] = "";
  }
  Draw();
}

void TPanel::Print(String str)
{
  if (index >= MAXLINE) // scroll?
  {
    for (int i =1; i<MAXLINE; i++)
      Line[i-1] = Line[i];
    index--;
    Line[index] ="";   
  }
  Line[index] = Line[index]+ str;
  Draw();  
}
void TPanel::Println(String str)
{
  if (index >= MAXLINE) // scroll?
  {
    for (int i =1; i<MAXLINE; i++)
      Line[i-1] = Line[i];
    index--; 
    Line[index] =""; 
  }
  Line[index] += str;
  index++;
  Draw();  
}
void TPanel::Print(char* str)
{
  Print((String) str);
}
void TPanel::Println(char* str)
{
Println((String) str);
}
void TPanel::clearPanel()
{
  Parent->fillRect(0,LHEIGHT,WIDTH,until,Color) ;
}
void TPanel::Draw(int from,int luntil)
{
  if (!active) return;
  until = luntil;
  int hi = 5;
  Parent->setFont(Arial14);
  Parent->fillRect(0,from,WIDTH,until,Color) ;
  Parent->setTextArea(10, LHEIGHT, WIDTH-20, until);
  Parent->setTextColor(ILI9341_WHITE,Color);
  for (int i = 0;i<MAXLINE;i++)
  {
    //clipping
    if (hi+5 > until) break;
    Parent->printAt(Line[i],10,hi);
    hi += Parent->getFontHeight()+LSPACE;
  }
}

void TPanel::Draw()
{
  if (!active) return;
//  Parent->fillRect(0,LHEIGHT,WIDTH,until,Color) ;
  Draw(from,until);  
}
//  --------TPanels -------------------------
TPanels::TPanels(ILI9341_due* parent)
{
    Parent = parent;
    Show(); // default to show 
    for (int i = 0; i < MAXBTS; i++)
      Bts[i] = NULL;
} 

void TPanels::Touch(int xt, int yt)
{
  int i;
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
void TPanels::unTouch(int xt, int yt)
{
  int i;
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
void TPanels::Draw()
{
  int i;
  int unti= HEIGHT-LHEIGHT;
  for (i = 0; i< MAXBTS; i++)
    if (Bts[i]!= NULL)
    {
      if (Bts[i]->isActive())
      {
        if (!Bts[i]->isDisplayed()) Bts[i]->Draw(); 
        if (unti > Bts[i]->getTop()-LHEIGHT)
          unti = Bts[i]->getTop()-LHEIGHT;
      }
    }
// show panel is a minimum ;-)

  Panel->Draw(LHEIGHT,unti);    
}
//  -------TStatus--------------------------
void TStatus::Draw()
{
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
int TStatus::Width() 
{ 
/*  Serial.println(label+caption);
  Serial.print("nb char :"); Serial.println(label.length()+caption.length());
  Serial.print("Width :"); Serial.println((label.length()+caption.length())*Parent->getCharWidth('_'));
  Serial.print("Width _:"); Serial.println(Parent->getCharWidth('_'));
*/ 
  return ((label.length()+caption.length())*Parent->getCharWidth('_')); 
} 
 
// --------TStBar--------------------------------
TStBar::TStBar(ILI9341_due* parent)
{
    Parent = parent; 
    for (int i = 0; i < MAXSTATUS; i++)
      Status[i] = NULL;
}  
// Draw all status if modified
void TStBar::Refresh()
{
  for (int i = 0; i<MAXSTATUS; i++)
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
  for (int i = 0; i<MAXSTATUS;i++)
  {
    if (Status[i] != NULL) Status[i]->Draw();
  }  
}
// Draw one status child
void TStBar::reDraw(int inde)
{
  Parent->fillRect(Status[inde]->At(),0,(Status[inde]->Width()),SHEIGHT,color) ;
  Parent->setFont(TSTFONT);
  Parent->setTextArea(0, 0, WIDTH-LWIDTH, SHEIGHT); 
  if ((inde <MAXSTATUS)&&(Status[inde] != NULL)) Status[inde]->Draw();
}

