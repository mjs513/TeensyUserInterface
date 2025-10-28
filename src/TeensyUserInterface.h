//      ******************************************************************
//      *                                                                *
//      *            Header file for TeensyUserInterface.cpp             *
//      *                                                                *
//      *              Copyright (c) S. Reifel & Co,  2022               *
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


#ifndef TeensyUserInterface_h
#define TeensyUserInterface_h

#if __has_include("ILI9341_t3.h")
#include <ILI9341_t3.h>
#define ILI9341_t3 ILI9341_t3
#elif __has_include("ST7796_t3.h")
#include <ST7796_t3.h>
#define ILI9341_t3 ST7796_t3
#elif __has_include("ILI9488_t3.h")
#define ILI9341_t3 ILI9488_t3
#endif

#include <string>

#include <ILI9341_fonts.h>
#include <font_Arial.h>

/********************************/
/*  For ILI9341 controls        */
/********************************/

#define G_REPAINT 0
#define G_DRAWOVER 1
#define BELOW 0
#define ABOVE 1

#define SLIDER_HANDLE_SIZE 16
#define HANDLE_NONE 0
#define HANDLE_CIRCLE 1
#define HANDLE_SQUARE 2
#define HANDLE_TRIANGLE_1 3
#define HANDLE_TRIANGLE_2 4
#define HANDLE_TRIANGLE_3 5
#define HANDLE_RECTANGLE 6

#define LOCATION_TOP 0
#define LOCATION_BOTTOM 1

#define DISABLED 0
#define ENABLED 1

#define MAX_GRAPHS 10


#define B_PRESSED true
#define B_RELEASED false
#define TFT_DEBOUNCE 100  // debounce delay to minimize screen repress

#define CORNER_AUTO   -1
#define CORNER_SQUARE  0

#define  MAX_OPTION 20
#define OPTION_BUTTON_RADIUS 10

#define CHECKBOX_SIZE 20

#define C_DISABLE_LIGHT 0xC618
#define C_DISABLE_MED	0x7BCF
#define C_DISABLE_DARK	0x3186
#define MAXCHARLEN 31
#define MAXOPTIONS 13

/**************************************/

//
// lcd display screen orientations
//
const int LCD_ORIENTATION_PORTRAIT_4PIN_TOP = 0;
const int LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT = 1;
const int LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM = 2;
const int LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT = 3;


// 
// 16 bit colors in rgb 565 format
//
const uint16_t LCD_BLACK =       0x0000; 
const uint16_t LCD_NAVY =        0x000F;
const uint16_t LCD_DARKGREEN =   0x03E0;
const uint16_t LCD_DARKCYAN =    0x03EF;
const uint16_t LCD_MAROON =      0x7800;
const uint16_t LCD_PURPLE =      0x780F;
const uint16_t LCD_OLIVE =       0x7BE0;
const uint16_t LCD_GREY =        0xC618;
const uint16_t LCD_LIGHTGREY =   0xC618;
const uint16_t LCD_DARKGREY =    0x7BEF;
const uint16_t LCD_BLUE =        0x001F;
const uint16_t LCD_LIGHTBLUE =   0x5ADF;
const uint16_t LCD_DARKBLUE =    0x0016;
const uint16_t LCD_GREEN =       0x07E0; 
const uint16_t LCD_CYAN =        0x07FF;
const uint16_t LCD_RED =         0xF800;
const uint16_t LCD_MAGENTA =     0xF81F;
const uint16_t LCD_YELLOW =      0xFFE0;
const uint16_t LCD_WHITE =       0xFFFF;
const uint16_t LCD_ORANGE =      0xFD20;
const uint16_t LCD_GREENYELLOW = 0xAFE5;
const uint16_t LCD_MDGREY =     0x7BCF;


//
// datatype for fonts
//
typedef ILI9341_t3_font_t ui_font;


//
// definition of a Button, the menu's colors and font are used 
//
typedef struct 
{
  const char *labelText;
  int centerX;
  int centerY;
  int width;
  int height;
} BUTTON;


//
// definition of a Button with extended options
//
typedef struct 
{
  const char *labelText;
  int centerX;
  int centerY;
  int width;
  int height;
  uint16_t buttonColor;
  uint16_t buttonSelectedColor;
  uint16_t buttonFrameColor;
  uint16_t buttonTextColor;
  const ui_font &buttonFont;
} BUTTON_EXTENDED;


//
// definition of a integer Number Box 
//
typedef struct 
{
  const char *labelText;
  int value;
  int minimumValue;
  int maximumValue;
  int stepAmount;
  int centerX;
  int centerY;
  int width;
  int height;
} NUMBER_BOX;


//
// definition of a float Number Box 
//
typedef struct 
{
  const char *labelText;
  float value;
  float minimumValue;
  float maximumValue;
  float stepAmount;
  int digitsRightOfDecimal;
  int centerX;
  int centerY;
  int width;
  int height;
} NUMBER_BOX_FLOAT;


//
// definition of a Selection Box 
//
typedef struct 
{
  const char *labelText;
  int value;
  const char *choice0Text;
  const char *choice1Text;
  const char *choice2Text;
  const char *choice3Text;
  int centerX;
  int centerY;
  int width;
  int height;
} SELECTION_BOX;


//
// definition of a Slider 
//
typedef struct 
{
  const char *labelText;
  int value;
  int minimumValue;
  int maximumValue;
  int stepAmount;
  int centerX;
  int centerY;
  int width;
  int state;
} SLIDER;


