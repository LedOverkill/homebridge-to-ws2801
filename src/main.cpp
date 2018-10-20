/*
 * Project ledcontroller
 * Description: control the led strip over tcp
 * Author: Kristin and Benjamin Fritsch
 * Date: 10/16/18
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <HSBColor.h>
#include <Adafruit_WS2801.h>

const int server_port = 8080;
const char* wifi_ssid = "Polynomialzeit-Turingreduktion";
const char* wifi_password = "";
const char* hostname = "ben-esp8266";

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

void colorWipe();
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


Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);
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

  WiFi.begin(wifi_ssid, wifi_password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  WiFi.hostname(hostname);

  if (!MDNS.begin(hostname)) {
    Serial.println("Error setting up MDNS responder!");
  }

  Serial.println("mDNS responder started");

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");

  stripState.h = 0;
  stripState.s = 0;
  stripState.b = 0;
  strip.begin();
  strip.show();
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
  colorWipe();
}

void colorWipe() {
  //Serial.println("Update color");
  int i;
  uint8_t wait = 50;
  int rgb_colors[3];
  
  H2R_HSBtoRGB(getHue(), getSaturation(), getBrightness(), rgb_colors);
  // Serial.println(rgb_colors[0]);
  // Serial.println(rgb_colors[1]);
  // Serial.println(rgb_colors[2]);
  H2R_HSBtoRGB(stripState.h, stripState.s, stripState.b, rgb_colors);

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, rgb_colors[0], rgb_colors[1], rgb_colors[2]);
      strip.show();
      delay(wait);
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
  return 1;
}

void setPowerState(int value) {
  Serial.println("Calling: setPowerState");
  Serial.print("setPowerState: ");
  Serial.println(value);
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
