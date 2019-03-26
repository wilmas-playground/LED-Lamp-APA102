/*
   Name: Stahlring LED Lampe v2
   Created: 29-01-2018
   Revision: A1
             A2 (17-11-2018): Kapazitiver Taster mit mechanischem ersetzt
             da weniger fehleranfällig. Ansteuerung der Taster LEDS mit RGB
   Author: David Schaffner & Sergio Miracco

   - Arduino Programm zur Ansteuerung der Lampe
   - Ein-, Auschalten & Dimmen mittels Taster
   - Farbwechsel mit Potentiometer
   - Printtaster um verschiedene Lichtmuster zu starten
   - LEDs: APA102 123 LEDs
*/

#include <EEPROM.h>
#include <FastLED.h>

#include "globals.h"
#include "effectsOnOff.h"
#include "animations.h";

void writeLed() {
  //Write Strip LEDs
  fill_solid(ledsStrip, NUM_LEDS_STRIP, hsvStrip);
  //Write ON OFF button LED
  hsv2rgb_rainbow(hsvTast, rgbTast); //Umrechnung HSV to RGB
  analogWrite(TAST_RED, rgbTast[0]);
  analogWrite(TAST_GREEN, rgbTast[1]);
  analogWrite(TAST_BLUE, rgbTast[2]);
  FastLED.show();
}

bool toggleOnOff(effects e) {
  switch (e) {
    case effects::m_WIPE:
      Wipe(actualState);
      break;
    case effects::m_DIM:
      Dim(actualState);
      break;
    case effects::m_PIXEL:
      Pixel(actualState);
      break;
    case effects::m_PONG:
      Pong(actualState);
      break;
    case effects::m_FILL:
      Fill(actualState);
      break;
    default:
      Wipe(actualState);
      break;
  }
  if (actualState == lampState::_ON) {
    actualState = lampState::_OFF;
    analogWrite(TAST_RED, 80);
    analogWrite(TAST_GREEN, 0);
    analogWrite(TAST_BLUE, 0);
  }
  else {
    actualState = lampState::_ON;
    hsv2rgb_rainbow(hsvTast, rgbTast); //Umrechnung HSV to RGB
    analogWrite(TAST_RED, rgbTast[0]);
    analogWrite(TAST_GREEN, rgbTast[1]);
    analogWrite(TAST_BLUE, rgbTast[2]);
  }
  return true;
}

void toggleAnimation() {
  if (actualAnimState == animationState::_OFF) {
    actualAnimState = animationState::_ON;
  }
  else {
    actualAnimState = animationState::_OFF;
    fill_solid(ledsStrip, NUM_LEDS_STRIP, CRGB::Black);
    FastLED.show();
  }
}

//Rauf- und Runterdimmen
void dimUpDown() {
  switch (dimStatus) {
    case 1:
      if (hsvStrip.value >= 255) {
        hsvStrip.value = 254;
      }
      hsvStrip.value = hsvStrip.value + dimProg;
      writeLed();
      delay(15);

      if (hsvStrip.value >= 255) {
        dimStatus = 2;
        delay(1500);
      }
      break;

    case 2:
      if (hsvStrip.value <= 30) {
        hsvStrip.value = 31;
      }
      hsvStrip.value = hsvStrip.value - dimProg;
      writeLed();
      delay(15);

      if (hsvStrip.value <= 31) {
        dimStatus = 1;
        delay(1500);
      }
      break;
  }
}

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void setup() {
  randomSeed(analogRead(0));
  //Add LEDs for Strip
  FastLED.addLeds<LED_TYPE_STRIP, DATA_PIN_STRIP, CLOCK_PIN_STRIP, COLOR_ORDER_STRIP>(ledsStrip, NUM_LEDS_STRIP);
  set_max_power_in_volts_and_milliamps(5, 4000); // FastLED Power management set at 5V, 4000mA.
  Serial.begin(9600); //Für Analog Pin
  pinMode(tastOnOff, INPUT);
  pinMode(tastAnimations, INPUT);
  fill_solid(ledsStrip, NUM_LEDS_STRIP, CRGB::Black);
  pinMode(TAST_RED, OUTPUT);
  pinMode(TAST_GREEN, OUTPUT);
  pinMode(TAST_BLUE, OUTPUT);
  analogWrite(TAST_RED, 80);
  analogWrite(TAST_GREEN, 0);
  analogWrite(TAST_BLUE, 0);
  FastLED.show();
}