//
// definition of an entry in menu's table
//
typedef struct _MENU_ITEM
{
  byte MenuItemType;
  const char *MenuItemText;
  void (*MenuItemFunction)();
  _MENU_ITEM *MenuItemSubMenu;
} MENU_ITEM;


//
// types of entries in a menu table
//
const byte MENU_ITEM_TYPE_MAIN_MENU_HEADER = 0;
const byte MENU_ITEM_TYPE_SUB_MENU_HEADER  = 1;
const byte MENU_ITEM_TYPE_SUB_MENU         = 2;
const byte MENU_ITEM_TYPE_COMMAND          = 3;
const byte MENU_ITEM_TYPE_TOGGLE           = 4;
const byte MENU_ITEM_TYPE_END_OF_MENU      = 5;


//
// number of columns of buttons in a menu
//
#define MENU_COLUMNS_1  ((void (*)()) 1)
#define MENU_COLUMNS_2  ((void (*)()) 2)
#define MENU_COLUMNS_3  ((void (*)()) 3)
#define MENU_COLUMNS_4  ((void (*)()) 4)

//
// types of touch events
//
const int TOUCH_NO_EVENT         = 0;       // no event from the touch screen (nothing being touched)
const int TOUCH_PUSHED_EVENT     = 1;       // touch screen just touched
const int TOUCH_RELEASED_EVENT   = 2;       // touch screen just released
const int TOUCH_REPEAT_EVENT     = 3;       // touch screen touched and event repeating


//
// the TeensyUserInterface class
//
class TeensyUserInterface
{
  public:
    //
    // public member variables
    //
    int lcdWidth;
    int lcdHeight;
    int displaySpaceWidth;
    int displaySpaceHeight;
    int displaySpaceLeftX;
    int displaySpaceRightX;
    int displaySpaceTopY;
    int displaySpaceBottomY;
    int displaySpaceCenterX;
    int displaySpaceCenterY;
    int touchEventType;
    int touchEventX;
    int touchEventY;
    byte toggleSelectNextStateFlg;
    const char *toggleText;


    //
    // public functions
    //
    TeensyUserInterface(void);
    void begin(int lcdCSPin, int LcdDCPin, int TouchScreenCSPin, int lcdOrientation, const ui_font &font);
    void setOrientation(int lcdOrientation);
    void setColorPaletteBlue(void);
    void setColorPaletteGray(void);

    void setMenuColors(uint16_t _menuBackgroundColor, uint16_t _menuButtonColor, uint16_t _menuButtonSelectedColor, uint16_t _menuButtonFrameColor, uint16_t _menuButtonTextColor);
    void setMenuFont(const ui_font &font);
    void selectAndDrawMenu(MENU_ITEM *menu, boolean drawMenuFlg);
    void displayAndExecuteMenu(MENU_ITEM *menu);
    void setInMenuCallbackFunction(void (*callbackFunction)());
 
    void setTitleBarColors(uint16_t _titleBarColor, uint16_t _titleBarTextColor, uint16_t _titleBarBackButtonColor, uint16_t _titleBarBackButtonSelectedColor);
    void setTitleBarFont(const ui_font &font);
    void drawTitleBar(const char *titleBarText);
    void drawTitleBarWithBackButton(const char *titleBarText);
    void drawTitleBarWithMenuButton(const char *titleBarText);
    boolean checkForBackButtonClicked(void);
    boolean checkForMenuButtonClicked(void);
    void clearDisplaySpace(void);
    void clearDisplaySpace(uint16_t backgroundColor);

    void drawButton(BUTTON &uiButton);
    void drawButton(BUTTON &uiButton, boolean buttonSelectedFlg);
    void drawButton(BUTTON_EXTENDED &uiButtonExt);
    void drawButton(BUTTON_EXTENDED &uiButtonExt, boolean buttonSelectedFlg);
    void drawButton(const char *buttonText, boolean buttonSelectedFlg, int buttonX, int buttonY, int buttonWidth, int buttonHeight);
    void drawButton(const char *buttonText, int buttonX, int buttonY, int buttonWidth, int buttonHeight, uint16_t buttonColor, uint16_t buttonFrameColor, uint16_t buttonTextColor, const ui_font &buttonFont);
    boolean breakStringAtWhiteSpace(const char *srcString, int *srcIndex, char *destString, int destBufferLength, int breakAtWhiteCount);
    boolean checkForButtonClicked(BUTTON &uiButton);
    boolean checkForButtonClicked(BUTTON_EXTENDED &uiButton);
    boolean checkForButtonAutoRepeat(BUTTON &uiButton);
    boolean checkForButtonAutoRepeat(BUTTON_EXTENDED &uiButton);
    boolean checkForButtonFirstTouched(BUTTON &uiButton);
    boolean checkForButtonFirstTouched(BUTTON_EXTENDED &uiButton);

    void drawNumberBox(NUMBER_BOX &numberBox);
    void drawNumberBox(NUMBER_BOX_FLOAT &numberBox);
    boolean checkForNumberBoxTouched(NUMBER_BOX &numberBox);
    boolean checkForNumberBoxTouched(NUMBER_BOX_FLOAT &numberBox);

    void drawSelectionBox(SELECTION_BOX &selectionBox);
    boolean checkForSelectionBoxTouched(SELECTION_BOX &selectionBox);

