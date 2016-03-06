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
}

void TButton::Draw(void)
{
 uint16_t color = Color;
 if (!active) return;
 Parent->setFont(Arial_bold_14);
 if (State&&BiStable&&(LogoOn==NULL))
 {
//      color = color&0x7BEF;
      color = color&0xC618;
 }
 Parent->drawRoundRect(Left+1,Top+PAD+2,Width,Height-(2*PAD),7,color&0xA514);
 Parent->fillRoundRect(Left,Top+PAD,Width,Height-(2*PAD),7,color);
 
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
 Parent->printAt(Caption,(Width -(Parent->getCharWidth('_')*(Caption.length())) )/2,9*Height/32);
 }
}

void TButton::Touch()
{  dbgprintln("Buttons touch");
  if (!active) return;
  uint16_t idleColor = Color;
  Color = Color&0xA514;  // clear 343 less bits of the 565 color
  if (BiStable)
  {
      State = !State; // toggle
  }
  Draw();
  Color = idleColor;
  if (Action != NULL) Action();
}

// ------TPKeyboard-------------------------
//
TKeyboard::TKeyboard(ILI9341_due* parent)
{
 uint16_t bwidth;
  uint16_t bheight = KHEIGHT/NRKBOARD;
  uint16_t pad = 2;
  Parent = parent;
  top = HEIGHT - KHEIGHT;
  topa = top -KCHEIGHT; // for the caption
  bwidth = (WIDTH - ((NRKEY+1)*pad))/NRKEY;
  for (uint16_t j=0 ; j<NRKBOARD; j++)
  {
    for (uint16_t i=0 ; i<NRKEY; i++)
    {
      Key[i][j] = new TButton(Parent,
                         pad+(bwidth*i)+(pad*i),
                         top+(j*bheight),
                         bwidth,
                         bheight );
      Key[i][j]->Color = btColor;           
    }
  }
   Set1();
}
void TKeyboard::Start(String banner, uint8_t set= KBMAJ)
{
  state = false;
  Caption = "";
  Banner = banner;
  switch(set){
    case KBMAJ: Set1();break;
    case KBMIN: Set3();break;
    case KBNUM: Set2();break;
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
  char white[] = "          ";
  for (uint16_t i=0 ; i<NRKEY; i++)
    Key[i][NRKBOARD-1]->Caption = white[i];  
  Key[0][NRKBOARD-1]->Caption = "Mn"; // Majuscule/ Minuscule
  Key[0][NRKBOARD-1]->Color &= 0xB618; // 
  Key[1][NRKBOARD-1]->Caption = "01"; // Numbers
  Key[1][NRKBOARD-1]->Color &= 0xB618; // 
  Key[NRKEY-3][NRKBOARD-1]->Caption = "XX"; // Cancel
  Key[NRKEY-1][NRKBOARD-1]->Caption = "OK"; // enter
  Key[NRKEY-2][NRKBOARD-1]->Caption = "<<"; // erase
  Key[NRKEY-1][NRKBOARD-1]->Color &=0xB618; // 
  Key[NRKEY-2][NRKBOARD-1]->Color &=0xB618; // 
  Key[NRKEY-3][NRKBOARD-1]->Color &=0xB618;
}

void TKeyboard::Set1()
{
    char table[] = "QWERTYUIOPASDFGHJKL;ZXCVBNM,./";  
//  char table[] = "AZERTYUIOPQSDFGHJKLMWXCVBN?./!";  
  for (uint16_t j=0 ; j<NRKBOARD; j++)
    for (uint16_t i=0 ; i<NRKEY; i++)
      Key[i][j]->Caption = table[i+(NRKEY*j)]; 
  SetBase();
}
void TKeyboard::Set2()
{
  char table[] = "1234567890@#$%&-+()*\"':;!?{}/\\"; 
  for (uint16_t j=0 ; j<NRKBOARD-1; j++)
    for (uint16_t i=0 ; i<NRKEY; i++)
      Key[i][j]->Caption = table[i+(NRKEY*j)];
  SetBase(); 
}
void TKeyboard::Set3()
{
    char table[] = "qwertyuiopasdfghjkl:zxcvbnm<>.";  
//  char table[] = "azertyuiopqsdfghjklmwxcvbn,;:!";  
  for (uint16_t j=0 ; j<NRKBOARD-1; j++)
    for (uint16_t i=0 ; i<NRKEY; i++)
      Key[i][j]->Caption = table[i+(NRKEY*j)];
  SetBase();
}
void TKeyboard::setTop(uint16_t Top)
{
  top = Top;
  topa = top - -KCHEIGHT; // for the caption
  for (uint16_t j=0 ; j<NRKBOARD-1; j++)  
    for (uint16_t i=0 ; i<NRKEY; i++)  
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
  Parent->setTextArea(KCHEIGHT,topa+1,WIDTH,KCHEIGHT/2); //Banner
  Parent->printAt(Banner,1, 4); 
  Parent->setTextArea(KCHEIGHT,topa+(KCHEIGHT/2)+1,WIDTH-2*KCHEIGHT,KCHEIGHT/2); //Caption
  Parent->fillRect(KCHEIGHT,topa+(KCHEIGHT/2)+1,WIDTH-2*KCHEIGHT,KCHEIGHT/2,ILI9341_WHITE) ; 
  Parent->setTextColor(ILI9341_BLACK); 
  Parent->printAt(Caption,4, 4); 
  Parent->setTextColor(ILI9341_NAVY);  
  for (uint16_t j=0 ; j<NRKBOARD; j++)  
    for (uint16_t i=0 ; i<NRKEY; i++)  
      Key[i][j]->Draw();
}

// a button is touched. Find it and call its touch procedure.
void TKeyboard::Touch(uint16_t xt, uint16_t yt)
{
  if (!active) return;
  for (uint16_t j=0 ; j<NRKBOARD; j++)  
  {
    for (uint16_t i=0 ; i<NRKEY; i++)  
    {
      if ((yt >= Key[i][j]->Top) && (yt <= Key[i][j]->Top + Key[i][j]->Height))
        if ((xt >= Key[i][j]->Left) && (xt <= Key[i][j]->Left + Key[i][j]->Width))
        {
           Key[i][j]->Touch();
           String caption = Key[i][j]->Caption;
           if (Key[i][j]->Caption == "Mn"){ SetMn();Draw();}
           else if (caption == "01"){ Set2();Draw();}
           else if (caption == "<<"){ Caption.remove(Caption.length()-1);}
           else if (caption == "OK"){ state = true;}
           else if (caption == "XX"){ Caption ="";state = true;}
           else Caption += caption;
           break;      
        }
    }  
  }
// redraw caption
  Parent->setTextArea(KCHEIGHT,topa+(KCHEIGHT/2)+1,WIDTH-KCHEIGHT,KCHEIGHT/2); //Caption
  Parent->fillRect(KCHEIGHT,topa+(KCHEIGHT/2)+1,WIDTH-2*KCHEIGHT,KCHEIGHT/2,ILI9341_WHITE) ; 
  Parent->setFont(Arial_bold_14);
  Parent->setTextColor(ILI9341_BLACK); 
  Parent->printAt(Caption,4, 4);   
}

void TKeyboard::unTouch(uint16_t xt, uint16_t yt)
{
  if (!active) return;
  for (uint16_t j=0 ; j<NRKBOARD; j++)  
    for (uint16_t i=0 ; i<NRKEY; i++)  
    {
      if ((yt >= Key[i][j]->Top) && (yt <= Key[i][j]->Top + Key[i][j]->Height))
      if ((xt >= Key[i][j]->Left) && (xt <= Key[i][j]->Left + Key[i][j]->Width))
      {
         Key[i][j]->unTouch();
         break;      
      }    
    }
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
  bwidth = (WIDTH - (space*PAD))/nb;
  for (uint16_t i=0 ; i<nb; i++)  
    Button[i] = new TButton(Parent,PAD+(bwidth*i)+(PAD*i),
                         top,
                         bwidth,
                         BHEIGHT);                   
  for (uint16_t i=nb ; i<MAXBT; i++)  
    Button[i] = NULL;                   
}
void TButtons::Grouped(bool state)
{
  grouped = state;
  for (uint16_t i=0 ; i<nb; i++)  
  {
     if (Button[i] != NULL) Button[i]->BiStable = grouped;
  }
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
{  dbgprintln("Buttons draw");
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
  if (grouped)
  {
    for (uint16_t i = 0;i < nb; i++)
      if (Button[i] != NULL)
      {
        if(Button[i]->State)
        {
          Button[i]->State = false ;
          Button[i]->Draw();
        }  
      }  
  }
  for (uint16_t i = 0;i < nb; i++)
  {
    if (Button[i] != NULL)
    {
      if ((xt >= Button[i]->Left) && (xt <= Button[i]->Left+Button[i]->Width))
      {
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

// ------TPanel------------
void TPanel::clearPanelLines()
{
  index = 0;
  for (uint16_t i = 0;i<MAXLINE;i++)
  {
    Line[i] = "";
  }
  Draw();
}

void TPanel::Print(String str)
{
  if (index >= MAXLINE) // scroll?
  {
    for (uint16_t i =1; i<MAXLINE; i++)
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
    for (uint16_t i =1; i<MAXLINE; i++)
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
void TPanel::Draw(uint16_t from,uint16_t luntil)
{  dbgprintln("panel draw until");
  if (!active) return;
  until = luntil;
  uint16_t hi = 5;
  Parent->setFont(Arial14);
  Parent->fillRect(0,from,WIDTH,until,Color) ;
  Parent->setTextArea(10, LHEIGHT, WIDTH-20, until);
  Parent->setTextColor(ILI9341_WHITE,Color);
  for (uint16_t i = 0;i<MAXLINE;i++)
  {
    //clipping
    if (hi+5 > until) break;
    Parent->printAt(Line[i],10,hi);
    hi += Parent->getFontHeight()+LSPACE;
  }
}

void TPanel::Draw()
{  dbgprintln("panel draw");
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

void TPanels::StartKeyboard(String banner, uint8_t set= KBMAJ)
{  dbgprintln("panels  startKeyboard");
// create Keyboard
  Keyboard = new TKeyboard(Parent);
// invalidate other child
  for (uint16_t i = 0; i< MAXBTS; i++)
    if (Bts[i]!= NULL)
      Bts[i]->Hide();
    Panel->Hide();  
  Keyboard->Start(banner,set);
  Keyboard->Show();
  Keyboard->Draw();
}

String TPanels::GetKeyboard()
{   dbgprintln("panels GetKeyboard");
String Caption = Keyboard->Caption;
// delete keyboard
   delete  Keyboard;
   Keyboard = NULL;
  Panel->Show();
  Draw();
  return Caption;
}

void TPanels::Touch(uint16_t xt, uint16_t yt)
{
  uint16_t i;

  if (Keyboard!= NULL)
      if ((Keyboard->isActive()) && (yt >= Keyboard->getTop()+KCHEIGHT) && (yt <= Keyboard->getTop()+KCHEIGHT+KHEIGHT)) 
      { 
        Keyboard->Touch(xt,yt);
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
  if (Keyboard!= NULL)
      if ((Keyboard->isActive()) && (yt >= Keyboard->getTop()+KCHEIGHT) && (yt <= Keyboard->getTop()+KCHEIGHT+KHEIGHT)) 
      { 
        Keyboard->unTouch(xt,yt);
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
void TPanels::Draw()
{ 
  dbgprintln("panels show");
  uint16_t i;
  uint16_t unti= HEIGHT-LHEIGHT;
  if (!active) return;
  for (i = 0; i< MAXBTS; i++)
    if (Bts[i]!= NULL)
    {  
      dbgprint("Bts ");dbgprintln(i); dbgprint("Active "); dbgprintln(Bts[i]->isActive()?"1 ":"0 ");
      if (Bts[i]->isActive())
      {
        if (!Bts[i]->isDisplayed()) Bts[i]->Draw(); 
        if (unti > Bts[i]->getTop()-LHEIGHT)
          unti = Bts[i]->getTop()-LHEIGHT;
      }
    }
  if (Keyboard != NULL)
  {
     if (Keyboard->isActive())
      {
        if (!Keyboard->isDisplayed()) Keyboard->Draw(); 
        if (unti > Keyboard->getTop()-LHEIGHT)
          unti =Keyboard->getTop()-LHEIGHT;
      }       
  }

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

