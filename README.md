# Projekt ATSHA204a

# Features of ATSHA204a

Náš projekt využíva zabezpečený kryptografický prvok ATSHA204a, ktorý poskytuje pokročilé funkcie pre bezpečnú autentifikáciu a šifrovanie v zabudovaných systémoch. Nižšie sú podrobne opísané kľúčové funkcie, ktoré zahrňujeme vo vývoji našich zariadení.

Tento dokument je užitočný ako zdroj informácií pre vývojové tímy alebo ako technická dokumentácia pre zainteresované strany v projekte.

| Funkcia                                     | Popis                                                                                                                                                  |
|---------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Zabezpečené hardvérové úložisko kľúčov**  | Poskytuje bezpečné hardvérové úložisko pre kryptografické kľúče, chrániace pred neoprávneným prístupom a únikom dát.                                    |
| **Autentifikácia**                          | Podporuje bezpečné symetrické autentifikačné operácie medzi hostiteľom a klientom.                                                                     |
| **SHA-256 Hashovací Algoritmus**            | Implementuje SHA-256 algoritmus s možnosťami MAC a HMAC pre bezpečné hashovanie správ.                                                                 |
| **Dĺžka a úložisko kľúčov**                  | Umožňuje použitie 256-bitových kľúčov a poskytuje úložisko pre až 16 kľúčov.                                                                            |
| **Unikátne sériové číslo**                   | Každé zariadenie má zaručené unikátne 72-bitové sériové číslo pre zlepšené možnosti sledovania a autentifikácie.                                       |
| **Generátor náhodných čísel (RNG)**         | Obsahuje interný generátor náhodných čísel pre bezpečné kryptografické operácie.                                                                       |
| **EEPROM**                                  | Ponúka 4.5 kb EEPROM pre uloženie kľúčov a dát, a 512-bit OTP pamäť pre fixné informácie.                                                              |
| **Viacnásobné I/O možnosti**                | Inkluduje UART-kompatibilné vysokorýchlostné jedno-drôtové rozhranie a 1 MHz I2C rozhranie.                                                            |
| **Rozsah napájacích a komunikačných napätí**| Pracuje v rozsahu napájacích napätí 2.0V do 5.5V a komunikačných napätí 1.8V do 5.5V.                                                                  |
| **Energeticky úsporný režim**               | Ponúka nízky spánkový prúd (<150 nA), zvyšujúci energetickú efektívnosť zariadenia.                                                                    |
| **Bezpečné sťahovanie a bootovanie**        | Zabezpečuje kontrolu ekosystému, bezpečnosť správ a ochranu proti klonovaniu pri spustení a aktualizácii zariadenia.                                   |
| **Dostupnosť v rôznych baleniach**          | Dostupné v baleniach ako 8-vývodový SOIC, 8-vývodový TSSOP, 3-vývodový SOT23, 8-políčkový UDFN a 3-vývodový CONTACT, poskytujúci flexibilitu v dizajne. |

