/*
 * Project ledcontroller
 * Description: control the led strip over tcp
 * Author: Kristin and Benjamin Fritsch
 * Date: 10/16/18
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <HSBColor.h>
#include "Adafruit_WS2801.h"

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!

// Mapping pin to GPIO pins on NodeMCU
// static const uint8_t D0   = 16;
// static const uint8_t D1   = 5;
// static const uint8_t D2   = 4;
// static const uint8_t D3   = 0;
// static const uint8_t D4   = 2;
// static const uint8_t D5   = 14;
// static const uint8_t D6   = 12;
// static const uint8_t D7   = 13;
// static const uint8_t D8   = 15;
// static const uint8_t D9   = 3;
// static const uint8_t D10  = 1;

uint8_t dataPin  = D2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = D3;    // Green wire on Adafruit Pixels

void colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait);

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.begin("Internet of Shit", "noaccessforyou");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
}

void loop() {
  int rgb_colors[3];
  H2R_HSBtoRGB(284, 46, 33, rgb_colors);
  colorWipe(rgb_colors[0], rgb_colors[1], rgb_colors[2], 50);
}

/* Helper functions */

void colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, r, g, b);
      strip.show();
      delay(wait);
  }
}