    void drawSlider(SLIDER &slider);
    void drawSliderBall(SLIDER &slider, uint16_t ballColor);
    boolean checkForSliderTouched(SLIDER &slider);

    boolean numericKeyPad(const char *titleBar, float &value, float minValue, float maxValue);
    boolean numericKeyPad(const char *titleBar, int &value, int minValue, int maxValue);

    boolean checkForTouchEventInRect(int eventType, int rectX1, int rectY1, int rectX2, int rectY2);
    void getTouchEvents(void);
    void setDefaultTouchScreenCalibrationConstants(int lcdOrientation);
    void setTouchScreenCalibrationConstants(int tsToLCDOffsetX_low, int tsToLCDOffsetX_high, int tsToLCDOffsetY_low, int tsToLCDOffsetY_high);
    boolean getTouchScreenCoords(int *xLCD, int *yLCD);

    void invertDisplay(bool invert);

    void lcdClearScreen(uint16_t color);
    void lcdDrawPixel(int x, int y, uint16_t color);
    void lcdDrawLine(int x1, int y1, int x2, int y2, uint16_t color);
    void lcdDrawHorizontalLine(int x, int y, int length, uint16_t color);
    void lcdDrawVerticalLine(int x, int y, int length, uint16_t color);
    void lcdDrawRectangle(int x, int y, int width, int height, uint16_t color);
    void lcdDrawRoundedRectangle(int x, int y, int width, int height, int radius, uint16_t color);
    void lcdDrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
    void lcdDrawCircle(int x, int y, int radius, uint16_t color);
    void lcdDrawFilledRectangle(int x, int y, int width, int height, uint16_t color);
    void lcdDrawFilledRoundedRectangle(int x, int y, int width, int height, int radius, uint16_t color);
    void lcdDrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
    void lcdDrawFilledCircle(int x, int y, int radius, uint16_t color);
    void lcdDrawImage(int x, int y, int width, int height, const uint16_t *image);
    void lcdSetFont(const ui_font &font);
    void lcdSetFontColor(uint16_t color);
    void lcdPrint(char *s);
    void lcdPrint(const char *s);
    void lcdPrint(int n);
    void lcdPrint(double n, int digitsRightOfDecimal = 5);
    void lcdPrintRightJustified(char *s);
    void lcdPrintRightJustified(const char *s);
    void lcdPrintRightJustified(int n);
    void lcdPrintRightJustified(double n, int digitsRightOfDecimal = 5);
    void lcdPrintCentered(char *s);
    void lcdPrintCentered(const char *s);
    void lcdPrintCentered(int n);
    void lcdPrintCentered(double n, int digitsRightOfDecimal = 5);
    void lcdPrintCharacter(byte character);
    int lcdStringWidthInPixels(char *s);
    int lcdStringWidthInPixels(const char *s);
    int lcdGetFontHeightWithoutDecenders(void);
    int lcdGetFontHeightWithDecentersAndLineSpacing(void);
    void lcdSetCursorXY(int x, int y);
    void lcdGetCursorXY(int *x, int *y);
    uint16_t lcdMakeColor(int red, int green, int blue);

    void writeConfigurationByte(int EEPromAddress, byte value);
    byte readConfigurationByte(int EEPromAddress, byte defaultValue);
    void writeConfigurationShort(int EEPromAddress, short value);
    short readConfigurationShort(int EEPromAddress, short defaultValue);
    void writeConfigurationInt(int EEPromAddress, int value);
    int readConfigurationInt(int EEPromAddress, int defaultValue);
    void writeConfigurationFloat(int EEPromAddress, float value);
    float readConfigurationFloat(int EEPromAddress, float defaultValue);
    boolean getRAWTouchScreenCoords(int *xRaw, int *yRaw);



  private:
    //
    // private member variables
    //
    MENU_ITEM *currentMenuTable;
    const ui_font *currentFont;
    void (*inMenuCallbackFunction)();

    uint16_t titleBarColor;
    uint16_t titleBarTextColor;
    uint16_t titleBarBackButtonColor;
    uint16_t titleBarBackButtonSelectedColor;
    const ui_font *titleBarFont;
    int buttonTypeOnTitleBar;

    uint16_t menuBackgroundColor;
    uint16_t menuButtonColor;
    uint16_t menuButtonSelectedColor;
    uint16_t menuButtonFrameColor;
    uint16_t menuButtonTextColor;
    const ui_font *menuButtonFont;

    int numberBoxRepeatCount;

    int touchScreenToLCDOffsetX_low;
    int touchScreenToLCDOffsetX_high;
    int touchScreenToLCDOffsetY_low;
    int touchScreenToLCDOffsetY_high;
    int touchState;
    int touchOrient;


    //
    // private functions
    //
    void executeMenuItem(int menuIdx);
    void drawMenu(void);
    void drawMenuItem(int menuIdx, boolean buttonSelectedFlg);
    int findMenuButtonForTouchEvent(void);
    void getMenuButtonSizeAndLocation(int menuButtonNumber, int *buttonX, int *buttonY, int *buttonWidth, int *buttonHeight);

    void drawTitleBar(const char *titleBarText, int buttonType);
    void drawTitleBarBackButton(boolean buttonSelectedFlg);
    void drawTitleBarMenuButton(boolean buttonSelectedFlg);
    void getBackButtonSizeAndLocation(int *buttonX, int *buttonY, int *buttonWidth, int *buttonHeight);
    void getMenuButtonSizeAndLocation(int *buttonX, int *buttonY, int *buttonWidth, int *buttonHeight);

