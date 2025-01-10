#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Encoder.h>
#include <RDA5807.h>
#include <Rtc_Pcf8563.h>
#include "LowPower.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

const int numItems = 6; // Počet položek v menu
String menuItems[numItems] = {"Radio", "Timer", "Alarm", "Equalizer", "Set Time", "Safe Station"};
int selectedItem = 0;

Encoder myEnc(4, 3); // Pin 4 jako DT, pin 3 jako CLK
const int buttonPin = 5; // Pin pro tlačítko
bool buttonPressed = false;

const int encoderThreshold = 4;
long lastPosition = 0;

void setup() {
  Serial.begin(9600);
  if (!display.begin(0x3C, true)) {
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
  if (abs(newPosition - lastPosition) >= encoderThreshold) {
    if (newPosition > lastPosition) {
      selectedItem = (selectedItem + 1) % numItems;
    } else if (newPosition < lastPosition) {
      selectedItem = (selectedItem - 1 + numItems) % numItems;
    }
    lastPosition = newPosition;
    drawMenu();
  }

  if (digitalRead(buttonPin) == LOW) {
    if (!buttonPressed) {
      buttonPressed = true;
      executeSelectedItem();
    }
  } else {
    buttonPressed = false;
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
  display.print("=>");

  display.setCursor(12, 54);
  display.print("<=");

  display.display();
}


void settime() {
  int minutes = 0;
  int hours = 0;

  // Inicializace stavu tlačítka a enkodéru
  lastPosition = myEnc.read();
  bool buttonPressed = false;

  // Čekání na uvolnění tlačítka před nastavením času
  while (digitalRead(buttonPin) == LOW) {
    delay(10); // Krátké čekání, aby uživatel nezačal se stisknutým tlačítkem
  }

  // Nastavení minut
  while (true) {
    long newPosition = myEnc.read();

    if (newPosition != lastPosition) {
      int delta = newPosition - lastPosition;

      // Aktualizace minut
      if (delta > 0 && minutes < 59) {
        minutes++;
      } else if (delta < 0 && minutes > 0) {
        minutes--;
      }

      lastPosition = newPosition;

      // Zobrazení minut na displeji
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(20, 0);
      display.println("SET MINUTES:");
      display.setCursor(20, 50);
      display.println(minutes);
      display.display();
    }

    // Kontrola tlačítka pro potvrzení
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
      buttonPressed = true; // Zaznamenáme stisk tlačítka
      delay(200); // Debounce
      while (digitalRead(buttonPin) == LOW); // Čekáme na uvolnění
      break; // Ukončení smyčky pro minuty
    } else if (digitalRead(buttonPin) == HIGH) {
      buttonPressed = false; // Reset stavu tlačítka
    }
  }

  // Resetace stavu tlačítka a enkodéru pro nastavení hodin
  lastPosition = myEnc.read();
  buttonPressed = false;

  // Nastavení hodin
  while (true) {
    long newPosition = myEnc.read();

    if (newPosition != lastPosition) {
      int delta = newPosition - lastPosition;

      // Aktualizace hodin
      if (delta > 0 && hours < 23) {
        hours++;
      } else if (delta < 0 && hours > 0) {
        hours--;
      }

      lastPosition = newPosition;

      // Zobrazení hodin na displeji
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(20, 0);
      display.println("SET HOURS:");
      display.setCursor(20, 50);
      display.println(hours);
      display.display();
    }

    // Kontrola tlačítka pro potvrzení
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
      buttonPressed = true; // Zaznamenáme stisk tlačítka
      delay(200); // Debounce
      while (digitalRead(buttonPin) == LOW); // Čekáme na uvolnění
      break; // Ukončení smyčky pro hodiny
    } else if (digitalRead(buttonPin) == HIGH) {
      buttonPressed = false; // Reset stavu tlačítka
    }
  }

  // Nastavení času (pokud je RTC modul použit)
  // rtc.setTime(hours, minutes, 0);
  drawMenu();
}

