//Include all necessary libraries
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h> //Graphics hardware library
#include <Adafruit_STMPE610.h> //Touchscreen hardware library
#include <InkShield.h> //inkshield library

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//initialize shield on pin 2
InkShieldA0A3 MyInkShield(2);
int enable = 1; //flag to only print when complete swipe

//initialize constants for designs
const int CharRows = 8; //how many rows each character/letter is (must be the same for all characters)
const int numOfChar = 30; //this is the number of letters/characters (including spaces) printed each time it is triggered. Change this as needed
const int fontSize = numOfChar*CharRows;


const word font[numOfChar][CharRows] = {
{
0b000000000000,
0b011111111000,
0b011111111100,
0b000001100110,
0b000001100110,
0b011111111100,
0b011111111000,
0b000000000000
},
{
0b000000000000,
0b001111111110,
0b011111111110,
0b011001100110,
0b001111111110,
0b001111111100,
0b000110011000,
0b000000000000
},
{
0b000000000000,
0b000111111000,
0b001111111100,
0b011100000110,
0b011100000110,
0b001110011100,
0b000110011000,
0b000000000000
},
{
0b000000000000,
0b011111111110,
0b011111111110,
0b011100000110,
0b011100000110,
0b001111111100,
0b000111111000,
0b000000000000
},
{
0b000000000000,
0b001111111100,
0b011111111110,
0b011001100110,
0b011001100110,
0b011001100110,
0b011001100110,
0b000000000000
},
{
0b000000000000,
0b000000000000,
0b011111111110,
0b011111111110,
0b000001100110,
0b000001100110,
0b000000000110,
0b000000000000
},
{
0b000000000000,
0b000111111000,
0b001111111100,
0b011100001110,
0b011101001110,
0b001111011100,
0b000111011000,
0b000000000000
},
{
0b000000000000,
0b011111111110,
0b011111111110,
0b000001100000,
0b000001100000,
0b011111111110,
0b011111111110,
0b000000000000
},
{
0b000000000000,
0b000000000000,
0b000000000000,
0b011111111110,
0b011111111110,
0b000000000000,
0b000000000000,
0b000000000000
},
{
0b000000000000,
0b001111000000,
0b011111000000,
0b011000000000,
0b011111111110,
0b001111111110,
0b000000000000,
0b000000000000
},
{
0b000000000000,
0b011111111110,
0b011111111110,
0b000000110000,
0b000000110000,
0b011111011110,
0b011111001110,
0b000000000000
},
{
0b000000000000,
0b011111111110,
0b011111111110,
0b011000000000,
0b011000000000,
0b011000000000,
0b011000000000,
0b000000000000
},
{
0b011111111100,
0b011111111110,
0b000000000110,
0b011111111110,
0b011111111110,
0b000000000110,
0b011111111110,
0b011111111100,
},
{
0b000000000000,
0b011111111100,
0b011111111110,
0b000000000110,
0b000000000110,
0b011111111110,
0b011111111100,
0b000000000000
},
{
0b000000000000,
0b001111111100,
0b011111111110,
0b010000000010,
0b010000000010,
0b011111111110,
0b001111111100,
0b000000000000
},
{
0b000000000000,
0b011111111110,
0b011111111110,
0b000001100110,
0b000001111110,
0b000000111100,
0b000000001000,
0b000000000000
},
{
0b000000000000,
0b000111111100,
0b001111111110,
0b001100000110,
0b001111111110,
0b010111111100,
0b000000000000,
0b000000000000
},
{
0b000000000000,
0b011111111110,
0b011111111110,
0b000001100110,
0b000001111110,
0b011111111110,
0b011111011000,
0b000000000000
},
{
0b000000000000,
0b011000010000,
0b011001111100,
0b011001101110,
0b011011000110,
0b011111000110,
0b001110000110,
0b000100000000
},
{
0b000000000000,
0b000000000110,
0b000000000110,
0b011111111110,
0b011111111110,
0b000000000110,
0b000000000110,
0b000000000000
},
{
0b000000000000,
0b001111111110,
0b011111111110,
0b011000000000,
0b011000000000,
0b011111111110,
0b001111111110,
0b000000000000
},
{
0b000000111110,
0b000111111110,
0b011111000000,
0b011000000000,
0b011111000000,
0b001111111110,
0b000000111110,
0b000000000000
},
{
0b000111111110,
0b001111111110,
0b011000000000,
0b001111111110,
0b001111111110,
0b011000000000,
0b001111111110,
0b000111111110
},
{
0b000000000000,
0b011100001110,
0b011110011110,
0b000011110000,
0b000011110000,
0b011110011110,
0b011100001110,
0b000000000000
},
{
0b000000000000,
0b000000011110,
0b000000011110,
0b011111110000,
0b011111110000,
0b000000011110,
0b000000011110,
0b000000000000
},
{
0b000000000000,
0b011100000110,
0b011110000110,
0b011111100110,
0b011001111110,
0b011000011110,
0b011000001110,
0b000000000000
},
{
0b000000000000,
0b000000000000,
0b000000000000,
0b000000000000,
0b000000000000,
0b000000000000,
0b000000000000,
0b000000000000
},
{
0b000000000000,
0b000000110000,
0b000001111000,
0b000011111000,
0b000111110000,
0b000011111000,
0b000001111000,
0b000000110000
},
{
0b000000000000,
0b000000000000,
0b011110000000,
0b011110000000,
0b011110000000,
0b000000000000,
0b000000000000,
0b000000000000
},
{
0b000000000000,
0b000000000000,
0b011001111110,
0b011001111110,
0b011001111110,
0b000000000000,
0b000000000000,
0b000000000000
}
};


void setup(void) {
  
  Serial.begin(9600);
  Serial.println(F("Preemadonna,Inc"));
  
  tft.begin();

  if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }

  //This makes a white screen with a horizontal magenta band, and a vertical green line
  //Feel free to explore other graphic primitives to design what the screen looks like
  tft.fillScreen(ILI9341_WHITE);
  tft.fillRect(180, 0, 40, 319, ILI9341_MAGENTA);
  tft.drawLine(0,150,250,150,ILI9341_GREEN);
  
}


void loop()
{
  // See if there's any  touch data for us
  if (ts.bufferEmpty()) {
    return;
  }

  // Retrieve a point  
  TS_Point p = ts.getPoint();
  
 //print the raw values
  Serial.print("X = "); Serial.print(p.x);
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print("\tPressure = "); Serial.println(p.z);  
 
 
  // Scale from ~0->4000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  //print the scaled values
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");

  char blah[] = "HELLO WORLD";
  
    if (p.y>100 && p.y<160)
    {
          if(enable==1)
          {

          for (int i=sizeof(blah)-2; i>=0; i--)
          {
            if (blah[i] == ' ')
              spray_letter(26);
            else if (blah[i] == '1')
              spray_letter(27);
            else if (blah[i] == '.')
              spray_letter(28);
            else if (blah[i] == '!')
              spray_letter(29);
            else
              spray_letter(blah[i] - 'A');
          }
          
          enable = 0; //reset enable
    }}
  
  if (p.y>200){enable=1;} //change enable only if the end of the swipe
  
}

void spray_letter(int letter)
{
  //loop through the rows of the letter
  for(int row=CharRows-1;row>-1;row--){
    //word strip = font[(letter*CharRows)+row];
    word strip = font[letter][row];
    //print the row (change the number of times it sprays to change the darkness)

    for (int i=0; i<3; i++)
     MyInkShield.spray_ink(strip);
  
    delay(8);
  }
}