    void drawDownButtonInNumberBox(NUMBER_BOX &numberBox, boolean showButtonTouchedFlg);
    void drawUpButtonInNumberBox(NUMBER_BOX &numberBox, boolean showButtonTouchedFlg);
    boolean updateNumberBoxNumber(NUMBER_BOX &numberBox, int stepAmount);
    void drawNumberInNumberBox(NUMBER_BOX &numberBox);
    void getNumberBoxCoordinates(NUMBER_BOX &numberBox, int *downButtonX, int *numberX, int *upButtonX, int *topY, int *buttonWidth, int *numberWidth, int *height);
    void drawDownButtonInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox, boolean showButtonTouchedFlg);
    void drawUpButtonInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox, boolean showButtonTouchedFlg);
    boolean updateNumberBoxNumberFloat(NUMBER_BOX_FLOAT &numberBox, float stepAmount);
    void drawNumberInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox);
    void getNumberBoxCoordinatesFloat(NUMBER_BOX_FLOAT &numberBox, int *downButtonX, int *numberX, int *upButtonX, int *topY, int *buttonWidth, int *numberWidth, int *height);

    void drawSelectionBoxCell(SELECTION_BOX &selectionBox, int cellNumber, boolean showButtonTouchedFlg);
    void getCoordsOfSelectionBoxCell(SELECTION_BOX &selectionBox, int cellNumber, int *X, int *Y, int *width, int *height);
    int countSelectionBoxChoices(SELECTION_BOX &selectionBox);

    int getSliderBallXPosition(SLIDER &slider);
    int getBallsValue(SLIDER &slider, int lcdX);

    void keypad_DisplayValueInStringBuf(void);
    void keypad_AddCharToStringBuf(char c, boolean &firstCharEntered);

    void touchScreenInitialize(int lcdOrientation);
    void touchScreenSetOrientation(int lcdOrientation);
     
    void lcdInitialize(int lcdOrientation, const ui_font &font);
    void lcdSetOrientation(int lcdOrientation);
};

/**********************/
/* ILI9341_controls.h */
/**********************/
class BarChartA {

public:

	BarChartA();

	void init(uint16_t ArcRadius, uint16_t ArcCenterY, uint16_t BarWidth, uint16_t OffsetFromTop, uint16_t SweepAngle, uint8_t Segments, float ScaleLow, float ScaleHigh);

	void draw(float Value);

	void setScale(float ScaleLow, float ScaleHigh);	

	void setBars(uint16_t SweepAngle, uint16_t Segments, uint16_t BarWidth, float GapSize);
	
	void setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV);
	
	void setSectionSize(float Divider1, float Divider2);

private:
	ILI9341_t3 			*d;
	float p1x= 0.0f, p1y= 0.0f, p2x= 0.0f, p2y= 0.0f, p3x= 0.0f, p3y= 0.0f, p4x= 0.0f, p4y= 0.0f, drawangle= 0.0f;
	uint16_t rad = 800, xoffset = 160, yoffset = 500, topoffset = 30;
	uint16_t sweepangle = 22;
	float startangle = 0.0f;
	uint16_t segments = 44;
	float width = 20, gap = 0.003;
	float arcangle = 0;
	uint16_t i = 0, barcolor;
	float low = 0.0f, high = 1024.0f;
	uint16_t	bars = 0;
	uint16_t barwidth = 0;
	uint8_t	divider = 0;
	float tempval = 0.0f;
	uint16_t color_l = 0x07E0;
	uint16_t color_m = 0xFFE0;
	uint16_t color_h = 0xF800;
	uint16_t color_v = 0x52AA;
	float divider_1= 0.5f;
	float divider_2= 0.75f;
	float divider1= 0.5f;
	float divider2=  0.75f;
	float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);
};


class BarChartH {

public:

	BarChartH();

	void init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc, const char *Title, uint16_t TextColor, uint16_t BorderColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );

  void setBars(uint16_t NumberofBars,float BarWidth, uint8_t DividerSize);

	void showScale(bool val);

	void setBarColor(uint16_t val = 0xF800);

	void draw(float val);

	void refresh();

	void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);	
	
	void setTitleText(const char *Title, const ILI9341_t3_font_t &TitleFont );	

	void showTitle(bool val);
	
	// allows bar segments as opposed to one large block, construct the object then override with these methods
	void useSegmentBars(bool val);
	void setSize(uint16_t Left, uint16_t Top, uint16_t Wide, uint16_t High, uint8_t Divider);
	void setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV);
	void setSectionSize(float Divider1, float Divider2);
	void setSectionSizeActual(float Divider1, float Divider2);
	float getBars();
	float getActualWidth();


