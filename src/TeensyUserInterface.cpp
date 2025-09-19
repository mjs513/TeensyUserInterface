//      ******************************************************************
//      *                                                                *
//      *                      Teensy User Interface                     *
//      *                                                                *
//      *            Stan Reifel                      5/20/2022          *
//      *               Copyright (c) S. Reifel & Co, 2022               *
//      *                                                                *
//      ******************************************************************


// MIT License
// 
// Copyright (c) 2022 Stanley Reifel & Co.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//
// The TeensyUserInterface library uses two other libraries which must be installed:
//    ILI9341_t3          - Driver for the LCD screen
//    XPT2046_Touchscreen - Driver for the touch screen
//

//
// This user interface works with Teensy applications having a 2.8" 320x240 ILI9341 
// LCD touch screen display.  It has only been tested with a Teensy 3.6 but will
// likely work with other Teensys that support the libraries listed above.  The display 
// is divided into two sections:  along the top is a Title Bar and below that is the 
// "Display Space".  The Display Space is where menus, message boxes, configuration 
// screens, along with the application's main display are shown.
//
// The heart of this user interface are menus.  Menus are display as rows and columns of
// touch sensitive buttons.  There are three types of menu buttons: Commands, Toggles 
// and Sub Menus.  Menus are displayed by creating a "Menu Table", then calling: 
// displayAndExecuteMenu()
//
// A typical main menu table might look like this:
//
//  MENU_ITEM mainMenu[] = {
//    {MENU_ITEM_TYPE_MAIN_MENU_HEADER, "My Application Menu",  MENU_COLUMNS_1,       mainMenu},
//    {MENU_ITEM_TYPE_COMMAND,          "Turn on motor",        turnOnMotorCommand,   NULL},
//    {MENU_ITEM_TYPE_COMMAND,          "Turn off motor",       turnOffMotorCommand,  NULL},
//    {MENU_ITEM_TYPE_TOGGLE,           "LED",                  enableLEDCallback,    NULL},
//    {MENU_ITEM_TYPE_SUB_MENU,         "Settings",             NULL,                 settingsMenu},
//    {MENU_ITEM_TYPE_END_OF_MENU,      "",                     NULL,                 NULL}
//  };
//
// The first line in the table always defines what type of menu it is, either a Main 
// Menu, or a Sub Menu.  The table's last line marks the end of the table.  In between 
// are menu items, one line for each button that's displayed on the menu.
//
// A menu can have as many buttons as you like, simply by adding more entries to the menu 
// table.  Buttons are sized such that they always fill the screen, adding more causes
// them to be shorter in height.  In many cases it is advantageous to arrange a menu's 
// buttons in two or more columns.  Columns of buttons often look better, are easier to  
// touch, and you can fit more on a single screen.
//
// The buttons on a menu can be arranged in 1, 2, 3 or 4 columns.  The number of columns 
// is set in the third field of the menu table's first line by inserting one of these 
// values:
//           MENU_COLUMNS_1, MENU_COLUMNS_2, MENU_COLUMNS_3, or MENU_COLUMNS_4

//
// There are 3 different types of buttons that can be added to a menu table:
//
// Command:  A MENU_ITEM_TYPE_COMMAND indicates that a function will be executed  
// when this menu button is pressed.  In the second field is the text displayed
// on the button.  A pointer to the function is entered into the third field of
// the table.
//
// Toggle:  A MENU_ITEM_TYPE_TOGGLE is used somewhat like a Radio Button in a dialog 
// box.  It allows the user to select one of a fixed number of choices (such as  
// On / Off,   or   Red / Green / Blue).  Each time the user presses this button, 
// it alternates the selection (i.e. toggles between On and Off, or rotates between 
// Red, Green and Blue).  The third field in this entry points to a callback 
// function that alternates the value.
//
// Sub Menu:  A MENU_ITEM_TYPE_SUB_MENU is used to select a different menu.  For 
// example, a main menu might reference a "Settings" sub menu. The fourth field  
// in this entry points to another menu table where the sub menu is defined.
//
// As described above, the first line in a menu table indicates the type of menu.  
// There are 2 types:
//
// Main Menu:  A MENU_ITEM_TYPE_MAIN_MENU_HEADER in the first line of the table  
// specifies that it is a main menu (a main menu may have child menus, but no 
// parent menus).  The three other columns in this line define more about the menu.  
// The second field contains text printed on the LCD's Title Bar when the menu is
// displayed.  The next column sets how the buttons are arranged on the screen.  
// Legal values for this field are: MENU_COLUMNS_1, MENU_COLUMNS_2, MENU_COLUMNS_3,  
// or MENU_COLUMNS_4.  The fourth field controls if a "Back" button is displayed on 
// the Title Bar for this menu.  Assigning the fourth field "mainMenu" (or what 
// every you named this menu table) will NOT show the "Back" button.  Alternately 
// if the fourth field is set to NULL, the "Back" button is included.  In this 
// case pressing "Back" will exit all menus, returning to the main application.
//
// Sub Menu:  A MENU_ITEM_TYPE_SUB_MENU_HEADER in the first line specifies that the 
// table defines a sub menu.  Sub menus are menus that are called from a main menu, 
// or another sub menu.  The fourth field in this line is a pointer back to the 
// parent menu table (typically this would be the main menu).  This is used to 
// reselect the parent menu when the user presses the "Back" button while the sub 
// menu is displayed.
//
// Typically applications have one or more menus, each having several buttons.  Each
// of these buttons invokes a separate "Command" that is written by the application's 
// developer.  These command may directly do something or display something; or they 
// might prompt the user for more information.
//
// To help the developer create screens that prompt the user for more information,
// this user interface include three touch sensitive widgets that can be easily  
// added to custom screens.  They are:
//
// Buttons:  Buttons are often used to exit screens with labels such as "OK" or   
// "Cancel", but can also be used to execute other commands such as "Start" or "Stop".
//
// Number Boxes:  Number boxes allow the user to enter a number by pressing Up and 
// Down.  Numbers can be integers or floats->
//
// Selection Boxes:  Selection boxes present choices to the user to pick between, 
// such as On / Off; or Low / Medium / High
//
// In addition to these widgets, there are many primitive functions for drawing to 
// the LCD display.  They include functions for drawing text, lines, circles, 
// rectangles...  For drawing text there are many font sizes to choose from.  The  
// Arial fonts that look best are: 8, 8Bold, 9, 9Bold, 12Bold, 13
//
//
// Hooking up the Teensy 3.6 and 4.1 to the LCD Touchscreen:
//    Teensy VIN        LCD VCC
//    Teensy GND        LCD GND
//    Teensy D10        LCD CS
//    Teensy 3.3V       LCD RESET
//    Teensy D9         LCD DC
//    Teensy D11        LCD SDI
//    Teensy D13        LCD CLK
//    Teensy 3.3V       LCD LED BACKLIGHT
//    Teensy D12        LCD SDO
//    Teensy D13        TOUCH CLK
//    Teensy D8         TOUCH CS 
//    Teensy D11        TOUCH DIN
//    Teensy D12        TOUCH D0
//

// ---------------------------------------------------------------------------------

#include <EEPROM.h>

#if __has_include("ILI9341_t3.h")
#include <ILI9341_t3.h>
#elif __has_include("ST7796_t3.h")
#include <ST7796_t3.h>
#define ILI9341_t3 ST7796_t3
#elif __has_include("ILI9488_t3.h")
#define ILI9341_t3 ILI9488_t3
#endif

#if __has_include("XPT2046_Touchscreen.h")
#include <XPT2046_Touchscreen.h>
#define TOUCH_MAP
#elif __has_include("Adafruit_FT6206.h")
#include <Adafruit_FT6206.h>
#define XPT2046_Touchscreen Adafruit_FT6206
#define TOUCH_PIXEL
#endif
#include "TeensyUserInterface.h"


//
// pointers to the LCD and Touch objects
//
ILI9341_t3 *lcd;
XPT2046_Touchscreen *ts;

//
// the size of features for drawing the user interface
//
const int titleBarHeight = 34;
const int arrowWidth = 8;


// ---------------------------------------------------------------------------------
//                       Setup functions for the User Interface
// ---------------------------------------------------------------------------------

//
// constructor for the TeensyUserInterface class
//
TeensyUserInterface::TeensyUserInterface(void)
{
}



//
// initialize the UI, display hardware and touchscreen hardware
//  Enter:  lcdCSPin = pin number for the LCD's CS pin
//          LcdDCPin = pin number for the LCD's DC pin
//          TouchScreenCSPin = pin number for the touchscreen's CS pin
//          lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//          font -> the font typeface to load, ei: Arial_10
//
void TeensyUserInterface::begin(int lcdCSPin, int LcdDCPin, int TouchScreenCSPin, int lcdOrientation, const ui_font &font)
{
  //
  // create the LCD and touchscreen objects
  //
#if __has_include("ST7796_t3.h")
  lcd = new ST7796_t3(lcdCSPin, LcdDCPin);
#else
  lcd = new ILI9341_t3(lcdCSPin, LcdDCPin);
#endif
#if __has_include("Adafruit_FT6206.h")
  ts = new Adafruit_FT6206();
#elif __has_include("XPT2046_Touchscreen.h")
  ts = new XPT2046_Touchscreen(TouchScreenCSPin);
#endif
  
  //
  // initialize the LCD and touch screen hardware
  //
  lcdInitialize(lcdOrientation, font);
  touchScreenInitialize(lcdOrientation);

  //
  // set the orientation for the screen
  //
  setOrientation(lcdOrientation);
  
  //
  // set some default colors and fonts for the UI
  //
  setColorPaletteBlue();
  setTitleBarFont(font);
  setMenuFont(font);

  //
  // disable the callback function executed while in a menu
  //
  inMenuCallbackFunction = NULL;
}



//
// set the orientation of the lcd and touch screen, this can be called to change 
// the orientation after it is initially set
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//
void TeensyUserInterface::setOrientation(int lcdOrientation)
{
  //
  // set the orientation of the hardware
  //
  lcdSetOrientation(lcdOrientation);
  touchScreenSetOrientation(lcdOrientation);

  //
  // set the orientation used by the software
  //
  displaySpaceWidth = lcdWidth - 2;
  displaySpaceHeight = lcdHeight - titleBarHeight - 1;
  displaySpaceLeftX = 1;
  displaySpaceRightX = displaySpaceLeftX + displaySpaceWidth - 1;
  displaySpaceTopY = titleBarHeight;
  displaySpaceBottomY = displaySpaceTopY + displaySpaceHeight - 1;
  displaySpaceCenterX = displaySpaceLeftX + displaySpaceWidth/2;
  displaySpaceCenterY = displaySpaceTopY + displaySpaceHeight/2;
}



//
// set color palette to Blue
//
void TeensyUserInterface::setColorPaletteBlue(void)
{
  setTitleBarColors(LCD_BLUE, LCD_WHITE, LCD_DARKBLUE, 0x8C5F);
  setMenuColors(LCD_BLACK, LCD_BLUE, 0x8C5F, LCD_LIGHTBLUE, LCD_WHITE);
}



//
// set color palette to Gray
//
void TeensyUserInterface::setColorPaletteGray(void)
{
  uint16_t red;
  uint16_t green;
  uint16_t blue;

  red = 11; green = 22; blue = 11-5;  uint16_t titleBarColor                   = lcdMakeColor(red, green, blue);
  red = 9; green = 18; blue = 9-4;    uint16_t titleBarBackButtonColor         = lcdMakeColor(red, green, blue);
  red = 16; green = 32; blue = 16-5;  uint16_t titleBarBackButtonSelectedColor = lcdMakeColor(red, green, blue);
                                      uint16_t titleBarTextColor               = LCD_WHITE;
                                      uint16_t menuBackgroundColor             = LCD_BLACK;
  red = 9; green = 18; blue = 9-4;    uint16_t menuButtonColor                 = lcdMakeColor(red, green, blue);
                                      uint16_t menuButtonSelectedColor         = titleBarBackButtonSelectedColor;
  red = 12; green = 24; blue = 12-4;  uint16_t menuButtonFrameColor            = lcdMakeColor(red, green, blue);
                                      uint16_t menuButtonTextColor             = LCD_WHITE;
                                      
  setTitleBarColors(titleBarColor, titleBarTextColor, titleBarBackButtonColor, titleBarBackButtonSelectedColor);
  setMenuColors(menuBackgroundColor, menuButtonColor, menuButtonSelectedColor, menuButtonFrameColor, menuButtonTextColor);
}


// ---------------------------------------------------------------------------------
//                                   Menu functions  
// ---------------------------------------------------------------------------------

//
// set the menu colors
//  Enter:  _menuBackgroundColor = RGB565 color for background of the menu
//          _menuButtonColor = RGB565 color for menu buttons
//          _menuButtonSelectedColor = RGB565 color for menu buttons that are selected
//          _menuButtonFrameColor = RGB565 color for highlight around the menu buttons
//          _menuButtonTextColor = RGB565 color for text of the buttons
//
void TeensyUserInterface::setMenuColors(uint16_t _menuBackgroundColor, uint16_t _menuButtonColor,
  uint16_t _menuButtonSelectedColor, uint16_t _menuButtonFrameColor, uint16_t _menuButtonTextColor)
{
    menuBackgroundColor = _menuBackgroundColor;
    menuButtonColor = _menuButtonColor;
    menuButtonSelectedColor = _menuButtonSelectedColor;
    menuButtonFrameColor = _menuButtonFrameColor;
    menuButtonTextColor = _menuButtonTextColor;
}



//
// set the menu font
//  Enter:  font -> the font to use when drawing the title bar
//          ie: Arial_8, Arial_8_Bold, Arial_9, Arial_9_Bold, Arial_10, Arial_10_Bold, Arial_11, Arial_11_Bold,
//              Arial_12, 13, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60, 72, 96
//
void TeensyUserInterface::setMenuFont(const ui_font &font)
{
  menuButtonFont = &font;
}



//
// display the top level menu, then execute the commands selected by the user
//  Enter:  menu -> the menu to display
//
void TeensyUserInterface::displayAndExecuteMenu(MENU_ITEM *menu)
{
  int menuIdx;
  MENU_ITEM *parentMenu;
  
  //
  // display the top level menu
  //
  selectAndDrawMenu(menu, true);
 
  //
  // check for screen touches and execute menu commands
  //
  while(true)
  {
    //
    // check if there is a new Touch Event
    //
    getTouchEvents();
    if (touchEventType != TOUCH_NO_EVENT)
    {
      //
      // check if user has pressed the menu's "Back" button on the title bar
      //
      if (checkForBackButtonClicked())
      {
        //
        // the menu's Back button pushed, get this menu's type
        //
        int menuItemType = currentMenuTable[0].MenuItemType;
  
        //
        // if this is a sub menu, select this menu's parent menu
        //
        if (menuItemType == MENU_ITEM_TYPE_SUB_MENU_HEADER)
        {
          parentMenu = currentMenuTable[0].MenuItemSubMenu;
          selectAndDrawMenu(parentMenu, true);
          continue;
        }
  
        //
        // if this is the Main menu, exit the menus and return to the app
        //
        if (menuItemType == MENU_ITEM_TYPE_MAIN_MENU_HEADER)
        {
          parentMenu = currentMenuTable[0].MenuItemSubMenu;
          if (parentMenu == NULL)
            return;
          continue;
        }
      }
  
      //
      // check if user has pressed one of the menu's buttons
      //
      menuIdx = findMenuButtonForTouchEvent();
      if (menuIdx > 0)
      {
        if (touchEventType == TOUCH_PUSHED_EVENT)
          drawMenuItem(menuIdx, true);
  
        if (touchEventType == TOUCH_RELEASED_EVENT)
        {
          drawMenuItem(menuIdx, false);
          executeMenuItem(menuIdx);
        }
      }
    }
    
    //
    // check if there is a callback function to execute while in a menu
    //
    if (inMenuCallbackFunction != NULL)
    {
      //
      // there is a call back function, so execute it
      //
      (inMenuCallbackFunction)();
    }
  }
}



//
// set a callback function that's periodically executed while the application 
// is showing a menu, for most applications setting a callback function is not needed
//  Enter:  callbackFunction -> function to execute continuously while a menu is presented,
//            set to NULL to disable
//
void TeensyUserInterface::setInMenuCallbackFunction(void (*callbackFunction)())
{
  inMenuCallbackFunction = callbackFunction;
}



//
// execute the given menu item
//  Enter:  menuIdx = index into the menu table of the menu item to execute
//
void TeensyUserInterface::executeMenuItem(int menuIdx)
{
  MENU_ITEM *subMenu;

  //
  // determine the type of menu entry, then execute it
  //
  switch(currentMenuTable[menuIdx].MenuItemType)
  {    
    //
    // select a "Sub menu" from the current menu
    //
    case MENU_ITEM_TYPE_SUB_MENU:
    {
      subMenu = currentMenuTable[menuIdx].MenuItemSubMenu;
      selectAndDrawMenu(subMenu, true);
      break;
    }
    
    //
    // execute the menu item's function
    //
    case MENU_ITEM_TYPE_COMMAND:
    {
      //
      // execute the menu item's function
      //
      (currentMenuTable[menuIdx].MenuItemFunction)();
      
      //
      // display the menu again
      //
      selectAndDrawMenu(currentMenuTable, true);
      break;
    }
    
    //
    // toggle the menu item, then redisplay
    //
    case MENU_ITEM_TYPE_TOGGLE:
    {
      toggleSelectNextStateFlg = true;
      (currentMenuTable[menuIdx].MenuItemFunction)();
      drawMenuItem(menuIdx, false);
    }
  }
}



//
// select and display a menu or submenu, for most applications this function is not used
//  Enter:  menu -> the menu to display
//          drawMenuFlg = true if should draw the new menu
//
void TeensyUserInterface::selectAndDrawMenu(MENU_ITEM *menu, boolean drawMenuFlg)
{   
  //
  // remember the currently selected menu
  //
  currentMenuTable = menu;

  //
  // check if drawing the menu, if not return
  //
  if (!drawMenuFlg)
    return;

  //
  // draw the title bar, decide if should include the "Back" button (don't show the 
  // Back button if this is the main menu and it links back to itself)
  //
  if ((currentMenuTable[0].MenuItemType == MENU_ITEM_TYPE_MAIN_MENU_HEADER) &&
    (currentMenuTable[0].MenuItemSubMenu != NULL))
    drawTitleBar(currentMenuTable[0].MenuItemText);
  else
    drawTitleBarWithBackButton(currentMenuTable[0].MenuItemText);
   
  //
  // clear the display space
  //
  clearDisplaySpace();

  //
  // draw all buttons in the menu
  //
  drawMenu();
}



//
// display the currently selected menu
//  Enter:  currentMenuTable -> the menu to display
//
void TeensyUserInterface::drawMenu(void)
{ 
  int menuIdx = 1;
 
  //
  // loop through the menu, drawing each button
  //
  while(true)
  {
    if(currentMenuTable[menuIdx].MenuItemType == MENU_ITEM_TYPE_END_OF_MENU)
      break;

    drawMenuItem(menuIdx, false);
    menuIdx++;
  }
}



//
// draw one button on the menu
//  Enter:  menuIdx = the index into the menu table of the button to draw
//          buttonSelectedFlg = true if should show the button selected
//
void TeensyUserInterface::drawMenuItem(int menuIdx, boolean buttonSelectedFlg)
{
  int buttonX;
  int buttonY;
  int buttonWidth;
  int buttonHeight;
 
  //
  // determine the coordinates and size of the button, then draw it
  //
  getMenuButtonSizeAndLocation(menuIdx, &buttonX, &buttonY, &buttonWidth, &buttonHeight);

  //
  // get the button text
  //
  const char *menuItemText = currentMenuTable[menuIdx].MenuItemText;
    
  //
  // determine the menu entry type and draw it
  //
  switch(currentMenuTable[menuIdx].MenuItemType)
  {    
    //
    // display a "sub menu" button
    //    
    case MENU_ITEM_TYPE_SUB_MENU:
    {
      //
      // draw the button
      //
      drawButton(menuItemText, buttonSelectedFlg, buttonX, buttonY, buttonWidth, buttonHeight);

      //
      // draw a triangle showing the button connects to a submenu
      //
      int arrowX = buttonX + buttonWidth - 18;
      int arrowCenterY = buttonY + buttonHeight / 2;
      lcdDrawFilledTriangle(arrowX,               arrowCenterY - arrowWidth/2,
                            arrowX + arrowWidth,  arrowCenterY,
                            arrowX,               arrowCenterY + arrowWidth/2,
                            menuButtonTextColor);
      break;
    }

    //
    // display a "command" button
    //
    case MENU_ITEM_TYPE_COMMAND:
    {
      drawButton(menuItemText, buttonSelectedFlg, buttonX, buttonY, buttonWidth, buttonHeight);
      break;
    }

    //
    // display a "toggle" button
    //    
    case MENU_ITEM_TYPE_TOGGLE:
    {
      //
      // execute the callback fuction to get the toggle button's text
      //
      toggleSelectNextStateFlg = false;
      (currentMenuTable[menuIdx].MenuItemFunction)();


      //
      // build the text to display on the button
      //
      char s[100];
      strcpy(s, menuItemText);
      strcat(s, ":  ");
      strcat(s, toggleText);
      drawButton(s, buttonSelectedFlg, buttonX, buttonY, buttonWidth, buttonHeight);
    }
  }
}



//
// find a menu button given LCD coords
//  Enter:  touchEventX, touchEventY = screen coordinates where touch event occurred
//  Exit:   index of menu entry returned if found, else 0
//
int TeensyUserInterface::findMenuButtonForTouchEvent(void)
{
  int menuIdx = 1;
  int buttonX, buttonY;
  int buttonWidth, buttonHeight;
 
  //
  // loop through the menu, testing each button location
  //
  while(true)
  {
    //
    // check if at end of table, indicating that a matching button wasn't found
    //
    if(currentMenuTable[menuIdx].MenuItemType == MENU_ITEM_TYPE_END_OF_MENU)
      return(0);

    //
    // check if the given coords were inside this button
    //
    getMenuButtonSizeAndLocation(menuIdx, &buttonX, &buttonY, &buttonWidth, &buttonHeight);

    if ((touchEventX >= buttonX) && (touchEventX <= buttonX + buttonWidth - 1))
    {
      if ((touchEventY >= buttonY) && (touchEventY <= buttonY + buttonHeight - 1))
        return(menuIdx);
    }
    menuIdx++;
  }
}



