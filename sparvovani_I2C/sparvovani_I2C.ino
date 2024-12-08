#include <RDA5807.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

RDA5807 rx;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
rx.setup();
rx.setVolume(1);
//rx.setRDS(true);
delay(500);

rx.setFrequency(9190);



display.begin(0x3C, true);
display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor((SCREEN_WIDTH - 30) / 2, 0);  // Zarovnání do středu
  display.print(rx.getFrequency());
display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