private:
		ILI9341_t3_font_t	tf;
		ILI9341_t3_font_t	sf;
		bool	st = true, ss = true;
		char	titxt[40];
		char	sc[20];
		char	cc[2] = "D";
		char	text[30];
		float	XLow;
		float	XHigh;
		float	XInc;
		int Dec, tLen, tHi;
		float	Low;
		float	High;
		float	Inc;
		float	barinc;
		uint16_t	bars = 0;
		uint16_t barwidth = 0;
		uint16_t barcolor = 0;
		uint8_t	divider = 0;
		float	gx;
		float	gy;
		float	gw;
		float	gh;
		uint16_t color_l = 0;
		uint16_t color_m = 0;
		uint16_t color_h = 0;
		uint16_t color_v = 0;
		bool bartype = false;
		float divider_1= 0.0f;
		float divider_2= 0.0f;
		float divider1= 0.0f;
		float divider2= 0.0f;
		uint16_t cnt = 0;
		uint16_t tc;
		uint16_t oc;
		uint16_t rc;
		uint16_t bc;
		uint16_t ac;
		bool redraw;
		float stepval, range, TempY, level, i, data;
		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);
};


class BarChartV {

public:

	BarChartV();

	void init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc, const char *Title, uint16_t TextColor, uint16_t BorderColor, uint16_t BarColor, uint16_t BarBlankColor, uint16_t BackgroundColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );

	void setBarColor(uint16_t val = 0xF800);

	void draw(float val);
	
	void refresh();

	void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);

	void showTitle(bool val);

	void showScale(bool val);
	
	// allows bar segments as opposed to one large block, construct the object then override with these methods
	void useSegmentBars(bool val);
	void setBars(uint16_t NumberofBars,uint8_t BarHeight, uint8_t DividerSize);
	void setSize(uint16_t Left, uint16_t Top, uint16_t Wide, uint16_t High, uint8_t Divider);
	void setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV);
	void setSectionSize(float Divider1, float Divider2);
	void setSectionSizeActual(float Divider1, float Divider2);
	float getBars();
	float getActualHeight();

private:

		ILI9341_t3_font_t	tf;
		ILI9341_t3_font_t	sf;
		bool	st = true, ss = true;
		char	ti[40];
		char	sc[20];
		char	cc[2] = "D";
		char	text[30];
		float	XLow;
		float	XHigh;
		float	XInc;
		int Dec, tLen, tHi;
		float	Low;
		float	High;
		float	Inc;
		float	barinc;
		uint16_t	bars = 0;
		uint8_t barheight = 0;
		uint16_t barcolor = 0;
		uint8_t	divider = 0;
		uint16_t color_l = 0;
		uint16_t color_m = 0;
		uint16_t color_h = 0;
		uint16_t color_v = 0;
		bool bartype = false;
		float divider_1= 0.0f;
		float divider_2= 0.0f;
		float divider1= 0.0f;
		float divider2= 0.0f;
		uint16_t cnt = 0;
		float	gx;
		float	gy;
		float	gw;
		float	gh;
		
		uint16_t tc;
		uint16_t oc;
		uint16_t rc;
		uint16_t bc;
		uint16_t ac;
		bool redraw;
		float stepval, range, TempY, level, i, data;
		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);
};

class CGraph {

public:

	CGraph( float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float XAxisLow, float XAxisHigh, float XAxisInc, float YAxisLow, float YAxisHigh, float YAxisInc);

	void init(const char *Title, const char *XAxis, const char *YAxis, uint16_t TextColor, uint16_t GridColor, uint16_t AxisColor, uint16_t BackColor,uint16_t PlotkColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &AxisFont );

	void plot(int ID, float y);

	void setX(float x);

	int add(const char * DataLabel, uint16_t DataColor );

	void setYAxis(float Ylow, float YHigh, float YInc);

	void setXAxis(float XAxisLow, float XAxisHigh, float XAxisInc);

	void showTitle(bool val);

	void showLegend(bool val);

	void showAxisLabels(bool val);

	void drawLegend(byte Location);
	
	void resetStart(int ID);
	
	void showXScale(bool val);
	
	void setXTextOffset(int val);
	
	void setYTextOffset(int val);
	
	void setYLegendOffset(int val);
		
	void setXTextScale(float val);
	
	void showYScale(bool val);

	void setMarkerSize(int ID, byte val);

	void setLineThickness(int ID, byte val);
	
	void setLineColor(int ID, uint16_t LineColor);

	void setTitle(const char *Title);

	void setXAxisName(const char *Name);

	void setYAxisName(const char *Name);

	void drawGraph();

private:
		ILI9341_t3_font_t	tf;
		ILI9341_t3_font_t	af;
		int ID = 0;
		float x, y;
		float	i, j;
		bool	st, sl, sal, sxs, sys;
		float	Delta;
		int k;
		float	XLow, XHigh, XInc;
		float	YLow, YHigh, YInc;
		float XTextScale;
		bool RedrawGraph = true;
		bool HaveFirstPoint[10];
		float	XPoint, YPoint, oXPoint[10], oYPoint[10], TextHeight;
		float	XDec = 0.0, YDec = 0.0;
		char	text[30];
		byte	oOrientation = 0;
		float	gx, gy, gw, gh;
		int StartPointX, StartPointY, XScaleOffset, YScaleOffset, YlegendOffset;
		char buf0[20], buf1[20], buf2[20], buf3[20], buf4[20], buf5[20], buf6[20], buf7[20], buf8[20], buf9[20];
		char    *dl[20] = {buf0, buf1, buf2, buf3, buf4, buf5, buf6, buf6, buf8, buf9};
		char	title[40];
		byte tl = 0; // title location
		char	xatitle[40];
		char	yatitle[40];
		uint16_t tc;
		uint16_t dc[10];
		uint16_t ac;
		uint16_t gc;
		uint16_t bc;
		uint16_t pc;
		uint16_t linecolor[10];
		byte pdia[10];
		byte linet[10];
		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);

};

