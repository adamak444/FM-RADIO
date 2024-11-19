#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Rtc_Pcf8563.h>
#include <Encoder.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define CLK_PIN 3
#define DT_PIN 4
#define SW_PIN 5

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
Rtc_Pcf8563 rtc;
Encoder myEnc(CLK_PIN, DT_PIN);

long oldPosition = 0;

void setup() {
  Serial.begin(9600);
  if (!display.begin(0x3C)) {
    Serial.println("OLED displej nenalezen");
    while (true);
  }
  
  rtc.initClock();

  if (rtc.getMinute() == 1 && rtc.getHour() == 1) {
    nastavCas(); // Zavolej funkci pro nastavení času
  }

}

void loop() {
  display.clearDisplay();

  char timeBuffer[9]; // "hh:mm:ss\0"
  snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d:%02d", rtc.getHour(), rtc.getMinute(), rtc.getSecond());

  char dateBuffer[11]; // "dd.mm.yyyy\0"
  snprintf(dateBuffer, sizeof(dateBuffer), "%02d.%02d.%04d", rtc.getDay(), rtc.getMonth(), rtc.getYear());

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(20, 30);
  display.println(timeBuffer);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(20, 50);
  display.println(dateBuffer);

  display.display(); 
}


void nastavCas() {
  int minutes = 0;
  int hours = 0;
  pinMode(SW_PIN, INPUT_PULLUP);

  // Nastavení minut
  Serial.println("Nastavení minut:");
  while (true) {
    long newPosition = myEnc.read();

    // Kontrola směru otáčení
    if (newPosition != oldPosition) {
      int delta = newPosition - oldPosition; // Zjistit, o kolik se změnila pozice

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

      oldPosition = newPosition; // Uložení nové pozice
      Serial.print("Nastavuji minuty: ");
      Serial.println(minutes);

      display.clearDisplay();

      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(20, 0);
      display.println("SET MINUTES:");

      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(20, 50);
      display.println(minutes);

      display.display(); 
    }

    // Kontrola tlačítka
    if (digitalRead(SW_PIN) == LOW) { // Tlačítko je stisknuto
      delay(200); // Debounce delay
      while (digitalRead(SW_PIN) == LOW); // Čekej, až se tlačítko uvolní
      break; // Když uživatel potvrdí, ukončíme nastavení minut
    }
  }

  // Nastavení hodin
  Serial.println("Nastavení hodin:");
  while (true) {
    long newPosition = myEnc.read();

    // Kontrola směru otáčení
    if (newPosition != oldPosition) {
      int delta = newPosition - oldPosition; // Zjistit, o kolik se změnila pozice

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

      oldPosition = newPosition; // Uložení nové pozice
      Serial.print("Nastavuji hodiny: ");
      Serial.println(hours);

      display.clearDisplay();

      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(20, 0);
      display.println("SET HOURS:");

      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(20, 50);
      display.println(hours);

      display.display(); 
    }

    // Kontrola tlačítka
    if (digitalRead(SW_PIN) == LOW) { // Tlačítko je stisknuto
      delay(200); // Debounce delay
      while (digitalRead(SW_PIN) == LOW); // Čekej, až se tlačítko uvolní
      rtc.setTime(hours, minutes, 0); // Nastaví čas do RTC
      Serial.println("Čas byl nastaven.");
      break; // Ukončíme nastavení hodin
    }
  }
}
