// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <math.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif

#define PIN 0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);

uint8_t  mode   = 3, // Current animation effect
         offset = 0; // Position of spinny eyes
byte hue = 0;
uint32_t color  = 0xFF0000; // Start red
uint32_t prevTime;
int colors[32];

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(255 / 3);
  prevTime = millis();
  for (int i = 0; i < 32; ++i)
    colors[i] = random(4);
}

void loop() {
  uint8_t  i;
  uint32_t t;

  switch (mode) {

    case 0: // Christmas!
      for (int j = 0; j < 32; ++j) {
        colors[j] = (colors[j] + random(3) + 1) % 4;
        switch (colors[j]) {
          case 0:
            //pixels.setPixelColor(j, 255, 0, 0);
            break;
          case 1:
            pixels.setPixelColor(j, 255, 0, 0);
            break;
          case 2:
            pixels.setPixelColor(j, 0, 255, 0);
            break;
          case 3:
            pixels.setPixelColor(j, 255, 255, 255);
            break;
        }
      }
      pixels.show();
      delay(500);
      for (int j = 0; j < 32; ++j)
        pixels.setPixelColor(j, 0);
      break;

    case 1: // Spinny wheels (8 LEDs on at a time)
      for (i = 0; i < 16; i++) {
        uint32_t c = 0;
        if (((offset + i) & 7) < 2) c = color; // 4 pixels on...
        pixels.setPixelColor(   i, c); // First eye
        pixels.setPixelColor(31 - i, c); // Second eye (flipped)
      }
      pixels.show();
      offset++;
      delay(50);
      break;

    case 2: // Rainbow cirlce 8
      for (int j = 0; j < 32; ++j) {
        //pixels.setPixelColor(j, Wheel(hue + 8*j));
        if (j == hue / 8) {
          if (j < 16)
            pixels.setPixelColor(j, Wheel(hue));
          else
            pixels.setPixelColor(47 - j, Wheel(hue));
        }
      }
      hue += 8;
      pixels.show();
      delay(50);
      for (int j = 0; j < 32; ++j)
        pixels.setPixelColor(j, 0);
      break;
    case 3:
      for (i = 0; i < 16; i++) {
        uint32_t c = 0;
        if (((offset + i) & 7) < 2) c = Wheel(hue % 255); // 4 pixels on...
        pixels.setPixelColor(   i, c); // First eye
        pixels.setPixelColor(31 - i, c); // Second eye (flipped)
      }
      pixels.show();
      hue += 8;
      offset++;
      delay(50);
      break;
  }

  t = millis();
  if ((t - prevTime) > 8000) {     // Every 8 seconds...
    mode++;                        // Next mode
    if (mode > 3) {                // End of modes?
      mode = 0;                    // Start modes over
      color >>= 8;                 // Next color R->G->B
      if (!color) color = 0xFF0000; // Reset to red
    }
    for (i = 0; i < 32; i++) pixels.setPixelColor(i, 0);
    prevTime = t;
  }
}

uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
