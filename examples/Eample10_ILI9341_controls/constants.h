
// barchartH
//
#define FONT_TITLE Arial_24
#define FONT_DATA Arial_10
#define LED_PIN A9
#define B_XORIGIN 20
#define B_YORIGIN 50
#define B_WIDE 200
#define B_HEIGHT 50
#define B_LOWSCALE 0.0
#define B_HIGHSCALE 4.0
#define B_SCALEINC .5

#define V_XORIGIN 40
#define V_YORIGIN 150
#define V_WIDE 200
#define V_HEIGHT 25
#define V_LOWSCALE 0.0
#define V_HIGHSCALE 4.0
#define V_SCALEINC .5

#define BORDERCOLOR LCD_WHITE
#define BACKCOLOR LCD_BLACK

#define TEXT_COLOR LCD_WHITE
#define BORDER_COLOR LCD_WHITE
#define BAR_COLOR LCD_BLUE
#define NULL_COLOR LCD_BLACK
#define BACK_COLOR LCD_BLACK
#define WARNING_COLOR LCD_RED

/** graphing **/
// defines for graph location and scales
#define X_ORIGIN    50
#define Y_ORIGIN    250
#define X_WIDE 		  250
#define Y_HIGH 		  150
#define X_LOSCALE 	0
#define X_HISCALE 	10
#define X_INC 	    2
#define Y_LOSCALE 	-1
#define Y_HISCALE 	1
#define Y_INC 	    0.2

#define TEXTCOLOR LCD_WHITE
#define GRIDCOLOR 0xC618
#define AXISCOLOR LCD_YELLOW
#define BACKCOLOR LCD_BLACK
#define PLOTCOLOR LCD_DARKGREY
#define VOLTSCOLOR LCD_RED
#define SINCOLOR LCD_GREEN
#define COSCOLOR LCD_BLUE

//Checkbox
#define LONG_PRESS_DURATION 1000
#define NO_PRESS 0
#define SHORT_PRESS 1
#define LONG_PRESS 2

#define ROW1 55
#define ROW2 90
#define ROW3 125
#define ROW4 160
#define ROW5 215

#define OUTLINECOLOR    LCD_GREY
#define CHECKEDCOLOR    LCD_GREEN
#define UNCHECKEDCOLOR  LCD_BLACK
#define BACKCOLOR       LCD_BLACK
#define TEXTCOLOR       LCD_WHITE

/*   Dial    */
#define FONT_TITLE1 Arial_16
#define FONT_DATA1 Arial_10_Bold
// defines for locations
#define BXCENTER 80
#define BYCENTER 120
#define BDIAMETER 75
#define BLOWSCALE 0
#define BHIGHSCALE 1200
#define BSCALEINC 200
#define BSWEEPANGLE 300

#define VXCENTER 240
#define VYCENTER 120
#define VDIAMETER 75
#define VLOWSCALE 0
#define VHIGHSCALE 4
#define VSCALEINC .5
#define VSWEEPANGLE 300

// defines for colors
#define BNEEDLECOLOR LCD_ORANGE
#define BDIALCOLOR LCD_DARKBLUE
#define BTEXTCOLOR LCD_WHITE
#define BTICCOLOR LCD_GREY

#define VNEEDLECOLOR LCD_WHITE
#define VDIALCOLOR LCD_DARKBLUE
#define VTEXTCOLOR LCD_WHITE
#define VTICCOLOR LCD_GREY


/*  Option button  */
#define FONT Arial_16
#define ROW11 60
#define ROW21 90
#define ROW31 120
#define ROW41 150
#define ROW51 200

#define COL1 10
#define COL2 180

/* Horizontal slider */
#define FONT Arial_16
#define ROW02 30
#define ROW12 70
#define ROW22 110
#define ROW32 150
#define ROW42 190

/* vertical slider */
#define BAND1 20
#define BAND2 60
#define BAND3 100
#define BAND4 140
#define BAND5 180
#define BAND6 220
#define BAND7 260
#define BAND8 300

#define SLIDECOLOR LCD_GREY
#define HANDLECOLOR LCD_ORANGE
#define BACKCOLOR LCD_BLACK
#define TEXTCOLOR LCD_WHITE
#define MINDB -12
#define MAXDB 12
#define TICK 2
#define SNAP 1

// note your HEIGHT / (max scale) should be an int
// otherwise you may get some draw errors
#define TOP 45
#define HEIGHT 168

// create some band variables
float Band1 = 4;
float Band2 = 3;
float Band3 = 2;
float Band4 = 1;
float Band5 = 0;
float Band6 = -1;
float Band7 = -2;
float Band8 = -3;