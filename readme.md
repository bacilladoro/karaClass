# karaClass

 *  KaraClass A basic user environment for arduino touch screen 
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
 
 A TButtons panel can be in group mode ie only all TButton are in toggle mode but only one can be On.<br/>
 A TButton can be a simple push button or a toggle button (bistable) with or without an image and with a caption.<br/>
 
 See the example to show these elements in action.<br/>
 The example needs additional libraries:<br/>
 RTClib<br/>
 SdFat<br/>
 SdFatUtil<br/>
 IniFile.h<br/>
 DueTimer cannot live without it <br/>

<img src="https://github.com/karawin/karaClass/blob/master/2016-03-04.10.52.02.jpg" alt="screenshot" border=0> 
<img src="https://github.com/karawin/karaClass/blob/master/IMG_20160305.jpg" alt="screenshot" border=0> 

Video:<br/>
<iframe width="420" height="315" src="https://www.youtube.com/embed/Zyi8rDFdiU4" frameborder="0" allowfullscreen></iframe>