//
// get the XY coords and size of a menu button 
//  Enter:  menuIdx = index into the menu table of the button to get coords for
//          buttonX, buttonY -> storage to return XY coords of the button
//          buttonWidth, buttonHeight -> storage to return size of the button
//
void TeensyUserInterface::getMenuButtonSizeAndLocation(int menuIdx, int *buttonX, int *buttonY, 
  int *buttonWidth, int *buttonHeight)
{
  int menuButtonNumber = menuIdx - 1;
  int buttonCountOnThisRow;
  
  //
  // count the total number of buttons
  //
  int idx = 1;
  int buttonCount = 0;
  while(true)
  {
    if(currentMenuTable[idx].MenuItemType == MENU_ITEM_TYPE_END_OF_MENU)
      break;

    idx++;
    buttonCount++;
  }

  //
  // determine the number of rows and columns of buttons
  //
  int columnsOfButtons = (int) currentMenuTable[0].MenuItemFunction;
  if ((columnsOfButtons < 1) || (columnsOfButtons > 4))
    columnsOfButtons = 1;
    
  int rowsOfButtons = (buttonCount + columnsOfButtons - 1) / columnsOfButtons;

  //
  // determine the width of the buttons
  //
  int paddingOnTheSidesOfButtons = 10;
  int paddingBetweenButtons = 10;
  *buttonWidth = (displaySpaceWidth - (paddingOnTheSidesOfButtons*2) - (paddingBetweenButtons*(columnsOfButtons-1))) / columnsOfButtons;
  paddingOnTheSidesOfButtons = (displaySpaceWidth - (*buttonWidth * columnsOfButtons) - (paddingBetweenButtons*(columnsOfButtons-1))) / 2;

  //
  // determine the height of the buttons
  //
  int paddingOnTopAndBottomOfButtons = 10;
  *buttonHeight = (displaySpaceHeight - (paddingOnTopAndBottomOfButtons*2) - (paddingBetweenButtons*(rowsOfButtons-1))) / rowsOfButtons;
  paddingOnTopAndBottomOfButtons = (displaySpaceHeight - (*buttonHeight * rowsOfButtons) - (paddingBetweenButtons*(rowsOfButtons-1))) / 2;

  //
  // determine the row and column of this button
  //
  int buttonRow = menuButtonNumber / columnsOfButtons;
  int buttonColumn = menuButtonNumber - (buttonRow * columnsOfButtons);

  //
  // determine the XY coords of the button's upper left corner
  //
  if ((buttonRow != rowsOfButtons - 1) || (buttonCount % columnsOfButtons == 0))
    buttonCountOnThisRow = columnsOfButtons;
  else
    buttonCountOnThisRow = buttonCount % columnsOfButtons;

  int leftMostButtonX = displaySpaceLeftX + (displaySpaceWidth - (*buttonWidth * buttonCountOnThisRow) - (paddingBetweenButtons*(buttonCountOnThisRow-1))) / 2;
  
  *buttonX = leftMostButtonX + (*buttonWidth + paddingBetweenButtons) * buttonColumn;
  *buttonY = displaySpaceTopY + paddingOnTopAndBottomOfButtons + (*buttonHeight + paddingBetweenButtons) * buttonRow;
}


// ---------------------------------------------------------------------------------
//                                 Title bar functions  
// ---------------------------------------------------------------------------------

const int TITLE_BAR_BUTTON_TYPE_NONE  = 0;
const int TITLE_BAR_BUTTON_TYPE_BACK  = 1;
const int TITLE_BAR_BUTTON_TYPE_MENU  = 2;


//
// set the title bar colors
//  Enter:  _titleBarColor = RGB565 color for the title bar background
//          _titleBarTextColor = RGB565 color for the title bar's text
//          _titleBarBackButtonColor = RGB565 color for the back button on the title bar
//
void TeensyUserInterface::setTitleBarColors(uint16_t _titleBarColor, uint16_t _titleBarTextColor, 
  uint16_t _titleBarBackButtonColor, uint16_t _titleBarBackButtonSelectedColor)
{
    titleBarColor = _titleBarColor;
    titleBarTextColor = _titleBarTextColor;
    titleBarBackButtonColor = _titleBarBackButtonColor;
    titleBarBackButtonSelectedColor = _titleBarBackButtonSelectedColor;
}



//
// set the title bar font
//  Enter:  font -> the font to use when drawing the title bar
//          ie: Arial_8, Arial_8_Bold, Arial_9, Arial_9_Bold, Arial_10, Arial_10_Bold, Arial_11, Arial_11_Bold,
//              Arial_12, 13, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60, 72, 96
//
void TeensyUserInterface::setTitleBarFont(const ui_font &font)
{
  titleBarFont = &font;
}


//
// draw the title bar (without the back or hamburger button)
//
void TeensyUserInterface::drawTitleBar(const char *titleBarText)
{
  drawTitleBar(titleBarText, TITLE_BAR_BUTTON_TYPE_NONE);
}


//
// draw the title bar with the back button
//
void TeensyUserInterface::drawTitleBarWithBackButton(const char *titleBarText)
{
  drawTitleBar(titleBarText, TITLE_BAR_BUTTON_TYPE_BACK);
}


//
// draw the title bar with the Menu button
//
void TeensyUserInterface::drawTitleBarWithMenuButton(const char *titleBarText)
{
  drawTitleBar(titleBarText, TITLE_BAR_BUTTON_TYPE_MENU);
}


//
// draw the title bar above the Display Space
//
void TeensyUserInterface::drawTitleBar(const char *titleBarText, int buttonType)
{
  //
  // remember if the title bar includes a button
  //
  buttonTypeOnTitleBar = buttonType;
  
  //
  // draw the "bar" of the title bar along the top of the screen
  //
  lcdDrawFilledRectangle(0, 0, lcdWidth, titleBarHeight, titleBarColor);

  //
  // draw the text on the title bar, first figure out how it will fit best
  //
  lcdSetFont(*titleBarFont);
  int titleBarTextX = (lcdWidth / 2) - (lcdStringWidthInPixels(titleBarText) / 2);
  if (titleBarTextX < 2) titleBarTextX = 2;

  //
  // check space is needed on the title bar for the Back button
  //
  if (buttonType == TITLE_BAR_BUTTON_TYPE_BACK)
  {
    int backButtonHeight;
    int backButtonWidth;
    int backButtonX;
    int backButtonY;

    getBackButtonSizeAndLocation(&backButtonX, &backButtonY, &backButtonWidth, &backButtonHeight);
    if (titleBarTextX < backButtonX + backButtonWidth + 6) 
      titleBarTextX = backButtonX + backButtonWidth + 6;
  }

  //
  // check space is needed on the title bar for the Menu button
  //
  if (buttonType == TITLE_BAR_BUTTON_TYPE_MENU)
  {
  int menuButtonHeight;
  int menuButtonWidth;
  int menuButtonX;
  int menuButtonY;
  
  getMenuButtonSizeAndLocation(&menuButtonX, &menuButtonY, &menuButtonWidth, &menuButtonHeight);
    if (titleBarTextX < menuButtonX + menuButtonWidth + 6) 
      titleBarTextX = menuButtonX + menuButtonWidth + 6;
  }

  //
  // draw the title bar text
  //
  lcdSetFont(*titleBarFont);
  lcdSetFontColor(titleBarTextColor);
  int titleBarTextY = (titleBarHeight / 2) - (lcdGetFontHeightWithoutDecenders() / 2);
  lcdSetCursorXY(titleBarTextX, titleBarTextY);
  lcdPrint(titleBarText); 

  //
  // optionally draw the Back button
  //
  if (buttonType == TITLE_BAR_BUTTON_TYPE_BACK)
    drawTitleBarBackButton(false);

  //
  // optionally draw the Hamburger button
  //
  if (buttonType == TITLE_BAR_BUTTON_TYPE_MENU)
    drawTitleBarMenuButton(false);
}



//
// draw the "Back" button on the title bar
//  Enter:  buttonSelectedFlg = true if should show the button as "pressed"
//
void TeensyUserInterface::drawTitleBarBackButton(boolean buttonSelectedFlg)
{
  //
  // get the location of the Back button
  //
  int backButtonHeight;
  int backButtonWidth;
  int backButtonX;
  int backButtonY;
  getBackButtonSizeAndLocation(&backButtonX, &backButtonY, &backButtonWidth, &backButtonHeight);
  int backButtonRadius = backButtonHeight / 2;

  //
  // draw the button as a rounded rect
  //
  uint16_t buttonColor;
  if (buttonSelectedFlg)
    buttonColor = titleBarBackButtonSelectedColor;
  else
    buttonColor = titleBarBackButtonColor;
  
  lcdDrawFilledRoundedRectangle(backButtonX, backButtonY, backButtonWidth, backButtonHeight, 
    backButtonRadius, buttonColor);

  //
  // place the text "Back" on the button
  //
  lcdSetFont(*titleBarFont);
  lcdSetFontColor(titleBarTextColor);
  lcdSetCursorXY(backButtonX + backButtonRadius + arrowWidth*2 - 2, backButtonY + backButtonHeight/2 - lcdGetFontHeightWithoutDecenders()/2 - 1);
  lcdPrint("Back");

  //
  // draw a triangle showing button goes back to previous menu
  //
  int arrowX = backButtonX + backButtonRadius - 2;
  int arrowCenterY = backButtonY + backButtonRadius - 1;
  lcdDrawFilledTriangle(arrowX,              arrowCenterY,
                        arrowX + arrowWidth, arrowCenterY - arrowWidth/2,
                        arrowX + arrowWidth, arrowCenterY + arrowWidth/2,
                        menuButtonTextColor);
}



//
// draw the Menu button on the title bar
//  Enter:  buttonSelectedFlg = true if should show the button as "pressed"
//
void TeensyUserInterface::drawTitleBarMenuButton(boolean buttonSelectedFlg)
{
  //
  // get the location of the Menu button
  //
  int menuButtonHeight;
  int menuButtonWidth;
  int menuButtonX;
  int menuButtonY;
  getMenuButtonSizeAndLocation(&menuButtonX, &menuButtonY, &menuButtonWidth, &menuButtonHeight);
  int menuButtonRadius = menuButtonHeight / 4;


  //
  // draw the button as a rounded rect
  //
  uint16_t buttonColor;
  if (buttonSelectedFlg)
    buttonColor = titleBarBackButtonSelectedColor;
  else
    buttonColor = titleBarBackButtonColor;
  
  lcdDrawFilledRoundedRectangle(menuButtonX, menuButtonY, menuButtonWidth, menuButtonHeight, 
    menuButtonRadius, buttonColor);

  //
  // draw a triangle showing button goes back to previous menu
  //
  int menuLinesWidth = menuButtonWidth / 2;
  int menuLinesLeftX = menuButtonX + menuButtonWidth/2 - menuLinesWidth/2;
  int menuLinesMiddleY =  menuButtonY + menuButtonHeight/2 - 1;
  
  lcdDrawFilledRectangle(menuLinesLeftX, menuLinesMiddleY, menuLinesWidth, 2, menuButtonTextColor);
  lcdDrawFilledRectangle(menuLinesLeftX, menuLinesMiddleY - 6, menuLinesWidth, 2, menuButtonTextColor);
  lcdDrawFilledRectangle(menuLinesLeftX, menuLinesMiddleY + 6, menuLinesWidth, 2, menuButtonTextColor);
}



//
// check if user has touched and released the title bar's Back button, this also highlights
// the button when the user first touches it
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Exit:   true returned if user has touched and released the Back button, else false
//
boolean TeensyUserInterface::checkForBackButtonClicked(void)
{
  int X1;
  int Y1;
  int buttonWidth;
  int buttonHeight;

  //
  // check if a Back button is displayed on the title bar
  //
  if (buttonTypeOnTitleBar == TITLE_BAR_BUTTON_TYPE_BACK)
  {
    //
    // return if there is No Event
    //
    if (touchEventType == TOUCH_NO_EVENT)
      return(false);
      
    //
    // get the coordinates of the Back button
    //
    getBackButtonSizeAndLocation(&X1, &Y1, &buttonWidth, &buttonHeight);
    int X2 = X1 + buttonWidth - 1;
    int Y2 = Y1 + buttonHeight - 1;
    
    //
    // check if most recent event was: this button "PUSHED"
    //
    if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
    {
      drawTitleBarBackButton(true);
      return(false);
    }
    
    //
    // check if most recent event was: this button "RELEASED"
    //
    if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
    {
      drawTitleBarBackButton(false);
      return(true);
    }
  }
  
  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// check if user has touched and released the title bar's Menu button, this also highlights
// the button when the user first touches it
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Exit:   true returned if user has touched and released the Menu button, else false
//
boolean TeensyUserInterface::checkForMenuButtonClicked(void)
{
  int X1;
  int Y1;
  int buttonWidth;
  int buttonHeight;

  //
  // check if a Menu button is displayed on the title bar
  //
  if (buttonTypeOnTitleBar == TITLE_BAR_BUTTON_TYPE_MENU)
  {
    //
    // return if there is No Event
    //
    if (touchEventType == TOUCH_NO_EVENT)
      return(false);
      
    //
    // get the coordinates of the Menu button
    //
    getMenuButtonSizeAndLocation(&X1, &Y1, &buttonWidth, &buttonHeight);
    int X2 = X1 + buttonWidth - 1;
    int Y2 = Y1 + buttonHeight - 1;
    
    //
    // check if most recent event was: this button "PUSHED"
    //
    if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
    {
      drawTitleBarMenuButton(true);
      return(false);
    }
    
    //
    // check if most recent event was: this button "RELEASED"
    //
    if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
    {
      drawTitleBarMenuButton(false);
      return(true);
    }
  }
  
  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// get the XY coords and size of a title bar "Back" button 
//  Enter:  buttonX, buttonY -> storage to return XY coords of the button
//          buttonWidth, buttonHeight -> storage to return size of the button
//
void TeensyUserInterface::getBackButtonSizeAndLocation(int *buttonX, int *buttonY, 
  int *buttonWidth, int *buttonHeight)
{
  lcdSetFont(*titleBarFont);

  const char *backButtonText = "Back";
  *buttonHeight = titleBarHeight - 6;
  int backButtonRadius = *buttonHeight / 2;
  *buttonWidth = backButtonRadius + arrowWidth*2 + lcdStringWidthInPixels(backButtonText) + backButtonRadius - 3;
  *buttonX = 4;
  *buttonY = (titleBarHeight - *buttonHeight) / 2;
}



//
// get the XY coords and size of a title bar "Menu" button 
//  Enter:  buttonX, buttonY -> storage to return XY coords of the button
//          buttonWidth, buttonHeight -> storage to return size of the button
//
void TeensyUserInterface::getMenuButtonSizeAndLocation(int *buttonX, int *buttonY, int *buttonWidth, int *buttonHeight)
{
  *buttonHeight = titleBarHeight - 6;
  *buttonWidth = (*buttonHeight * 18) / 10;
  *buttonX = 4;
  *buttonY = (titleBarHeight - *buttonHeight) / 2;
}

// ---------------------------------------------------------------------------------
//                               Display space functions  
// ---------------------------------------------------------------------------------

//
// clear the screen's "display space" using the menu's background color and draw a frame around it 
//
void TeensyUserInterface::clearDisplaySpace(void)
{
  clearDisplaySpace(menuBackgroundColor);
}

//
// clear the screen's "display space" using the given background color and draw a frame around it
//  Enter:  backgroundColor = color to fill the display space
//
void TeensyUserInterface::clearDisplaySpace(uint16_t backgroundColor)
{  
  //
  // draw the frame
  //
  lcdDrawLine(displaySpaceRightX+1, displaySpaceTopY,         displaySpaceRightX+1, displaySpaceBottomY+1,   titleBarColor);
  lcdDrawLine(displaySpaceRightX+1, displaySpaceBottomY+1,    displaySpaceLeftX-1,  displaySpaceBottomY+1,   titleBarColor);
  lcdDrawLine(displaySpaceLeftX-1,  displaySpaceBottomY+1,    displaySpaceLeftX-1,  displaySpaceTopY,        titleBarColor);

  //
  // blank the display space
  //
  lcdDrawFilledRectangle(displaySpaceLeftX, displaySpaceTopY, displaySpaceWidth, displaySpaceHeight, backgroundColor);
}


// ---------------------------------------------------------------------------------
//                                  Button functions  
// ---------------------------------------------------------------------------------

//
// draw a rectangular button using the colors and font defined for the menu
//  Enter:  uiButton -> the specifications for the button to draw
//
void TeensyUserInterface::drawButton(BUTTON &uiButton)
{
  int buttonX = uiButton.centerX - uiButton.width/2;
  if (buttonX < 0) buttonX = 0;
  int buttonY = uiButton.centerY - uiButton.height/2;
  if (buttonY < 0) buttonY = 0;
  
  drawButton(uiButton.labelText, buttonX, buttonY, uiButton.width, uiButton.height, menuButtonColor, 
    menuButtonFrameColor, menuButtonTextColor, *menuButtonFont);
}

//
// draw a rectangular button using the colors and font defined for the menu
//  Enter:  uiButton -> the specifications for the button to draw
//          showButtonTouchedFlg = true to draw button showing it's being touched, false to draw normal
//
void TeensyUserInterface::drawButton(BUTTON &uiButton, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;

  int buttonX = uiButton.centerX - uiButton.width/2;
  if (buttonX < 0) buttonX = 0;
  int buttonY = uiButton.centerY - uiButton.height/2;
  if (buttonY < 0) buttonY = 0;

  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;
  
  drawButton(uiButton.labelText, buttonX, buttonY, uiButton.width, uiButton.height, buttonColor, 
    menuButtonFrameColor, menuButtonTextColor, *menuButtonFont);
}

//
// draw a rectangular button with extended options for setting color and font
//  Enter:  uiButton -> the specifications for the button to draw
//
void TeensyUserInterface::drawButton(BUTTON_EXTENDED &uiButtonExt)
{
  int buttonX = uiButtonExt.centerX - uiButtonExt.width/2;
  if (buttonX < 0) buttonX = 0;
  int buttonY = uiButtonExt.centerY - uiButtonExt.height/2;
  if (buttonY < 0) buttonY = 0;
  
  drawButton(uiButtonExt.labelText, buttonX, buttonY, uiButtonExt.width, uiButtonExt.height, 
    uiButtonExt.buttonColor, uiButtonExt.buttonFrameColor, uiButtonExt.buttonTextColor, uiButtonExt.buttonFont);
}

//
// draw a rectangular button with extended options for setting color and font
//  Enter:  uiButton -> the specifications for the button to draw
//          showButtonTouchedFlg = true to draw button showing it's being touched, false to draw normal
//
void TeensyUserInterface::drawButton(BUTTON_EXTENDED &uiButtonExt, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;

  int buttonX = uiButtonExt.centerX - uiButtonExt.width/2;
  if (buttonX < 0) buttonX = 0;
  int buttonY = uiButtonExt.centerY - uiButtonExt.height/2;
  if (buttonY < 0) buttonY = 0;

  if (showButtonTouchedFlg)
    buttonColor = uiButtonExt.buttonSelectedColor;
  else
    buttonColor = uiButtonExt.buttonColor;
  
  drawButton(uiButtonExt.labelText, buttonX, buttonY, uiButtonExt.width, uiButtonExt.height, 
    buttonColor, uiButtonExt.buttonFrameColor, uiButtonExt.buttonTextColor, uiButtonExt.buttonFont);
}

//
// draw a rectangular button using the colors and font defined for the menu
//  Enter:  labelText -> text to display on the button's face
//          showButtonTouchedFlg = true to draw button showing it's being touched, false to draw normal
//          buttonX, buttonY = screen coords for the button's upper left corner
//          buttonWidth, buttonHeight = size of the button
//
void TeensyUserInterface::drawButton(const char *labelText, boolean showButtonTouchedFlg, 
  int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
  uint16_t buttonColor;

  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;

  drawButton(labelText, buttonX, buttonY, buttonWidth, buttonHeight, buttonColor, 
    menuButtonFrameColor, menuButtonTextColor, *menuButtonFont);
}

//
// draw a rectangular button
//  Enter:  labelText -> text to display on the button's face
//          buttonX, buttonY = screen coords for the button's upper left corner
//          buttonWidth, buttonHeight = size of the button
//          buttonColor = color of the button's face
//          buttonFrameColor = color to make the button look raised
//          buttonTextColor = color for the button's text
//          buttonFont -> font for the button's text
//
void TeensyUserInterface::drawButton(const char *labelText, int buttonX, int buttonY, int buttonWidth, 
  int buttonHeight, uint16_t buttonColor, uint16_t buttonFrameColor, uint16_t buttonTextColor, 
  const ui_font &buttonFont)
{
  const int buttonTextBufferLength = 40;
  char buttonTextBufferLine1[buttonTextBufferLength];
  const char *buttonTextLine2;
  int srcIndexStart;
  int srcIndex;
  int breakAtWhiteCount;
  boolean finishedFlg;
  int maxTextWidthInPixels;
  int textWidthInPixels;
  int lineCount;
  
  //
  // draw the button's face with raised edges
  //
  lcdDrawLine(buttonX,  buttonY + buttonHeight-1,   buttonX,                  buttonY,   buttonFrameColor);
  lcdDrawLine(buttonX,  buttonY,                    buttonX + buttonWidth-1,  buttonY,   buttonFrameColor);
  lcdDrawFilledRectangle(buttonX+1, buttonY+1, buttonWidth-1, buttonHeight-1, buttonColor);

  //
  // break the button's text into 1 or 2 lines insuring that the text fits on the button
  //
  maxTextWidthInPixels = buttonWidth - 8;
  lineCount = 1;

  //
  // find the first line
  //
  srcIndexStart = 0;
  breakAtWhiteCount = 1;
  buttonTextBufferLine1[0] = 0;
  while(true)
  {
    srcIndex = srcIndexStart;
    finishedFlg = breakStringAtWhiteSpace(labelText, &srcIndex, buttonTextBufferLine1, buttonTextBufferLength, breakAtWhiteCount);
    textWidthInPixels = lcdStringWidthInPixels(buttonTextBufferLine1);
   
    if ((textWidthInPixels > maxTextWidthInPixels) && (breakAtWhiteCount == 1))
      break;
 
    if ((textWidthInPixels < maxTextWidthInPixels) && (!finishedFlg))
    {
      breakAtWhiteCount++;
      continue;
    }
 
    if ((textWidthInPixels <= maxTextWidthInPixels) && (finishedFlg))
      break;
   
    if (textWidthInPixels > maxTextWidthInPixels)
    {
      breakAtWhiteCount--;
      srcIndex = srcIndexStart;
      finishedFlg = breakStringAtWhiteSpace(labelText, &srcIndex, buttonTextBufferLine1, buttonTextBufferLength, breakAtWhiteCount);
      break;
    }

    break;
  }


  //
  // check if there is a second line
  //
  if (!finishedFlg)
  {
    lineCount++;
    buttonTextLine2 = labelText + srcIndex;
  }


  //
  // draw the text on the button, either 1 line or two
  //
  lcdSetFont(buttonFont);
  lcdSetFontColor(buttonTextColor);

  if (lineCount == 1)
  {
    lcdSetCursorXY(buttonX + buttonWidth/2, buttonY + (buttonHeight / 2) - (lcdGetFontHeightWithoutDecenders()/2));  
    lcdPrintCentered(buttonTextBufferLine1);
  }

  else
  {
    lcdSetCursorXY(buttonX + buttonWidth/2, buttonY + (buttonHeight / 2) - (4 + lcdGetFontHeightWithoutDecenders()));  
    lcdPrintCentered(buttonTextBufferLine1);

    lcdSetCursorXY(buttonX + buttonWidth/2, buttonY + (buttonHeight / 2) + 2);  
    lcdPrintCentered(buttonTextLine2);
  }
}



//
// copy string until the nth white space character
//    Exit:   true returned if up to the end of the srcString has been copied
//
boolean TeensyUserInterface::breakStringAtWhiteSpace(const char *srcString, int *srcIndex, char *destString, int destBufferLength, int breakAtWhiteCount)
{
  int destIndex = 0;
  while(true)
  {
    char c = srcString[*srcIndex];
    *srcIndex = *srcIndex + 1;

    if (c == 0)
    {
      destString[destIndex] = 0;
      return(true);
    }
   
    if (c == ' ')
    {
      breakAtWhiteCount--;
      if (breakAtWhiteCount == 0)
      {
        destString[destIndex] = 0;
        return(false);
      }
    }

    destString[destIndex++] = c;
    if (destIndex >= destBufferLength-1)
    {
        destString[destIndex] = 0;
        return(true);
    }
  }
}



//
// check if user has touched and released the given button, this also highlights the button
// when the user first touches it
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if user has touched and released this button, else false
//
boolean TeensyUserInterface::checkForButtonClicked(BUTTON &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(false);
  }
  
  //
  // check if most recent event was: this button "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, false);
    return(true);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}

//
// check if user has touched and released the given button, this also highlights the button
// when the user first touches it
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if user has touched and released this button, else false
//
boolean TeensyUserInterface::checkForButtonClicked(BUTTON_EXTENDED &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(false);
  }
  
  //
  // check if most recent event was: this button "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, false);
    return(true);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// check if user is holding down the given button and it's now "Auto repeating"
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if there's an Auto Repeat event for this button, else false
//
boolean TeensyUserInterface::checkForButtonAutoRepeat(BUTTON &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(true);
  }
  
  //
  // check if most recent event was: this button "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
    return(true);
  
  //
  // check if most recent event was: this button "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, false);
    return(false);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}

