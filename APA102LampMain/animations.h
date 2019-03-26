/*
   Animations when lamp is off
*/

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    ledsStrip[random16(NUM_LEDS_STRIP)] += CRGB::White;
  }
}

void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow(ledsStrip, NUM_LEDS_STRIP, gHue, 7);
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(ledsStrip, NUM_LEDS_STRIP, 10);
  int pos = random16(NUM_LEDS_STRIP);
  ledsStrip[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(ledsStrip, NUM_LEDS_STRIP, 20);
  int pos = beatsin16(13, 0, NUM_LEDS_STRIP - 1);
  ledsStrip[pos] += CHSV(gHue, 255, 192);
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS_STRIP; i++) { //9948
    ledsStrip[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(ledsStrip, NUM_LEDS_STRIP, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    ledsStrip[beatsin16(i + 7, 0, NUM_LEDS_STRIP - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void fire2012() {
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS_STRIP];
  // Step 1.  Cool down every cell a little
  for (int i = 0; i < NUM_LEDS_STRIP; i++) {
    heat[i] = qsub8(heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS_STRIP) + 2));
  }
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS_STRIP - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < SPARKING) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }
  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < NUM_LEDS_STRIP; j++) {
    CRGB color = HeatColor( heat[j]);
    int pixelnumber;
    if (gReverseDirection) {
      pixelnumber = (NUM_LEDS_STRIP - 1) - j;
    } else {
      pixelnumber = j;
    }
    ledsStrip[pixelnumber] = color;
  }
}

void colorWipe() {
  ledsStrip[gPixel] = CHSV(gHueWipe, 255, 128);
  gPixel++;
  if (gPixel == NUM_LEDS_STRIP) {
    gPixel = 0;
    gHueWipe = gHueWipe + 30;
  }
  delay(15);
}

void colorWipeGlitter() {
  ledsStrip[gPixel] = CHSV(gHueWipe, 255, 128);
  gPixel++;
  if (gPixel == NUM_LEDS_STRIP) {
    gPixel = 0;
    gHueWipe = gHueWipe + 30;
  }
  addGlitter(30);
  delay(15);
}

//blur1d
void blur1d() {
  uint8_t blurAmount = dim8_raw( beatsin8(3, 64, 192) );    // A sinewave at 3 Hz with values ranging from 64 to 192.
  blur1d(ledsStrip, NUM_LEDS_STRIP, blurAmount);   // Apply some blurring to whatever's already on the strip, which will eventually go black.

  uint8_t  i = beatsin8(  9, 0, NUM_LEDS_STRIP);
  uint8_t  j = beatsin8( 7, 0, NUM_LEDS_STRIP);
  uint8_t  k = beatsin8(  5, 0, NUM_LEDS_STRIP);

  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();
  ledsStrip[(i + j) / 2] = CHSV( ms / 29, 200, 255);
  ledsStrip[(j + k) / 2] = CHSV( ms / 41, 200, 255);
  ledsStrip[(k + i) / 2] = CHSV( ms / 73, 200, 255);
  ledsStrip[(k + i + j) / 3] = CHSV( ms / 53, 200, 255);
}

/* void dotTrail() {
  fadeToBlackBy(ledsStrip, NUM_LEDS_STRIP, 50);
  byte pos = (beat8(40) * NUM_LEDS_STRIP) / 255;
  ledsStrip[pos] = CHSV(gHue, 255, 255);
  } */

// List of animation patterns to cycle through.  Each is defined as a separate function below.
SimplePatternList gPatterns = {confetti, sinelon, juggle, bpm, rainbow, rainbowWithGlitter, fire2012,
                               colorWipe, blur1d, colorWipeGlitter
                              };
