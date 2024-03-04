#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_DATA_PIN 3
#define NEOPIXEL_RING_LENGTH 45

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_RING_LENGTH, NEOPIXEL_DATA_PIN, NEO_GRB + NEO_KHZ800);

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void standardSolidBlue() {
  colorWipe(strip.Color(24, 239, 255), 25);
}

void setup()
{
  strip.begin();
  strip.setBrightness(20);
  strip.show();
  standardSolidBlue();
}

void loop()
{
  // put your main code here, to run repeatedly:
}