//
// check if user is holding down the given button and it's now "Auto repeating"
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if there's an Auto Repeat event for this button, else false
//
boolean TeensyUserInterface::checkForButtonAutoRepeat(BUTTON_EXTENDED &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(true);
  }
  
  //
  // check if most recent event was: this button "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
    return(true);
  
  //
  // check if most recent event was: this button "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, false);
    return(false);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// check if user has just touched the given button, this happens once each time a button is
// pressed, in most cases using checkForButtonClicked() is preferred
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if user has touched and released this button, else false
//
boolean TeensyUserInterface::checkForButtonFirstTouched(BUTTON &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(true);
  }
 
  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// check if user has just touched the given button, this happens once each time a button is
// pressed, in most cases using checkForButtonClicked() is preferred
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if user has touched and released this button, else false
//
boolean TeensyUserInterface::checkForButtonFirstTouched(BUTTON_EXTENDED &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(true);
  }
 
  //
  // most recent event didn't match anything here
  //
  return(false);
}


// ---------------------------------------------------------------------------------
//                                 Number Box functions  
// ---------------------------------------------------------------------------------

//
// draw a Number Box
//  Enter:  numberBox -> the specifications of the Number Box to draw
//
void TeensyUserInterface::drawNumberBox(NUMBER_BOX &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;
  
  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);

  //
  // the frame of the Number Box
  //
  int overallWidth = downButtonX + buttonWidth - upButtonX;
  int rightMostX = upButtonX + overallWidth;
  lcdDrawFilledRectangle(upButtonX,    topY,              overallWidth, 3,      menuButtonColor);
  lcdDrawFilledRectangle(upButtonX,    topY + height-3,   overallWidth, 3,      menuButtonColor);
  lcdDrawFilledRectangle(upButtonX,    topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(numberX-3,    topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(downButtonX,  topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(rightMostX-3, topY,              3,            height, menuButtonColor);

  //
  // draw a highlight around the button making it look raised
  //
  lcdDrawLine(upButtonX-1,  topY + height-1,    upButtonX-1,   topY-1,   menuButtonFrameColor);
  lcdDrawLine(upButtonX-1,  topY-1,             rightMostX-1,  topY-1,   menuButtonFrameColor);

  //
  // draw the Up and Down buttons
  //
  drawUpButtonInNumberBox(numberBox, false);
  drawDownButtonInNumberBox(numberBox, false);

  //
  // fill in the number
  //
  drawNumberInNumberBox(numberBox);

  //
  // draw optional text above the Number Box
  //
  if (numberBox.labelText[0] != 0)
  {
    int textHeight = lcdGetFontHeightWithDecentersAndLineSpacing();
    lcdSetCursorXY(numberX + numberWidth/2, topY - ((textHeight * 16) / 10));
    lcdPrintCentered(numberBox.labelText);
  }
}



//
// check if user is touching the Number Box, pressing Up or Down
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  numberBox -> the Number Box to check
//  Exit:   true returned if the Number Box's Value changed
//
boolean TeensyUserInterface::checkForNumberBoxTouched(NUMBER_BOX &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;
  
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  //
  // get the coordinates of the DOWN button
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  int X1 = downButtonX;
  int Y1 = topY;
  int X2 = downButtonX + buttonWidth - 1;
  int Y2 = topY + height - 1;
  
  //
  // Down button: check if justed "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawDownButtonInNumberBox(numberBox, true);
    updateNumberBoxNumber(numberBox, -numberBox.stepAmount);
    numberBoxRepeatCount = 0;
    return(true);
  }
  
  //
  // Down button: check if "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
  {
    numberBoxRepeatCount++;
    int stepSize = numberBox.stepAmount * ((numberBoxRepeatCount / 16) + 1);
    updateNumberBoxNumber(numberBox, -stepSize);
    return(true);
  }
  
  //
  // Down button: check if "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawDownButtonInNumberBox(numberBox, false);
    return(false);
  }


  //
  // get the coordinates of the UP button
  //
  X1 = upButtonX;
  X2 = upButtonX + buttonWidth - 1;
  
  //
  // Up button: check if justed "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawUpButtonInNumberBox(numberBox, true);
    updateNumberBoxNumber(numberBox, numberBox.stepAmount);
    numberBoxRepeatCount = 0;
    return(true);
  }
  
  //
  // Up button: check if "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
  {
    numberBoxRepeatCount++;
    int stepSize = numberBox.stepAmount * ((numberBoxRepeatCount / 16) + 1);
    updateNumberBoxNumber(numberBox, stepSize);
    return(true);
  }
  
  //
  // Up button: check if "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawUpButtonInNumberBox(numberBox, false);
    return(false);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// draw Down button in the number box
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawDownButtonInNumberBox(NUMBER_BOX &numberBox, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  
  //
  // draw the "down" button
  //
  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;

  lcdDrawFilledRectangle(downButtonX+3, topY+3, buttonWidth-6, height-6, buttonColor);
  
  int arrowCenterX = downButtonX + buttonWidth/2;
  int arrowCenterY = numberBox.centerY;
  int arrowHalfWidth = 5;
  lcdDrawFilledTriangle(arrowCenterX - arrowHalfWidth,  arrowCenterY - arrowHalfWidth,
                        arrowCenterX + arrowHalfWidth,  arrowCenterY - arrowHalfWidth,
                        arrowCenterX,                   arrowCenterY + arrowHalfWidth,
                        menuButtonTextColor);
}



//
// draw Up button in the number box
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawUpButtonInNumberBox(NUMBER_BOX &numberBox, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  
  //
  // draw the "up" button
  //
  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;
     
  lcdDrawFilledRectangle(upButtonX+3, topY+3, buttonWidth-6, height-6, buttonColor);
  
  int arrowCenterX = upButtonX + buttonWidth/2;
  int arrowCenterY = numberBox.centerY;
  int arrowHalfWidth = 5;
  lcdDrawFilledTriangle(arrowCenterX,                   arrowCenterY - arrowHalfWidth,
                        arrowCenterX - arrowHalfWidth,  arrowCenterY + arrowHalfWidth,
                        arrowCenterX + arrowHalfWidth,  arrowCenterY + arrowHalfWidth,
                        menuButtonTextColor);
}



//
// update the Number Box's Value
//  Enter:  numberBox -> the specifications of the Number Box
//          stepAmount = signed amount to changes the Value by
//  Exit:   true returned if the value changed, else false
//
boolean TeensyUserInterface::updateNumberBoxNumber(NUMBER_BOX &numberBox, int stepAmount)
{
  //
  // check if Adding
  //
  if (stepAmount > 0)
  {
    int newValue = numberBox.value + stepAmount;
    if (newValue > numberBox.maximumValue)
      newValue = numberBox.maximumValue;
    if (numberBox.value == newValue)
      return(false);
    numberBox.value = newValue;
    drawNumberInNumberBox(numberBox);
    return(true);
  }

  //
  // check if Subtracting
  //
  if (stepAmount < 0)
  {
    int newValue = numberBox.value + stepAmount;
    if (newValue < numberBox.minimumValue)
      newValue = numberBox.minimumValue;
    if (numberBox.value == newValue)
      return(false);
    numberBox.value = newValue;
    drawNumberInNumberBox(numberBox);
    return(true);
  }

  return(false);
}



//
// draw the number field in the number box
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawNumberInNumberBox(NUMBER_BOX &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);

  //
  // first blank then number field
  //
  lcdSetFont(*menuButtonFont);
  lcdSetFontColor(menuButtonTextColor);
  int fontHeight = lcdGetFontHeightWithoutDecenders();
  int textY = numberBox.centerY -  fontHeight/2;
  lcdDrawFilledRectangle(numberX+3, textY, numberWidth-6, fontHeight + 1, menuBackgroundColor);

  //
  // draw the number
  //
  lcdSetCursorXY(numberX + numberWidth/2, textY);
  lcdPrintCentered(numberBox.value);
}



//
// get LCD coordinates and sizes about the given Number Box
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::getNumberBoxCoordinates(NUMBER_BOX &numberBox, int *downButtonX, int *numberX, 
  int *upButtonX, int *topY, int *buttonWidth, int *numberWidth, int *height)
{
  *buttonWidth = (numberBox.height * 16) / 10;
  if (*buttonWidth > (numberBox.width * 4) / 10)
    *buttonWidth = (numberBox.width * 4) / 10;
  if (*buttonWidth < 30)
    *buttonWidth = 30;

  *numberWidth = numberBox.width - (2 * *buttonWidth);
  if (*numberWidth < 30)
    *numberWidth = 30;

  *upButtonX = numberBox.centerX - numberBox.width/2;
  *numberX = *upButtonX + *buttonWidth;
  *downButtonX = *numberX + *numberWidth;

  *topY = numberBox.centerY - numberBox.height/2;
  *height = numberBox.height;
}



//
// draw a Number Box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box to draw
//
void TeensyUserInterface::drawNumberBox(NUMBER_BOX_FLOAT &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;
  
  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, 
    &buttonWidth, &numberWidth, &height);

  //
  // the frame of the Number Box
  //
  int overallWidth = downButtonX + buttonWidth - upButtonX;
  int rightMostX = upButtonX + overallWidth;
  lcdDrawFilledRectangle(upButtonX,    topY,              overallWidth, 3,      menuButtonColor);
  lcdDrawFilledRectangle(upButtonX,    topY + height-3,   overallWidth, 3,      menuButtonColor);
  lcdDrawFilledRectangle(upButtonX,    topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(numberX-3,    topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(downButtonX,  topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(rightMostX-3, topY,              3,            height, menuButtonColor);

  //
  // draw a highlight around the button making it look raised
  //
  lcdDrawLine(upButtonX-1,  topY + height-1,    upButtonX-1,   topY-1,   menuButtonFrameColor);
  lcdDrawLine(upButtonX-1,  topY-1,             rightMostX-1,  topY-1,   menuButtonFrameColor);

  //
  // draw the Up and Down buttons
  //
  drawUpButtonInNumberBoxFloat(numberBox, false);
  drawDownButtonInNumberBoxFloat(numberBox, false);

  //
  // fill in the number
  //
  drawNumberInNumberBoxFloat(numberBox);

  //
  // draw the text above the Number Box
  //
  if (numberBox.labelText[0] != 0)
  {
    int textHeight = lcdGetFontHeightWithDecentersAndLineSpacing();
    lcdSetCursorXY(numberX + numberWidth/2, topY - ((textHeight * 16) / 10));
    lcdPrintCentered(numberBox.labelText);
  }
}



//
// check if user is touching the Number Box, pressing Up or Down (FLOAT)
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  numberBox -> the Number Box to check
//  Exit:   true returned if the Number Box's Value changed
//
boolean TeensyUserInterface::checkForNumberBoxTouched(NUMBER_BOX_FLOAT &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;
  
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  //
  // get the coordinates of the DOWN button
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  int X1 = downButtonX;
  int Y1 = topY;
  int X2 = downButtonX + buttonWidth - 1;
  int Y2 = topY + height - 1;
  
  //
  // Down button: check if justed "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawDownButtonInNumberBoxFloat(numberBox, true);
    updateNumberBoxNumberFloat(numberBox, -numberBox.stepAmount);
    numberBoxRepeatCount = 0;
    return(true);
  }
  
  //
  // Down button: check if "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
  {
    numberBoxRepeatCount++;
    float stepSize = numberBox.stepAmount * (float)((numberBoxRepeatCount / 16) + 1);
    updateNumberBoxNumberFloat(numberBox, -stepSize);
    return(true);
  }
  
  //
  // Down button: check if "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawDownButtonInNumberBoxFloat(numberBox, false);
    return(false);
  }


  //
  // get the coordinates of the UP button
  //
  X1 = upButtonX;
  X2 = upButtonX + buttonWidth - 1;
  
  //
  // Up button: check if justed "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawUpButtonInNumberBoxFloat(numberBox, true);
    updateNumberBoxNumberFloat(numberBox, numberBox.stepAmount);
    numberBoxRepeatCount = 0;
    return(true);
  }
  
  //
  // Up button: check if "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
  {
    numberBoxRepeatCount++;
    float stepSize = numberBox.stepAmount * (float)((numberBoxRepeatCount / 16) + 1);
    updateNumberBoxNumberFloat(numberBox, stepSize);
    return(true);
  }
  
  //
  // Up button: check if "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawUpButtonInNumberBoxFloat(numberBox, false);
    return(false);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// draw Down button in the number box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//          showButtonTouchedFlg = true if button should be highlighted
//
void TeensyUserInterface::drawDownButtonInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  
  //
  // draw the "down" button
  //
  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;

  lcdDrawFilledRectangle(downButtonX+3, topY+3, buttonWidth-6, height-6, buttonColor);
  
  int arrowCenterX = downButtonX + buttonWidth/2;
  int arrowCenterY = numberBox.centerY;
  int arrowHalfWidth = 5;
  lcdDrawFilledTriangle(arrowCenterX - arrowHalfWidth,  arrowCenterY - arrowHalfWidth,
                        arrowCenterX + arrowHalfWidth,  arrowCenterY - arrowHalfWidth,
                        arrowCenterX,                   arrowCenterY + arrowHalfWidth,
                        menuButtonTextColor);
}



//
// draw Up button in the number box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawUpButtonInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  
  //
  // draw the "up" button
  //
  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;
     
  lcdDrawFilledRectangle(upButtonX+3, topY+3, buttonWidth-6, height-6, buttonColor);
  
  int arrowCenterX = upButtonX + buttonWidth/2;
  int arrowCenterY = numberBox.centerY;
  int arrowHalfWidth = 5;
  lcdDrawFilledTriangle(arrowCenterX,                   arrowCenterY - arrowHalfWidth,
                        arrowCenterX - arrowHalfWidth,  arrowCenterY + arrowHalfWidth,
                        arrowCenterX + arrowHalfWidth,  arrowCenterY + arrowHalfWidth,
                        menuButtonTextColor);
}



//
// update the Number Box's Value (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//          stepAmount = signed amount to changes the Value by
//  Exit:   true returned if the value changed, else false
//
boolean TeensyUserInterface::updateNumberBoxNumberFloat(NUMBER_BOX_FLOAT &numberBox, float stepAmount)
{
  //
  // check if Adding
  //
  if (stepAmount > 0)
  {
    float newValue = numberBox.value + stepAmount;
    if (newValue > numberBox.maximumValue)
      newValue = numberBox.maximumValue;
    if (numberBox.value == newValue)
      return(false);
    numberBox.value = newValue;
    drawNumberInNumberBoxFloat(numberBox);
    return(true);
  }

  //
  // check if Subtracting
  //
  if (stepAmount < 0)
  {
    float newValue = numberBox.value + stepAmount;
    if (newValue < numberBox.minimumValue)
      newValue = numberBox.minimumValue;
    if (numberBox.value == newValue)
      return(false);
    numberBox.value = newValue;
    drawNumberInNumberBoxFloat(numberBox);
    return(true);
  }

  return(false);
}



//
// draw the number field in the number box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawNumberInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox)
{
  char stringBuffer[20];
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);

  //
  // first blank then number field
  //
  lcdSetFont(*menuButtonFont);
  lcdSetFontColor(menuButtonTextColor);
  int fontHeight = lcdGetFontHeightWithoutDecenders();
  int textY = numberBox.centerY -  fontHeight/2;
  lcdDrawFilledRectangle(numberX+3, textY, numberWidth-6, fontHeight + 1, menuBackgroundColor);

  //
  // draw the number
  //
  dtostrf(numberBox.value, 1, numberBox.digitsRightOfDecimal, stringBuffer);
  lcdSetCursorXY(numberX + numberWidth/2, textY);
  lcdPrintCentered(stringBuffer);
}



//
// get LCD coordinates and sizes about the given Number Box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::getNumberBoxCoordinatesFloat(NUMBER_BOX_FLOAT &numberBox, int *downButtonX, int *numberX, 
  int *upButtonX, int *topY, int *buttonWidth, int *numberWidth, int *height)
{
  *buttonWidth = (numberBox.height * 16) / 10;
  if (*buttonWidth > (numberBox.width * 4) / 10)
    *buttonWidth = (numberBox.width * 4) / 10;
  if (*buttonWidth < 30)
    *buttonWidth = 30;

  *numberWidth = numberBox.width - (2 * *buttonWidth);
  if (*numberWidth < 30)
    *numberWidth = 30;

  *upButtonX = numberBox.centerX - numberBox.width/2;
  *numberX = *upButtonX + *buttonWidth;
  *downButtonX = *numberX + *numberWidth;

  *topY = numberBox.centerY - numberBox.height/2;
  *height = numberBox.height;
}


// ---------------------------------------------------------------------------------
//                               Selection Box functions  
// ---------------------------------------------------------------------------------

//
// draw a Selection Box
//  Enter:  selectionBox -> the specifications of the Selection Box to draw
//
void TeensyUserInterface::drawSelectionBox(SELECTION_BOX &selectionBox)
{
  int X, Y;
  int width, height;
  
  //
  // determine how many cells in the selection box & coordinates of the first one
  //
  int numberOfCells = countSelectionBoxChoices(selectionBox);
  getCoordsOfSelectionBoxCell(selectionBox, 0, &X, &Y, &width, &height);
  
  //
  // draw a box to contain call the cells
  //
  int overallWidth = (width * numberOfCells) + 2;
  lcdDrawRectangle(X-1,  Y-1,  overallWidth, height+2,  menuButtonColor);

  //
  // draw each cell
  //
  for (int cellNumber = 0; cellNumber < numberOfCells; cellNumber++)
    drawSelectionBoxCell(selectionBox, cellNumber, false);
  
  //
  // draw optional text above the Selection Box
  //
  if (selectionBox.labelText[0] != 0)
  {
    int textHeight = lcdGetFontHeightWithDecentersAndLineSpacing();
    lcdSetCursorXY(X-1 + overallWidth/2, Y - ((textHeight * 16) / 10));
    lcdPrintCentered(selectionBox.labelText);
  }
}



//
// check if user is touching the Selection Box, pressing one of the choices
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  selectionBox -> the selection Box to check
//  Exit:   true returned if the Selection Box's Value changed
//
boolean TeensyUserInterface::checkForSelectionBoxTouched(SELECTION_BOX &selectionBox)
{
  int X, Y;
  int width, height;
  
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  //
  // loop through each of the choices in the Selection BO
  //
  int numberOfCells = countSelectionBoxChoices(selectionBox);
  for (int cellNumber = 0; cellNumber < numberOfCells; cellNumber++)
  {
    //
    // check this cell to see if it matches the current event
    //
    getCoordsOfSelectionBoxCell(selectionBox, cellNumber, &X, &Y, &width, &height);
    if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT,   X, Y,   X + width-1, Y + height-1))
    {
      //
      // select the touched cell and redraw all to show it highlighted
      //
      int oldSelectionValue = selectionBox.value;
      selectionBox.value = cellNumber;
      for (int cell = 0; cell < numberOfCells; cell++)
        drawSelectionBoxCell(selectionBox, cell, true);

      //
      // return "true" if the selection changed
      //
      if (oldSelectionValue != selectionBox.value)
         return(true);
      else
        return(false);
    }

    if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT,   X, Y,   X + width-1, Y + height-1))
    {
      //
      // Selection Box is no longer being touched, draw all call untouched
      //
      for (int cell = 0; cell < numberOfCells; cell++)
        drawSelectionBoxCell(selectionBox, cell, false);
      return(false);
    }
  }

  //
  // no match found for the current events
  //
  return(false);
}
  


//
// draw one cell of the Selection Box
//  Enter:  selectionBox -> Selection Box to draw
//          cellNumber = cell number to draw (0 - 3)
//          showButtonTouchedFlg = true if cell should be highlighted
//
void TeensyUserInterface::drawSelectionBoxCell(SELECTION_BOX &selectionBox, int cellNumber, boolean showButtonTouchedFlg)
{
  int X, Y;
  int width, height;
  uint16_t cellColor;
  
  getCoordsOfSelectionBoxCell(selectionBox, cellNumber, &X, &Y, &width, &height);

  //
  // choose the color to fill the cell with
  //
  if ((cellNumber == selectionBox.value) && (showButtonTouchedFlg))
    cellColor = menuButtonSelectedColor;
  else if (cellNumber == selectionBox.value)
    cellColor = menuButtonColor;
  else
    cellColor = menuBackgroundColor;

  //
  // draw a border around the cell and fill the interior
  //
  lcdDrawRectangle(X, Y, width, height, menuButtonColor);
  lcdDrawFilledRectangle(X+1, Y+1, width-2, height-2, cellColor);

  //
  // draw the cell's text
  //
  const char *cellLabel;
  if      (cellNumber == 0) cellLabel = selectionBox.choice0Text;
  else if (cellNumber == 1) cellLabel = selectionBox.choice1Text;
  else if (cellNumber == 2) cellLabel = selectionBox.choice2Text;
  else if (cellNumber == 3) cellLabel = selectionBox.choice3Text;
  else cellLabel = "Undefined";

  lcdSetFont(*menuButtonFont);
  lcdSetFontColor(menuButtonTextColor);
  int fontHeight = lcdGetFontHeightWithoutDecenders();
  int textY = selectionBox.centerY - fontHeight/2;
  lcdSetCursorXY(X + width/2, textY);
  lcdPrintCentered(cellLabel);
}



//
// get the coordinates of one cell in the selection box
//  Enter:  selectionBox -> the Selection Box
//          cellNumber = cell number (0 - 3)
//          X, Y -> storage to return upper left corner of cell
//          width, height -> storage to return size of the cell
//
void TeensyUserInterface::getCoordsOfSelectionBoxCell(SELECTION_BOX &selectionBox, int cellNumber, int *X, 
  int *Y, int *width, int *height)
{
  int numberOfCells = countSelectionBoxChoices(selectionBox);
  int cellWidth = (selectionBox.width - 3) / numberOfCells;
  int overallWidth = cellWidth * numberOfCells;

  *Y = selectionBox.centerY - (selectionBox.height-3)/2;
  *height = selectionBox.height-3;
  *width = cellWidth;
 
  *X = (selectionBox.centerX - overallWidth/2) + (cellNumber * cellWidth);
}