class Dial {
public:
	Dial(int CenterX, int CenterY, int DialRadius, float LowVal , float HiVal , float ValInc, float SweepAngle);
	
	void init(uint16_t NeedleColor, uint16_t DialColor, uint16_t TextColor, uint16_t TickColor, const char *Title, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &DataFont );

	void draw(float val);
		
private:
	
	bool Redraw = true;
	ILI9341_t3 			*d;			
	ILI9341_t3_font_t	tf;
	ILI9341_t3_font_t	df;
	char t[40];
	int cx;
	int cy;
	int dr;
	float lv;
	float hv;
	float inc;
	float sa;
	uint16_t nc;
	uint16_t dc;
	uint16_t tc;
	uint16_t ic;
	float degtorad;
	float offset, stepval, angle, data;
	float i;

	//variables to track new needle values
	float ix;
	float iy;
	float ox;
	float oy;
	float tx;
	float ty;
	float dx;
	float dy;
	float lx;
	float rx;
	float ly;
	float ry;
	int tLen, tHi;
	char buf[38];
	int dec;
	// variables to track previous needle values
	float px;
	float py;
	float pix;
	float piy;
	float plx;
	float ply;
	float prx;
	float pry;

};

class SliderH {

 public:

	SliderH();		// class constructor
		
	void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, float ScaleLow, float ScaleHi, float Scale, float Snap, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);		// initializer
  
	void draw(float val);					// method to draw complete slider
		
	bool slide(float ScreenX, float ScreenY);			// method to move handle as user drags finger over handle, this method automatically looks for a valid range press
  
	void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);		// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t HandleColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void setDisableColor(uint16_t HandleColor, uint16_t SliderColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void setHandleSize(int size);

	void setHandleSize(int size, int width);

	void disable();

	void enable();

	void show();

	void hide();

    void setBarThickness(byte Thickness);

	void setHandleShape(byte shape);

	void drawSliderColor(bool color);

	void setPressDebounce(byte Debounce);

	void resetScale(float ScaleLow, float ScaleHi, float Scale, float Snap);

	float value;

private:

	ILI9341_t3 *d;			// the display object
	uint16_t sColor;		// the slider color
	uint16_t bColor;		// the slider background color
	uint16_t hColor;		// the sliders drag handle
	uint16_t dsColor;
	uint16_t dhColor;
	uint16_t tsColor;
	uint16_t ssColor;
	uint16_t thColor;
	float x;
	float y;
	uint16_t l;			// the left coordinate of the scale
	uint16_t t;			// the top coordinate of the scale
	uint16_t w;			// the width of the scale
	byte bt;			// the thockness of the bar
	float ox;			// the old screen x value where user pressed
	bool enabled;
	float sl;				// the scale lower value
	float sh;				// the scale upper value
	float pos;				// the position on the scale
	float sn;				// the snap increment
	float sc;				// the scale increment
	float ce;				// the tick mark where zero is (for drawing heavy line on +/- scales
	float i;				// loop counter
	int handlesize;
	int handlewidth;	
	byte handleshape;
	bool visible;
	bool colorscale;		// flag to draw slider in handle color
	float MapFloat(float x, float fromLow, float fromHigh, float toLow, float toHigh); // why Arduino has no mapping for floats is beyond me, here it is...
	byte debounce;
	
  };

class SliderV {

 public:

	SliderV(); // class constructor
  
	void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderH, float ScaleLow, float ScaleHi, float scale, float snap, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);	// initializer
   
	void draw(float val);						// method to draw complete slider
   
	bool slide(uint16_t ScreenX, uint16_t ScreenY);	   // method to move handle as user drags finger over handle, this method automatically looks for a valid range press
    
	void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);	// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t HandleColor);		// method to just draw the handle (useful for showing handle in green for OK value

	void setHandleSize(int val);

	void setHandleSize(int size, int width);
	
	void setHandleShape(byte val);

	void drawSliderColor(bool val);

	void setDisableColor(uint16_t HandleColor, uint16_t SliderColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void setScale(float ScaleLow, float ScaleHi, float scale = 0.0, float snap= 0.0);

	void setBarThickness(byte Thickness);

	void disable();

	void enable();

	void show();

	void hide();

	void setPressDebounce(byte Debounce);

	float value;

private:

	uint16_t sColor;		// the slider color
	uint16_t bColor;		// the slider background color
	uint16_t hColor;		// the sliders drag handle
	uint16_t dsColor;
	uint16_t dhColor;
	uint16_t tsColor;
	uint16_t thColor;
	uint16_t x;			// the left coordinate of the scale
	uint16_t y;			// the top coordinate of the scale
	uint16_t l;			// the left coordinate of the scale
	uint16_t t;			// the top coordinate of the scale
	uint16_t w;			// the with of the scale
	uint16_t h;			// the with of the scale
	byte bt;			// the thockness of the bar
	float oy;			// the old screen y value where user pressed
	float sl;				// the scale lower value
	float sh;				// the scale upper value
	float pos;				// the screen coordinate position
	float sn;				// the snap increment
	float sc;				// the scale increment
	float ce;				// the tick mark where zero is (for drawing heavy line on +/- scales
	float i;				// loop counter
	byte tl;
	bool colorscale;		// flag to draw slider in handle color
	float MapFloat(float x, float fromLow, float fromHigh, float toLow, float toHigh);// why Arduino has no mapping for floats is beyond me, here it is...
	int tLen, tHi;
	int handlesize;
	int handlewidth;
	byte handleshape;
	bool enabled;
	bool visible;
	byte debounce;
  };

