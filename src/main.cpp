#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <HSBColor.h>

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
}

void loop() {
  int rgb_colors[3];
  H2R_HSBtoRGB(360, 50, 50, rgb_colors);

  Serial.println("R: ");
  Serial.print(rgb_colors[0]);
  Serial.println("G: ");
  Serial.print(rgb_colors[1]);
  Serial.println("B: ");
  Serial.print(rgb_colors[2]);
}