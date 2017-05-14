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
//int unused_Pin = 8;
int Flash_Pin = 7;
//////////////////////////////
/////////Outputs/////////////////////
int Speaker_Power_Pin = A4;
// LED pin defined above
///////////////////////////////////
///////// Phsyical inputs //////////////////
int Brightness_Pot = A2;
int Btn1_Pin = 2; // snooze
int Btn2_Pin = 3; // pause play for now
int Btn3_Pin = 4; // on off lights
////////////////////////////////////////

// fading variables
unsigned long Now = 0;
unsigned long Start = 0;
unsigned long Reading_Start = 0;
unsigned long Reading_Fade_Start = 0;
int Reading_duration = 900; // 15 minutes in seconds
int fade_time = 600; // 10 minutes in seconds


int Brightness = 0;
bool Lights_ON = false;
bool Lights_Change = false;
bool Flashing_Lights = false;
bool Speaker_Power = false;
bool Btn1_ON = false;
bool Btn2_ON = false;
bool Btn3_ON = false;

bool Snooze_Speakers = false;

bool Reading_Lights = false;

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
  Now = millis() / 1000; // everything is done in seconds

  Lights_ON = not digitalRead(Lights_Pin);
  Flashing_Lights = not digitalRead(Flash_Pin);
  Speaker_Power = not digitalRead(Speaker_in_Pin);
  Brightness = analogRead(Brightness_Pot);
  Brightness = map(Brightness, 0, 1023, 0, 255);
  Btn1_ON = not digitalRead(Btn1_Pin);
  Btn2_ON = not digitalRead(Btn2_Pin);
  Btn3_ON = not digitalRead(Btn3_Pin);

  if (Btn1_ON && not Reading_Lights) {
    Reading_Start = Now;
    Reading_Lights = true;
  }
  else if (not Reading_Lights) {
    Reading_Start = 0;
  }
  else if (Btn2_ON && Reading_Lights){
    Reading_Lights = false;
  }

  if (Speaker_Power && Btn3_ON){
    Snooze_Speakers = true;
  }
  if (Speaker_Power and not Snooze_Speakers) {
    digitalWrite(Speaker_Power_Pin, HIGH);
  }
  else {
    digitalWrite(Speaker_Power_Pin, LOW);
  }
  if (not Speaker_Power && Snooze_Speakers){
    Snooze_Speakers = false;
  }

  // one shot to reset light fade timer
  if (Lights_ON and not Lights_Change) {
    Lights_Change = true;
    Start = Now;
  }
  else if (not Lights_ON) {
    Lights_Change = false;
    Start = 0;
  }


  if (Flashing_Lights) {
    Brightness = 255; // set full brightness for flashing even if potentiometer is turned down
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    SetupBlackAndWhiteStripedPalette();
    currentBlending = LINEARBLEND;
    FillLEDsFromPaletteColors(startIndex, (int) Brightness);
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
  else if (Lights_ON) {
//    if ((Now - Start) > fade_time) {
//      Brightness = 255;
//    }
//    else if ((Now - Start > 0) && (Now - Start < fade_time)) {
//      Brightness = (int) (Now - Start) / fade_time;
//    }
//    else {
//      Brightness = 255;
//    }

    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    //    currentPalette = LavaColors_p;
    //    currentPalette = CloudColors_p;
    //    currentPalette = ForestColors_p;
    currentPalette = OceanColors_p;
    FillLEDsFromPaletteColors(startIndex, (int) Brightness);

    //    for ( int i = 0; i < NUM_LEDS; i++) {
    //      leds[i] = ColorFromPalette(LavaColors_p, startIndex, 0, currentBlending);
    //      startIndex += 3;
    //    }
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
  else if (Reading_Lights) {
//    if ((Now - Reading_Start > Reading_duration) && (Reading_Fade_Start == 0)) {
//      Reading_Fade_Start = Now;
//      Serial.print("Start fade");
//    }
//    else if (Now - Reading_Start < Reading_duration) {
//      Serial.print(Now - Reading_Start);
//      Reading_Fade_Start = 0;
//    }
//    if (Reading_Fade_Start > 0) {
//      Serial.print(Now - Reading_Fade_Start);
//      Brightness = (int) ((1-((Now - Reading_Fade_Start) / fade_time)) * 255);
//    }
//    if (Now - Reading_Fade_Start >= fade_time) {
//      Serial.println("Lights off");
//      Reading_Lights = false;
//    }
//    Serial.print("Reading");
//    Serial.println(Brightness);
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    currentPalette = LavaColors_p;
    //    currentPalette = CloudColors_p;
    //    currentPalette = ForestColors_p;
    //    currentPalette = OceanColors_p;
    FillLEDsFromPaletteColors(startIndex, (int) Brightness);
    FastLED.show();
    FastLED.delay(1000 / (UPDATES_PER_SECOND / 5));
  }
  else {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    fill_solid( currentPalette, 16, CRGB::Black);
    for ( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette( currentPalette, startIndex, 0, currentBlending);
      startIndex += 3;
    }
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }

  // debug printing
  //  Serial.print(Flashing_Lights);
  //  Serial.println(Brightness);
  //  Serial.println(Lights_ON);
  //    Serial.println(digitalRead(Speaker_in_Pin));
  //    Serial.println(digitalRead(Lights_Pin));
//  Serial.println(digitalRead(Flash_Pin));
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
