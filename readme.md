# karaClass

 *  KaraClass A basic GUI graphical user interface environment for arduino touch screen 
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

 KARACLASS_VERSION 1.03
 
 
 karaClass is a skeleton to build structured user screen on ILI9341 touch screen (around 6$ in china) for arduino due.
 Probably it can work on other boards like mega or teensy.<br/>
 
 Defined main classes are:<br/>
 TScreen: the screen itself<br/>
 TPanels: a container of TPanel and TButtons<br/>
 TPanel:  the main part of the screen<br/>
 TButtons: a container of raw of TButtons.  displayed from bottom to top of TPanels<br/>
 TButton: a button<br/>
 TStBar: a panel at the top of the screen. It contains TStatus object.<br/>
 TKeyboard: a complete qwerty keyboard<br/>
 TBigTime: A panel displaying date and time
 TSlider: A panel with a slider.
 Added a screensaver timeout (BigTime after a minute of inactivity)
 
 A TButtons panel can be in group mode ie only all TButton are in toggle mode but only one can be On.<br/>
 A TButton can be a simple push button or a toggle button (bistable) with or without an image and with a caption.<br/>
 
 On many components, a user action may be called for each touch, untouch and slide events.
 
 To add your code, please use only definitions beggining with userXXX
 void loop() {
     myScreen.Task();// mandatory. The engine
}

In your project.ino
////MODIFY///////////////////////////////////////////////
// Called in the TScreen::Task(). Put your  code here
void TScreen::userTask()
{
 ; 
}
// Called every second. Put your code here
void TScreen::userSecond()
{
;
}

In KaraScreenConfig.h:
/////////////////////////////////////////////////////////
// must be modified depending of the screen configuration
// Complete screen configuration, init ans start
void TScreen::userBegin()
{
...
 
 
 See the example to show these elements in action.<br/>
 The example needs additional libraries:<br/>
 RTClib<br/>
 SdFat<br/>
 SdFatUtil<br/>
 IniFile.h<br/>
 DueTimer cannot live without it <br/>
<br/>
<img src="https://github.com/karawin/karaClass/blob/master/IMG_20160305_180548.jpg" alt="screenshot" border=0> 
Panel of buttons. <br/>
<img src="https://github.com/karawin/karaClass/blob/master/IMG_20160305.jpg" alt="screenshot" border=0> 
Keyboard QWERTY<br/>
<img src="https://github.com/karawin/karaClass/blob/master/2016-03-14 19.49.18.jpg" alt="screenshot" border=0> 
Keyboard phone mode<br/>
<img src="https://github.com/karawin/karaClass/blob/master/2016-03-14 19.49.43.jpg" alt="screenshot" border=0> 
BigTime<br/>
<img src="https://github.com/karawin/karaClass/blob/master/2016-03-14 19.50.13.jpg" alt="screenshot" border=0> 
Radio buttons panel<br/>
<img src="https://github.com/karawin/karaClass/blob/master/2016-03-16 17.28.01.jpg" alt="screenshot" border=0> 
Slider<br/>

Video:<br/>
https://www.youtube.com/watch?v=f44uVu8w-bA