//
// count the number of selection choices in a Selection Box
//  Enter:  selectionBox -> the Selection Box
//  Exit:   number of choices returned (1 - 4)
//
int TeensyUserInterface::countSelectionBoxChoices(SELECTION_BOX &selectionBox)
{
  if (selectionBox.choice1Text[0] == 0)
    return(1);

  if (selectionBox.choice2Text[0] == 0)
    return(2);

  if (selectionBox.choice3Text[0] == 0)
    return(3);

  return(4);
}

// ---------------------------------------------------------------------------------
//                                 Slider functions  
// ---------------------------------------------------------------------------------

//
// constants used by the Slider
//
const int SLIDER_BALL_RADIUS = 10;


//
// draw a Slider
//  Enter:  slider -> the specifications of the Slider to draw
//
void TeensyUserInterface::drawSlider(SLIDER &slider)
{  
  //
  // draw the Slider's line and ball
  //
  drawSliderBall(slider, menuButtonColor);
  int halfWidth = slider.width / 2;
  lcdDrawHorizontalLine(slider.centerX - halfWidth, slider.centerY, halfWidth * 2, menuButtonColor);
  
  //
  // draw optional text above the Slider
  //
  if (slider.labelText[0] != 0)
  {
    int textHeight = lcdGetFontHeightWithDecentersAndLineSpacing();
    lcdSetCursorXY(slider.centerX, slider.centerY - SLIDER_BALL_RADIUS - (textHeight+3));
    lcdPrintCentered(slider.labelText);
  }
}



//
// draw the ball at the Slider's location
//    Enter:  slider -> the Slider
//
void TeensyUserInterface::drawSliderBall(SLIDER &slider, uint16_t ballColor)
{
  int x = getSliderBallXPosition(slider);
  lcdDrawFilledCircle(x, slider.centerY, SLIDER_BALL_RADIUS, ballColor);
}



//
// check if user is touching the Slider (dragging the ball right/left)
//    Enter:  slider -> the Slider
//    Exit:   true returned if the Slider's Value has changed
//
boolean TeensyUserInterface::checkForSliderTouched(SLIDER &slider)
{
  int touchXlcd;
  int touchYlcd;
  int originalValue = slider.value;
  int newValue;

  //
  // get the coords, if any, where the user is touching
  //
  if (!getTouchScreenCoords(&touchXlcd, &touchYlcd))
  {
    slider.state = 0;           // user not touching, just return
    return(false);
  }

  //
  // user is touching, check if this is the first touch
  //
  if (slider.state == 0)
  {
    //
    // check if the touch position is on the ball
    //
    int sliderX = getSliderBallXPosition(slider);
    
    int ballLeft = sliderX - (SLIDER_BALL_RADIUS + 2);
    if (ballLeft < 0) ballLeft = 0;
    
    int ballRight = sliderX + (SLIDER_BALL_RADIUS + 2);
    if (ballRight > lcdWidth - 1) ballRight = lcdWidth - 1;
    
    int ballTop = slider.centerY - (SLIDER_BALL_RADIUS + 2);
    if (ballTop < 0) ballTop = 0;
    
    int ballBottom = slider.centerY + (SLIDER_BALL_RADIUS + 2);
    if (ballBottom > lcdHeight - 1) ballBottom = lcdHeight - 1;
    
    if ((touchXlcd >= ballLeft) && (touchXlcd <= ballRight) && (touchYlcd >= ballTop) && (touchYlcd <= ballBottom))
      slider.state = 1;       // user is touching the ball
  }

  //
  // check if user has already started dragging the ball
  //
  else if (slider.state == 1)
  {
    //
    // user is dragging the ball, compute a new "value" based on the current touch position
    //
    newValue = getBallsValue(slider, touchXlcd);

    //
    // check if the value has changed
    //
    if (newValue != originalValue)
    {
      //
      // value changed
      //
      drawSliderBall(slider, menuBackgroundColor);      // undraw the ball

      slider.value = newValue;                          // update Slider's new value
                               
      drawSliderBall(slider, menuButtonColor);          // redraw the slider with ball and line
      int halfWidth = slider.width / 2;
      lcdDrawHorizontalLine(slider.centerX - halfWidth, slider.centerY, halfWidth * 2, menuButtonColor);
      return(true);
    }
  }

  else
    slider.state = 0;
  
  return(false);
}



//
// get the Slider's ball position on the LCD give its value
//    Enter:  slider -> the slider
//
int TeensyUserInterface::getSliderBallXPosition(SLIDER &slider)
{
  float f = slider.value - slider.minimumValue;
  f = f / (slider.maximumValue - slider.minimumValue);
  int i = (int) ((f * slider.width) + .5);
  return(slider.centerX - (slider.width/2) + i);
}



//
// get the ball's "value" given the its position
//    Enter:  slider -> the slider
//            lcdX = position on the LCD screen
//
int TeensyUserInterface::getBallsValue(SLIDER &slider, int lcdX)
{
  int sliderPos = lcdX - (slider.centerX - (slider.width/2));
  int value = ((sliderPos * (slider.maximumValue - slider.minimumValue)) / slider.width) + slider.minimumValue;
  
  if (value < slider.minimumValue) value = slider.minimumValue;
  if (value > slider.maximumValue) value = slider.maximumValue;
  
  value = ((value + slider.stepAmount/2) / slider.stepAmount) * slider.stepAmount;
  
  if (value < slider.minimumValue) value = slider.minimumValue;
  if (value > slider.maximumValue) value = slider.maximumValue;
  return(value);
}


// ---------------------------------------------------------------------------------
//          Numeric Keypad - Allows user to enter a number (float or int)
// ---------------------------------------------------------------------------------

const int DIGIT_BUTTONS_WIDTH = 56;
const int DIGIT_BUTTONS_HEIGHT = 44;
const int DIGIT_BUTTONS_HORZ_SPACING = DIGIT_BUTTONS_WIDTH + 7;
const int DIGIT_BUTTONS_VERT_SPACING = DIGIT_BUTTONS_HEIGHT + 7;
const int DIGIT_BUTTONS_X = 35;
const int DIGIT_BUTTONS_Y = 61;
const int NUMBER_FIELD_WIDTH = 117;
const int NUMBER_FIELD_HEIGHT = 32;
const int NUMBER_FIELD_X = 196;
const int NUMBER_FIELD_Y = 46;
const int OTHER_BUTTONS_X = 254;
const int OTHER_BUTTONS_Y = 111;
const int OTHER_BUTTONS_WIDTH = 80;
const int OTHER_BUTTONS_HEIGHT = DIGIT_BUTTONS_HEIGHT;
const int OTHER_BUTTONS_SPACING = DIGIT_BUTTONS_VERT_SPACING;

const int MAX_CHARACTERS = 12;
char valueStr[MAX_CHARACTERS + 15];


