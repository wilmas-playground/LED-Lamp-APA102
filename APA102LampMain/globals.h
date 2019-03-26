/*
 * Global Variables definition
 */

//Strip
#define NUM_LEDS_STRIP 123
#define LED_TYPE_STRIP APA102
#define COLOR_ORDER_STRIP BGR
#define DATA_PIN_STRIP 2 //Green Data Pin APA102
#define CLOCK_PIN_STRIP 3 //Yellow Clock Pin APA102

//Block of memory to store and manipulate LED data for the strip
struct CRGB ledsStrip[NUM_LEDS_STRIP];

//On Off Taster LED (PWM Ausgänge)
#define TAST_RED 5
#define TAST_GREEN 6
#define TAST_BLUE 9

//Animation stuff
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define FRAMES_PER_SECOND  120
#define FRAMES_PER_SECOND_FIRE 60
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  100
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 150

typedef void (*SimplePatternList[])();
uint8_t gCurrentPatternNumber = 3; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t gHueWipe = 0;
uint8_t gPixel = 0;
uint8_t fadeval = 224;  
bool gReverseDirection = false;

//HSV Farbenarray Strip
CHSV hsvStrip(60, 255, 100);
//HSV/RGB Farbenarray Taster
CHSV hsvTast(0,0,80);
CRGB rgbTast(0, 0, 0);

// Lamp Status
enum struct lampState : bool { _OFF = false, _ON = true };
lampState actualState = lampState::_OFF;

// Animation Status
enum struct animationState : bool {_OFF = false, _ON = true };
animationState actualAnimState = animationState::_OFF;

//Input Effects
const uint8_t tastAnimations = 12;
//Input ON / OFF
const uint8_t tastOnOff = 13;

//Analoger Eingang für Poti (Change of color)
const uint8_t potiIn = A3;

//Zustände Poti
float potiVal = 0;
float lastPotiVal = 0;

//Zustand kapazitiver Taster
bool onKeyState = LOW;
bool lastOnKeyState = LOW;

//Zustand Animation Taster
bool onAnimationState = LOW;
bool lastOnAnimationState = LOW;

//Dimmstatus
uint8_t dimStatus = 2;
//Dimmprogress Stufe
float dimProg = 1;
