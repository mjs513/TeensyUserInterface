
//      ******************************************************************
//      *                                                                *
//      *       Example shows build screens that display information     *
//      *                                                                *
//      *            S. Reifel & Co.                2/18/2022            *
//      *                                                                *
//      ******************************************************************

//
// DOCUMENTATION:
//
// Documentation for the "Teensy User Interface" library can be found at:
//    https://github.com/Stan-Reifel/TeensyUserInterface



// ***********************************************************************

//#include <font_Arial.h>
//#include <font_ArialBold.h>
#include <ST7796_t3.h>
#include <st7735_t3_font_Arial.h>
//#include <st7735_t3_font_ArialBold.h>
#include <font_ArialBold.h>
#include <XPT2046_Touchscreen.h>
#include <TeensyUserInterface.h>


//
// create the user interface object
//
TeensyUserInterface ui;

//
// Create LCD_controls
#include "constants.h"
BarChartA BarGraphBits;
BarChartA BarGraphVolts;

BarChartH A7Volts;
BarChartH A8Volts;

BarChartV A71Volts;
BarChartV A81Volts;

CGraph MyGraph(X_ORIGIN, Y_ORIGIN, X_WIDE, Y_HIGH, X_LOSCALE, X_HISCALE, X_INC, Y_LOSCALE, Y_HISCALE, Y_INC);

//create an object for each check box
CheckBox CB1;
CheckBox CB2;
CheckBox CB3;
CheckBox CB4;
CheckBox CB5;

// create the dial object(s)
Dial Bits(BXCENTER, BYCENTER, BDIAMETER, BLOWSCALE , BHIGHSCALE, BSCALEINC, BSWEEPANGLE);
Dial Volts(VXCENTER, VYCENTER, VDIAMETER, VLOWSCALE , VHIGHSCALE, VSCALEINC, VSWEEPANGLE);

// option button GROUP (not individual options), pass in the display object
OptionButton OBA;
OptionButton OBB;
CheckBox CB11;
CheckBox CB21;

// Ring graph
SliderD Vol;
SliderD Mid;
SliderD Base;
SliderD Treb;

//Horizontal slider
SliderH Bright;
SliderH Red;
SliderH Green;
SliderH Blue;

// create the slider objects
SliderV sBand1;
SliderV sBand2;
SliderV sBand3;
SliderV sBand4;
SliderV sBand5;
SliderV sBand6;
SliderV sBand7;
SliderV sBand8;

// color565		- converts 565 format 16 bit color to RGB
  static uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  }


// ---------------------------------------------------------------------------------
//                                 Setup the hardware
// ---------------------------------------------------------------------------------

void setup() 
{
  //
  // pin numbers used in addition to the default SPI pins
  //
  const int LCD_CS_PIN = 10;
  const int LCD_DLCD_PIN = 9;
  const int TOUCH_CS_PIN = 27; // 8;
  
  //
  // setup the LCD orientation, the default font and initialize the user interface
  //
  ui.begin(LCD_CS_PIN, LCD_DLCD_PIN, TOUCH_CS_PIN, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT, Arial_9_Bold);
  ui.setTouchScreenCalibrationConstants(150, 3921, 206, 3934);

  ui.invertDisplay(false);
  //
  // use a grayscale color palette
  //
  ui.setColorPaletteBlue();
}


// ---------------------------------------------------------------------------------
//           Define the menus and top level loop, place menus after setup()
// ---------------------------------------------------------------------------------


//
// for each menu, create a forward declaration with "extern"
//
extern MENU_ITEM mainMenu[];


//
// the main menu
//
MENU_ITEM mainMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,  "Example 10 - ILI9341 controls",   MENU_COLUMNS_2,           mainMenu},
  {MENU_ITEM_TYPE_COMMAND,           "Arc Bar Chart",                   commandArcBarChart,       NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Bar Chart H",                     commandBarChartH,         NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Bar Chart V",                     commandBarChartV,         NULL},
  {MENU_ITEM_TYPE_COMMAND,           "CCGraph",                         commandGraphing,          NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Check Box",                       commandCheckBox,          NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Dial",                            commandDial,              NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Option Buttons",                  commandOptions,           NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Ring Graph",                      commandRing,               NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Slider Horizontal",               commandHorizontalSlider,   NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Slider Vertical",                 commandVerticalSlider,    NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,       "",                                     NULL,                 NULL}
};



//
// display the menu, then execute commands selected by the user
//
void loop() 
{  
  ui.displayAndExecuteMenu(mainMenu);
}



// ---------------------------------------------------------------------------------
//                            Commands executed from the menu
// ---------------------------------------------------------------------------------

