#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Encoder.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

const int numItems = 6; // Počet položek v menu
String menuItems[numItems] = {"Radio", "Timer", "Alarm", "Equalizer", "Set Time", "Info"};
int selectedItem = 0;

// Připojení encoderu
Encoder myEnc(4, 3); // Pin 4 jako DT, pin 3 jako CLK
const int buttonPin = 5; // Pin pro tlačítko
bool buttonPressed = false;

const int encoderThreshold = 4; // Hranice kroků encoderu pro změnu
long lastPosition = 0; // Poslední zaznamenaná pozice encoderu

void setup() {
  if (!display.begin(0x3C, true)) { // Adresa displeje 0x3C
    while (1) delay(10);
  }
  display.clearDisplay();
  display.display();

  // Inicializace tlačítka
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  long newPosition = myEnc.read();
  if (abs(newPosition - lastPosition) >= encoderThreshold) { // Kontrola hysterese
    if (newPosition > lastPosition) {
      selectedItem = (selectedItem + 1) % numItems; // Posun nahoru
    } else if (newPosition < lastPosition) {
      selectedItem = (selectedItem - 1 + numItems) % numItems; // Posun dolů
    }
    lastPosition = newPosition; // Aktualizace poslední pozice
    drawMenu();
  }

  // Kontrola stavu tlačítka
  if (digitalRead(buttonPin) == LOW) { // Tlačítko je stisknuté
    if (!buttonPressed) {
      buttonPressed = true; // Potvrzení stisknutí
      executeSelectedItem();
    }
  } else {
    buttonPressed = false; // Resetuje stav po uvolnění tlačítka
  }
}

void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  for (int i = 0; i < numItems; i++) {
    if (i == selectedItem) {
      display.setTextColor(SH110X_BLACK, SH110X_WHITE); // Inverzní barvy pro vybranou položku
    } else {
      display.setTextColor(SH110X_WHITE);
    }
    display.setCursor(10, i * 10);
    display.print(menuItems[i]);
  }

  display.display();
}

void executeSelectedItem() {
  // Funkce spojené s výběrem položky
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 20);

  switch (selectedItem) {
    case 0:
      display.print("Spouštím Radio...");
      break;
    case 1:
      display.print("Otevírám Timer...");
      break;
    case 2:
      display.print("Spouštím Alarm...");
      break;
    case 3:
      display.print("Equalizer...");
      break;
    case 4:
      display.print("Set Time...");
      break;
    case 5:
      display.print("Info...");
      break;
  }

  display.display();
  delay(1000); // Zobrazí zprávu po dobu 1 sekundy
  drawMenu(); // Vrácí zpět do menu
}