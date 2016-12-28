#include <Adafruit_NeoPixel.h>
#include <stdlib.h>

#define PIN 6
#define PIXEL_COUNT 117
#define ADJACENT 3 // How many pixels in the same color

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  srand(23);
}

void loop() {
  int steps = (strip.numPixels()+ADJACENT-1) / ADJACENT;
    for (int i = 0; i < steps; i+=2) {
      for (int j = 0; j < ADJACENT; ++j) {
        strip.setPixelColor(i*ADJACENT+j, strip.Color(255, 60, 0));
      }
      for (int j = 0; j < ADJACENT; ++j) {
        strip.setPixelColor((i+1)*ADJACENT+j, strip.Color(0, 0, 255));
      }
    }
    strip.show();
    delay(200);
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