//
// menu command that demonstrates the use of the "Back" button
//
void commandArcBarChart(void)
{
  //
  // clear the screen and draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Arc Bar Chart Example");
  ui.clearDisplaySpace(LCD_LIGHTGREY);
  
  int a7Bits;
  float a7Volts;
  
  // default settings, scale is 0-1024
  BarGraphBits.init(800, 220, 30, 50, 10, 20, 0.0f, 1024.0f);

  // overrides
  BarGraphVolts.init(60, 80, 20, 50, 20, 40, 0.0f, 3.3f);
  // make a 270 degree arc, 15 segments, 15 pixels wide,  and 0.06 radian gap between segments
  BarGraphVolts.setBars(270, 15, 15, .06);
  // change colors for low medium high and null bar colors
  BarGraphVolts.setSectionColors(LCD_BLUE, LCD_WHITE, LCD_RED, 0x3186);
  // set low 0-33%, med 33-66%, and red above 66%
  BarGraphVolts.setSectionSize(.33, .66);

  //
  // wait for the user to press the "Back" button, then return to the main menu
  //
  while(true)
  {
    // take some readings, temp, speed, etc.
    a7Bits = analogRead(A7);

    // convert bits as needed
    a7Volts = a7Bits * 3.3f / 1024.0f;

    // simply draw the data, make sure you bounds check
    BarGraphBits.draw(a7Bits);
    BarGraphVolts.draw(a7Volts);

    delay(10);
  
    ui.getTouchEvents();

    if (ui.checkForBackButtonClicked())
      return;
  }
}



//
// menu command that demonstrates the use of an "OK" button and
// barchartH
//

void commandBarChartH(void)
{
  //
  // clear the screen and draw title bar showing without the "Back" button
  //
  ui.drawTitleBar("Horizontal Bar Chart");
  ui.clearDisplaySpace();

  //
  // define and display an "OK" button 
  //
  int buttonX = ui.displaySpaceCenterX;
  int buttonY = ui.displaySpaceBottomY-30;
  int buttonWidth = 120;
  int buttonHeight = 35;
  BUTTON okButton  = {"OK", buttonX, buttonY, buttonWidth, buttonHeight};
  ui.drawButton(okButton);
  
  int a7Bits, a8Bits;
  float a7Volts, a8Volts;

  pinMode(A1, INPUT);
  pinMode(A2, OUTPUT);
  
  digitalWrite(LED_PIN, HIGH);

  // step 3 initialize
  // this init will create bar segment type display
  A7Volts.init(10, 55, 200, 25, 0, 50, 10,"NULL", LCD_WHITE, LCD_GREEN, LCD_YELLOW, LCD_RED, LCD_BLACK, FONT_TITLE, FONT_DATA);

  // this init will create a simple solid bar graph
  A8Volts.init(V_XORIGIN, V_YORIGIN, V_WIDE, V_HEIGHT, V_LOWSCALE, V_HIGHSCALE, V_SCALEINC, "A8 Voltage", TEXT_COLOR, BORDER_COLOR, BAR_COLOR,
               NULL_COLOR, BACK_COLOR, FONT_TITLE, FONT_DATA);

  // optional hide scales
  A7Volts.showScale(false); // will have no effect since I don't support scale on segment graphps
  A7Volts.showTitle(false);
  // set colors for 3 color segments and 4th for null color
  // A7Volts.setSectionColors(LCD_GREEN, LCD_YELLOW, LCD_RED, 0x2104);
  A7Volts.setSectionSize(2.8, 3);
  //
  // wait for the user to press the "OK" button, then return to the main menu
  //
  while(true)
  {
    a7Bits = analogRead(A1);
    a8Bits = analogRead(A2);

    a7Volts = a7Bits * 3.3 / 1024.0f;
    a8Volts = a8Bits * 3.3 / 1024.0f;

    // optional but shows how to change the color after init
    if (a8Volts > 0.87) {
      A8Volts.setBarColor(WARNING_COLOR);
    } else {
      A8Volts.setBarColor(BAR_COLOR);
    }

    // step 5 draw
    A7Volts.draw(a7Volts);
    A8Volts.draw(a8Volts);
    
    delay(50);
    //
    // get touch events on the LCD
    //
    ui.getTouchEvents();

    //
    // check for a touch event on the OK button, if so return to the main menu
    //
    if (ui.checkForButtonClicked(okButton))
      return;
  }
}



