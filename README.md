# Radio Projekt s Displejem

## Popis projektu
Tento projekt se zaměřuje na stavbu FM rádia s displejem, který zobrazuje různé informace v reálném čase. Cílem je vytvořit funkční a přehledné rádio, které nejen přijímá rádio signály, ale také zobrazuje informace jako je čas, název stanice, frekvence stanice a ukládání stanic. Projekt obsahuje menu s možnostmi: rádio, časovač, budík, ekvalizér, přenastavení reálného času a informace.

## Funkce
- **Přijímání FM rádiového signálu**
- **Zobrazení aktuálního času** na displeji
- **Název stanice** na středové části displeje
- **Frekvence stanice** na spodní části displeje
- **Ukládání oblíbených stanic**
- **Menu s možnostmi**: rádio, časovač, budík, ekvalizér, přenastavení reálného času, informace

## Použité komponenty
- **Arduino Nano**: Řídící jednotka projektu.
- **Displej 128x64 I2C** s čipem **SH1106**: Pro zobrazení všech informací.
- **Rádio modul**: **RDA5807** pro příjem FM signálů.
- **RTC modul**: **PCF8563** pro zajištění aktuálního času.
- **Audio čip**: **PT2314** pro zpracování zvuku.
- **3 tlačítka**: Pro ovládání a navigaci v menu
- **Rotary encoder**: Pro intuitivní ovládání, ladění stanic a nastavování hlasitosti

## Použité knihovny
- **Adafruit_SH110X**: Pro ovládání displeje.
- **Rtc_Pcf8563**: Pro práci s RTC modulem.
- **Encoder**: Pro použití s rotary encoderem.
- **PT2314**: Pro ovládání audio čipu.
- **LowPower**: Pro režim spánku
- **RDA5807**: Pro ovládání FM rádia.

## Kontakt
fialaadamak2006@gmail.com

![celek v6](https://github.com/user-attachments/assets/b35fa04c-9619-4d42-a303-4d095582e08e)