void loop() {

  //Zustand merken
  lastOnKeyState = onKeyState;
  onKeyState = digitalRead(tastOnOff);

  if (actualState == lampState::_OFF) {
    onAnimationState = digitalRead(tastAnimations);
    if (lastOnAnimationState == LOW && onAnimationState == HIGH) {
      toggleAnimation();
    }
    if (actualAnimState == animationState::_ON) {
      FastLED.setBrightness(128); //set Brightness of strip leds
      gPatterns[gCurrentPatternNumber](); // Call the current pattern function once, updating the 'leds' array

      if (gCurrentPatternNumber == 6) {
        FastLED.delay(1000 / FRAMES_PER_SECOND_FIRE);
      }
      else {
        FastLED.delay(1000 / FRAMES_PER_SECOND);
      }
      FastLED.show();

      EVERY_N_MILLISECONDS(20) {
        gHue++; // slowly cycle the "base color" through the rainbow
      }
      EVERY_N_SECONDS(60) {
        nextPattern(); // change patterns periodically
      }
    }
    lastOnAnimationState = onAnimationState;
  }

  else if (actualState == lampState::_ON) {
    //Poti einlesen
    potiVal = analogRead(potiIn);  //Potentiometer ist der Hue-Kanal.
    //Änderung des Poti feststellen
    if ((potiVal > lastPotiVal + 10) || (potiVal < lastPotiVal - 10)) {
      //Analoger Eingang (Potentiometer) lesen und skalieren auf 0 - 255 (H-Werte für HSV to RGB Umwandlung)
      hsvStrip.hue = map(potiVal, 0, 1023, 0, 255);
      if (hsvStrip.hue <= 1) {
        hsvStrip.hue = 0; // --> Hue-Kanal auf 0° = Rot
      }
      else if (hsvStrip.hue >= 250) {
        hsvStrip.saturation = 0; //Sättigung auf 0 --> Weiss
      }
      else {
        if (hsvStrip.saturation != 255) {
          hsvStrip.saturation = 255; //Sättigung wieder 100% -> Alle Farben
        }
      }
      lastPotiVal = potiVal;
      hsvTast.hue = hsvStrip.hue;
      hsvTast.saturation = hsvStrip.saturation;
      writeLed();
    }
  }

  if (actualAnimState == animationState::_OFF) {
    // Steigende Flanke erkennen um Lampe ein- / auszuschalten
    if (lastOnKeyState == LOW && onKeyState == HIGH) {
      if (actualState == lampState::_ON) { //Lampe ausschalten
        delay(500); //Wartezeit
        lastOnKeyState = onKeyState;
        onKeyState = digitalRead(tastOnOff);
        if (onKeyState == LOW) { //Zum Ausschalten der Lampe
          toggleOnOff(effects(random(0, int(effects::m_FILL) + 1)));
        }
      }
      else if (actualState == lampState::_OFF) { //Lampe einschalten
        toggleOnOff(effects(random(0, int(effects::m_FILL) + 1)));
      }
    }

    //Zum Dimmen
    if (lastOnKeyState == HIGH && onKeyState == HIGH) {
      dimUpDown();
      //Abfallende Flanke detektieren um Dimmabfolge zu ändern.
      lastOnKeyState = onKeyState;
      onKeyState = digitalRead(tastOnOff);
      if (onKeyState == LOW) {
        if (dimStatus == 1) {
          dimStatus = 2;
        }
        else {
          dimStatus = 1;
        }
      }
    }
  }
}