//
// menu command that demonstrates the use of "OK" & "Cancel" buttons
//
void commandBarChartV(void)
{
  //
  // clear the screen and draw title bar showing without the "Back" button
  //
  ui.drawTitleBar("Vertical Bar Example");
  ui.clearDisplaySpace();
  
  int buttonX = ui.displaySpaceCenterX;
  int buttonY = ui.displaySpaceBottomY-30;
  int buttonWidth = 120;
  int buttonHeight = 35;
  BUTTON okButton  = {"OK", buttonX, buttonY, buttonWidth, buttonHeight};
  ui.drawButton(okButton);
  

  int a7Bits, a8Bits;
  float a7Volts, a8Volts;
  
  A71Volts.init(10, 250, 100, 198, 0.0, 3.3, 0.1, "A7", LCD_WHITE, LCD_WHITE, LCD_RED, LCD_BLACK, LCD_BLACK, FONT_TITLE, FONT_DATA);

  // optional hide scales
  A71Volts.showScale(false);
  A71Volts.showTitle(false);

  // optional draw bars with segments (looks like a segment UV meter)
  A71Volts.useSegmentBars(true);
  //1 set colors for 3 color segments and 4th for null color
  A7Volts.setSectionColors(LCD_GREEN, LCD_YELLOW, LCD_RED, 0x2104);
  // how many bars for each (these are percentages (70% for green, 20% for yellow, and rest red))
  A71Volts.setSectionSize(.70, .90);
  // independent scale for bars (so you can have say 50 bars but numerical scale of 1-2-3-4-5)
  A71Volts.setBars(50, .1, 1);
  // caution you will need to do some math to ensure the resulting total bar height is within you set height
  // bar height is computed and if not and integer, we'll make it one. Makes guarenteeting no odd dividers but height may be off
  // of you must have fixed height and same divider bars, make sure barheight is integer

  // bars = (ScaleHigh - ScaleLow) / BarInc;
  // barheight = GraphHeight / bars;
  // note: uint16_t used for barheight, but this will return a float so you can verify graph hight and bar inc scaler
  Serial.println(A71Volts.getBars());
  Serial.println(A71Volts.getActualHeight());

  // initalize another for a different example
  A81Volts.init(200, 250, 50, 180, 0, 4.0, 0.5, "A8", LCD_WHITE, LCD_BLACK, LCD_GREEN, LCD_BLACK, LCD_BLACK, FONT_TITLE, FONT_DATA);

  //
  // wait for the user to press the "OK" button, then return to the main menu
  //
  while(true)
  {

    a7Bits = analogRead(A7);
    a8Bits = analogRead(A8);

    a7Volts = a7Bits * 3.3 / 1024.0f;
    a8Volts = a8Bits * 3.3 / 1024.0f;

    // optional but shows how to change the color after init
    if (a8Volts > 2) {
      A81Volts.setBarColor(LCD_RED);
    } else {
      A81Volts.setBarColor(LCD_GREEN);
    }

    // step 5 draw the bar chart objects
    A71Volts.draw(a7Volts);
    A81Volts.draw(a8Volts);

    delay(50);
    
    //
    // get touch events on the LCD
    //
    ui.getTouchEvents();


    //
    // check for a touch event on the OK button, if so do stuff then return to the main menu
    //
    if (ui.checkForButtonClicked(okButton))
    {
      //
      // return to the main menu
      //
      return;
    }

      
    //
    // check for a touch event on the Cancel button, if so return to the main menu
    //
    if (ui.checkForButtonClicked(okButton))
      return;
  }
}



//
// menu command that demonstrates printing in columns
//
void commandGraphing(void)
{
  //
  // clear the screen and draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Graphing Example");
  ui.clearDisplaySpace();

  // used to monitor elaspsed time
  unsigned long oldTime;

  // create a variable for each data data point
  float x, volts;

  // create an ID for each data to be plotted
  int VoltID, SinID, CosID;
  MyGraph.init("Teensy Graphing", "Time [sec]", "Volts", TEXTCOLOR, GRIDCOLOR, AXISCOLOR, BACKCOLOR, PLOTCOLOR, FONT_TITLE, FONT_DATA);

  // use the add method to create a plot for each data
  // PlotID = MyGraph.Add(data title, data color);
  // 
  VoltID = MyGraph.add("Volts", VOLTSCOLOR);
  SinID = MyGraph.add("sin(x)", SINCOLOR);

  // these call are all optional
  MyGraph.drawGraph();		// draw empty graph if you have a long delay before any plottable data
  MyGraph.setMarkerSize(VoltID, 0); 
  MyGraph.setMarkerSize(SinID, 1);
  // MyGraph.setYAxis(-1.5, 2.5, 0.5);  // reset the y axis at any time in your program
  // MyGraph.showTitle(false);          //  hide the title--good for big graph in tight space
  // MyGraph.showLegend(false);         //  hide the legend--good for big graph in tight space
  // MyGraph.showXScale(false);         //  hide the x scale--good for big graph in tight space
  // MyGraph.showYScale(false);         //  hide the y scale--good for big graph in tight space

  //
  // wait for the user to press the "Back" button, then return to the main menu
  //
  while(true)
  {
    // set the x value, should be in order and match the x scale (again you will
    // need to do your own math to get the plot to match the updte time in this loop

    volts = analogRead(A0) / 1024.0;
    MyGraph.setX(x);

    MyGraph.plot(VoltID, volts);
    MyGraph.plot(SinID, sin(x));


    // bump the x value
    x += .01;
    delay(10);
    
    ui.getTouchEvents();

    if (ui.checkForBackButtonClicked())
      return;
  }
}



