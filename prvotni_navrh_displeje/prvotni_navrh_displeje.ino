#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void drawTime() {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor((SCREEN_WIDTH - 30) / 2, 0);  // Zarovnání do středu
  display.print("12:34");
}

void drawStationInfo() {
  display.setTextSize(2);
  display.setCursor((SCREEN_WIDTH - 60) / 2, 20);  // Zarovnání do středu
  display.print("Radio1");

  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 45) / 2, 40);
  display.print("Pop Music");

  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 45) / 2, 54);
  display.print("101.1 MHz");
}

void setup() {
  display.begin(0x3C, true);  // 0x3C je adresa pro SH1106 displeje
  display.clearDisplay();
  
  // Spuštění animace antény a signálu

  
  // Zobrazení statických informací po animaci
  drawTime();
  drawStationInfo();

  display.display();
}

void loop() {
  // Sem můžeš později přidat kód pro aktualizaci informací
}