//
// display a keypad allowing user to enter a float number
//  Enter:  titleBar -> text to display on the titlebar
//          value -> storage for initial value and returned value
//          minValue = minimum value (a lower will disable the "OK" button)
//          maxValue = maximum value (a higher will disable the "OK" button)
//  Exit:   true returned if user pressed "OK", false returned if user pressed "Cancel"
//
boolean TeensyUserInterface::numericKeyPad(const char *titleBar, float &value, float minValue, float maxValue)
{
  boolean firstFlg = true;

  
  //
  // draw title bar without a "Back" button
  //
  drawTitleBar(titleBar);
  clearDisplaySpace();


  //
  // define the digits buttons, along with .  &  -
  //
  int row = 0;
  int col = 0;
  BUTTON button7 = {"7", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button8 = {"8", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button9 = {"9", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};

  row = 1;
  col = 0;
  BUTTON button4 = {"4", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button5 = {"5", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button6 = {"6", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};

  row = 2;
  col = 0;
  BUTTON button1 = {"1", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button2 = {"2", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button3 = {"3", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};

  row = 3;
  col = 0;
  BUTTON button0     = {"0", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON buttonDot   = {".", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON buttonMinus = {"+/-", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};

  //
  // display the digits buttons
  //
  drawButton(button1);
  drawButton(button2);
  drawButton(button3);
  drawButton(button4);
  drawButton(button5);
  drawButton(button6);
  drawButton(button7);
  drawButton(button8);
  drawButton(button9);
  drawButton(button0);
  drawButton(buttonDot);
  drawButton(buttonMinus);


  //
  // draw the number field
  //
  lcdDrawRectangle(NUMBER_FIELD_X, NUMBER_FIELD_Y, NUMBER_FIELD_WIDTH, NUMBER_FIELD_HEIGHT,  LCD_WHITE);

  
  //
  // define and display "Del", "OK" and "Cancel" buttons
  //
  BUTTON okButton           = {"OK",     OTHER_BUTTONS_X, OTHER_BUTTONS_Y + 0*OTHER_BUTTONS_SPACING, OTHER_BUTTONS_WIDTH, OTHER_BUTTONS_HEIGHT};
  drawButton(okButton);

  BUTTON cancelButton       = {"Cancel", OTHER_BUTTONS_X, OTHER_BUTTONS_Y + 1*OTHER_BUTTONS_SPACING, OTHER_BUTTONS_WIDTH, OTHER_BUTTONS_HEIGHT};
  drawButton(cancelButton);

  BUTTON delButton =          {"<",      OTHER_BUTTONS_X, OTHER_BUTTONS_Y + 2*OTHER_BUTTONS_SPACING, OTHER_BUTTONS_WIDTH, OTHER_BUTTONS_HEIGHT};
  drawButton(delButton);

  //
  // convert the initial value into a string, remove trailing zeros, then display it
  //
  dtostrf(value, 0, 4, valueStr);

  int i = strlen(valueStr);

  if (i > MAX_CHARACTERS)
  {
    valueStr[MAX_CHARACTERS] = 0;
    i = strlen(valueStr);
  }
    
  if (i > 0) i--;

  while(i > 1)
  {
    if (valueStr[i] != '0')
      break;
    valueStr[i] = 0;
    i--;
  }
  if (valueStr[i] == '.')
     valueStr[i] = 0;
  
  keypad_DisplayValueInStringBuf();
  

  //
  // process touch events
  //
  while(true)
  {
    getTouchEvents();
    
    //
    // check if 0 - 9 button pressed
    //
    if (checkForButtonClicked(button0)) keypad_AddCharToStringBuf('0', firstFlg);
    if (checkForButtonClicked(button1)) keypad_AddCharToStringBuf('1', firstFlg);
    if (checkForButtonClicked(button2)) keypad_AddCharToStringBuf('2', firstFlg);
    if (checkForButtonClicked(button3)) keypad_AddCharToStringBuf('3', firstFlg);
    if (checkForButtonClicked(button4)) keypad_AddCharToStringBuf('4', firstFlg);
    if (checkForButtonClicked(button5)) keypad_AddCharToStringBuf('5', firstFlg);
    if (checkForButtonClicked(button6)) keypad_AddCharToStringBuf('6', firstFlg);
    if (checkForButtonClicked(button7)) keypad_AddCharToStringBuf('7', firstFlg);
    if (checkForButtonClicked(button8)) keypad_AddCharToStringBuf('8', firstFlg);
    if (checkForButtonClicked(button9)) keypad_AddCharToStringBuf('9', firstFlg);

    //
    // check if the "dot" button pressed
    //
    if (checkForButtonClicked(buttonDot)) 
    {
      int len = strlen(valueStr);
      int dotCount = 0;
      for (int i = 0; i < len; i++)
      {
        if (valueStr[i] == '.')
          dotCount++; 
      }
      if (dotCount == 0)
        keypad_AddCharToStringBuf('.', firstFlg);
    }
    
    
    //
    // check if the "minus" button pressed
    //
    if (checkForButtonClicked(buttonMinus)) 
    {
      if ((firstFlg == true) || (strlen(valueStr) == 0))
        keypad_AddCharToStringBuf('-', firstFlg);
    }


    //
    // if delete button pressed, delete the last character
    //
    if (checkForButtonClicked(delButton))
    {
      int i = strlen(valueStr);
      if (i > 0) valueStr[i-1] = 0;
      keypad_DisplayValueInStringBuf();
      firstFlg = false;
    }
  
    //
    // check if the "OK" button pressed
    //
    if (checkForButtonClicked(okButton))
    {
      value = atof(valueStr);
      if ((value >= minValue) && (value <= maxValue))
        return(true);
      else
      {
        drawTitleBar(">>> NUMBER OUT OF RANGE <<<");
        delay(1500);
        drawTitleBar(titleBar);
      }
    }
  
     //
     // check if the "Cancel" button pressed
     //
     if (checkForButtonClicked(cancelButton))
       return(false);
  }
}



//
// display a keypad allowing user to enter a int number
//  Enter:  titleBar -> text to display on the titlebar
//          value -> storage for initial value and returned value
//          minValue = minimum value (a lower will disable the "OK" button)
//          maxValue = maximum value (a higher will disable the "OK" button)
//  Exit:   true returned if user pressed "OK", false returned if user pressed "Cancel"
//
boolean TeensyUserInterface::numericKeyPad(const char *titleBar, int &value, int minValue, int maxValue)
{
  boolean firstFlg = true;

  
  //
  // draw title bar without a "Back" button
  //
  drawTitleBar(titleBar);
  clearDisplaySpace();


  //
  // define the digits buttons, along with .  &  -
  //
  int row = 0;
  int col = 0;
  BUTTON button7 = {"7", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button8 = {"8", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button9 = {"9", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};

  row = 1;
  col = 0;
  BUTTON button4 = {"4", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button5 = {"5", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button6 = {"6", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};

  row = 2;
  col = 0;
  BUTTON button1 = {"1", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button2 = {"2", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON button3 = {"3", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};

  row = 3;
  col = 1;
  BUTTON button0     = {"0", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};
  BUTTON buttonMinus = {"+/-", DIGIT_BUTTONS_X + (col++*DIGIT_BUTTONS_HORZ_SPACING), DIGIT_BUTTONS_Y + (row*DIGIT_BUTTONS_VERT_SPACING), DIGIT_BUTTONS_WIDTH, DIGIT_BUTTONS_HEIGHT};

  //
  // display the digits buttons
  //
  drawButton(button1);
  drawButton(button2);
  drawButton(button3);
  drawButton(button4);
  drawButton(button5);
  drawButton(button6);
  drawButton(button7);
  drawButton(button8);
  drawButton(button9);
  drawButton(button0);
  drawButton(buttonMinus);


  //
  // draw the number field
  //
  lcdDrawRectangle(NUMBER_FIELD_X, NUMBER_FIELD_Y, NUMBER_FIELD_WIDTH, NUMBER_FIELD_HEIGHT,  LCD_WHITE);

  
  //
  // define and display "Del", "OK" and "Cancel" buttons
  //
  BUTTON okButton           = {"OK",     OTHER_BUTTONS_X, OTHER_BUTTONS_Y + 0*OTHER_BUTTONS_SPACING, OTHER_BUTTONS_WIDTH, OTHER_BUTTONS_HEIGHT};
  drawButton(okButton);

  BUTTON cancelButton       = {"Cancel", OTHER_BUTTONS_X, OTHER_BUTTONS_Y + 1*OTHER_BUTTONS_SPACING, OTHER_BUTTONS_WIDTH, OTHER_BUTTONS_HEIGHT};
  drawButton(cancelButton);

  BUTTON delButton =          {"<",      OTHER_BUTTONS_X, OTHER_BUTTONS_Y + 2*OTHER_BUTTONS_SPACING, OTHER_BUTTONS_WIDTH, OTHER_BUTTONS_HEIGHT};
  drawButton(delButton);

  //
  // convert the initial value into a string, remove trailing zeros, then display it
  //
  dtostrf(value, 0, 4, valueStr);

  int i = strlen(valueStr);

  if (i > MAX_CHARACTERS)
  {
    valueStr[MAX_CHARACTERS] = 0;
    i = strlen(valueStr);
  }
    
  if (i > 0) i--;

  while(i > 1)
  {
    if (valueStr[i] != '0')
      break;
    valueStr[i] = 0;
    i--;
  }
  if (valueStr[i] == '.')
     valueStr[i] = 0;
  
  keypad_DisplayValueInStringBuf();
  

  //
  // process touch events
  //
  while(true)
  {
    getTouchEvents();
    
    //
    // check if 0 - 9 button pressed
    //
    if (checkForButtonClicked(button0)) keypad_AddCharToStringBuf('0', firstFlg);
    if (checkForButtonClicked(button1)) keypad_AddCharToStringBuf('1', firstFlg);
    if (checkForButtonClicked(button2)) keypad_AddCharToStringBuf('2', firstFlg);
    if (checkForButtonClicked(button3)) keypad_AddCharToStringBuf('3', firstFlg);
    if (checkForButtonClicked(button4)) keypad_AddCharToStringBuf('4', firstFlg);
    if (checkForButtonClicked(button5)) keypad_AddCharToStringBuf('5', firstFlg);
    if (checkForButtonClicked(button6)) keypad_AddCharToStringBuf('6', firstFlg);
    if (checkForButtonClicked(button7)) keypad_AddCharToStringBuf('7', firstFlg);
    if (checkForButtonClicked(button8)) keypad_AddCharToStringBuf('8', firstFlg);
    if (checkForButtonClicked(button9)) keypad_AddCharToStringBuf('9', firstFlg);
    
    
    //
    // check if the "minus" button pressed
    //
    if (checkForButtonClicked(buttonMinus)) 
    {
      if ((firstFlg == true) || (strlen(valueStr) == 0))
        keypad_AddCharToStringBuf('-', firstFlg);
    }


    //
    // if delete button pressed, delete the last character
    //
    if (checkForButtonClicked(delButton))
    {
      int i = strlen(valueStr);
      if (i > 0) valueStr[i-1] = 0;
      keypad_DisplayValueInStringBuf();
      firstFlg = false;
    }
  
    //
    // check if the "OK" button pressed
    //
    if (checkForButtonClicked(okButton))
    {
      value = atof(valueStr);
      if ((value >= minValue) && (value <= maxValue))
        return(true);
      else
      {
        drawTitleBar(">>> NUMBER OUT OF RANGE <<<");
        delay(1500);
        drawTitleBar(titleBar);
      }
    }
  
     //
     // check if the "Cancel" button pressed
     //
     if (checkForButtonClicked(cancelButton))
       return(false);
  }
}


//
// print the "value" as the user is entering it
//  Enter:  valueStr contains string of the value
//
void TeensyUserInterface::keypad_DisplayValueInStringBuf(void)
{
  lcdDrawFilledRectangle(NUMBER_FIELD_X + 15, NUMBER_FIELD_Y + 11, NUMBER_FIELD_WIDTH-30, 10, LCD_BLACK);
  
  //setMenuFont(menuButtonFont);
  //setMenuColors(LCD_WHITE);
  lcdSetCursorXY(NUMBER_FIELD_X + NUMBER_FIELD_WIDTH/2, NUMBER_FIELD_Y + 11);
  lcdPrintCentered(valueStr);
}


//
// add a character to the string buffer
//
void TeensyUserInterface::keypad_AddCharToStringBuf(char c, boolean &firstCharEntered)
{
  int len = strlen(valueStr);
  if (len >= MAX_CHARACTERS)
    return;

  if (firstCharEntered == true)
    len = 0;
  firstCharEntered = false;

  valueStr[len] = c;
  valueStr[len+1] = 0;

  keypad_DisplayValueInStringBuf();
}


// ---------------------------------------------------------------------------------
//                                Touch screen functions  
// ---------------------------------------------------------------------------------

//
// values for: touchState
//
const byte WAITING_FOR_TOUCH_DOWN_STATE                 = 0;
const byte CONFIRM_TOUCH_DOWN_STATE                     = 1;
const byte WAITING_FOR_TOUCH_UP_STATE                   = 2;
const byte WAITING_FOR_TOUCH_UP_AFTER_AUTO_REPEAT_STATE = 3;
const byte CONFIRM_TOUCH_UP_STATE                       = 4;


//
// delay periods for dealing with touches in milliseconds
//
const long TOUCH_DEBOUNCE_PERIOD = 30;
const long TOUCH_AUTO_REPEAT_DELAY = 800;
const long TOUCH_AUTO_REPEAT_RATE = 120;


// ---------------------------------------------------------------------------------

//
// initialize the touch screen
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//
void TeensyUserInterface::touchScreenInitialize(int lcdOrientation)
{
#if __has_include("Adafruit_FT6206.h")
  ts->begin(40);
#elif __has_include("XPT2046_Touchscreen.h")
  ts->begin();
#endif
  touchScreenSetOrientation(lcdOrientation);
}



//
// set the touch screen orientation
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//
void TeensyUserInterface::touchScreenSetOrientation(int lcdOrientation)
{
#ifdef TOUCH_MAP
  ts->setRotation((lcdOrientation + 2) % 4);
  setDefaultTouchScreenCalibrationConstants(lcdOrientation);
#endif
  touchState = WAITING_FOR_TOUCH_DOWN_STATE;
}



//
// check if the most recent touch event happened inside the given rectangle
//  Enter:  eventType = the type of event to look for (ie TOUCH_PUSHED_EVENT, TOUCH_RELEASED_EVENT, TOUCH_REPEAT_EVENT)
//          rectX1, rectY1 = upper left corner of the test area
//          rectX2, rectY2 = lower right corner of the test area
//  Exit:   true returned if most recent touch event matches the given parameters
//
boolean TeensyUserInterface::checkForTouchEventInRect(int eventType, int rectX1, int rectY1, int rectX2, int rectY2)
{
  //
  // check if the most recent event matches what we are looking for
  //
  if (eventType != touchEventType)
    return(false);

  //
  // check if the most recent event happened inside the rectangle we are looking for
  //
  if ((touchEventX >= rectX1) && (touchEventX <= rectX2))
  {
    if ((touchEventY >= rectY1) && (touchEventY <= rectY2))
      return(true);
  }
  return(false);
}



//
// check touch screen for new events
//  Exit:   touchEventType = touch event type, TOUCH_NO_EVENT if no event
//          touchEventX, touchEventY = LCD coordinates of touch event 
//
void TeensyUserInterface::getTouchEvents(void)
{
  boolean currentlyTouched;
  int currentTouchX;
  int currentTouchY;
  unsigned long currentTime = millis();
  static unsigned long touchEventStartTime;
  static int recordedTouchX;
  static int recordedTouchY;

  touchEventType = TOUCH_NO_EVENT;                          // assume there will be no touch event

  //
  // check if anything is touched now
  //
  currentlyTouched = getTouchScreenCoords(&currentTouchX, &currentTouchY);

  //
  // select the current touch state
  //
  switch(touchState)
  {
    //
    // check if waiting for the screen to be touched
    //
    case WAITING_FOR_TOUCH_DOWN_STATE:
    {
      if (currentlyTouched)
      {
         touchState = CONFIRM_TOUCH_DOWN_STATE;             // screen is touched, start timer to confirm touch
         touchEventStartTime = currentTime;
      }
      return;
     } 


    //
    // we think the screen has a new touch, let's double check
    //
    case CONFIRM_TOUCH_DOWN_STATE:
    {
      if (currentTime < (touchEventStartTime + TOUCH_DEBOUNCE_PERIOD))
        return;                                             // wait until debounce period complete
  
      //
      // check if the screen didn't stay touched
      //
      if (!currentlyTouched)
      {
        touchState = WAITING_FOR_TOUCH_DOWN_STATE;          // false alarm, don't report the touch
        return;
      }

      //
      // the screen has been touched
      //
      recordedTouchX = currentTouchX;                       // save coordinates where the touch occurred
      recordedTouchY = currentTouchY;
      
      touchEventStartTime = currentTime;                    // start auto repeat timer
      touchState = WAITING_FOR_TOUCH_UP_STATE;  

      touchEventX = recordedTouchX;                         // return "screen has been pressed" event
      touchEventY = recordedTouchY;
      touchEventType = TOUCH_PUSHED_EVENT; 
      return;
    }


    //
    // check if waiting for the touch to be released
    //      
    case WAITING_FOR_TOUCH_UP_STATE:
    {
      //
      // screen has been continuously touched, check if no longer touched
      //
      if (!currentlyTouched)
      {
        touchState = CONFIRM_TOUCH_UP_STATE;                // not touched now, go debounce the released
        return;
      }

      //
      // still touched, check if time to auto repeat
      //
      if (currentTime < (touchEventStartTime + TOUCH_AUTO_REPEAT_DELAY))
        return;                                             // no, return no event
                                           
      touchEventStartTime = currentTime;                    // yes auto repeat, reset auto repeat timer
      touchState = WAITING_FOR_TOUCH_UP_AFTER_AUTO_REPEAT_STATE;

      touchEventX = recordedTouchX;                         // return "touch is auto repeating" event
      touchEventY = recordedTouchY;
      touchEventType = TOUCH_REPEAT_EVENT;
      return; 
    }
     

    //
    // check if auto repeat has started, waiting more repeats
    //
    case WAITING_FOR_TOUCH_UP_AFTER_AUTO_REPEAT_STATE:
    {
      //
      // screen has been continuously touched & auto repeat started, check if still touched
      //
      if (!currentlyTouched)
      {
        touchState = CONFIRM_TOUCH_UP_STATE;                // not touched now, wait to debounce
        return;
      }
 
      //
      // still touched, check if time to auto repeat
      //
      if (currentTime < (touchEventStartTime + TOUCH_AUTO_REPEAT_RATE))
        return;                        // no, return no event
                                           
      touchEventStartTime = currentTime;                    // yes, reset auto repeat timer

      touchEventX = recordedTouchX;                         // return "auto repeat" event
      touchEventY = recordedTouchY;
      touchEventType = TOUCH_REPEAT_EVENT;
      return;
    }
  

    //
    // touch has been released, verify that it stays released for a period of time
    //
    case CONFIRM_TOUCH_UP_STATE:
    {
      //
      // screen should not be touched, reset debounce time if it is
      //
      if (currentlyTouched)
      {
        touchEventStartTime = currentTime;                  // touched again, reset timer & return no event
        return;
      }
      
      //
      // check if debounce period has elapsed
      //
      if (currentTime < (touchEventStartTime + TOUCH_DEBOUNCE_PERIOD))
        return;                                             // delay period not up, return no event
  
      //
      // touch has been released now
      //
      touchState = WAITING_FOR_TOUCH_DOWN_STATE;            // the touch cycle of events is now complete

      touchEventX = recordedTouchX;                         // return touch "Released" event
      touchEventY = recordedTouchY;
      touchEventType = TOUCH_RELEASED_EVENT;
      return;
    }
  }
}



//
// set default calibration constants for converting to LCD coordinates
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//
void TeensyUserInterface::setDefaultTouchScreenCalibrationConstants(int lcdOrientation)
{
  switch(lcdOrientation)
  {
    case LCD_ORIENTATION_PORTRAIT_4PIN_TOP:
    {
      setTouchScreenCalibrationConstants(150, 3769, 355, 3838);
      break;
    }

    case LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT:
    {
      setTouchScreenCalibrationConstants(228, 3786, 133, 3768);
      break;
    }
    
    case LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM:
    {
     setTouchScreenCalibrationConstants(317, 3924, 220, 3759);
     break;
    }
    
    case LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT:
    default:
    {
      setTouchScreenCalibrationConstants(337, 3861,  327, 3948);
      break;
    }
  }
}



//
// set the touch screen calibration constants used for converting from
// touch coordinates to LCD coordinates
//  Enter:  tsToLCDOffsetX_low = touch screen calibration min X
//          tsToLCDOffsetX_high = touch screen calibration max X
//          tsToLCDOffsetY_low = touch screen calibration min Y
//          tsToLCDOffsetY_high = touch screen calibration min Y
//
void TeensyUserInterface::setTouchScreenCalibrationConstants(int tsToLCDOffsetX_low, 
  int tsToLCDOffsetX_high, int tsToLCDOffsetY_low, int tsToLCDOffsetY_high)
{
  touchScreenToLCDOffsetX_low = tsToLCDOffsetX_low;
  touchScreenToLCDOffsetX_high = tsToLCDOffsetX_high;
  touchScreenToLCDOffsetY_low = tsToLCDOffsetY_low;
  touchScreenToLCDOffsetY_high = tsToLCDOffsetY_high;
}



//
// get the XY values of where to touch screen is being touched (in LCD space)
//  Enter:  xLCD, yLCD -> storage to return X and Y coordinates
//  Exit:   true returned if touch screen is currently being touch, else false
//
boolean TeensyUserInterface::getTouchScreenCoords(int *xLCD, int *yLCD)
{
  int xRaw;
  int yRaw;

  //
  // check if the screen is currently being touched
  //
  if (getRAWTouchScreenCoords(&xRaw, &yRaw) == false)
    return(false);

  //
  // convert the coordinates into LCD space
  //
#ifdef TOUCH_MAP
  int x = map(xRaw, touchScreenToLCDOffsetX_low, touchScreenToLCDOffsetX_high, 0, lcdWidth);
  *xLCD = constrain(x, 0, lcdWidth - 1);

  int y = map(yRaw, touchScreenToLCDOffsetY_low, touchScreenToLCDOffsetY_high, 0, lcdHeight);
  *yLCD = constrain(y, 0, lcdHeight - 1);
#endif

#ifdef TOUCH_PIXEL
  switch(touchOrient)
  {
    case LCD_ORIENTATION_PORTRAIT_4PIN_TOP:
    {
      *xLCD = xRaw;
      *yLCD = yRaw;
      break;
    }
    
    case LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT:
    {
      *xLCD = yRaw;
      *yLCD = lcdHeight - xRaw;
      break;
    }
    
    case LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM:
    {
      *xLCD = lcdWidth - xRaw;
      *yLCD = lcdHeight - yRaw;
      break;
    }
    
    case LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT:
    default:
    {
      *xLCD = lcdWidth - yRaw;
      *yLCD = xRaw;
      break;
    }
  }
#endif

  return(true);
}



//
// get the raw XY values of where to touch screen is being touched (in touch screen space)
//  Enter:  xRaw, yRaw -> storage to return X and Y raw coordinates
//  Exit:   true returned if touch screen is currently being touch, else false
//
boolean TeensyUserInterface::getRAWTouchScreenCoords(int *xRaw, int *yRaw)
{
  //
  // check if the screen is currently being touched
  //
  if (ts->touched() == false)
    return(false);

  //
  // get the raw coordinates and return them
  //
  TS_Point rawTouchPoint = ts->getPoint();
  *xRaw = rawTouchPoint.x;
  *yRaw = rawTouchPoint.y;
  return(true);
}


// ---------------------------------------------------------------------------------
//                                    LCD functions  
// ---------------------------------------------------------------------------------

//
// initialize the LCD display
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//          font -> the font typeface to load, ei: Arial_10
//
void TeensyUserInterface::lcdInitialize(int lcdOrientation, const ui_font &font)
{
#if __has_include("ST7796_t3.h")
  lcd->init(320, 480);
  lcd->invertDisplay(true); 
#else
  lcd->begin();
#endif
  lcdSetOrientation(lcdOrientation);
  lcdClearScreen(LCD_BLACK);
  lcdSetFontColor(LCD_WHITE);  
  lcdSetFont(font);
  touchOrient = lcdOrientation;
}



//
// set the LCD display's orientation
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//
void TeensyUserInterface::lcdSetOrientation(int lcdOrientation)
{
  lcd->setRotation(lcdOrientation);
  lcdWidth = lcd->width();
  lcdHeight = lcd->height();
  lcdSetCursorXY(0, 0);
}


//
// invert display
//
void TeensyUserInterface::invertDisplay(bool invert)
{
  if(invert) {
    lcd->invertDisplay(true);
  } else {
    lcd->invertDisplay(false);
  }
}

//
// fill the entire lcd screen with the given color
//  Enter:  color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdClearScreen(uint16_t color)
{
  lcd->fillScreen(color);
}



//
// draw one pixel with the given coords and color
//  Enter:  x, y = coords of the pixel to draw
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawPixel(int x, int y, uint16_t color)
{
  lcd->drawPixel(x, y, color);
}



//
// draw a line with the given coords and color
//  Enter:  x1, y1 = first endpoint of line
//          x2, y2 = second endpoint of line
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawLine(int x1, int y1, int x2, int y2, uint16_t color)
{
  lcd->drawLine(x1, y1, x2, y2, color);
}



//
// draw a horizontal line with the given coords, length and color
//  Enter:  x, y = first endpoint of line
//          length = length of line
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawHorizontalLine(int x, int y, int length, uint16_t color)
{
  lcd->drawFastHLine(x, y, length, color);
}



//
// draw a vertical line with the given coords, length and color
//  Enter: x, y = first endpoint of line
//          length = length of line
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawVerticalLine(int x, int y, int length, uint16_t color)
{
  lcd->drawFastVLine(x, y, length, color);
}



//
// draw rectangle at the given coords, length, width and color
//  Enter:  x, y = upper left corner of rect
//          width = width of rectangle
//          height = height of rectangle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawRectangle(int x, int y, int width, int height, uint16_t color)
{
  lcd->drawRect(x, y, width, height, color);
}



//
// draw rounded rectangle at the given coords, length, width and color
//  Enter:  x, y = upper left corner of rect
//          width = width of rectangle
//          height = height of rectangle
//          radius = radius of the corners
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawRoundedRectangle(int x, int y, int width, int height, int radius, uint16_t color)
{
  lcd->drawRoundRect(x, y, width, height, radius, color);
}



//
// draw a circle at the given coords, radius and color
//  Enter:  x0, y0 = endpoint 0 of the triangle
//          x1, y1 = endpoint 1 of the triangle
//          x2, y2 = endpoint 2 of the triangle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color)
{
  lcd->drawTriangle(x0, y0, x1, y1, x2, y2, color);
}



//
// draw a triangle at the given coords and color
//  Enter:  x, y = upper left corner of rect
//          radius = radius of the circle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawCircle(int x, int y, int radius, uint16_t color)
{
  lcd->drawCircle(x, y, radius, color);
}



//
// draw a filled rectangle at the given coords, length, width and color
//  Enter:  x, y = upper left corner of rect
//          width = width of rectangle
//          height = height of rectangle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawFilledRectangle(int x, int y, int width, int height, uint16_t color)
{
  lcd->fillRect(x, y, width, height, color);
}



//
// draw a filled rounded rectangle at the given coords, length, width and color
//  Enter:  x, y = upper left corner of rect
//          width = width of rectangle
//          height = height of rectangle
//          radius = radius of the corners
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawFilledRoundedRectangle(int x, int y, int width, int height, int radius, uint16_t color)
{
  lcd->fillRoundRect(x, y, width, height, radius, color);
}



//
// draw a circle at the given coords, radius and color
//  Enter:  x0, y0 = endpoint 0 of the triangle
//          x1, y1 = endpoint 1 of the triangle
//          x2, y2 = endpoint 2 of the triangle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color)
{
  lcd->fillTriangle(x0, y0, x1, y1, x2, y2, color);
}



//
// draw a filled circle at the given coords, radius and color
//  Enter:  x, y = upper left corner of rect
//          radius = radius of the circle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawFilledCircle(int x, int y, int radius, uint16_t color)
{
  lcd->fillCircle(x, y, radius, color);
}




//
// draw an image
//  Enter:  x, y = coords of upper left corner on LCD where the image will be displayed
//          width, height =  size of the image, this must be the same as the image data
//          image -> image data, 2 bytes/pixel in the RGB565 format stored in PROGMEM
// Note: use this utility to convert an image to C sourse code:
//       www.rinkydinkelectronics.com/_t_doimageconverter565.php
//
void TeensyUserInterface::lcdDrawImage(int x, int y, int width, int height, const uint16_t *image)
{
  lcd->writeRect(x, y, width, height, image);
}



//
// set the text font for the "print" functions
//  Enter:  font -> the font typeface to load
//          ie: Arial_8, Arial_8_Bold, Arial_9, Arial_9_Bold, Arial_10, Arial_10_Bold, Arial_11, Arial_11_Bold,
//              Arial_12, 13, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60, 72, 96
//
void TeensyUserInterface::lcdSetFont(const ui_font &font)
{
  lcd->setFont(font);
  currentFont = &font;
}



//
// set the foreground color for the "print" functions
//  Enter:  color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdSetFontColor(uint16_t color)
{
  lcd->setTextColor(color);
}



//
// print a string to the LCD display
//  Enter:  s -> a null terminated string 
//
void TeensyUserInterface::lcdPrint(char *s)
{
  lcd->print(s);
}

void TeensyUserInterface::lcdPrint(const char *s)
{
  lcd->print(s);
}



//
// print a signed int at location of the cursor
//  Enter:  n = signed number to print 
//
void TeensyUserInterface::lcdPrint(int n)
{
  char stringBuffer[14];
  
  itoa(n, stringBuffer, 10);
  lcdPrint(stringBuffer);
}



//
// print a float or double at location of the cursor
//  Enter:  n = signed number to print 
//          digitsRightOfDecimal = number of digits to display right of decimal point (optional)
//
void TeensyUserInterface::lcdPrint(double n, int digitsRightOfDecimal)
{
  char stringBuffer[40];

  dtostrf(n, 1, digitsRightOfDecimal, stringBuffer);
  lcdPrint(stringBuffer);
}



//
// print a string to the LCD, right justified at the cursor
//  Enter:  s -> string to print 
//
void TeensyUserInterface::lcdPrintRightJustified(char *s)
{
  int cursorX;
  int cursorY;

  lcdGetCursorXY(&cursorX, &cursorY);           // get the current cursor position
  int stringWidth = lcdStringWidthInPixels(s);  // get the width of the string to print
  cursorX = cursorX - stringWidth;              // determine new X coord for cursor to right justify

  if (cursorX < 0)                              // set the new cursor position
    cursorX = 0;
  lcdSetCursorXY(cursorX, cursorY);
  
  lcdPrint(s);                                  // print the string
}

void TeensyUserInterface::lcdPrintRightJustified(const char *s)
{
  lcdPrintRightJustified((char *)s);
}



//
// print a signed int on the LCD, right justify at the cursor 
//  Enter:  n = signed number to print 
//
void TeensyUserInterface::lcdPrintRightJustified(int n)
{
  char stringBuffer[14];
  
  itoa(n, stringBuffer, 10);
  lcdPrintRightJustified(stringBuffer);
}



//
// print a float on the LCD, right justify at the cursor 
//  Enter:  n = signed number to print 
//          digitsRightOfDecimal = number of digits to display right of decimal point (optional)
//
void TeensyUserInterface::lcdPrintRightJustified(double n, int digitsRightOfDecimal)
{
  char stringBuffer[40];

  dtostrf(n, 1, digitsRightOfDecimal, stringBuffer);
  lcdPrintRightJustified(stringBuffer);
}



//
// print a string to the LCD, centered side-to-side at the cursor 
//  Enter:  s -> string to print 
//
void TeensyUserInterface::lcdPrintCentered(char *s)
{
  int cursorX;
  int cursorY;

  lcdGetCursorXY(&cursorX, &cursorY);           // get the current cursor position
  int stringWidth = lcdStringWidthInPixels(s);  // get the width of the string to print
  cursorX = cursorX - stringWidth/2;            // determine new X coord for cursor to center

  if (cursorX < 0)                              // set the new cursor position
    cursorX = 0;
  lcdSetCursorXY(cursorX, cursorY);
  
  lcdPrint(s);                                  // print the string
}

void TeensyUserInterface::lcdPrintCentered(const char *s)
{
  lcdPrintCentered((char *) s);
}



//
// print a signed int to the LCD, centered side-to-side at the cursor
//  Enter:  n = signed number to print 
//
void TeensyUserInterface::lcdPrintCentered(int n)
{
  char stringBuffer[14];
  
  itoa(n, stringBuffer, 10);
  lcdPrintCentered(stringBuffer);
}



//
// print a float to the LCD, centered side-to-side at the cursor
//          digitsRightOfDecimal = number of digits to display right of decimal point (optional)
//  Enter:  n = signed number to print 
//
void TeensyUserInterface::lcdPrintCentered(double n, int digitsRightOfDecimal)
{
  char stringBuffer[40];

  dtostrf(n, 1, digitsRightOfDecimal, stringBuffer);
  lcdPrintCentered(stringBuffer);
}



//
// print one ASCII charater to the LCD, at location of the cursor
//  Enter:  c = character to display
//
void TeensyUserInterface::lcdPrintCharacter(byte character)
{
  lcd->drawFontChar(character);
}



//
// get the width of a string in pixels
//
int TeensyUserInterface::lcdStringWidthInPixels(char *s)
{
  return(lcd->strPixelLen(s));
}

int TeensyUserInterface::lcdStringWidthInPixels(const char *s)
{
  return(lcd->strPixelLen((char *) s));
}



//
// get the height of the selected font in pixels, excluding decenders
//
int TeensyUserInterface::lcdGetFontHeightWithoutDecenders(void)
{
  return(currentFont->cap_height);
}



//
// get the height of the selected font in pixels, including decenders & line spacing
//
int TeensyUserInterface::lcdGetFontHeightWithDecentersAndLineSpacing(void)
{
  return(currentFont->line_space);
}



//
// set the cursor coords in pixels
//  Enter:  x  0 = left most pixel
//          y  0 = left most pixel
//
void TeensyUserInterface::lcdSetCursorXY(int x, int y)
{
  if ((x < 0) || (x >= lcdWidth)) return;
  if ((y < 0) || (y >= lcdHeight)) return;
  
  lcd->setCursor(x, y);
}



//
// get the cursor coords in pixels
//  Enter:  x -> storage to return X,  0 = left most pixel
//          y -> storage to return Y,  0 = left most pixel
//
void TeensyUserInterface::lcdGetCursorXY(int *x, int *y)
{
  int16_t x1;
  int16_t y1;

  lcd->getCursor(&x1, &y1);
  *x = x1;
  *y = y1;
}



//
// make a RGB565 color
//  Enter:  red (0 to 31)
//          green (0 to 63) Note: green value 0x20 has same intensity as blue value 0x10
//          blue (0 to 31)
//  Exit:   16 bit color returned, bit format: rrrrrggggggbbbbb
//
uint16_t TeensyUserInterface::lcdMakeColor(int red, int green, int blue)
{
  return((red << 11) + (green << 5) + (blue));
}


// ---------------------------------------------------------------------------------
//                                   EEPROM functions
// ---------------------------------------------------------------------------------


//
// write a configuration byte (8 bit) to the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to write 
//          value = 8 bit value to write to EEPROM
//          note: 2 bytes of EEPROM space are used 
//
void TeensyUserInterface::writeConfigurationByte(int EEPromAddress, byte value)
{
  if (EEPROM.read(EEPromAddress) == 0xff)
    EEPROM.write(EEPromAddress, 0);
	
  EEPROM.write(EEPromAddress + 1, value);
}



//
// read a configuration byte (8 bit) from the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to read from 
//          defaultValue = default value to return if value has never been 
//            written to the EEPROM
//          note: 2 bytes of EEPROM space are used 
//  Exit:   byte value from EEPROM (or default value) returned
//
byte TeensyUserInterface::readConfigurationByte(int EEPromAddress, byte defaultValue)
{
  if (EEPROM.read(EEPromAddress) == 0xff)
    return(defaultValue);
  else
    return(EEPROM.read(EEPromAddress + 1));
}



//
// write a configuration short (16 bit) to the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to write 
//          value = 16 bit value to write to EEPROM
//          note: 3 bytes of EEPROM space are used 
//
void TeensyUserInterface::writeConfigurationShort(int EEPromAddress, short value)
{
  byte *dataPntr;

  if (EEPROM.read(EEPromAddress) == 0xff)
    EEPROM.write(EEPromAddress, 0);

	dataPntr = (byte*) (&value);
  
  EEPROM.write(EEPromAddress + 1, dataPntr[0]);
  EEPROM.write(EEPromAddress + 2, dataPntr[1]);
}



//
// read a configuration short (16 bit) from the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to read from 
//          defaultValue = default value to return if value has never been 
//            written to the EEPROM
//          note: 3 bytes of EEPROM space are used 
//  Exit:   short value from EEPROM (or default value) returned
//
short TeensyUserInterface::readConfigurationShort(int EEPromAddress, short defaultValue)
{
  short value;
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    return(defaultValue);

  dataPntr = (byte*) (&value);

  dataPntr[0] = EEPROM.read(EEPromAddress + 1);
  dataPntr[1] = EEPROM.read(EEPromAddress + 2);  
  return(value);
}



//
// write a configuration int (32 bit) to the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to write 
//          value = 32 bit value to write to EEPROM
//          note: 5 bytes of EEPROM space are used 
//
void TeensyUserInterface::writeConfigurationInt(int EEPromAddress, int value)
{
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    EEPROM.write(EEPromAddress, 0);

  dataPntr = (byte*) (&value);
  
  EEPROM.write(EEPromAddress + 1, dataPntr[0]);
  EEPROM.write(EEPromAddress + 2, dataPntr[1]);
  EEPROM.write(EEPromAddress + 3, dataPntr[2]);
  EEPROM.write(EEPromAddress + 4, dataPntr[3]);
}



//
// read a configuration int (32 bit) from the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to read from 
//          defaultValue = default value to return if value has never been 
//            written to the EEPROM
//          note: 5 bytes of EEPROM space are used 
//  Exit:   long value from EEPROM (or default value) returned
//
int TeensyUserInterface::readConfigurationInt(int EEPromAddress, int defaultValue)
{
  int value;
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    return(defaultValue);

  dataPntr = (byte*) (&value);

  dataPntr[0] = EEPROM.read(EEPromAddress + 1);
  dataPntr[1] = EEPROM.read(EEPromAddress + 2);
  dataPntr[2] = EEPROM.read(EEPromAddress + 3);
  dataPntr[3] = EEPROM.read(EEPromAddress + 4);
  return(value);
}



//
// write a configuration float (32 bit) to the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to write 
//          value = 32 bit float to write to EEPROM
//          note: 5 bytes of EEPROM space are used 
//
void TeensyUserInterface::writeConfigurationFloat(int EEPromAddress, float value)
{
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    EEPROM.write(EEPromAddress, 0);

  dataPntr = (byte*) (&value);
  
  EEPROM.write(EEPromAddress + 1, dataPntr[0]);
  EEPROM.write(EEPromAddress + 2, dataPntr[1]);
  EEPROM.write(EEPromAddress + 3, dataPntr[2]);
  EEPROM.write(EEPromAddress + 4, dataPntr[3]);
}



//
// read a configuration float (32 bit) from the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to read from 
//          defaultValue = default value to return if value has never been 
//            written to the EEPROM
//          note: 5 bytes of EEPROM space are used 
//  Exit:   float value from EEPROM (or default value) returned
//
float TeensyUserInterface::readConfigurationFloat(int EEPromAddress, float defaultValue)
{
  float value;
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    return(defaultValue);

  dataPntr = (byte*) (&value);

  dataPntr[0] = EEPROM.read(EEPromAddress + 1);
  dataPntr[1] = EEPROM.read(EEPromAddress + 2);
  dataPntr[2] = EEPROM.read(EEPromAddress + 3);
  dataPntr[3] = EEPROM.read(EEPromAddress + 4);
  return(value);
}


// -------------------------------------- End --------------------------------------

/*
  The MIT License (MIT)

  library writen by Kris Kasprzak
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  On a personal note, if you develop an application or product using this library 
  and make millions of dollars, I'm happy for you!
	
*/

float degtorad = .0174532778;


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


arc shaped bar chart


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

BarChartA::BarChartA() { }

void BarChartA::init(uint16_t ArcRadius, uint16_t ArcCenterX, uint16_t BarWidth, uint16_t OffsetFromTop, uint16_t SweepAngle, uint8_t Segments, float ScaleLow, float ScaleHigh){

	rad = ArcRadius;
	xoffset = ArcCenterX;
	barwidth = BarWidth;
	topoffset= OffsetFromTop;
	sweepangle= SweepAngle;
	segments= Segments;
	low = ScaleLow;
	high = ScaleHigh;

}

void BarChartA::draw(float Value){
	
	yoffset = rad + topoffset;
	
	// very weird but displays +y is down so as opposed to handling that transform when drawing
	// i'm just rotating the draw transform coordinates by 270 degrees
	// this works and still honors the dislay.setRotation(1-4)
	// and as a result segment #0 is the clockwise most segment--hence we draw CW so reverse the draw order
	startangle = (gap / 2) + ((270.0f + (sweepangle / 2.0f)) * degtorad);
	arcangle = ((float)(sweepangle / (float)segments) * degtorad) - gap;
  
  
  	if (divider_1 < low){
		divider_1 = low;
	}

	if (divider_2 > high){
		divider_2 = high;
	}

	divider1 = map(divider_1, low, high, 0, bars);		
	divider2 = map(divider_2, low, high, 0, bars);
	
	
	for (i = segments; i > 0; i--) {
		drawangle = ((float) startangle - ((float)arcangle + gap) * (float) i); 
		p1x = ((rad)*cos(drawangle)) + xoffset;
		p1y = ((rad)*sin(drawangle)) + yoffset;
		p2x = ((rad - barwidth) * cos(drawangle)) + xoffset;
		p2y = ((rad - barwidth) * sin(drawangle)) + yoffset;
		p3x = ((rad - barwidth) * cos(drawangle + arcangle)) + xoffset;
		p3y = ((rad - barwidth) * sin(drawangle + arcangle)) + yoffset;
		p4x = ((rad)*cos(drawangle + arcangle)) + xoffset;
		p4y = ((rad)*sin(drawangle + arcangle)) + yoffset;

		// again another hack to get the colors drawing colockwise for increased Value
		// due to CCW most segment is the max segment count, but lowest scaled value
		tempval = MapFloat(Value, low, high, (float) segments + 1.0f, 0.0f);

		if (i > (divider1)) {
		  if (i > tempval) {
			barcolor = color_l;
		  } else {
			barcolor = color_v;
		  }
		} else if (i > (divider2)) {
		  if (i > tempval) {
			barcolor = color_m;
		  } else {
			barcolor = color_v;
		  }
		} else {
		  if (tempval < i) {
			barcolor = color_h;
		  } else {
			barcolor = color_v;
		  }
		}
		// display does not have a 4 point square so we're drawing 2 triangles
		// luckily the overlap between the triangles leaves no stray pixels
		lcd->fillTriangle(p1x, p1y, p2x, p2y, p3x, p3y, barcolor);
		lcd->fillTriangle(p3x, p3y, p4x, p4y, p1x, p1y, barcolor);
	}
	
}

// wanna reset the scale?
void BarChartA::setScale(float ScaleLow, float ScaleHigh){
	
	low = ScaleLow;
	high = ScaleHigh;
	
}

// need a different sweep angle, # of drawn segments, etc?
void BarChartA::setBars(uint16_t SweepAngle, uint16_t Segments, uint16_t BarWidth, float GapSize){
	
	sweepangle= SweepAngle;
	segments= Segments;
	barwidth = BarWidth;
	gap = GapSize;
	
}

// default is low=green, med=yellow, high=red and null is dark grey, change with this method
void BarChartA::setSectionColors(uint16_t ColorL, uint16_t ColorM, uint16_t ColorH, uint16_t ColorV){
	
	color_l= ColorL;
	color_m= ColorM;
	color_h = ColorH;	
	color_v = ColorV;
	
}

// default is 50 and 75% note Divider2 is not the difference from Divider2 but from 0. 
void BarChartA::setSectionSize(float Divider1, float Divider2){
	
	divider_1= Divider1;
	divider_2= Divider2;
		
}

// internal map--I sure wish Arduino platform did NOT make map with long ints
float BarChartA::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  if (in_min < 0) {
    in_max = in_max + abs(in_min);
    in_min = 0.0;
  }

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


horizontal bar chart


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

BarChartH::BarChartH() {}

void BarChartH::init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc, const char *Title, uint16_t TextColor, uint16_t BorderColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont ){

	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	gx = GraphXLoc;

	gy = GraphYLoc;
	gw = GraphWidth;
	gh = GraphHeight;
	
	divider = 1; // distance between bars (we are just going to reduce width)
	bars = (High - ScaleLow) / ScaleInc;	
	barwidth = gw / bars;	
	
	strncpy(titxt, Title, MAXCHARLEN-1);
	bartype = false;
	tf = TitleFont;
	sf = ScaleFont;
	tc = TextColor;
	oc = BorderColor;
	rc = BarColor;
	ac = BarBColor;
	bc = BackColor;

	redraw = true;

}

void BarChartH::draw(float val){
	
	stepval = (High-Low) / Inc;
		
	if (redraw == true) {
		redraw = false;		   

		// step val basically scales the hival and low val to the height
		// deducting a small value to eliminate round off errors
		// this val may need to be adjusted
		cnt = 0;
		
		if (ss){
			
			lcd->setFont(sf);

			// paint over previous y scale
			tHi = sf.cap_height * 2 + 8;
			lcd->fillRect(gx-10, gy + gh + 1, gw+30, tHi, bc);
			lcd->setTextColor(tc, bc);

			for (i = Low; i <= High; i += Inc) {

				lcd->drawFastVLine((cnt*barwidth) + gx, gy + gh+ 1,  5, tc);
				// draw lables
				if (High < .1) {
					Dec = 3;
				}
				else  if (High <= 1) {
					Dec = 2;
				}
				else  if (High <= 10) {
					Dec = 1;
				}
				else   {
					Dec = 0;
				}
				data =  i ;

				dtostrf(data, 0, Dec,text);
				tLen = lcd->strPixelLen(text) * 1.2;
				tHi =sf.cap_height;
				lcd->setCursor((cnt*barwidth) + gx - (tLen / 2) , gy + gh + 10);
				cnt++;
				lcd->print(text);
			}
		}

		if(st){
			lcd->setTextColor(tc, bc);
			lcd->setFont(tf);
			tHi = sf.cap_height * 2 + 12;
			lcd->setCursor(gx , gy -tHi );
			lcd->print(titxt);
		}

	}
	
	// compute level of bar graph that is scaled to the  height and the hi and low vals

	
  // draw the bar graph
  // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update
  
  if (!bartype){
	  
		if (val >= High) {
			val = High;
		}
		if (val <= Low) {
			val = Low;
		}
	  level = MapFloat( val, Low, High, gx, gx+gw);

	lcd->fillRect(level, gy + 1, gx+gw - level, gh - 2,ac);
	lcd->fillRect(gx, gy + 1 , level - gx,  gh - 2, rc);
	lcd->drawRect(gx , gy, gw, gh, oc);
  }
  else {
  
	stepval = MapFloat(val, Low, High, 0.0f, (float) bars);	 
	
	if (divider_1 < Low){
		divider_1 = Low;
	}

	if (divider_2 > High){
		divider_2 = High;
	}

	divider1 = map(divider_1, Low, High, 0, bars);		
	divider2 = map(divider_2, Low, High, 0, bars);
	
	for (i = 1; i < bars; i++){		

		if (i <= (divider1)){
			if (i <= stepval){
				barcolor = color_l;
			}
			else {
				barcolor = color_v;
			}
		}
		else if (i <= (divider2)){
			if (i <= stepval){
				barcolor = color_m;
			}
			else {
				barcolor = color_v;
			}
		}
		else if (i <= bars){
			if (i <= stepval){
				barcolor = color_h;
			}
			else {
				barcolor = color_v;
			}
		}
		else {
			barcolor = color_v;
		}
		
		lcd->fillRect(gx + (i*(barwidth + divider)), gy, barwidth, gh,barcolor);
	}

  }

}

void BarChartH::setBarColor(uint16_t BarColor){	
	rc = BarColor;		
}

void BarChartH::refresh(){
	
	redraw = true;
		
}


void BarChartH::setScale(float ScaleLow, float ScaleHigh, float ScaleInc){
	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	redraw = true;
}

void BarChartH::showTitle(bool val){	
	st = val;		
}

void BarChartH::showScale(bool val){
	
	ss = val;
		
}

void BarChartH::useSegmentBars(bool val){
	bartype = val;
}

void BarChartH::setBars(uint16_t NumberofBars,float BarWidth, uint8_t DividerSize){
	divider = DividerSize;
	barwidth = BarWidth;	
	bars = NumberofBars;

}
void BarChartH::setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV){	
	color_l = ColorL;
	color_m = ColorM;
	color_h = ColorH;
	color_v = ColorV;	
}
void BarChartH::setSectionSize(float Divider1, float Divider2){	
	divider_1 = Divider1;
	divider_2 = Divider2;
}

void BarChartH::setSize(uint16_t Left, uint16_t Top, uint16_t Wide, uint16_t High, uint8_t Divider){	
	gx = Left;
	gy = Top;
	gw = Wide;
	gh = High;
	divider = Divider;	
}

float BarChartH::getBars(){	
	return bars;
}
float BarChartH::getActualWidth(){	
	return (barwidth+divider) * bars;
}


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


vertical bar chart


*///////////////////////////////////////////////////////////////////////////////////////////////////////////
BarChartV::BarChartV() {}

void BarChartV::init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc, const char *Title, uint16_t TextColor,uint16_t BorderColor, uint16_t BarColor, uint16_t BarBlankColor,uint16_t BackgroundColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont ){

	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	gx = GraphXLoc;
	gy = GraphYLoc;
	gw = GraphWidth;
	gh = GraphHeight;
	strncpy(ti, Title, MAXCHARLEN-1);
	bartype = false;
	divider = 1; // distance between bars (we are just going to reduce width)
	bars = (High - Low) / Inc;	
	barheight = gh / bars;	
	
	tf = TitleFont;
	sf = ScaleFont;
	tc = TextColor;
	oc = BorderColor;
	rc = BarColor;
	ac = BarBlankColor;
	bc = BackgroundColor;
	redraw = true;
	
}


void BarChartV::draw(float val){
	
	stepval = (High-Low) / Inc;

	if (redraw == true) {
		redraw = false;	   

		// step val basically scales the hival and low val to the height
		// deducting a small value to eliminate round off errors
		// this val may need to be adjusted
		cnt = 0;
		
		if (ss) {
			lcd->setFont(sf);
			stepval = (High - Low) / Inc;
			
			// paint over previous y scale
			lcd->fillRect(gx + gw, gy - gh - 15, 70, gh + 30, bc);
			lcd->setTextColor(tc, bc);
			for (i = 0; i <= stepval; i++) {
				
				TempY =  gy - ((gh / stepval) * i);

				lcd->drawFastHLine(gx + gw , TempY,  5, tc);
				data = Low + (Inc * i);
				// draw lables
		 
				if (Inc < .1) {
					Dec = 2;
				}
				else  if (Inc < 1) {
					Dec = 1;
				}
				else  if (Inc < 10) {
					Dec = 0;
				}
				else   {
					Dec = 0;
				}

				dtostrf(data, 0, Dec,text);
				tLen = lcd->strPixelLen(text) * 1.2;
				tHi =sf.cap_height;
				lcd->setCursor(gx + gw + 12, TempY - (tHi / 2) );
				lcd->print(text);
			}
		}
		if (st){
			lcd->setTextColor(tc, bc);
			lcd->setFont(tf);
			tHi =sf.cap_height + 15;
			lcd->setCursor(gx , gy - gh -tHi );
			lcd->print(ti);
		}
	}
	// compute level of bar graph that is scaled to the  height and the hi and low vals
	// this is needed to accompdate for +/- range
	if (!bartype){
		if (val >= High) {
			val = High;
		}
		if (val <= Low) {
			val = Low;
		}
		level = (gh * (((val - Low) / (High - Low))));

		// draw the bar graph
		// write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update

		lcd->fillRect(gx+1, gy - gh, gw - 2, gh - level, ac);
		lcd->fillRect(gx+1, gy - level , gw - 2,  level, rc);
		lcd->drawRect(gx , gy - gh - 1 , gw, gh+2, oc);
	}
	else {
		
		if (divider_1 < Low){
			divider_1 = Low;
		}

		if (divider_2 > High){
			divider_2 = High;
		}

		divider1 = map(divider_1, Low, High, 0, bars);		
		divider2 = map(divider_2, Low, High, 0, bars);

		stepval = MapFloat(val, Low, High, 0.0f, (float) bars);	
			
		for (int i = 1; i <= bars; i++){		

			if (i <= (divider1)){
				if (i <= stepval){
				barcolor = color_l;
				}
				else {
					barcolor = color_v;
				}
			}
			else if (i <= (divider2)){
				if (i <= stepval){
				barcolor = color_m;
				}
				else {
					barcolor = color_v;
				}
			}
			else if (i <= bars){
				if (i <= stepval){
				barcolor = color_h;
				}
				else {
					barcolor = color_v;
				}
			}
			else {
				barcolor = color_v;
			}		
			lcd->fillRect(gx , gy - (i*(barheight + divider)), gw, barheight, barcolor);

		}		
		
	}
}

void BarChartV::refresh(){
	
	redraw = true;
		
}

void BarChartV::setBarColor(uint16_t BarColor){
	
	rc = BarColor;
		
}

void BarChartV::showTitle(bool val){
	
	st = val;
		
}

void BarChartV::showScale(bool val){
	
	ss = val;
		
}


void BarChartV::setScale(float ScaleLow, float ScaleHigh, float ScaleInc){
	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	redraw = true;
	gh = (barheight+divider) * bars;
}

void BarChartV::useSegmentBars(bool val){
	bartype = val;
}

void BarChartV::setBars(uint16_t NumberofBars,uint8_t BarHeight, uint8_t DividerSize){
	bars = NumberofBars;
	divider = DividerSize;
	barheight = BarHeight;	
	
}
void BarChartV::setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV){	
	color_l = ColorL;
	color_m = ColorM;
	color_h = ColorH;
	color_v = ColorV;	
}

void BarChartV::setSectionSize(float Divider1, float Divider2){	
	
	divider_1 = Divider1;		
	divider_2 = Divider2;		
	
}

void BarChartV::setSize(uint16_t Left, uint16_t Top, uint16_t Wide, uint16_t High, uint8_t Divider){	
	gx = Left;
	gy = Top;
	gw = Wide;
	gh = High;
	divider = Divider;	
}

float BarChartV::getBars(){	
	return bars;
}
float BarChartV::getActualHeight(){	
	return (barheight+divider) * bars;
}
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


cartesian style graphing functions


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

CGraph::CGraph(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float XAxisLow, float XAxisHigh, float XAxisInc, float YAxisLow, float YAxisHigh, float YAxisInc){

		XLow = XAxisLow;
		XHigh = XAxisHigh;
		XInc = XAxisInc;

		YLow = YAxisLow;
		YHigh = YAxisHigh;
		YInc = YAxisInc;

		gx = GraphXLoc;
		gy = GraphYLoc;
		gw = GraphWidth;
		gh = GraphHeight;
		
}

void CGraph::init(const char *Title, const char *XAxis, const char *YAxis, uint16_t TextColor, uint16_t GridColor, uint16_t AxisColor, uint16_t BackColor, uint16_t PlotColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &AxisFont ){

	strncpy(title, Title, MAXCHARLEN-1);
	strncpy(xatitle, XAxis, MAXCHARLEN-1);
	strncpy(yatitle, YAxis, MAXCHARLEN-1);	

	tf = TitleFont;
	af = AxisFont;
	tc = TextColor;

	gc = GridColor;
	ac = AxisColor;
	bc = BackColor;
	pc = PlotColor;

	st = true;
	sl = true;
	sal = true;
	sxs = true;
	sys = true;
	
	XScaleOffset = 5;
	YScaleOffset = 40;
	XTextScale = 1.0;
	YlegendOffset = 0;

	Delta = XHigh - XLow;

	RedrawGraph = true;

	TextHeight = tf.cap_height;
	ID = 0;
	}

int CGraph::add(const char *name, uint16_t color){
		
	if (ID > MAXOPTIONS){
		return -1;
	}
	pdia[ID] = 0;
	linet[ID] = 1;	
	dl[ID] = name;
	dc[ID] = color;
	HaveFirstPoint[ID] = false;
	ID++;	
	return ID-1;
	
}

void CGraph::setX(float xpoint){

		x = xpoint;

}

void CGraph::setLineColor(int ID, uint16_t LineColor){
		
	dc[ID] = LineColor;
	
}

void CGraph::setYLegendOffset(int val){
	
	YlegendOffset = val;
	
}

void CGraph::showAxisLabels(bool val){

		sal = val;

}

void CGraph::plot(int cID, float y){
	
	if (RedrawGraph){
		RedrawGraph = false;
		drawGraph();
	}
	if (y != y){
		y = YLow;
	}
	// plot the data
	XPoint = MapFloat(x, XLow, XHigh, gx, gx + gw);
	YPoint = MapFloat(y, YHigh, YLow, gy - gh, gy);

	
	if ((YPoint > gy)) {
		YPoint = gy;
	}
	if (YPoint < (gy - gh)) {	
		YPoint = gy - gh;
	}
	
	if ((XPoint > gx) && (XPoint < gx + gw)) {
		if (HaveFirstPoint[cID]){
			for(j = 0; j < linet[cID]; j++){
				lcd->drawLine(oXPoint[cID], oYPoint[cID]+j, XPoint, YPoint+j, dc[cID]);				
			}

			if ( pdia[cID] > 1){
				lcd->fillCircle(XPoint, YPoint, pdia[cID],  dc[cID]);
			}
		}
	}

	HaveFirstPoint[cID] = true;
	oYPoint[cID] = YPoint;
	oXPoint[cID] = XPoint;

	// test to see if we need to redraw
	if (XPoint + 2 > gx + gw) {
		Delta = XHigh - (XLow);
		XLow = XHigh;
		XHigh = XHigh + Delta;
		RedrawGraph = true;
		HaveFirstPoint[cID] = false;	
	}
 
}

void CGraph::setMarkerSize(int cID, byte val){

	pdia[cID] = val;

}

void CGraph::setLineThickness(int cID, byte val){
	
	linet[cID] = val;

}

void CGraph::setTitle(const char *Title){

	strncpy(title, Title, MAXCHARLEN-1);

}
/*
void CGraph::setXAxisName(const char *XAxis){

	strncpy(xatitle, XAxis, MAXCHARLEN-1);

}
*/
void CGraph::setXTextOffset(int val){
	
	XScaleOffset = val;

}

void CGraph::setYTextOffset(int val){
	
	YScaleOffset = val;
}

void CGraph::setXTextScale(float val){
	
	XTextScale = val;
}
/*
void CGraph::setYAxisName(const char *YAxis){

	strncpy(yatitle, YAxis, MAXCHARLEN-1);

}
*/
void CGraph::resetStart(int ID){

	HaveFirstPoint[ID] = false;

}


void CGraph::drawLegend(byte Location){

	tl = Location;

}

void CGraph::drawGraph() {

	RedrawGraph = false;

	float xDiv =  ((XHigh-XLow)/XInc);
	float yDiv = ((YHigh-YLow)/YInc);
	float ylen = gh /  yDiv;
	float xlen = gw / xDiv;

	 lcd->setTextColor(tc, bc);

	// draw title
	if (st){
		lcd->setFont(tf);
		lcd->setCursor(gx, gy - gh - TextHeight-10);
		lcd->print(title);
	}

	// draw grid lines
	// first blank out xscale for redrawing
	lcd->fillRect(gx-10, gy+2, gw+20,25, bc);
	lcd->setFont(af);
	lcd->fillRect(gx, gy - gh-4, gw, gh+8, bc);
	lcd->fillRect(gx, gy - gh, gw, gh, pc);

	// draw vertical lines
	for (j = 0; j <= xDiv; j++) {
		if(j > 0){
			lcd->drawFastVLine(gx + ((0+j) * xlen), gy - gh, gh, gc);
		}
		if (xDiv < .1) {
			XDec = 2;
		}
		else if (xDiv < 1) {
			XDec = 1;
		}
		else {
			XDec = 0;
		}
		
		//hard code since not working for < 10, fix later
		XDec = 1;
		
		//get text offsets

		if (sxs){
			dtostrf((XLow+(XInc*j)) * XTextScale, 0, XDec,text);
			
      #if __has_include("ST7796_t3.h")
         uint16_t _txtWidth = lcd->strPixelLen(text);
         lcd->setCursor(gx + 10 + (j * xlen) - _txtWidth, gy+XScaleOffset);
      #else      
        lcd->setCursor(gx + (j * xlen)- (d->measureTextWidth(text)/2), gy+XScaleOffset);
			#endif
			
			lcd->print(text);
			
		}
	}

	lcd->fillRect(gx-30,  gy -gh-TextHeight+4, 27, gh+TextHeight, bc);

	// draw horizontal lines
	for (i = 0; i <= yDiv; i++) {

		lcd->drawFastHLine(gx, gy - (ylen * (0+i)), gw, gc);

		if (YInc < .1) {
			YDec = 2;
		}
		else if (YInc < 1) {
			YDec = 1;
		}
		else {
			YDec = 0;
		}
		if (sys){
			
			dtostrf(YLow+(YInc*i), 0, YDec,text);
      #if __has_include("ST7796_t3.h")
        uint16_t _txtWidth = lcd->strPixelLen(text);
        lcd->setCursor(gx-YScaleOffset, gy - (ylen * i)-(_txtWidth/2));
      #else
        lcd->setCursor(gx-YScaleOffset, gy - (ylen * i)-(lcd->measureTextHeight(text)/2));
			#endif
			//lcd->setCursor(gx + (j * xlen)- (lcd->measureTextWidth(text)/2), gy+XScaleOffset);
			
			
			lcd->print(text);

		}
	}

	// put the y axis at the zero point
	if ((YLow < 0) &&  (YHigh > 0)) {
		YPoint = MapFloat(0, YHigh, YLow, gy - gh, gy);
	}
	else {
		YPoint = MapFloat(YLow, YHigh, YLow, gy - gh, gy);
	}

	
	lcd->drawFastHLine(gx,  YPoint, gw, ac);
	lcd->drawFastHLine(gx,  YPoint-1, gw, ac);

	lcd->drawFastVLine(gx-1, gy - gh, gh+1, ac);
	lcd->drawFastVLine(gx-2, gy - gh, gh+1, ac);
	
	// draw legend
	if (sal){
		// draw y label
		oOrientation = lcd->getRotation();
		lcd->setTextColor(tc, bc);
		lcd->setRotation(oOrientation - 1);
		lcd->setCursor(lcd->width()-gy,gx-44);	
		lcd->print(yatitle);
		lcd->setRotation(oOrientation);
		//Serial.println(yatitle);

    	// draw x label
		lcd->setTextColor(tc, bc);
		lcd->setCursor(gx,gy+ TextHeight + 5);	
		lcd->print(xatitle);

		//Serial.println(xatitle);

	}
	if (sl) {
		// draw legend
		StartPointX = gx-20;
		
    #if __has_include("ST7796_t3.h")
      uint16_t _txtWidth = lcd->strPixelLen(xatitle);
    	StartPointX = gx + _txtWidth + 10;
    #else
      StartPointX = gx + lcd->measureTextWidth(xatitle) + 10;
		#endif			
		if (tl == LOCATION_TOP){
			StartPointY = gy - gh;
		}
		else if (tl == LOCATION_BOTTOM) {
			StartPointY = gy+ TextHeight + YlegendOffset;	
		}

		for (k = 0; k <= ID; k++){
			lcd->setCursor(StartPointX, StartPointY);
			lcd->print(dl[k]);
			StartPointX = lcd->getCursorX();
			lcd->fillRect(StartPointX+3, StartPointY+TextHeight/4 , 20, linet[k], dc[k]);

			StartPointX += 30;
		}
		
	}
	
}

void CGraph::setYAxis(float Ylow, float Yhigh, float Yinc){

	YLow = Ylow;
	YHigh = Yhigh;
	YInc = Yinc;
	RedrawGraph = true;
}

void CGraph::setXAxis(float XAxisLow, float XAxisHigh, float XAxisInc){

	XLow = XAxisLow;
	XHigh = XAxisHigh;
	XInc = XAxisInc;
	RedrawGraph = true;
}

void CGraph::showTitle(bool val){
	st = val;
}

void CGraph::showLegend(bool val){

	sl = val;
}
void CGraph::showXScale(bool val){
	sxs = val;
}
void CGraph::showYScale(bool val){
	sys = val;
}


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


dial type control


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

Dial::Dial(int CenterX, int CenterY, int DialRadius, float LowVal , float HiVal , float ValInc, float SweepAngle){

	degtorad = .0174532778;

	cx = CenterX;
	cy = CenterY;
	dr = DialRadius;
	lv = LowVal;
	hv = HiVal;
	inc = ValInc;
	sa = SweepAngle;
	
	// store initial values
	// this looks silly but we don't have needle size calculated yet but to keep needle from first draw being weird, just make a dot at the center
	px = CenterX;
	py = CenterY;
	pix = CenterX;
	piy = CenterY;
	plx = CenterX;
	ply = CenterY;
	prx = CenterX;
	pry = CenterY;

	Redraw = true;

}

void Dial::init(uint16_t NeedleColor, uint16_t DialColor, uint16_t TextColor, uint16_t TickColor, const char *Title, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &DataFont ) {

	tf = TitleFont;
	df = DataFont;
	strncpy(t, Title, 10);
	nc = NeedleColor;
	dc = DialColor;
	tc = TextColor;
	ic = TickColor;
	
}

void Dial::draw(float Val) {


	// draw the dial only one time--this will minimize flicker
	if ( Redraw == true) {
		Redraw = false;
		lcd->fillCircle(cx, cy, dr , dc);
		lcd->drawCircle(cx, cy, dr, ic);
		lcd->drawCircle(cx, cy, dr - 1, ic);
	}

	// draw the current value
	lcd->setFont(df);
	lcd->setTextColor(tc, dc);
	lcd->setCursor(cx - 25, cy + 20 );
	//disp.println(Format(curval, dig, dec));
	// center the scale about the vertical axis--and use this to offset the needle, and scale text
	offset = (270 +  (sa / 2)) * degtorad;
	// find hte scale step value based on the hival low val and the scale sweep angle
	// deducting a small value to eliminate round off errors
	// this val may need to be adjusted
	stepval = (inc) * (sa / (float (hv - lv)));
	// draw the scale and numbers
	// note draw this each time to repaint where the needle was
		
	// first draw the previous needle in dial color to hide it
	lcd->fillTriangle (pix, piy,plx, ply, prx, pry, dc);

	for (i = 0.0; i <= (sa + 0.0001); i += stepval) {

		angle = ( i  * degtorad);
		angle = offset - angle ;
		ox =  (float) (dr - 2) * cos(angle) + cx;
		oy =  (float) (dr - 2) * sin(angle) + cy;
		ix =  (float) (dr - 10) * cos(angle) + cx;
		iy =  (float) (dr - 10) * sin(angle) + cy;
		tx =  (float) (dr - 30) * cos(angle) + cx;
		ty =  (float) (dr - 30) * sin(angle) + cy;
		dx =  (float) (dr - 20) * cos(angle) + cx;
		dy =  (float) (dr - 20) * sin(angle) + cy;

		lcd->drawLine(ox, oy, ix, iy, ic);

	
		if (hv < .1) {
			dec = 3;
		}
		else  if (hv <= 1) {
			dec = 2;
		}
		else  if (hv <= 10) {
			dec = 1;
		}
		else   {
			dec = 0;
		}

		data = hv - ( i * (inc / stepval)) ;
		dtostrf(data, 0, dec,buf);
		tLen = lcd->strPixelLen(buf);
		tHi = df.cap_height;
		lcd->setCursor(dx - (tLen/2), dy - (tHi/2));
	
		lcd->print(buf);
	

	}

	// compute and draw the needle
	angle = (sa * (1.0 - (((Val - lv) / (hv - lv)))));
	angle = angle * degtorad;
	angle = offset - angle  ;

	// draw a triangle for the needle (compute and store 3 vertiticies)
	// 5.0 is width of needle at center
	ix =  (float)(dr - 10.0) * cos(angle) + cx;
	iy =  (float)(dr - 10.0) * sin(angle) + cy;
	lx =  6.0 * cos(angle - 90.0 * degtorad) + cx;
	ly =  6.0 * sin(angle - 90.0 * degtorad) + cy;
	rx =  6.0 * cos(angle + 90.0 * degtorad) + cx;
	ry =  6.0 * sin(angle + 90.0 * degtorad) + cy;
		
	// then draw the new needle in need color to display it
	lcd->fillTriangle (ix, iy, lx, ly, rx, ry, nc);

	// draw a cute little dial center
	lcd->fillCircle(cx, cy, 8, tc);

	// print the title
	lcd->setTextColor(tc, dc);
	lcd->setFont(tf);
	tLen = lcd->strPixelLen(t);
			
	lcd->setCursor(cx - tLen/2, cy + 10);
	lcd->println(t);
	
	//save all current to old so the previous dial can be hidden
	pix = ix;
	piy = iy;
	plx = lx;
	ply = ly;
	prx = rx;
	pry = ry;

}


/*

class for a vertical type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/

SliderV::SliderV()
{
}



/*

initializer for the vertical slider, here we pass in the scale values

note user can specify a scale (0 for none), and a snap capability (0 for none). You can draw a 0-100 scale with tick marks every 25 but snap to each integer (1,2,3...)

*/

void SliderV::init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderH, float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize,uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor ) {
    
	// map arguements to class variables
	sl = ScaleLow;
	sh = ScaleHi;
	sc = 0.0;
	sn = 0.0;
	oy = -1;
	colorscale = true;
	handlesize = SLIDER_HANDLE_SIZE;
	handlewidth = handlesize / 2;
	handleshape = HANDLE_CIRCLE;
	enabled = true;
	visible = true;
	l = SliderX;
	t = SliderY;
	h = SliderH;
	bt = 3;
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;
	debounce = TFT_DEBOUNCE;
	dsColor = C_DISABLE_MED;
	dhColor = C_DISABLE_LIGHT;

	// compute scale increments and snap increments
	if (ScaleSize != 0) {
		sc = (sh - sl) / ScaleSize;
		ce = abs(sh / ScaleSize);
	}
	if (SnapSize != 0) {
		sn = (sh - sl) / SnapSize;
	}

}


void SliderV::setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

	// map arguements to class variables
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;

}

/*

method to set hust the handle color, this is useful for drawing handle in warning color

*/

/*

method to draw the slider

blank out old handle scale (needed if user calls draw to refresh the slider)
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/
void  SliderV::draw(float val) {

	
	if  (!visible) {

		if (!visible) {
			lcd->fillRect(l - handlesize , t - (handlesize / 2), handlesize * 2, h+ handlesize, bColor); // erase it
		}
		return;
	}

	value = val;

	if (enabled) {
		tsColor = sColor;
		thColor = hColor;
	}
	else {
		tsColor = dsColor;
		thColor = dhColor;
	}

	pos = MapFloat(value, (float) sl, (float)sh, (float)(t + h), (float)t );

	// blannk ouut any previous--note draw can be called w/o slide in cases where sliders are tied together
	
	if ((pos != oy) && (oy >= 0)) {
				// blank out the old one
			if (handleshape == HANDLE_CIRCLE) {
				lcd->fillCircle(l, oy, handlesize/2, bColor);
			}
			else if (handleshape == HANDLE_SQUARE) {
				lcd->fillRect(l - (handlesize / 2), oy- (handlesize / 2), handlesize, handlesize, bColor);
			}
			else if (handleshape == HANDLE_TRIANGLE_1) {
				// weird i know but need to draw the black out slightly larger due to round off errors
				lcd->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
			}
			else if (handleshape == HANDLE_TRIANGLE_2) {
				// weird i know but need to draw the black out slightly larger due to round off errors
				lcd->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
			}
			else if (handleshape == HANDLE_TRIANGLE_3) {
				// weird i know but need to draw the black out slightly larger due to round off errors
				lcd->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
				lcd->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
			}
			else if (handleshape == HANDLE_RECTANGLE) {
				// weird i know but need to draw the black out slightly larger due to round off errors
				lcd->fillRect(l - (handlesize / 2), oy- (handlewidth / 2), handlesize, handlewidth, bColor);
			}
				
	}

	if (colorscale){
		lcd->fillRect(l - (bt/2) , t, bt, pos - t, tsColor); // draw new slider
		lcd->fillRect(l - (bt/2) , pos, bt, h - pos + t, thColor); // draw new slider
	}
	else {
		lcd->fillRect(l - (bt/2), t, bt, h, tsColor);
	}
	
	if (sc != 0.0) {
		for (i = 0; i <= sc; i++){

			lcd->fillRect(l - 3,(i * (h / sc) ) + t, 7, 1, tsColor);

			if ((i == ce) || (i == 0) || (i == sc)) {
				lcd->fillRect(l - 3, (i * (h / sc)) + t, 7, 4, tsColor);
			}
		}
	}




	if (handleshape == HANDLE_CIRCLE) {
		lcd->fillCircle(l,pos,handlesize/2,thColor);
		lcd->drawCircle(l,pos,handlesize/2,tsColor);
		lcd->drawCircle(l,pos,(handlesize/2)-1,tsColor);
	}
	else if (handleshape == HANDLE_SQUARE) {
		lcd->fillRect(l - (handlesize / 2), pos- (handlesize / 2), handlesize, handlesize, thColor);
		lcd->drawRect(l - (handlesize / 2),pos- (handlesize / 2),handlesize, handlesize,tsColor);
		lcd->drawRect(l - (handlesize / 2) +1,pos- (handlesize / 2)+1,handlesize-2,handlesize-2,tsColor);
	}

	else if (handleshape == HANDLE_TRIANGLE_1) {
		lcd->fillTriangle(l - handlesize,	  pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , thColor);
		lcd->drawTriangle(l - handlesize,   pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_2) {
		lcd->fillTriangle(l + handlesize,	  pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , thColor);
		lcd->drawTriangle(l + handlesize,   pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_3) {
		lcd->fillTriangle(l - handlesize,	  pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , thColor);
		lcd->fillTriangle(l + handlesize,	  pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , thColor);
		lcd->drawTriangle(l - handlesize,   pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , tsColor);
		lcd->drawTriangle(l + handlesize,   pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , tsColor);
	}
	else if (handleshape == HANDLE_RECTANGLE) {
		lcd->fillRect(l - (handlesize / 2),   pos- (handlewidth / 2),  handlesize,  handlewidth,  thColor);
		lcd->drawRect(l - (handlesize / 2),   pos- (handlewidth / 2),  handlesize,  handlewidth,  tsColor);
		lcd->drawRect(l - (handlesize / 2) +1,pos- (handlewidth / 2)+1,handlesize-2,handlewidth-2,tsColor);
	}
	oy = pos;

	pos = value;
	
		   
}

/*

method to more the slider
optionally get the snap increment
see if screen press is in controls range
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

bool SliderV::slide(uint16_t ScreenX, uint16_t ScreenY){
	
	bool pressed = false;

	x = ScreenX;
	y = ScreenY;

	if ((!enabled) || (!visible)) {
		//return pos;
		return pressed;
	}

	if (sn != 0.0) {
		y = y - t;
		y =  (y /  (h / sn));
		y = (y *  (h / sn)) + t;
	}

	if (y != oy){
		
		if (abs(x -l ) <= handlesize) {
			
			if ((y >= t) & (y <= (t + h))) {
				pressed = true;
	
				// it's in rage of ball

				// blank out the old one
				if (handleshape == HANDLE_CIRCLE) {
					lcd->fillCircle(l, oy, handlesize/2, bColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					lcd->fillRect(l - (handlesize / 2), oy- (handlesize / 2), handlesize, handlesize, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					// weird i know but need to draw the black out slightly larger due to round off errors
					lcd->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					// weird i know but need to draw the black out slightly larger due to round off errors
					lcd->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					// weird i know but need to draw the black out slightly larger due to round off errors
					lcd->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
					lcd->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
				}
				else if (handleshape == HANDLE_RECTANGLE) {
					lcd->fillRect(l - (handlesize / 2), oy- (handlewidth / 2), handlesize, handlewidth, bColor);
				}
				// draw slider 
				if (colorscale){
					lcd->fillRect(l - (bt/2) , t, bt, y - t, sColor); // draw new slider
					lcd->fillRect(l - (bt/2) , y, bt, h - y + t, hColor); // draw new slider
				}
				else {
					lcd->fillRect(l - (bt/2), t, bt, h, sColor);
				}
	
				// draw tick marks
				if (sc != 0.0) {
					for (i = 0; i <= sc; i++){

						lcd->fillRect(l - 3,(i * (h / sc) ) + t, 7, 1, sColor);

						if ((i == ce) | (i == 0) | (i == sc)) {
							lcd->fillRect(l - 3, (i * (h / sc)) + t, 7, 4, sColor);
						}
					}
				}
				// draw new handle
				if (handleshape == HANDLE_CIRCLE) {
					lcd->fillCircle(l,y,handlesize/2,hColor);
					lcd->drawCircle(l,y,handlesize/2,sColor);
					lcd->drawCircle(l,y,(handlesize/2)-1,sColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					lcd->fillRect(l - (handlesize / 2), y- (handlesize / 2), handlesize, handlesize, hColor);
					lcd->drawRect(l - (handlesize / 2),y- (handlesize / 2),handlesize, handlesize,sColor);
					lcd->drawRect(l - (handlesize / 2) +1,y- (handlesize / 2)+1,handlesize-2,handlesize-2,sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					lcd->fillTriangle(l - handlesize,	  y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					lcd->drawTriangle(l - handlesize,   y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					lcd->fillTriangle(l + handlesize,	  y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					lcd->drawTriangle(l + handlesize,   y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					lcd->fillTriangle(l - handlesize,	  y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					lcd->fillTriangle(l + handlesize,	  y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					lcd->drawTriangle(l - handlesize,   y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
					lcd->drawTriangle(l + handlesize,   y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
				}
				else if (handleshape == HANDLE_RECTANGLE) {
					lcd->fillRect(l - (handlesize / 2),   y- (handlewidth / 2),  handlesize,   handlewidth, hColor);
					lcd->drawRect(l - (handlesize / 2),   y- (handlewidth / 2),  handlesize,   handlewidth,sColor);
					lcd->drawRect(l - (handlesize / 2) +1,y- (handlewidth / 2)+1,handlesize-2, handlewidth-2,sColor);
				}
				oy = y;

				// get scaled val and pass back and store in the public variable--in case anyone needs easy access...
				
				pos = MapFloat(y, t, (t + h), sh, sl);
				value = pos;
				delay(debounce);
			}
		}
	}
	//return pos;
	return pressed;
}


/*

method to set hust the handle color, this is useful for drawing handle in warning color

*/
void  SliderV::setHandleColor(uint16_t HandleColor) {

	hColor = HandleColor;

}

void SliderV::drawSliderColor(bool state){

	colorscale = state;

}

void SliderV::setBarThickness(byte Thickness){

	bt = Thickness;


}

void  SliderV::setDisableColor(uint16_t HandleColor, uint16_t SliderColor) {

	dsColor = SliderColor;
	dhColor = HandleColor;

}

void SliderV::setScale(float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize ) {
    
	// map arguements to class variables
	sl = ScaleLow;
	sh = ScaleHi;
	sc = 0.0;
	sn = 0.0;
	// compute scale increments and snap increments
	if (ScaleSize != 0) {
		sc = (sh - sl) / ScaleSize;
		ce = abs(sh / ScaleSize);
	}
	if (SnapSize != 0) {
		sn = (sh - sl) / SnapSize;
	}

}


void  SliderV::enable() {

	enabled = true;

}

void  SliderV::disable() {
	enabled = false;

}

void  SliderV::show() {

	visible = true;

}

void  SliderV::hide() {

	visible = false;
	
}

void SliderV::setHandleSize(int value){

	if (value < 4) {
		handlesize = 4;
	}
	else if (value > 100) {
		handlesize = 100;
	}
	else {
		handlesize = value;
	}

}

void SliderV::setHandleSize(int value, int width){

	if (value < 4) {
		handlesize = 4;
	}
	else if (value > 40) {
		handlesize = 40;
	}
	else {
		handlesize = value;
	}

	if (width < 4) {
		handlewidth = 4;
	}
	else if (width > 100) {
		handlewidth = 100;
	}
	else {
		handlewidth= width;
	}

}
void SliderV::setHandleShape(byte value){

	if (value < 0) {
		handleshape = HANDLE_CIRCLE;
	}
	else if (value > 6) {
		handleshape = HANDLE_CIRCLE;
	}
	else {
		handleshape = value;
	}

}
void SliderV::setPressDebounce(byte Debounce) { 
	debounce = Debounce;
}





/*

class for a horizontal type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/
SliderH::SliderH()
{
}

void SliderH::init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor ) {
	// map arguements to class variables
	sl = ScaleLow;
	sh = ScaleHi;
	sc = 0.0;
	sn = 0.0;
	l = SliderX;
	t = SliderY;
	w = SliderW;
	bt = 3;
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;
	debounce = TFT_DEBOUNCE;
	dsColor = C_DISABLE_MED;
	dhColor = C_DISABLE_LIGHT;
	enabled = true;
	visible = true;
	ox = -1;
	colorscale = true;
	handlesize = SLIDER_HANDLE_SIZE;
	handlewidth = handlesize / 2;
	handleshape = HANDLE_CIRCLE;

	if (ScaleSize != 0) {
		sc =  (sh - sl ) /  ScaleSize ;
		ce = abs(sl / ScaleSize);
	}
	if (SnapSize != 0) {
		sn = (sh - sl) / SnapSize;
	}
	
}


void SliderH::resetScale(float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize) {

	sl = ScaleLow;
	sh = ScaleHi;
	sc = 0.0;
	sn = 0.0;
	if (ScaleSize != 0) {
		sc =  (sh - sl ) /  ScaleSize ;
		ce = abs(sl / ScaleSize);
	}
	if (SnapSize != 0) {
		sn = (sh - sl) / SnapSize;
	}

}


/*

method to set colors, this is useful for disabling a slider by setting it to a dimmed color and ignoring screen presses

*/

void SliderH::setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

	// map arguements to class variables
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;

}

/*

method to set colors, this is useful for disabling a slider by setting it to a dimmed color and ignoring screen presses

*/


void  SliderH::setHandleColor(uint16_t HandleColor) {

	hColor = HandleColor;

}

void  SliderH::setBarThickness(byte Thickness) {

	bt = Thickness;

}

void  SliderH::setDisableColor(uint16_t DisableHandleColor, uint16_t DisableSliderColor) {

	dsColor = DisableSliderColor;
	dhColor = DisableHandleColor;
}

void  SliderH::enable() {

	enabled = true;

}

void  SliderH::disable() {

	enabled = false;


}

void  SliderH::show() {

	visible = true;

}

void  SliderH::hide() {

	visible = false;

}


/*

method to draw the horizontal slider

blank out old handle scale (needed if user calls draw to refresh the slider)
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original

*/
void  SliderH::draw(float val) {
	
	if (!visible){

		if(!visible){
			lcd->fillRect(l - handlesize , t - handlesize, w + (2*handlesize), handlesize * handlesize, bColor);
		}
		// no need to draw anything
		return;
	}

	value = val;
	
	if (enabled) {
		tsColor = sColor;
		thColor = hColor;
		ssColor = sColor;
	}
	else {
		tsColor = dsColor;
		thColor = dhColor;
		ssColor = dsColor;
	}




	////////////////////////

	// draw the slider
	pos = MapFloat(value, (float) sl, (float)sh, (float) l, (float)(w+l) );
	//////////////////////////
	// seems odd to blank out old but of draw is called in .ino, need to clean up old stuff
	if ((ox != pos) && (ox > 0)){
		if (handleshape == HANDLE_CIRCLE) {
			lcd->fillCircle(ox, t, handlesize/2, bColor);
		}
		else if (handleshape == HANDLE_SQUARE) {
			lcd->fillRect(ox- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_1) {
			lcd->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_2) {
			lcd->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_3) {
			lcd->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
			lcd->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
		}
		else if (handleshape == HANDLE_RECTANGLE) {
			lcd->fillRect(ox- (handlewidth / 2), t - (handlesize / 2), handlewidth, handlesize, bColor);
		}
	}



	if (colorscale){
		lcd->fillRect(l, t-(bt/2), abs(pos-l), bt, thColor);
		lcd->fillRect(pos, t-(bt/2), abs(w - pos + l), bt, tsColor);
	}
	else{
		lcd->fillRect(l, t-(bt/2), w + 1, bt, ssColor);
	}

	// draw any tick marks
	if (sc != 0.0) {
		for (i = 0; i <= sc; i++){
				
			lcd->fillRect((i * (w / sc) ) + l, t-3, 1, 7, tsColor);

			if ((i == ce) | (i == 0) | (i == sc)) {
				lcd->fillRect((i * (w / sc)) + l-1, t - 3, 3, 7, tsColor);
			}
		}
	}
	
	
	// draw new handle
	if (handleshape == HANDLE_CIRCLE) {
		lcd->fillCircle(pos, t, handlesize/2, thColor);
		lcd->drawCircle(pos, t, handlesize/2,tsColor);
		lcd->drawCircle(pos, t, (handlesize/2) - 1,tsColor);
	}
	else if (handleshape == HANDLE_SQUARE) {
		lcd->fillRect(pos- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, thColor);
		lcd->drawRect(pos- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, tsColor);
		lcd->drawRect(pos- (handlesize / 2)+1, t - (handlesize / 2)+1, handlesize-2, handlesize-2, tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_1) {
		lcd->fillTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos,t, thColor);
		lcd->drawTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos,t, tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_2) {
		lcd->fillTriangle(pos - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos,t, thColor);
		lcd->drawTriangle(pos - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos,t, tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_3) {
		lcd->fillTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos,t, thColor);
		lcd->fillTriangle(ox - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos,t, thColor);

		lcd->drawTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos,t, tsColor);
		lcd->drawTriangle(pos - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos,t, tsColor);

	}
	else if (handleshape == HANDLE_RECTANGLE) {
		lcd->fillRect(pos- (handlewidth / 2)  , t - (handlesize / 2)  , handlewidth, handlesize, thColor);
		lcd->drawRect(pos- (handlewidth / 2)  , t - (handlesize / 2)  , handlewidth, handlesize, tsColor);
		lcd->drawRect(pos- (handlewidth / 2)+1, t - (handlesize / 2)+1, handlewidth-2, handlesize-2, tsColor);
	}
	ox = pos;
	pos = value;
	
}




/*

method to more the slider

optionally get the snap increment
see if the press location is in the controls range
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

bool  SliderH::slide(float ScreenX,float ScreenY){

	bool pressed = false;

	x = ScreenX;
	y = ScreenY;

	if (!enabled) {
		return pressed;
	}

	if (enabled) {
		tsColor = sColor;
		thColor = hColor;
		ssColor = sColor;
	}
	else {
		tsColor = dsColor;
		thColor = dhColor;
		ssColor = dsColor;
	}

	// Serial.print("sn "); Serial.println(sn);
	if (sn != 0.0 ) {
 		
		x = x - l;
		// Serial.print("x b "); Serial.println(x);
		x = round( x / ( w / sn));
		//Serial.print("x a "); Serial.println(x);
		x = x * ( w / sn);
		x = x + l;
	}
	else {
		x = x; 
	}

	// draw ball and scale
	if (x != ox){
		
		if ((x >= l) & (x <= (l + w))) {

			if ((abs(y - t )) <= handlesize) {

				pressed = true;
				if (handleshape == HANDLE_CIRCLE) {
					lcd->fillCircle(ox, t, handlesize/2, bColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					lcd->fillRect(ox- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					lcd->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					lcd->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					lcd->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
					lcd->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
				}
				else if (handleshape == HANDLE_RECTANGLE) {
					lcd->fillRect(ox- (handlewidth/ 2), t - (handlesize / 2), handlewidth, handlesize, bColor);
				}

				if (colorscale){
					lcd->fillRect(l, t-(bt/2), x-l, bt, hColor);
					lcd->fillRect(x, t-(bt/2), w - x + l, bt, sColor);
				}
				else{
					lcd->fillRect(l, t-(bt/2), w+1, bt, ssColor);
				}
			

				if (sc != 0.0) {
					for (i = 0; i <= sc; i++){
				
						lcd->fillRect((i * (w / sc) ) + l, t-3, 1, 7, sColor);

						if ((i == ce) | (i == 0) | (i == sc)) {
							lcd->fillRect((i * (w / sc)) + l-1, t - 3, 3, 7, sColor);
						}
					}
				}

				if (handleshape == HANDLE_CIRCLE) {
					lcd->fillCircle(x, t, handlesize/2, hColor);
					lcd->drawCircle(x, t, handlesize/2,sColor);
					lcd->drawCircle(x, t, (handlesize/2) - 1,sColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					lcd->fillRect(x- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, hColor);
					lcd->drawRect(x- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, sColor);
					lcd->drawRect(x- (handlesize / 2)+1, t - (handlesize / 2)+1, handlesize-2, handlesize-2, sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					lcd->fillTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, hColor);
					lcd->drawTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					lcd->fillTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, hColor);
					lcd->drawTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					lcd->fillTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, hColor);
					lcd->fillTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, hColor);

					lcd->drawTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, sColor);
					lcd->drawTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, sColor);

				}
				else if (handleshape == HANDLE_RECTANGLE) {
					lcd->fillRect(x- (handlewidth / 2)  , t - (handlesize / 2)  , handlewidth  , handlesize  , hColor);
					lcd->drawRect(x- (handlewidth / 2)  , t - (handlesize / 2)  , handlewidth  , handlesize  , sColor);
					lcd->drawRect(x- (handlewidth / 2)+1, t - (handlesize / 2)+1, handlewidth-2, handlesize-2, sColor);
				}
				ox = x;

				// get scaled val and pass back and store in the public variable in case anyone wants it
				
				pos = MapFloat(x, l, l + w, sl, sh);
				//Serial.print("pos "); Serial.println(pos);
				value = pos;
				//Serial.print("value "); Serial.println(value);
				delay(debounce);
			}
		}

	}
	
	return pressed;
}

void SliderH::drawSliderColor(bool color){

	colorscale = color;

}



void SliderH::setHandleSize(int size){

	if (size < 4) {
		handlesize = 4;
	}
	else if (size > 40) {
		handlesize = 40;
	}
	else {
		handlesize = size;
	}

}

void SliderH::setHandleSize(int size, int width){

	if (size < 4) {
		handlesize = 4;
	}
	else if (size > 40) {
		handlesize = 40;
	}
	else {
		handlesize = size;
	}

	if (width < 4) {
		handlewidth = 4;
	}
	else if (width > 40) {
		handlewidth = 40;
	}
	else {
		handlewidth = width;
	}


}

void SliderH::setHandleShape(byte shape){

	if (shape < 0) {
		handleshape = HANDLE_CIRCLE;
	}
	else if (shape > 6) {
		handleshape = HANDLE_CIRCLE;
	}
	else {
		handleshape = shape;
	}

}

void SliderH::setPressDebounce(byte Debounce) { 

	debounce = Debounce;

}


/*

class for a simpel slider-type on off switch, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/
SliderOnOff::SliderOnOff( uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, uint16_t SliderH, uint16_t SliderColor, uint16_t BackColor, uint16_t OnColor, uint16_t OffColor)

{
	// map arguements to class variables
	_l = SliderX;
	_t = SliderY;
	_w = SliderW;
	_h = SliderH;
	_sColor = SliderColor;
	_bColor = BackColor;
	_onColor = OnColor;
	_offColor = OffColor;

}

/*

method to more the on/off slider

see if the screen press is in the controls range
optionally get the snap increment
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

bool SliderOnOff::slide(float ScreenX, float ScreenY){

	_changed = false;

	if ((ScreenX >= _l) && (ScreenX <= (_l + _w))) {


		if (abs(ScreenY - (_t + (_h / 2))) < _h) {

			_changed = true;

			// it's in range of slider ball
			if (ScreenX < (_l + (_w / 2))){
				// press in the off range
				if (_pos){

					// clear on button
					lcd->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _bColor);
					// draw off button
					lcd->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _offColor);
					_pos = false;
				}
			}
			else {
				// it's in the on range
				if (!_pos){
					// clear on button
					lcd->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _bColor);
					// draw off button
					lcd->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _onColor);

					_pos = true;
				}
			}

		}
	}

	return _pos;

}



void  SliderOnOff::draw(bool state) {

	_pos = state;

	lcd->fillRoundRect(_l, _t, _w, _h, _h / 2, _bColor);
	lcd->drawRoundRect(_l, _t, _w, _h, _h / 2, _sColor);
	lcd->drawRoundRect(_l + 1, _t + 1, _w - 2, _h, _h / 2, _sColor);
	

	if (state) {
		// draw on button
		lcd->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _onColor);
	}

	else {
		// draw off button
		lcd->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _offColor);

	}

}


/*

method to see of the user just turned the switch on or off

*/
bool SliderOnOff::changed(){

	return _changed;

}

bool SliderOnOff::getValue(){

	return _pos;

}



/*

the mapF for mapping float values

*/

float BarChartV::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  if (in_min < 0) {
    in_max = in_max + abs(in_min);
    in_min = 0.0;
  }

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float BarChartH::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  if (in_min < 0) {
    in_max = in_max + abs(in_min);
    in_min = 0.0;
  }

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float CGraph::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float SliderH::MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {

  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}

float SliderV::MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}

float SliderD::MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}

/*

class for a vertical type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/

SliderD::SliderD()
{
}



void SliderD::init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderR, float SweepAngle, float ScaleLow, float ScaleHi, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor ) {
    
	// map arguements to class variables
	x = SliderX;
	y = SliderY;
	r = SliderR;
	sl = ScaleLow;
	sh = ScaleHi;
	sa = SweepAngle;
	as = 180.0 + 90.0 -((360.0 - sa)/2.0);
	ae = -(90.0-(360.0-sa)/2.0);
	colorscale = true;
	handlesize = SLIDER_HANDLE_SIZE;
	enabled = true;
	visible = true;
	dt = 3;
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;
	debounce = TFT_DEBOUNCE;
	dsColor = C_DISABLE_LIGHT;
	dhColor = C_DISABLE_MED;
	redraw = true;
	state = true;
}


void SliderD::setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

	// map arguements to class variables
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;

}

void SliderD::DrawRing(float Start, float End, uint16_t color){

	for (i = Start; i > End; i--){
		hx = (r * (cos(i * degtorad))) + x;
		hy = -(r * (sin(i * degtorad))) + y;

		lcd->fillCircle(hx, hy, dt, color); 

		if (color == bColor){
			lcd->fillCircle(hx, hy, dt+1, color); 
		}		
	}
}

void SliderD::DrawHandle(float angle, uint16_t hColor, uint16_t sColor){
	
	hx = (r * (cos(angle * degtorad))) + x;
	hy = -(r * (sin(angle * degtorad))) + y;

	if (hColor == bColor){
		lcd->fillCircle(hx, hy, handlesize, bColor); // erase it
	}
	else {
		lcd->fillCircle(hx, hy, handlesize, hColor); 
		lcd->drawCircle(hx, hy, handlesize-1, sColor); 
		lcd->drawCircle(hx, hy, handlesize-2, sColor); 
	}
}


void  SliderD::draw(float val) {
	
	pressed = false;

	if (!redraw){
		redraw = !redraw;
		return;
	}

	if ((val < sl) || (val > sh)){
		return;
	}
	if  (!visible) {
		if (!visible) {
			DrawRing(as, ae, bColor);
			DrawHandle(angle, bColor, bColor);
		}
		return;
	}

	value = val;

	if (enabled) {
		tsColor = sColor;
		thColor = hColor;
	}
	else {
		tsColor = dsColor;
		thColor = dhColor;
	}
	// get position angle
	angle = MapFloat(value, sl, sh, (float)as, (float)ae );
	
	if (colorscale){
		DrawRing(as, angle, thColor);
	}
	else {
		DrawRing(as, angle, tsColor);
	}

	DrawRing(angle, ae, tsColor);
	
	DrawHandle(angle, thColor, tsColor);
	
	oangle = angle;

	value = val;
		   
}

bool SliderD::slide(uint16_t ScreenX, uint16_t ScreenY){
	
	pressed = false;
	
	if ((!enabled) || (!visible)) {
		//return pos;
		return pressed;
	}

	// is press in range
	
	dist = sqrt( ((ScreenX - x)*(ScreenX - x)) + ((ScreenY - y)*(ScreenY - y))   );

	angle = (atan( (abs(y - (float)ScreenY)) / ( abs(x - (float) ScreenX)) )) / degtorad;
	// cases

	// quad 1
	if ((ScreenX >= x) && (ScreenY <= y)){
		angle = angle;
	}
	//quad 4
	else if ((ScreenX > x) && (ScreenY > y)){
		angle = -angle;
	}
	// quad 2
	else if ((ScreenX <= x) && (ScreenY <= y)){
		angle = 180-angle;
	}
	// quad 3
	else if ((ScreenX <= x) && (ScreenY >= y)){
		angle = 180+angle;
	}

	if (   (abs(angle - oangle) > 0 ) && (abs(dist-r)) < (handlesize/2) && (angle <= as) && (angle >= ae)){

		pressed = true;
		redraw = true;
		DrawHandle(oangle, bColor, bColor);

		if (colorscale){
			DrawRing(as, angle, hColor);
		}
		else {
			DrawRing(as, angle, sColor);
		}
		DrawRing(angle, ae, sColor);
	
		DrawHandle(angle, hColor, sColor);
	
		oangle = angle;

		value = MapFloat(angle,as,ae,sl, sh);
			
	}
	return pressed; 
}

void  SliderD::setHandleColor(uint16_t HandleColor) {

	hColor = HandleColor;

}

void SliderD::drawSliderColor(bool state){

	colorscale = state;

}

void SliderD::setRingThickness(byte Thickness){

	dt = Thickness;
	
}

void  SliderD::setDisableColor(uint16_t HandleColor, uint16_t SliderColor) {

	dsColor = SliderColor;
	dhColor = HandleColor;

}

void SliderD::setScale(float ScaleLow, float ScaleHi) {
    
	sl = ScaleLow;
	sh = ScaleHi;

}


void  SliderD::enable() {

	enabled = true;
	redraw = false;
	if(!state) {
		state = true;
		redraw = true;
	}
}

void  SliderD::disable() {
	enabled = false;
	redraw = false;
	if(state) {
		state = false;
		redraw = true;
	}
}

void  SliderD::show() {

	visible = true;

}

void  SliderD::hide() {

	visible = false;
	
}

void SliderD::setHandleSize(int value){

	if (value < 4) {
		handlesize = 4;
	}
	else if (value > 100) {
		handlesize = 100;
	}
	else {
		handlesize = value;
	}

}

void SliderD::setPressDebounce(byte Debounce) { 
	debounce = Debounce;
}

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


checkbox


*///////////////////////////////////////////////////////////////////////////////////////////////////////////
CheckBox::CheckBox() {};

void CheckBox::init(int16_t ButtonX, uint16_t ButtonY, uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor, uint16_t TextColor, uint16_t BackgroundColor, int TextOffsetX,int TextOffsetY, const char *Text, const ILI9341_t3_font_t &TextFont ) {

  x = ButtonX ;
  y = ButtonY + CHECKBOX_SIZE;
  s = CHECKBOX_SIZE;
  ct = 3;
  oc = OutlineColor;
  uc = UPColor;
  dc = DownColor;
  tc = TextColor;
  bc = BackgroundColor;
  doc = C_DISABLE_LIGHT;
  duc = C_DISABLE_MED;
  ddc = C_DISABLE_DARK;
  dtc = C_DISABLE_MED;
  strncpy(label,Text, 60);
  tox = TextOffsetX;
  toy = TextOffsetY;
  f = TextFont;
  enabled = true;
  state = true; // true=up, false=down
  visible = true;
  value = -1;
  debounce = TFT_DEBOUNCE;

}

void CheckBox::draw(bool val) {

  uint16_t fill, outline, tcolor;

  if (!visible) {
    lcd->fillRoundRect(x, y-s, s, s, ct, bc);
    return;
  }
  if (val){
    // last exit was no press so restore val from internal saved state
    val = state;
  }
  state = val;
  // store the variable in the public variable
  value = val;
  if (state) {
    fill = uc;
    outline = oc;
    tcolor = tc;
  } 
  else {
    fill =  dc;
    outline = oc;
    tcolor = tc;
  }

  if (!enabled){
    if (state) {
      fill = duc;
      outline = doc;
      tcolor = dtc;
    } 
    else {
      fill =  ddc;
      outline = doc;
      tcolor = dtc;
    }
  }

  lcd->fillRoundRect(x, y-s, s, s, ct, fill);
  lcd->drawRoundRect(x, y-s, s, s, ct, outline);
  
  lcd->setFont(f);
  lcd->setTextColor(tcolor);
  
  if (toy == 0) {
    #if __has_include("ST7796_t3.h")
      uint16_t _txtWidth = lcd->strPixelLen(label);
      lcd->setCursor(x + tox+(s/2), y - (s/2)- (_txtWidth/2));
    #else
			lcd->setCursor(x + tox+(s/2), y - (s/2)- (d->measureTextHeight(label)/2));
    #endif
  }
  else {
    lcd->setCursor(x + tox+(s/2), y - s + toy);
  }
  
  lcd->print(label);
}

bool CheckBox::press(int16_t SceenX, int16_t ScreenY) {
  bool pressed = false;

  if ((!visible) || (!enabled)) {
    return pressed;
  }

  if (   ( (SceenX >= x) && (SceenX <= (x + s) )) && ((ScreenY >= y-s) && (ScreenY <= (y)))   ) {	
    state = !state;
    draw(state);
    //delay(debounce);
    // store the variable in the public variable
    value = state; 
    pressed = true;
  }
  return pressed;
}

void CheckBox::show() {
  visible = true;
}


void CheckBox::hide() {
  visible = false;
}

void CheckBox::disable() { 
  enabled = false;
}
void CheckBox::enable() { 
  enabled = true;
}


void CheckBox::resize(int16_t ButtonX, int16_t ButtonY, uint8_t Size) {
  hide();
  draw(state);
  x = ButtonX;
  y = ButtonY;
  s = Size;
  show();
  draw(state);
  
}

void CheckBox::setColors(uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor, uint16_t BackgroundColor, uint16_t DisableOutlineColor,  uint16_t DisableTextColor, uint16_t DisableUPColor, uint16_t DisableDownColor) {
  
  oc = OutlineColor;
  uc = UPColor;
  dc = DownColor;
  bc = BackgroundColor;
  doc = DisableOutlineColor;
  duc = DisableUPColor;
  ddc = DisableDownColor;
  dtc = DisableTextColor;
  
}

void CheckBox::setText(int TextOffsetX,int TextOffsetY, const char *Text, const ILI9341_t3_font_t &TextFont) {

  tox = TextOffsetX;
  toy = TextOffsetY;
  strncpy(label,Text, 60);
  f = TextFont;

}

void CheckBox::setCornerRadius(int val) {
  ct = val;

}
  
bool CheckBox::isEnabled() { 
  return enabled;
}

bool CheckBox::isVisibled() { 
  return visible;
}

void CheckBox::setPressDebounce(byte Debounce) { 
  debounce = Debounce;
}

  
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


option box


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

OptionButton::OptionButton() { }

void OptionButton::init(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, int16_t TextColor, uint16_t BackgroundColor, int TextOffsetX,int TextOffsetY, const ILI9341_t3_font_t &TextFont) {

  r = OPTION_BUTTON_RADIUS;
  oc = OutlineColor;
  sc = SelectedColor;
  uc = UnSelectedColor;
  bc = BackgroundColor;
  tc = TextColor;
  dsc = C_DISABLE_MED;
  duc = C_DISABLE_DARK;
  doc = C_DISABLE_LIGHT;
  tox = TextOffsetX;
  toy = TextOffsetY;
  dtc = C_DISABLE_MED;
  f = TextFont;
  enabled = true;
  current = 0;
  ID = 0;
  visible = true;
  value = current;
  debounce = TFT_DEBOUNCE;
}

int OptionButton::add(uint16_t ButtonX, uint16_t ButtonY,const char *Text, float OptionValue ) {

  strncpy(label[ID],Text, 60);
  
  x[ID] = ButtonX;
  y[ID] = ButtonY;
  // this next part gives users to specify what the value will be
  // you can defaul to the ID,or specify it, good use is if option ID=3 is pressed and you want value to be 24
  // very odd value but chances of someone wanting this val?
  if (OptionValue == -32001) {
    rv[ID] = ID;
  }
  else {
    rv[ID] = OptionValue;
  }

  ID++;
  return(ID-1);
  
}


void OptionButton::draw(int OptionID) {

  uint16_t toc, tsc, tuc, tcolor;

  current = OptionID;

  if (!visible) {

    for (i = 0; i < ID; i++){
      lcd->fillCircle(x[i]+r/2, y[i]+r/2, r, bc);
      lcd->setCursor(x[i] + tox, y[i] + toy);
      lcd->setFont(f);
      lcd->setTextColor(bc);
      lcd->print(label[i]);
    }
    return;
  }

  toc = oc;
  tsc =  sc;
  tuc =  uc;
  tcolor = tc;

  if (!enabled){
    toc = doc;
    tsc = dsc;
    tuc = duc;
    tcolor = dtc;
  }
  
  for (i = 0; i < ID; i++){
    if (i == current) {
      lcd->fillCircle(x[i]+r/2, y[i]+r/2, r, tsc);
      value =  rv[i];
      option =  i;
    }
    else {
      lcd->fillCircle(x[i]+r/2, y[i]+r/2, r, tuc);
    }
    lcd->drawCircle(x[i]+r/2, y[i]+r/2, r, toc);
    lcd->setCursor(x[i] + tox, y[i] + toy);
    lcd->setFont(f);
    lcd->setTextColor(tcolor);
    lcd->print(label[i]);
  }

  // store the variable in the public variable
  

}

bool OptionButton::press(uint16_t ScreenX, uint16_t ScreenY) {
  bool pressed = false;
  
    


  for (i = 0; i < ID; i++){
    if (  (   (ScreenX >= (x[i] - 2*r) ) && (ScreenX <= (x[i] +  2*r))  ) && (  (ScreenY >= (y[i] -  2*r)) && (ScreenY <= (y[i] +  2*r))    )       ) {
      current = i; 
      if ((!visible) || (!enabled)) {
        pressed = false;
      }
      else {
        value = rv[i];
        option =  i;
        pressed = true;
        break;
      }
    }
  }
  if (pressed){
  draw(current);
  }
  //delay(debounce);
  
  return pressed;	

}

void OptionButton::select(int val) {
  value = rv[val];
  option = val;
  current = val;
}

void OptionButton::show() {
  visible = true;
}

void OptionButton::hide() {
  visible = false;
}

void OptionButton::disable() { 
  enabled = false;
}

void OptionButton::enable() { 
  enabled = true;
}

void OptionButton::resize(byte radius) {
  // caution, only all this before first draw--code will not remember old size
  r = radius;
}

void OptionButton::setColors(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, uint16_t TextColor, uint16_t BackgroundColor, uint16_t DisableOutlineColor, uint16_t DisableSelColor, uint16_t DisableUnSelColor, uint16_t DisableTextColor) {
  
  oc = OutlineColor;
  sc = SelectedColor;
  uc = UnSelectedColor;
  tc = TextColor;
  bc = BackgroundColor;
  doc = DisableOutlineColor;
  dsc = DisableSelColor;
  duc = DisableUnSelColor;
  dtc = DisableTextColor;
      
}

void OptionButton::setFont(int TextOffsetX,int TextOffsetY, const ILI9341_t3_font_t &TextFont) {
  
  tox = TextOffsetX;
  toy = TextOffsetY;
  f = TextFont;
      
}

void OptionButton::setText(int ID, const char *Text) {
  strncpy(label[ID],Text, 60);
}

int OptionButton::selected() { 
  return current;
}

bool OptionButton::isEnable() { 
  return enabled;
}

bool OptionButton::isVisible() { 
  return visible;
}

void OptionButton::setPressDebounce(byte Debounce) { 
  debounce = Debounce;
}


/************* End ILI9341_controls ******************/