Podrobné špecifikácie a technické údaje vychádzajú z [Datasheet ATSHA204a](https://ww1.microchip.com/downloads/en/DeviceDoc/ATSHA204A-Data-Sheet-40002025A.pdf).

## Prehľad

Hlavný program `main.c` demonštruje rôzne kryptografické operácie umožnené čipom ATSHA204a, ako je bezpečné ukladanie kľúčov, generovanie náhodných čísel a kryptografické výpočty. Kód inicializuje hardvér dosky, konfiguruje komunikačné nastavenia a vykonáva sériovú komunikáciu s ATSHA204a.

## Použité knižnice

### Štandardné knižnice AVR

- `<avr/io.h>`: Používané pre funkcie hardvérového vstupu/výstupu.
- `<avr/interrupt.h>`: Používané pre správu hardvérových prerušení.
- `<util/delay.h>`: Poskytuje funkcie oneskorenia.

Tieto knižnice sú základné pre rozhranie s hardvérom na platforme AVR, spracovanie prerušení a zavedenie potrebných oneskorení pre časovanie a synchronizáciu s čipom ATSHA204a.

### Štandardné knižnice C

- `<stdio.h>`: Používané pre operácie vstupu a výstupu.
- `<stdlib.h>`: Zahrnuje funkcie zahŕňajúce alokáciu pamäte, kontrolu procesov, konverzie a iné.
- `<string.h>`: Poskytuje funkcie na spracovanie reťazcov.

### Vlastné knižnice pre ATSHA204a

- `sha204_comm.h`: Zaoberá sa nižšie úrovňovými komunikačnými príkazmi s ATSHA204a.
- `sha204_comm_marshaling.h`: Uľahčuje maršálovanie príkazov a odpovedí medzi hostiteľom a ATSHA204a.
- `sha204_examples.h`: Obsahuje príklady použitia funkcií ATSHA204a.
- `sha204_i2c.h`: Riadi špecifiká komunikácie I2C s ATSHA204a.
- `sha204_return_codes.h`: Definuje návratové kódy pre operácie ATSHA204a.

Tieto knižnice sú súčasťou softvérovej knižnice ATSHA204a poskytovanej výrobcom čipu. Abstrahujú zložitosť surových kryptografických operácií a komunikácie s čipom.

### Knihovna UART

- `uart/uart.h`: Riadi sériovú komunikáciu, ktorá je nevyhnutná na odosielanie a prijímanie údajov z čipu ATSHA204a a na účely ladenia.

Táto vlastná knižnica je kľúčová pre nastavenie a operácie sériovej komunikácie, ktorá je hlavnou metódou rozhrania s ATSHA204a v tejto demonštrácii.

## Kľúčové funkcie

# Inicializácia sériovej komunikácie

Táto časť kódu inicializuje sériovú komunikáciu na zvolenej rýchlosti a nastavuje stdout na sériový port.

```c
// Definícia štruktúry pre štandardný výstup
FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

// Deklarácia funkcie pre inicializáciu dosky
void board_init();

// Implementácia funkcie pre inicializáciu dosky
void board_init()
{
    // Zakázanie všetkých prerušení
    cli();
    // Inicializácia sériovej komunikácie s rýchlosťou 38400 bodov za sekundu
    UART_init(38400);
    // Povolenie prerušenia pri príjme dát
    UCSR1B |= (1 << RXCIE1);
    // Nastavenie stdout na sériový port
    stdout = &uart_str;
    // Povolenie globálnych prerušení
    sei();
}
```
# `main()`

- Inicializuje dosku volaním `board_init()`.
- Odosiela príkazy na vyčistenie obrazovky a nastavenie atribútov textu pre lepšiu čitateľnosť.
  
```c
int main(void)
{
    // Inicializácia dosky
    board_init();

    // Odoslanie úvodných príkazov pre nastavenie sériovej komunikácie
    UART_SendChar(27);                // escape
    UART_SendString("[2J");           // clear and home
    UART_SendChar(27);                // escape
    UART_SendString("[0;32;40m");     // barva pozadi a textu

    // Inicializácia kryptografického obvodu ATSHA204
    sha204p_init();

    // Deklarácia a inicializácia premenných pre odpoveď z kryptografického obvodu
    uint8_t response[SHA204_RSP_SIZE_MAX];
    uint8_t wakeup_status = sha204c_wakeup(response);
```

### `board_init()`

Inicializuje hardvérové konfigurácie a nastavenia UART. Táto funkcia je nevyhnutná pre nastavenie potrebných hardvérových rozhraní a zabezpečenie pripravenosti mikrokontroléra na komunikáciu s ATSHA204a.

### `main()`

- Inicializuje dosku volaním `board_init()`.
- Odosiela príkazy na vyčistenie obrazovky a nastavenie atribútov textu pre lepšiu čitateľnosť.
- Prebúdza čip ATSHA204a a kontroluje jeho stav.
- Demonštruje odosielanie príkazu a prijímanie odpovede.
- Vykonáva rôzne kryptografické operácie, ako je čítanie sériového čísla, generovanie náhodného čísla, výpočet CRC a ďalšie.

Hlavná funkcia orchesteruje tok operácií, demonštruje schopnosti čipu ATSHA204a štruktúrovaným spôsobom.

## Kryptografické funkcie

- **Generovanie náhodných čísel:** Funkcia `sha204m_random()` generuje kryptograficky bezpečné náhodné čísla, ktoré môžu byť použité pre rôzne bezpečnostné aplikácie.
- **Čítanie sériového čísla:** Funkcia `sha204e_read_serial_number()` umožňuje čítanie jedinečného sériového čísla čipu, ktoré môže byť použité na autentifikáciu zariadenia.
- **Výpočet CRC:** Funkcia `sha204c_calculate_crc()` vypočíta kontrolný súčet pre overenie integrity údajov.

Tieto funkcie sú nevyhnutné pre zabezpečenie integrity a autenticity v komunikačných a kryptografických procesoch.

## Záver

Táto dokumentácia poskytuje vysokoúrovňový prehľad zdrojového súboru `main.c` pre demonštráciu ATSHA204a. Každá knižnica a funkcia je využívaná na predvedenie komplexného prístupu k používaniu kryptografických funkcií v zabudovaných systémoch.