class SliderOnOff {

 public:
	
	SliderOnOff(uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, uint16_t SliderH, uint16_t SliderColor, uint16_t BackColor, uint16_t OnColor, uint16_t OffColor);// class constructor
  
	void draw(bool state);			// method to draw complete slider
   
	bool slide(float ScreenX,float ScreenY);	// method to move handle as user drags finger over handle, this method automatically looks for a valid range press

	bool changed();						// method to return if state change, useful for determining if a something should be done but not done unless state change

	bool getValue();
     
private:

	uint16_t _sColor;		// the slider color
	uint16_t _bColor;		// the slider background color
	uint16_t _onColor;		// the sliders on color
	uint16_t _offColor;		// the sliders on color
	uint16_t _l;				// the left coordinate of the scale
	uint16_t _t;				// the top coordinate of the scale
	uint16_t _w;				// the with of the scale
	uint16_t _h;				// the with of the scale
	bool _pos;				// the screen coordinate position
	bool _changed;			//flag to track if button was just changed
  };


/*

Checkbox class

*/


class CheckBox {
public:
	CheckBox();

	void init(int16_t ButtonX, uint16_t ButtonY, uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor, uint16_t TextColor, uint16_t BackgroundColor, int TextOffsetX,int TextOffsetY, const char *Text, const ILI9341_t3_font_t &TextFont );

	void draw(bool val); 
	bool press(int16_t SceenX, int16_t ScreenY); 
	void show();
	void hide();
	void disable();
	void enable();
	void resize(int16_t ButtonX, int16_t ButtonY, uint8_t Size);
	void setColors(uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor, uint16_t BackgroundColor, uint16_t DisableOutlineColor,  uint16_t DisableTextColor, uint16_t DisableUPColor, uint16_t DisableDownColor); 
	void setText(int TextOffsetX,int TextOffsetY, const char *Text, const ILI9341_t3_font_t &TextFont); 
	void setCornerRadius(int val);		
	bool isEnabled();;
	bool isVisibled();;
	void setPressDebounce(byte Debounce);
	bool value;

private:
	char label[60];
	ILI9341_t3_font_t f;
	int16_t x, y;
	uint16_t s, ct;
	uint16_t oc, uc, dc, bc, doc, duc, ddc, dtc, tc;
	bool state;
	int tox, toy;
	bool enabled;
	bool visible;
	byte debounce;
};


/*

Checkbox class

*/

class OptionButton {

	
public:
	OptionButton();

	void init(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, int16_t TextColor, uint16_t BackgroundColor, int TextOffsetX,int TextOffsetY, const ILI9341_t3_font_t &TextFont);
	int add(uint16_t ButtonX, uint16_t ButtonY,const char *Text, float OptionValue = -32001 ); 
	void draw(int OptionID); 
	bool press(uint16_t ScreenX, uint16_t ScreenY);
	void select(int val);	
	void show(); 
	void hide();
	void disable();
	void enable();
	void resize(byte radius);
	void setColors(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, uint16_t TextColor, uint16_t BackgroundColor, uint16_t DisableOutlineColor, uint16_t DisableSelColor, uint16_t DisableUnSelColor, uint16_t DisableTextColor); 
	void setFont(int TextOffsetX,int TextOffsetY, const ILI9341_t3_font_t &TextFont);

	void setText(int ID, const char *Text);;
	int selected(); 
	bool isEnable();
	bool isVisible();
	void setPressDebounce(byte Debounce);
  
	float value;
	int option;

private:
	char label[MAX_OPTION][60];
	ILI9341_t3_font_t f;
	uint16_t x[MAX_OPTION], y[MAX_OPTION];
	float rv[MAX_OPTION];
	uint16_t r;
	int i, tox, toy;
	int ID;
	int current;
	uint16_t oc, sc, uc, bc, tc, doc, dsc, duc, dtc;
	bool enabled;
	bool visible;
	byte debounce;
};


class SliderD {

 public:

	SliderD(); // class constructor
  
	void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderR, float SweepAngle, float ScaleLow, float ScaleHi, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);	// initializer
   
	void draw(float val);						// method to draw complete slider
   
	bool slide(uint16_t ScreenX, uint16_t ScreenY);	   // method to move handle as user drags finger over handle, this method automatically looks for a valid range press
    
	void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);	// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t HandleColor);		// method to just draw the handle (useful for showing handle in green for OK value

	void setHandleSize(int val);

	void drawSliderColor(bool val);

	void setDisableColor(uint16_t HandleColor, uint16_t SliderColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void setScale(float ScaleLow, float ScaleHi);

	void setRingThickness(byte Thickness);

	void disable();

	void enable();

	void show();

	void hide();

	void setPressDebounce(byte Debounce);

	float value;
	bool state;

private:

	uint16_t sColor;		// the slider color
	uint16_t bColor;		// the slider background color
	uint16_t hColor;		// the sliders drag handle
	uint16_t dsColor;
	uint16_t dhColor;
	uint16_t tsColor;
	uint16_t thColor;
	uint16_t x;			// the left coordinate of the scale
	uint16_t y;			// the top coordinate of the scale
	uint16_t r;			// the left coordinate of the scale
	float sa, as, ae; // sweep angle, start angle, end angle
	byte dt;
	float sl;				// the scale lower value
	float sh;				// the scale upper value
	float angle, oangle, hx, hy;				// the screen coordinate position
	float i;				// loop counter
	float dist;
	bool colorscale;		// flag to draw slider in handle color
	bool pressed = false;
	float MapFloat(float x, float fromLow, float fromHigh, float toLow, float toHigh);// why Arduino has no mapping for floats is beyond me, here it is...
	int handlesize;
	bool enabled;
	bool visible, redraw;
	byte debounce;
	void DrawRing(float start, float end, uint16_t color);
	void DrawHandle(float angle, uint16_t hColor, uint16_t sColor);
  };

