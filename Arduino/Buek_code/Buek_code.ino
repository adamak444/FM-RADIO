#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <RDA5807.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

char *stationName;
char *programInfo;
char *rdsTime;

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RDA5807 rx;

void display_time(int i) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor((SCREEN_WIDTH - 30) / 2, 0);  // Zarovnání do středu
  display.print(i);
  display.display();
}

void show_freq(float freq) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor((SCREEN_WIDTH - 30) / 2, 10);  // Zarovnání do středu
  display.print(freq);
  display.display();
}

void setup() {
  Serial.begin(9600);

  display.begin(0x3C, true);  // 0x3C je adresa pro SH1106 displeje
  display_time(millis() / 1000);

  delay(2000);

  rx.setup();
  rx.setFrequency(8950);
  rx.setRDS(true);
  rx.setRdsFifo(true);
  rx.setLnaPortSel(3);  // Trying improve sensitivity.
  rx.setAFC(true);      // Sets Automatic Frequency Control
}

void loop() {
  display_time(millis() / 1000);
  //show_freq(12);

  Serial.print(rx.getRdsReady());
  Serial.print('/');
  Serial.println(rx.hasRdsInfo());

  if (rx.getRdsReady()) {
    if (rx.hasRdsInfo()) {
      stationName = rx.getRdsText0A();
      Serial.println(stationName);
    }
  }

  delay(3000);
}