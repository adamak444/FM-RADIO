#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Adafruit_SH110X.h>
#include <RDA5807.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MAX_DELAY_RDS 80  //  polling method
#define MAX_DELAY_STATUS 5000
#define MAX_DELAY_SHOW_RDS 80
long rds_elapsed = millis();
long status_elapsed = millis();

uint8_t showrRdsInfo = 3;  // Default: show RDS time.

RDA5807 rx;

char* stationName;

void setup() {

  rx.setup();

  rx.setRDS(true);  // Turns RDS on

  rx.setVolume(1);

  delay(500);

  rx.setFrequency(9190);

  // RDS setup
  rx.setRDS(true);
  rx.setRdsFifo(true);
  rx.setLnaPortSel(3);  // Trying improve sensitivity.
  rx.setAFC(true);      // Sets Automatic Frequency Control

  display.begin(0x3C, true);
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 45) / 2, 40);
  display.print("TEST");
  display.display();

  delay(2000);
}

void loop() {
display.clearDisplay();
drawStationInfo();
display.display();

}

void drawStationInfo() {
if (rx.getRdsReady()) {
    if (rx.hasRdsInfo()) {
      stationName = rx.getRdsStationName();
    }
  }


  display.setTextSize(2);
  display.setCursor((SCREEN_WIDTH - 60) / 2, 20);  // Zarovnání do středu
  display.print(stationName);

  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 45) / 2, 54);
  display.print(rx.getFrequency());
}