// ------------------------------------ End ---------------------------------

class sliderM : TeensyUserInterface
{
  public:
    enum class SLIDER_MODE
    {
      SLIDER_MODE_HORIZONTAL,
      SLIDER_MODE_VERTICAL
    };

    struct Slider
    {
      std::string symbol = "?";
      unsigned int   xCenterLoc;   // center of slider x location
      unsigned int   yCenterLoc;   // center of slider y location
      unsigned int   xSize;        // Length of slider body
      unsigned int   ySize;        // thickness of slider body
      float          value;        // initial value of sliders handle
      unsigned int   minorTickSections;  // normally = 10
      unsigned int   majorTickSections;  // normally =  2
      unsigned int   placesBeforeTheDecimal;  //next 3 describe the value printed on the screen format
      unsigned int   placesAfterTheDecimal;
      boolean        showPlusMinusSign;
                      //Slider Range
      float          minValue;  // for HORIZONTAL = all the way to the left, for VERTICAL, all the way up
      float          maxValue;  // for HORIZONTAL = all the way to the right, for VERTICAL, all the way down
                      //enable or disable bump arrows
      boolean        withBumpUpArrow;
      boolean        withBumpDownArrow;
      float          bumpValue;          //Increment of bump
                      // Slider Value location to be printed on Screen
      unsigned int   xValueCenterLoc;
      unsigned int   yValueCenterLoc;
      int16_t        textWidthPrev;
                      //colors
      uint16_t       valueColor;
      uint16_t       valueBgColor;
      uint16_t       backgroundColor;          //Color of interior of slider boder
      uint16_t       borderColor;              // Border color of slider - all
      uint16_t       scaleColor;               // color of the scale lines of the slider
      uint16_t       handleColor;              // Color of the indicator handle
      uint16_t       handleBorderColor;        // Color of the indicator handle border
      uint16_t       bumpBackgroundColor;      // Background colore for bump box
      
                      //Set colors for slider if it has been disabled
      uint16_t       backgroundColorDisabled = LCD_MDGREY;
      uint16_t       borderColorDisabled = LCD_MDGREY;
      uint16_t       scaleColorDisabled = LCD_MDGREY;
      uint16_t       handleColorDisabled = LCD_BLACK;
      uint16_t       handleBorderColorDisabled = LCD_BLACK;

      boolean        activated;
      boolean        repeatEnabled;
      boolean        previouslyTouched;
      unsigned long  touchStartMillis;
      unsigned int   repeatMilliseconds;
      SLIDER_MODE    orientation;

      const ILI9341_t3_font_t* font = &Arial_12;

      //Slider() = default;
      //Slider(const std::string& symbol, SLIDER_MODE type);  // Constructor
    };

    sliderM(const std::string& symbol, SLIDER_MODE type);
    //Slider createSlider(const std::string& symbol, SLIDER_MODE type);
    void init(int x, int y, int xsize, int ysize, bool bumpUp, bool bumpDwn);
    void setEnable(bool active);

    void setPosition(int x, int y);
    void setSliderColors(uint16_t valueColor, uint16_t valueBgColor, uint16_t backgroundColor, uint16_t borderColor);
    void setAxis(float minValue, float maxValue, int minorTicks, int majorTicks);
    void setHandle(int xCenter, int yCenter, uint16_t handleColor, uint16_t handleBorderColor);
    void setHandleColors(uint16_t handleColor, uint16_t handleBorderColor);
    void setBump(float bumpValue, bool repeatEnabled, bool bumpUp, bool bumpDwn);
    void setBumpColor(uint16_t bumpBackgroundColor);
    void setValue(float value);
    float getValue();
    void setValueFont(const ILI9341_t3_font_t *f, uint16_t color, uint16_t bgColor);
    void setSliderPrint(int placesBeforeTheDecimal, int placesAfterTheDecimal,bool showPlusMinusSign);

    int16_t centerDrawText( const String text, unsigned int xCenterLoc, unsigned int yCenterLoc, uint16_t textColor, uint16_t textBackground, int16_t w_prev);
    void drawSlider();

    boolean checkSliderBumpUp(int screenX, int screenY, bool previouslyTouched);
    boolean checkSliderBumpDown(int screenX, int screenY, bool previouslyTouched);
    boolean checkSlider(int screenX, int screenY, bool previouslyTouched);
    void setDisabledColors(uint16_t bgColor, uint16_t borderColor, 
                        uint16_t scaleColor, uint16_t handleColor, 
                        uint16_t handleBorderColor);
	boolean checkSliderT();

  private:
    Slider          s;  // instance of the data structure defined above.
    uint32_t BUMP_REPEAT_START_DELAY_MILLISECONDS = 750;
};


#endif
