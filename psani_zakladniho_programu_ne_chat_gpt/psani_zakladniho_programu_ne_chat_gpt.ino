#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void setup() {
  Serial.begin(9600);

  if (!display.begin(0x3C)) {
    Serial.println("OLED displej nenalezen");
    while (true);
  }

  display.clearDisplay();
  //tft .setFont (FreeSansBoldOblique12pt7b);
  display.setFont(&FreeSansBoldOblique12pt7b);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(4, 20);
  display.println("EUPHORIA");
  display.display();  
}

void loop() {
  // Váš kód sem
}