//
// menu command that demonstrates the use of fonts and color
//
void commandCheckBox(void)
{
  //
  // draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Show Colors and Fonts");

  //
  // clear the display space with a color instead of the default black
  //
  ui.clearDisplaySpace(LCD_NAVY);
  
  int BtnX, BtnY;
  
  // create  each check box, passing in the location, colors, text offsets (to let you control text location
  // text and ILI9341 text font, note size and disable colors have defaults that can be overridded before draw
  // method or change the #define in the .h file
  CB1.init(50, ROW1, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Bananas", Arial_14 );
  CB2.init(50, ROW2, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Apples", Arial_14 );
  CB3.init(50, ROW3, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Cherries", Arial_14 );
  CB4.init(50, ROW4, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Pickles", Arial_14 );
  CB5.init(50, ROW5, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Only allow fruit", Arial_14 );

  // draw the check box controls
  // optional, you can set other parameters before the draw method (size for example)
  CB1.draw(true);
  CB2.draw(true);
  CB3.draw(true);
  CB4.draw(true);
  CB5.draw(false);

  //
  // wait for the user to press the "Back" button, then return to the main menu
  //
  while(true)
  {
    // upon press of the checkbox process accordingly
    // optional, you can set other parameters upon pressing a check box
    // such as setting color or text (could be useful in re-drawing the control
    // with different text, enabling /disabling a control
    // just call the draw event after chaning parameters

    ui.getTouchScreenCoords(&BtnX, &BtnY);

    CB1.press(BtnX, BtnY);
    CB2.press(BtnX, BtnY);
    CB3.press(BtnX, BtnY);
    CB4.press(BtnX, BtnY);
    // if you really need to monitor if the control was clicked
    // here's how, this may be needed if you have a complicated UI
    // and don't need to automatically change things--code here automatically
    // updates items based on check box value (see if else sections, these
    // run even of check box was not pressed
    // do not use the return value as check box state
    // only use Checkbox.value for the state
    bool changed = CB5.press(BtnX, BtnY);

// as mentioned above, you code processes regardless if checkbox was clicked
    if (CB1.value) {
      // un checked your code goes here
    }
    else {
      // checked, your code goes here
    }


    if (CB2.value) {
      // un checked your code goes here
    }
    else {
      // checked, your code goes here
    }
    if (CB3.value) {
      // un checked your code goes here
    }
    else {
      // checked, your code goes here
    }
    if (CB4.value) {
      // un checked your code goes here
    }
    else {
      // checked, your code goes here
    }

    // as mentioned above, you you don't want to process code unless checkbox was clicked
    // wrap a change test around checkbox values
    if (changed) {
      if (CB5.value) {
        CB4.disable();
        CB4.draw(false); // your call, but if option disabled, maybe force state to false
      }
      else {
        CB4.enable();
        CB4.draw(CB4.value);
      }
    }
    
    delay(50);
    
    ui.getTouchEvents();

    if (ui.checkForBackButtonClicked())
      return;
  }
}



//
// menu command that demonstrates drawing with graphics
//
void commandDial(void)
{
  //
  // draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Draw a graph");
  ui.clearDisplaySpace();
  
  int bBits;
  float bVolts;
  float bData;

  Bits.init(BNEEDLECOLOR, BDIALCOLOR, BTEXTCOLOR, BTICCOLOR, "Bits", FONT_TITLE1, FONT_DATA1);
  Volts.init(VNEEDLECOLOR, VDIALCOLOR, VTEXTCOLOR, VTICCOLOR, "Volts", FONT_TITLE1, FONT_DATA1);
  
  //
  // wait for the user to press the "Back" button, then return to the main menu
  //
  while(true)
  {
    // get some data
    bBits = analogRead(A4);
    bVolts = bBits * 3.3 / 1024;

    // update the dials
    Bits.draw(bBits);
    Volts.draw(bVolts);

    delay(150);
    
    ui.getTouchEvents();

    if (ui.checkForBackButtonClicked())
      return;
  }
}



//
// menu command that demonstrates display live numeric values
//
void commandOptions(void)
{
  ui.drawTitleBarWithBackButton("Option Buttons");
  ui.clearDisplaySpace();

  int OptionA0ID, OptionA1ID, OptionA2ID, OptionA3ID, OptionA4ID;
  int OptionB0ID, OptionB1ID, OptionB2ID, OptionB3ID, OptionB4ID;
  int BtnX, BtnY;
  
  // step 3 initialize each option button group, passing in colors,
  // button text offsets (to help manage text location on the button and the ILI9341 font
  OBA.init(LCD_WHITE, LCD_GREEN, LCD_DARKGREY, LCD_WHITE, LCD_BLACK, 20, -2, Arial_14 );

  // step 4 Add an option to be listed in the button group, passing in location,
  // and text
  OptionA0ID = OBA.add(COL1, ROW11, "Apples");
  OptionA1ID = OBA.add(COL1, ROW21, "Oranges");
  OptionA2ID = OBA.add(COL1, ROW31, "Mangos");
  OptionA3ID = OBA.add(COL1, ROW41, "Strawberries");

  OBB.init(LCD_WHITE, LCD_GREEN, LCD_DARKGREY, LCD_WHITE, LCD_DARKGREY, 20, -2, Arial_14 );
  OptionB0ID = OBB.add(COL2, ROW1, "Steak");
  OptionB1ID = OBB.add(COL2, ROW2, "Pork");
  OptionB2ID = OBB.add(COL2, ROW3, "Chicken");
  OptionB3ID = OBB.add(COL2, ROW4, "Fish");

  CB11.init(COL1, ROW51, LCD_WHITE, LCD_GREEN, LCD_DARKGREY, LCD_WHITE, LCD_BLACK, 30, 2, "Enable", Arial_14 );
  CB21.init(COL2, ROW51, LCD_WHITE, LCD_GREEN, LCD_DARKGREY, LCD_WHITE, LCD_BLACK, 30, 2, "Show", Arial_14 );


  ui.lcdDrawFilledRectangle(5, 55, 320 / 2 - 10, 240 - 55 - 10, LCD_DARKGREY);
  ui.lcdDrawFilledRectangle(320 / 2 + 10, 55, 320 / 2 - 10, 240 - 55 - 10, LCD_DARKGREY);

  // optional, you can set other parameters before the draw method (color and default selected
  // option for example)
  // step 5 draw each option button group (this method will automatically draw
  // all items in the group

  OBA.draw(0);
  OBB.draw(1);

  CB11.draw(true);
  CB21.draw(true);
  //
  // continuously update the display until the user presses "OK"
  //
  while(true)
  {
    ui.getTouchScreenCoords(&BtnX, &BtnY);
    
    // if you really need to monitor if the control was clicked
    // here's how, this may be needed if you have a complicated UI
    // and don't need to automatically change things--code here shows 2 cases
    // 1) automatically updates items based on check box value (see if else sections, these
    // run even of check box was not pressed
    // and 2) only updating items if the check box was changed
    // do not use the return value as check box state
    // only use Checkbox.value for the state
    bool changed = CB11.press(BtnX, BtnY);
    CB21.press(BtnX, BtnY);


    // step 6 proces the option group (this mehod will pass back the selected option
    // so no need to rip through each option to test if selected. this call will pass back
    // the selected option
    OBA.press(BtnX, BtnY);
    OBB.press(BtnX, BtnY);

    // process option A group
    if (OBA.value == OptionA0ID) {
      // your code goes here
    }
    else if (OBA.value == OptionA1ID) {
      // your code goes here
    }
    else if (OBA.value == OptionA2ID) {
      // your code goes here
    }
    else if (OBA.value == OptionA3ID) {
      // your code goes here
    }

    //process option B group
    if (OBB.value == OptionB0ID) {
      // your code goes here
    }
    else if (OBB.value == OptionB1ID) {
      // your code goes here
    }
    else if (OBB.value == OptionB2ID) {
      // your code goes here
    }
    else if (OBB.value == OptionB3ID) {
      // your code goes here
    }

    // as mentioned above, you you don't want to process code unless checkbox was clicked
    // wrap a change test around checkbox values
    if (changed) {
      if (CB11.value) {
        Serial.println("enabling");
        OBA.enable();
        OBA.draw(OBA.value);
      }
      else {
        Serial.println("disabling");
        OBA.disable();
        OBA.draw(OBA.value);
      }
    }

    // as mentioned above, you code processes regardless if checkbox was clicked
    if (CB21.value) {
      Serial.println("showing");
      OBB.show();
      OBB.draw(OBB.value);
    }
    else {
      Serial.println("hiding");
      OBB.hide();
      OBB.draw(OBB.value);
    }

    delay(150);
    //
    // get touch events, return to the main menu when the user presses OK
    //
    ui.getTouchEvents();
    if (ui.checkForBackButtonClicked())
      return;
  }
}



//
// menu command that demonstrates doing something while pressing a button
//
void commandRing(void)
{
  //
  // clear the screen and draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Ring Graph");
  ui.clearDisplaySpace();

  int BtnX, BtnY;  // holders for screen coordinate drawing

  Vol.init (80, 95, 40, 270, 0, 100, LCD_WHITE, LCD_BLACK, LCD_RED);
  Base.init (240, 95, 40, 270, 0, 100, LCD_WHITE, LCD_BLACK, LCD_BLUE);
  Mid.init (80, 215, 40, 270, 0, 100, LCD_WHITE, LCD_BLACK, LCD_GREEN);
  Treb.init (240, 215, 40, 270, 0, 100, LCD_WHITE, LCD_BLACK, LCD_YELLOW);
  
  // draw the controls with an initial value
  // note that value must be in the scale range
  Vol.draw(0);
  Base.draw(50);
  Mid.draw(70);
  Treb.draw(28);
  
  //ui.setTextColor(LCD_WHITE, LCD_BLACK);
  // draw text if you need
  ui.lcdSetCursorXY (65, 80);
  ui.lcdPrint("Vol");
  ui.lcdSetCursorXY(225, 80);
  ui.lcdPrint("Bas");
  ui.lcdSetCursorXY  (65, 200);
  ui.lcdPrint("Mid");
  ui.lcdSetCursorXY  (225, 200);
  ui.lcdPrint("Trb");

  ui.lcdSetCursorXY (65, 95);
  ui.lcdPrint(Vol.value, 0);
  ui.lcdSetCursorXY(225, 95);
  ui.lcdPrint(Base.value, 0);
  ui.lcdSetCursorXY  (65, 215);
  ui.lcdPrint(Mid.value, 0);
  ui.lcdSetCursorXY  (225, 215);
  ui.lcdPrint(Treb.value, 0);


  //
  // continuously check if any buttons are pressed by the user
  //
  while(true)
  {
    //
    // get events from the touch screen
    //
    ui.getTouchEvents();

    //
    // check if the user pressed and released the Back button, if so return to the main menu
    //
    if (ui.checkForBackButtonClicked())
      return;

    ui.getTouchScreenCoords(&BtnX, &BtnY);

    // if press is in range of the ring, slide will occur
    //example in how to test for a changed state and disable a control
    if (Vol.slide(BtnX, BtnY)) {

      if (Vol.value > 80) {
        Base.disable();
        Base.draw(Base.value);
      }
      else {
        Base.enable();
        Base.draw(Base.value);
      }
    }
    Base.slide(BtnX, BtnY);
    Mid.slide(BtnX, BtnY);
    Treb.slide(BtnX, BtnY);

    // use the value property to report the controls value

    ui.lcdSetCursorXY (65, 95);
    ui.lcdDrawFilledRectangle(65, 95, 35, 15, LCD_BLACK);
    ui.lcdPrint(Vol.value, 0);
    ui.lcdSetCursorXY(225, 95);
    ui.lcdDrawFilledRectangle(225, 95, 35, 15, LCD_BLACK);
    ui.lcdPrint(Base.value, 0);
    ui.lcdSetCursorXY  (65, 215);
    ui.lcdDrawFilledRectangle(65, 215, 35, 15, LCD_BLACK);
    ui.lcdPrint(Mid.value, 0);
    ui.lcdSetCursorXY  (225, 215);
    ui.lcdDrawFilledRectangle(225, 215, 35, 15, LCD_BLACK);
    ui.lcdPrint(Treb.value, 0);
    
    delay(150);

  }
}


void commandHorizontalSlider(void)
{
  //
  // clear the screen and draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Hozizontal Slider");
  ui.clearDisplaySpace();

  byte RedByte = 0, GreenByte = 0, BlueByte = 210, BrightByte = 255;
  int BtnX, BtnY;

  char buf[22];
  uint16_t color, oldcolor;

  // low limit, high limit, scale val (0 is off), snap value (0 is off))
  Bright.init (20, ROW12, 100, 55, 255,     0, 0, LCD_WHITE, LCD_BLACK, LCD_GREY);

  Red.init    (20, ROW22, 100, 0, 255, 127.5, 0, LCD_WHITE, LCD_BLACK, LCD_RED);
  Green.init  (20, ROW32, 100, 0, 255, 63.75, 0, LCD_WHITE, LCD_BLACK, LCD_GREEN);
  Blue.init   (20, ROW42, 100, 0, 255,     0, 0, LCD_WHITE, LCD_BLACK, LCD_BLUE);
  
  // Step 4 draw each slider
  Red.draw(RedByte);
  Green.draw(GreenByte);
  Blue.draw(BlueByte);
  Bright.draw(BrightByte);
  
  color = color565(RedByte, GreenByte, BlueByte);

  // draw a color box
  ui.lcdDrawRectangle(140, 80, 100, 100, LCD_WHITE);
  ui.lcdDrawRectangle(141, 81, 98, 98, LCD_WHITE);
  ui.lcdDrawFilledRectangle(142, 82, 96, 96, color);

  sprintf(buf, "(%3d),%3d,%3d,%3d", color, RedByte, GreenByte, BlueByte);
  ui.lcdSetCursorXY(50 , 220 );
  ui.lcdPrint(buf);

  //
  // continuously check if any buttons are pressed by the user
  //
  while(true)
  {
    //
    // get events from the touch screen
    //
    ui.getTouchEvents();

    //
    // check if the user pressed and released the Back button, if so return to the main menu
    //
    if (ui.checkForBackButtonClicked())
      return;

    // Step 5 allow user to slide the control and proces results
    // if you really need to monitor if the control was clicked
    // here's how, this may be needed if you have a complicated UI
    // and don't need to automatically change things--code here shows 2 cases
    // 1) automatically updates items based on slider (see if else sections, these
    // run even if slider was not pressed
    // and 2) only updating items if the slider was changed
    // do not use the return value as slider value
    // only use SliderV.value for the value

    ui.getTouchScreenCoords(&BtnX, &BtnY);

    bool changed = Bright.slide(BtnX, BtnY);
    // note if touch is not in range of slider, operation will be ignored
    // meaning you don't need to test press location--the lib does that
    Red.slide(BtnX, BtnY);
    Green.slide(BtnX, BtnY);
    Blue.slide(BtnX, BtnY);

    // update the color swatch
    // as mentioned above, you you don't want to process code unless slider was moved
    // wrap a change test around checkbox values
    // here only the band 1 screen text updates
    if (!changed) {
      color = color565(Red.value, Green.value, Blue.value);
      ui.lcdDrawRectangle(140, 80, 100, 100, LCD_WHITE);
      ui.lcdDrawRectangle(141, 81, 98, 98, LCD_WHITE);
      ui.lcdDrawFilledRectangle(142, 82, 96, 96, color);
      ui.lcdDrawFilledRectangle(50, 220, 200, 50, LCD_BLACK);
      sprintf(buf, "(%3d),%3d,%3d,%3d", color, (int)Red.value, (int) Green.value, (int) Blue.value);
      ui.lcdSetCursorXY(50 , 220 );
      ui.lcdPrint(buf);
    }
    // as mentioned above, you code processes regardless if slider was moved
    // here we reset brightness on ANY slider move--don't like it? add changed check to
    // only process if bright slider was moved
    //analogWrite(LCD_PIN, Bright.value);
    
    delay(150);
  }
}

void commandVerticalSlider(void)
{
  //
  // clear the screen and draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Equalizer");
  ui.clearDisplaySpace();

  int BtnX, BtnY;


  // initialize the sliders
  sBand1.init(BAND1, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand2.init(BAND2, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand3.init(BAND3, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand4.init(BAND4, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand5.init(BAND5, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand6.init(BAND6, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand7.init(BAND7, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand8.init(BAND8, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  
  // draw the slider controls with initial settings
  sBand1.draw(Band1);
  sBand2.draw(Band2);
  sBand3.draw(Band3);
  sBand4.draw(Band4);
  sBand5.draw(Band5);
  sBand6.draw(Band6);
  sBand7.draw(Band7);
  sBand8.draw(Band8);
  

  ui.lcdSetCursorXY(BAND1 , 30 ); ui.lcdPrint(Band1, 0);
  ui.lcdSetCursorXY(BAND2 , 30 ); ui.lcdPrint(Band2, 0);
  ui.lcdSetCursorXY(BAND3 , 30 ); ui.lcdPrint(Band3, 0);
  ui.lcdSetCursorXY(BAND4 , 30 ); ui.lcdPrint(Band4, 0);
  ui.lcdSetCursorXY(BAND5 , 30 ); ui.lcdPrint(Band5, 0);
  ui.lcdSetCursorXY(BAND6 , 30 ); ui.lcdPrint(Band6, 0);
  ui.lcdSetCursorXY(BAND7 , 30 ); ui.lcdPrint(Band7, 0);
  ui.lcdSetCursorXY(BAND8 , 30 ); ui.lcdPrint(Band8, 0);

  ui.lcdSetCursorXY(BAND1 , 220 ); ui.lcdPrint("63");
  ui.lcdSetCursorXY(BAND2 , 220 ); ui.lcdPrint("125");
  ui.lcdSetCursorXY(BAND3 , 220 ); ui.lcdPrint("250");
  ui.lcdSetCursorXY(BAND4 , 220 ); ui.lcdPrint("500");
  ui.lcdSetCursorXY(BAND5 , 220 ); ui.lcdPrint("1K");
  ui.lcdSetCursorXY(BAND6 , 220 ); ui.lcdPrint("2K");
  ui.lcdSetCursorXY(BAND7 , 220 ); ui.lcdPrint("4K");
  ui.lcdSetCursorXY(BAND8 , 220 ); ui.lcdPrint("8K");

  
  //
  // continuously check if any buttons are pressed by the user
  //
  while(true)
  {
    //
    // get events from the touch screen
    //
    ui.getTouchEvents();

    //
    // check if the user pressed and released the Back button, if so return to the main menu
    //
    if (ui.checkForBackButtonClicked())
      return;
    
    ui.getTouchScreenCoords(&BtnX, &BtnY);

    bool changed = sBand1.slide(BtnX, BtnY);
    sBand2.slide(BtnX, BtnY);
    sBand3.slide(BtnX, BtnY);
    sBand4.slide(BtnX, BtnY);
    sBand5.slide(BtnX, BtnY);
    sBand6.slide(BtnX, BtnY);
    sBand7.slide(BtnX, BtnY);
    sBand8.slide(BtnX, BtnY);

    // as mentioned above, you you don't want to process code unless slider was moved
    // wrap a change test around checkbox values
    // here only the band 1 screen text updates
    if (changed) {
      ui.lcdDrawFilledRectangle(BAND1, 28, BAND2 - BAND1, 10, BACKCOLOR);
      ui.lcdSetCursorXY(BAND1 , 30 ); ui.lcdPrint(sBand1.value, 0);
    }

    // as mentioned above, you code processes regardless if clider was moved
    // all remaining band values will redraw (and cause a slight flicker
    // if you hate flicker, add a change test around each one
    ui.lcdDrawFilledRectangle(BAND2, 28, BAND3 - BAND2, 10, BACKCOLOR);
    ui.lcdSetCursorXY(BAND2 , 30 ); ui.lcdPrint(sBand2.value, 0);

    ui.lcdDrawFilledRectangle(BAND3, 28, BAND4 - BAND3, 10, BACKCOLOR);
    ui.lcdSetCursorXY(BAND3 , 30 ); ui.lcdPrint(sBand3.value, 0);

    ui.lcdDrawFilledRectangle(BAND4, 28, BAND5 - BAND4, 10, BACKCOLOR);
    ui.lcdSetCursorXY(BAND4 , 30 ); ui.lcdPrint(sBand4.value, 0);

    ui.lcdDrawFilledRectangle(BAND5, 28, BAND6 - BAND5, 10, BACKCOLOR);
    ui.lcdSetCursorXY(BAND5 , 30 ); ui.lcdPrint(sBand5.value, 0);

    ui.lcdDrawFilledRectangle(BAND6, 28, BAND7 - BAND6, 10, BACKCOLOR);
    ui.lcdSetCursorXY(BAND6 , 30 ); ui.lcdPrint(sBand6.value, 0);

    ui.lcdDrawFilledRectangle(BAND7, 28, BAND8 - BAND7, 10, BACKCOLOR);
    ui.lcdSetCursorXY(BAND7 , 30 ); ui.lcdPrint(sBand7.value, 0);

    ui.lcdDrawFilledRectangle(BAND8, 28, 320 - BAND8, 10, BACKCOLOR);
    ui.lcdSetCursorXY(BAND8 , 30 ); ui.lcdPrint(sBand8.value, 0);
    
    delay(150);

  }
}