void setTimer() {
  int timerMinutes = 0;

  // Inicializace stavu tlačítka a enkodéru
  long lastPosition = myEnc.read();
  bool buttonPressed = false;

  // Čekání na uvolnění tlačítka před nastavením času
  while (digitalRead(buttonPin) == LOW) {
    delay(10); // Krátké čekání, aby uživatel nezačal se stisknutým tlačítkem
  }

  // Nastavení timeru
  while (true) {
    long newPosition = myEnc.read();

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.println("SET TIMER:");
    display.setCursor(20, 50);
    display.println(timerMinutes);
    display.display();

    if (newPosition != lastPosition) {
      int delta = newPosition - lastPosition;

      // Aktualizace timeru (po 5 minutách)
      if (delta > 0) {
        timerMinutes += 5; // Zvětšení po 5 minutách
      } else if (delta < 0) {
        timerMinutes -= 5; // Snížení po 5 minutách
      }

      lastPosition = newPosition;

      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(20, 0);
      display.println("SET TIMER:");
      display.setCursor(20, 50);
      display.println(timerMinutes);
      display.display();
    }

    // Kontrola tlačítka pro potvrzení
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
      buttonPressed = true; // Zaznamenáme stisk tlačítka
      delay(200); // Debounce
      while (digitalRead(buttonPin) == LOW); // Čekáme na uvolnění
      break; // Ukončení smyčky pro timer
    } else if (digitalRead(buttonPin) == HIGH) {
      buttonPressed = false; // Reset stavu tlačítka
    }
  }
  // Timer je nastaven, můžeš zavolat jinou funkci nebo přejít zpět na menu
  drawMenu();
}

void setAlarm() {
  int minutes = 0;
  int hours = 0;

  // Inicializace stavu tlačítka a enkodéru
  lastPosition = myEnc.read();
  bool buttonPressed = false;

  // Čekání na uvolnění tlačítka před nastavením času
  while (digitalRead(buttonPin) == LOW) {
    delay(10); // Krátké čekání, aby uživatel nezačal se stisknutým tlačítkem
  }

  // Nastavení minut
  while (true) {
    long newPosition = myEnc.read();

    if (newPosition != lastPosition) {
      int delta = newPosition - lastPosition;

      // Aktualizace minut
      if (delta > 0 && minutes < 59) {
        minutes++;
      } else if (delta < 0 && minutes > 0) {
        minutes--;
      }

      lastPosition = newPosition;

      // Zobrazení minut na displeji
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(20, 0);
      display.println("SET MINUTES:");
      display.setCursor(20, 50);
      display.println(minutes);
      display.display();
    }

    // Kontrola tlačítka pro potvrzení
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
      buttonPressed = true; // Zaznamenáme stisk tlačítka
      delay(200); // Debounce
      while (digitalRead(buttonPin) == LOW); // Čekáme na uvolnění
      break; // Ukončení smyčky pro minuty
    } else if (digitalRead(buttonPin) == HIGH) {
      buttonPressed = false; // Reset stavu tlačítka
    }
  }

  // Resetace stavu tlačítka a enkodéru pro nastavení hodin
  lastPosition = myEnc.read();
  buttonPressed = false;

  // Nastavení hodin
  while (true) {
    long newPosition = myEnc.read();

    if (newPosition != lastPosition) {
      int delta = newPosition - lastPosition;

      // Aktualizace hodin
      if (delta > 0 && hours < 23) {
        hours++;
      } else if (delta < 0 && hours > 0) {
        hours--;
      }

      lastPosition = newPosition;

      // Zobrazení hodin na displeji
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(20, 0);
      display.println("SET HOURS:");
      display.setCursor(20, 50);
      display.println(hours);
      display.display();
    }

    // Kontrola tlačítka pro potvrzení
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
      buttonPressed = true; // Zaznamenáme stisk tlačítka
      delay(200); // Debounce
      while (digitalRead(buttonPin) == LOW); // Čekáme na uvolnění
      break; // Ukončení smyčky pro hodiny
    } else if (digitalRead(buttonPin) == HIGH) {
      buttonPressed = false; // Reset stavu tlačítka
    }
  }

  // Nastavení času (pokud je RTC modul použit)
  // rtc.setTime(hours, minutes, 0);
  drawMenu();
}

void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  for (int i = 0; i < numItems; i++) {
    if (i == selectedItem) {
      display.setTextColor(SH110X_BLACK, SH110X_WHITE);
    } else {
      display.setTextColor(SH110X_WHITE);
    }
    display.setCursor(10, i * 10);
    display.print(menuItems[i]);
  }

  display.display();
}

void executeSelectedItem() {
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
      setTimer();
      break;
    case 2:
      // Alarm
      setAlarm();
      break;
    case 3:
      // Equalizer
      break;
    case 4:
      settime();
      break;
    case 5:
      // Safe station
      break;
  }
}
