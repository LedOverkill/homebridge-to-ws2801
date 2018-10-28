/*
 * Project ledcontroller
 * Description: control the led strip over tcp
 * Author: Kristin and Benjamin Fritsch
 * Date: 10/16/18
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>

const int server_port = 8080;
const char* wifi_ssid = "Polynomialzeit-Turingreduktion";
const char* wifi_password = "";
const char* hostname = "ben-esp8266";

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

#define NUM_LEDS 25
#define DATA_PIN 2 // Yellow wire on Adafruit Pixels
#define CLOCK_PIN 3 // Green wire on Adafruit Pixels
CRGB leds[NUM_LEDS];

void parseCommand(String request, String &command, int &value);
void updateStrip();

int getPowerState();
void setPowerState(int);
int getBrightness();
void setBrightness(int);
int getHue();
void setHue(int);
int getSaturation();
void setSaturation(int);

WiFiServer server(server_port);

struct state {
  char powerState;
  int h;
  int s;
  int b;
} stripState;

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.hostname(hostname);
  WiFi.begin(wifi_ssid, wifi_password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");

  stripState.h = 0;
  stripState.s = 0;
  stripState.b = 0;

  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  FastLED.show();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(10);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');

  String command = "";
  int value = -1;

  Serial.println("default command: " + command);
  Serial.print("default value: ");
  Serial.println(value);

  parseCommand(req, command, value);

  client.flush();

  Serial.println("command: " + command);
  Serial.print("value: ");
  Serial.println(value);

  if (command.equals("getSaturation")) {
    client.print(getSaturation());
  } else if (command.equals("setSaturation")) {
    setSaturation(value);
  } else if (command.equals("getPowerState")) {
    client.print(getPowerState());
  } else if (command.equals("setPowerState")) {
    setPowerState(value);
  } else if (command.equals("getBrightness")) {
    client.print(getBrightness());
  } else if (command.equals("setBrightness")) {
    setBrightness(value);
  } else if (command.equals("getHue")) {
    client.print(getHue());
  } else if (command.equals("setHue")) {
    setHue(value);
  } else {
    Serial.println("Command not found: " + command);
  }

  delay(10);
  Serial.println("Client disonnected");
  Serial.println("------------------------------------------------");
}

/* Helper functions */
void updateStrip() {
  for(int dot = 0; dot < NUM_LEDS; dot++) {
      //Using a 'rainbow' color with hue 0-255, saturating 0-255, and brightness (value) 0-255
      leds[dot] = CHSV(stripState.h, stripState.s, stripState.b);
      FastLED.show();
      delay(30);
  }
}

void parseCommand(String request, String &command, int &value) {
  for (unsigned int i = 0; i < request.length(); i++) {
    if (request.substring(i, i+1) == ":") {
      command = request.substring(0, i);
      value = request.substring(i+1).toInt();
      break;
    }
  }
}

int getPowerState() {
  Serial.println("Calling: getPowerState");
  return stripState.b;
}

void setPowerState(int value) {
  Serial.println("Calling: setPowerState");
  Serial.print("setPowerState: ");
  Serial.println(value);

  if (value == 0) {
    stripState.b = 0;
  } else {
    stripState.b = 50;
  }

  updateStrip();
}

int getBrightness() {
  Serial.println("Calling: getBrightness");
  return stripState.b;
}

void setBrightness(int value) {
  Serial.println("Calling: setBrightness");
  Serial.print("setBrightness: ");
  Serial.println(value);
  stripState.b = value;
  updateStrip();
}

int getHue() {
  Serial.println("Calling: getHue");
  return stripState.h;
}

void setHue(int value) {
  Serial.println("Calling: setHue");
  Serial.print("setHue: ");
  Serial.println(value);
  stripState.h = value;
  updateStrip();
}

int getSaturation() {
  Serial.println("Calling: getSaturation");
  return stripState.s;
}

void setSaturation(int value) {
  Serial.println("Calling: setSaturation");
  Serial.print("setSaturation: ");
  Serial.println(value);
  stripState.s = value;
  updateStrip();
}
