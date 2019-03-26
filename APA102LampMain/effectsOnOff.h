/*
   On Off Effects
*/

enum struct effects {
  m_WIPE,
  m_DIM,
  m_PIXEL,
  m_PONG,
  m_FILL
};

//Strip mit Wipe füllen / leeren
void Wipe(lampState &as) {
  for (int i = 0; i < NUM_LEDS_STRIP; i++) {
    if (as == lampState::_OFF) {
      ledsStrip[i] = CHSV(hsvStrip);
    }
    else {
      ledsStrip[i] = CRGB::Black;
    }
    delay(20);
    FastLED.show();
  }
}

//Strip mit random Pixeln füllen / leeren
void Pixel(lampState &as) {
  for (int i = 0; i <= 3 * NUM_LEDS_STRIP; i++) {
    int j = random(0, NUM_LEDS_STRIP);
    if (as == lampState::_OFF) {
      ledsStrip[j] = CHSV(hsvStrip);
    }
    else {
      ledsStrip[j] = CRGB::Black;
    }
    FastLED.show();
    delay(20);
  }
  if (as == lampState::_OFF) {
    fill_solid(ledsStrip, NUM_LEDS_STRIP, hsvStrip);
  }
  else {
    fill_solid(ledsStrip, NUM_LEDS_STRIP, CRGB::Black);
  }
  FastLED.show();
}

//Strip mit Dimmen Ein- Ausschalten
void Dim(lampState &as) {
  if (as == lampState::_OFF) {
    for (int i = 15; i <= hsvStrip.value; i++) {
      fill_solid(ledsStrip, NUM_LEDS_STRIP, CHSV(hsvStrip.hue, hsvStrip.saturation, i));
      FastLED.show();
      delay(7);
    }
  }
  else {
    for (int i = hsvStrip.value; i >= 15; i--) {
      fill_solid(ledsStrip, NUM_LEDS_STRIP, CHSV(hsvStrip.hue, hsvStrip.saturation, i));
      FastLED.show();
      delay(7);
    }
    fill_solid(ledsStrip, NUM_LEDS_STRIP, CRGB::Black);
    FastLED.show();
  }
}

//Strip mittels Pixel PingPong füllen / leeren
void Pong(lampState &as) {
  int i = 0;
  if (as == lampState::_OFF) {
    while (i < NUM_LEDS_STRIP) {
      for (int j = i; j < NUM_LEDS_STRIP - i; j++) {
        ledsStrip[j] = CHSV(hsvStrip);
        if (j - 1 > i) {
          ledsStrip[j - 1] = CHSV(hsvStrip);
        }
        if (j - 2 > i) {
          ledsStrip[j - 2] = CRGB::Black;
        }
        FastLED.show();
      }
      for (int k = NUM_LEDS_STRIP - i - 1; k > i; k--) {
        ledsStrip[k] = CHSV(hsvStrip);
        if ((k + 1) < (NUM_LEDS_STRIP - i - 1)) {
          ledsStrip[k + 1] = CHSV(hsvStrip);
        }
        if ((k + 2) < (NUM_LEDS_STRIP - i - 2)) {
          ledsStrip[k + 2] = CRGB::Black;
        }
        FastLED.show();
      }
      i = i + 2;
    }
  }
  else { //Still to do: Pong for turning off
    Wipe(actualState);
  }
}

//Strip mit Pixeln der Reihe nach füllen / leeren
void Fill(lampState &as) {
  int i = 0;
  if (as == lampState::_OFF) {
    while (i < NUM_LEDS_STRIP) {
      for (int j = 0; j < NUM_LEDS_STRIP - i; j++) {
        ledsStrip[j] = CHSV(hsvStrip);
        if (j > 0) {
          ledsStrip[j - 1] = CHSV(hsvStrip);
        }
        if (j > 1) {
          ledsStrip[j - 2] = CHSV(hsvStrip);
        }
        if (j > 2) {
          ledsStrip[j - 3] = CRGB::Black;
        }
        FastLED.show();
      }
      i = i + 3;
      if (i == 123) {
        i = 122;
      }
    }
  }
  else {
    while (i < NUM_LEDS_STRIP) {
      for (int j = i; j >= 0; j--) {
        if (j > 0) {
          ledsStrip[j - 1] = CHSV(hsvStrip);
        }
        if (j > 1) {
          ledsStrip[j - 2] = CHSV(hsvStrip);
        }
        if (j > 2) {
          ledsStrip[j - 3] = CHSV(hsvStrip);
        }
        ledsStrip[j] = CRGB::Black;
        FastLED.show();
      }
      i = i + 3;
      if (i == 123) {
        i = 122;
      }
    }
  }
}
