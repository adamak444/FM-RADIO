#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Funkce pro kreslení centrální antény
void drawAntenna() {
  int antennaX = SCREEN_WIDTH / 2;  // Střed X
  int antennaY = SCREEN_HEIGHT / 2; // Střed Y

  // Kreslení antény
  display.fillCircle(antennaX, antennaY, 5, SH110X_WHITE);  // Kruh pro anténu
  display.drawLine(antennaX, antennaY + 5, antennaX, SCREEN_HEIGHT, SH110X_WHITE);  // Tyč antény
}

// Funkce pro kreslení signálových vln (kruhové oblouky)
void drawSignalWave(int strength) {
  int antennaX = SCREEN_WIDTH / 2;
  int antennaY = SCREEN_HEIGHT / 2;
  
  // Postupné kreslení vln podle síly signálu
  for (int i = 1; i <= strength; i++) {
    display.drawCircle(antennaX, antennaY, i * 10, SH110X_WHITE);  // Kruhové vlny
  }
}

// Funkce pro animaci antény a přibývajícího signálu
void playStartupAnimation() {
  display.clearDisplay();
  
  // Kreslení statické antény
  drawAntenna();
  display.display();
  delay(500);
  
  // Postupné kreslení vln síly signálu
  for (int i = 1; i <= 3; i++) {
    drawAntenna();  // Znovu kreslí anténu, aby zůstala viditelná
    drawSignalWave(i);
    display.display();
    delay(500);
    display.clearDisplay();  // Vyčistí displej před dalším krokem animace
  }
  
  delay(500);  // Pauza po skončení animace
}

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
  playStartupAnimation();
  
  // Zobrazení statických informací po animaci
  drawTime();
  drawStationInfo();

  display.display();
}

void loop() {
  // Sem můžeš později přidat kód pro aktualizaci informací
}
