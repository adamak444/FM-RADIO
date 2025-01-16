#include <RDA5807.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

RDA5807 rx;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define MAX_DELAY_RDS 80  //  polling method
#define MAX_DELAY_STATUS 5000
#define MAX_DELAY_SHOW_RDS 80
long rds_elapsed = millis();
long status_elapsed = millis();

uint8_t showrRdsInfo = 3;  // Default: show RDS time.

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

char* stationName;

void setup() {
  Serial.begin(9600);
  Serial.println("starting...");
  rx.setup();
  rx.setVolume(1);
  rx.setRDS(true);
  rx.setRdsFifo(true);
  rx.setLnaPortSel(3);
  rx.setAFC(true);
  delay(500);
  rx.setFrequency(8950);


  display.begin(0x3C, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor((SCREEN_WIDTH - 30) / 2, 0);  // Zarovnání do středu
  display.print(" ");
  display.display();
  Serial.println("DONE");
}

void loop() {
  display.clearDisplay();
  drawStationInfo();
  display.display();

  Serial.print("name: ");
  Serial.println(stationName);
  delay(500);
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
