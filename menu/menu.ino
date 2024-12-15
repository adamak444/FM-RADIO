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
long lastPosition = 0; // Poslední zaznamenaná pozice

void setup() {
  if (!display.begin(0x3C, true)) { // Adresa displeje 0x3C
    while (1) delay(10);
  }
  display.clearDisplay();
  display.display();

  // Inicializace tlačítka
  pinMode(buttonPin, INPUT_PULLUP);
  boot();
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

void boot() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor((SCREEN_WIDTH - 30) / 2, 10);
  display.setTextColor(SH110X_WHITE);
  display.print("BOOT");
  display.display();
  delay(1000);
  radio();
}

void radio() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(90, 0);
  display.print("16:15");

  display.setTextSize(2);
  display.setCursor(10, 15);
  display.print("Radio 1");

  display.setTextSize(1);
  display.setCursor(45, 40);
  display.print("115 FM");

  display.drawLine(0, 50, SCREEN_WIDTH, 50, SH110X_WHITE);

  display.setTextSize(1);
  display.setCursor(45, 54);
  display.print("Menu");

  display.setCursor(100, 54);
  display.print("Safe");

  display.display();
}


void settime() {
  int minutes = 0;
  int hours = 0;

  // Nastavení minut
  while (true) {
    long newPosition = myEnc.read();

    // Kontrola směru otáčení
    if (newPosition != lastPosition) {
      int delta = newPosition - lastPosition; // Zjistit, o kolik se změnila pozice

      // Pokud se změnila pozice, zaktualizuj hodnotu minut
      if (delta > 0) { // Otočeno doprava
        if (minutes < 59) {
          minutes++;
        }
      } else if (delta < 0) { // Otočeno doleva
        if (minutes > 0) {
          minutes--;
        }
      }

      lastPosition = newPosition; // Uložení nové pozice

      // Aktualizace displeje
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(20, 0);
      display.println("SET MINUTES:");
      display.setCursor(20, 50);
      display.println(minutes);
      display.display();
    }

    // Kontrola tlačítka
    if (digitalRead(buttonPin) == LOW) { // Tlačítko je stisknuto
      delay(200); // Debounce delay
      while (digitalRead(buttonPin) == LOW); // Čekej, až se tlačítko uvolní
      break; // Když uživatel potvrdí, ukončíme nastavení minut
    }
  }
  
    // Nastavení hodin
  while (true) {
    long newPosition = myEnc.read();

    // Kontrola směru otáčení
    if (newPosition != lastPosition) {
      int delta = newPosition - lastPosition; // Zjistit, o kolik se změnila pozice

      // Pokud se změnila pozice, zaktualizuj hodnotu hodin
      if (delta > 0) { // Otočeno doprava
        if (hours < 23) {
          hours++;
        }
      } else if (delta < 0) { // Otočeno doleva
        if (hours > 0) {
          hours--;
        }
      }

      lastPosition = newPosition; // Uložení nové pozice

      // Aktualizace displeje
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(20, 0);
      display.println("SET HOURS:");
      display.setCursor(20, 50);
      display.println(hours);
      display.display();
    }

    // Kontrola tlačítka
    if (digitalRead(buttonPin) == LOW) { // Tlačítko je stisknuto
      delay(200); // Debounce delay
      while (digitalRead(buttonPin) == LOW); // Čekej, až se tlačítko uvolní
      //rtc.setTime(hours, minutes, 0); // Nastaví čas do RTC
      break; // Ukončíme nastavení hodin
    }
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
      radio();
      break;
    case 1:
      // Timer
      break;
    case 2:
      // Alarm
      break;
    case 3:
      // Equalizer
      break;
    case 4:
      settime();
      break;
    case 5:
      // Info
      break;
  }
}
