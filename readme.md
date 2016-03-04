# karaClass

 * Classes for  DUE project 
 * Copyright: Jean-Pierre Cocatrix jp@cocatrix.fr http://www.karawin.fr
 * https://github.com/karawin/karClass
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
 Probably it can work on other boards like mega or teensy.
 
 Defined main classes are:
 TScreen: the screen itself
 TPanels: a container of TPanel and TButtons
 TPanel:  the main part of the screen
 TButtons: a container of raw of TButtons.  displayed from bottom to top of TPanels
 TButton: a button
 TStBar: a panel at the top of the screen. It contains TStatus object.
 
 A TButtons panel can be in group mode ie only all TButton are in toggle mode but only one can be On.
 A TButton can be a simple push button or a toggle button (bistable) with or without an image and with a caption.
 
 See the example to show these elements in action.
 The example needs additional libraries:
 RTClib
 SdFat
 SdFatUtil
 IniFile.h
 DueTimer cannot live without it 

