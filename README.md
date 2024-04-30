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

## Kontrola stavu prebudenia kryptografického obvodu

- Po inicializácii komunikácie s ATSHA204 sa pošle príkaz na jeho prebudenie.
- Návratový kód tohto príkazu sa uloží do premennej `wakeup_status`.
- Ak je `wakeup_status` rôzny od `SHA204_SUCCESS`, znamená to, že prebudenie zlyhalo.
- V takom prípade sa vypíše chybová hláška s konkrétnym návratovým kódom.
- Ak je `wakeup_status` rovnaký ako `SHA204_SUCCESS`, znamená to, že prebudenie bolo úspešné.
- V takom prípade sa vypíše správa o úspešnom prebudení zariadenia.
```c
// Kontrola stavu prebudenia kryptografického obvodu
if (wakeup_status != SHA204_SUCCESS)
{
    printf("Error: SHA204 wakeup failed! Return code: %d\n\r", wakeup_status);
}
else
{
    printf("SHA204 device woken up successfully!\n\r");
}
```

### Odeslanie príkazu kryptografickému obvodu

V tejto časti kódu je definovaný príkaz `my_command`, ktorý sa odosiela kryptografickému obvodu ATSHA204. Príkaz je reprezentovaný polem bajtov.

Funkcia `sha204p_send_command()` sa používa na odoslanie príkazu kryptografickému obvodu ATSHA204. Parametre funkcie sú veľkosť príkazu a samotný príkaz uložený v premennej `my_command`.

Návratový kód tejto funkcie sa uloží do premennej `send_status`.

Ak je `send_status` rôzny od `SHA204_SUCCESS`, znamená to, že odoslanie príkazu zlyhalo. V takom prípade sa vypíše chybová hláška s konkrétnym návratovým kódom.

Ak je `send_status` rovnaký ako `SHA204_SUCCESS`, znamená to, že príkaz bol úspešne odoslaný kryptografickému obvodu ATSHA204. V takom prípade sa vypíše správa o úspešnom odoslaní.

```c
// Definícia príkazu pre komunikáciu s kryptografickým obvodom ATSHA204
uint8_t my_command[] = { 0x03, 0x00, 0x00, 0x00 };

// Odoslanie príkazu kryptografickému obvodu ATSHA204 a uloženie návratového kódu
uint8_t send_status = sha204p_send_command(sizeof(my_command), my_command);

// Kontrola návratového kódu odosielania príkazu
if (send_status != SHA204_SUCCESS)
{
    // Ak odoslanie príkazu zlyhalo, vypíše sa chybová hláška s návratovým kódom
    printf("Error: Failed to send command to SHA204 device! Return code: %d\n\r", send_status);
}
else
{
    // Ak odoslanie príkazu bolo úspešné, vypíše sa správa o úspechu
    printf("Command sent successfully to SHA204 device!\n\r");
}
```

### Čítanie sériového čísla

V tejto časti kódu je vykonané čítanie sériového čísla z kryptografického obvodu ATSHA204.

```c
// Definícia premenných pre buffer a sériové číslo
uint8_t tx_buffer[10];
uint8_t sn[9];

// Čítanie sériového čísla z kryptografického obvodu ATSHA204
volatile uint8_t sn_read_status = sha204e_read_serial_number(tx_buffer, sn);

// Kontrola úspešnosti čítania sériového čísla
if (sn_read_status == SHA204_SUCCESS)
{
    // Ak bolo čítanie úspešné, vypíše sa sériové číslo
    printf("Serial number read successfully: ");
    for (int i = 0; i < 9; i++)
    {
        printf("%02X ", sn[i]);
    }
}
else
{
    // Ak čítanie zlyhalo, vypíše sa chybová hláška s návratovým kódom
    printf("Error reading serial number! Return code: %d", sn_read_status);
}

printf("\n\r");

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
