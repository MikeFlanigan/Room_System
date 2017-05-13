#include <FastLED.h>

#define LED_PIN     11
#define NUM_LEDS    150
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

/////////Raspberry pi digital coms/////////////////////
int Speaker_in_Pin = 12;
int Lights_Pin = 9;
int Flash_Pin = 8;
//int unused_Pin = 7;
//////////////////////////////
/////////Outputs/////////////////////
int Speaker_Power_Pin = 13;
// LED pin defined above
///////////////////////////////////
///////// Phsyical inputs //////////////////
int Brightness_Pot = A2;
int Btn1_Pin = 2; // snooze
int Btn2_Pin = 3; // pause play for now
int Btn3_Pin = 4; // on off lights
////////////////////////////////////////


int Brightness = 0;
bool Lights_ON = false;
bool Flashing_Lights = false;
bool Speaker_Power = false;
bool Btn1_ON = false;
bool Btn2_ON = false;
bool Btn3_ON = false;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  Serial.begin(9600);
  pinMode(Lights_Pin, INPUT_PULLUP);
  pinMode(Flash_Pin, INPUT_PULLUP);
  pinMode(Speaker_in_Pin, INPUT_PULLUP);
  pinMode(Speaker_Power_Pin, OUTPUT);

  pinMode(Btn1_Pin, INPUT_PULLUP);
  pinMode(Btn2_Pin, INPUT_PULLUP);
  pinMode(Btn3_Pin, INPUT_PULLUP);
}

void loop() {
  Lights_ON = not digitalRead(Lights_Pin);
  Flashing_Lights = not digitalRead(Flash_Pin);
  Speaker_Power = not digitalRead(Speaker_in_Pin);
  Brightness = analogRead(Brightness_Pot);
  Brightness = map(Brightness, 0, 1023, 0, 255);
  Btn1_ON = not digitalRead(Btn1_Pin);
  Btn2_ON = not digitalRead(Btn2_Pin);
  Btn3_ON = not digitalRead(Btn3_Pin);



  if (Speaker_Power) {
    digitalWrite(Speaker_Power_Pin, HIGH);
  }
  else {
    digitalWrite(Speaker_Power_Pin, LOW);
  }

  
  if (Flashing_Lights) {
    // nothing for now
    //    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    //    delay(1000);              // wait for a second
    //    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    //    delay(1000);              // wait for a second
    //    Serial.print("Flash");
  }
  else {
    // nothing for now
  }

  
  if (Lights_ON) {
    ChangePalettePeriodically();
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors(startIndex, (int) Brightness);
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
  else {
    fill_solid( currentPalette, 16, CRGB::Black);
    FastLED.show();
  }


  // debug printing
  //  Serial.print(Flashing_Lights);
  //  Serial.println(Brightness);
    Serial.println(digitalRead(Lights_Pin));
}



///////////////// LED CONTROL FUNCTIONS ////////////////////////

void FillLEDsFromPaletteColors( uint8_t colorIndex, uint8_t brightness)
{
  //    uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 10)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 15)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 20)  {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 25)  {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 30)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = NOBLEND;
    }
    if ( secondHand == 35)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 40)  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 45)  {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 50)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 55)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = LINEARBLEND;
    }
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};
