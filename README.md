# Projekt ATSHA204a

# Úvod

Cieľom tohto projektu je implementovať ovládač/knižnicu pre kryptografický obvod ATSHA204a. Táto knižnica poskytuje rozhranie pre základné operácie s obvodom, ako je ukladanie kľúčov pre AES128, identifikácia zariadenia, využitie jedinečného identifikátora (ID), hashovanie a generovanie náhodných čísel.

## Funkcie knižnice:

- **Ukladanie kľúčov pre AES128**: Implementácia funkcií na ukladanie a čítanie kľúčov pre šifrovanie a dešifrovanie pomocou algoritmu AES128.
- **Identifikácia zariadenia**: Rozšírené funkcie na identifikáciu zariadenia založené nielen na jednoduchom identifikátore.
- **Jedinečný identifikátor (ID)**: Využitie jedinečného identifikátora zariadenia na rôzne účely, ako je napríklad priradenie sieťových adries alebo autentifikácia.
- **Hashovanie**: Implementácia funkcií na vytváranie hashov z dát získaných zo zariadenia, čo môže byť užitočné pri overovaní autenticity dát.
- **Generovanie náhodných čísel**: Funkcie na generovanie náhodných čísel pomocou kryptografického obvodu ATSHA204a, ktoré môžu byť využité pri rôznych kryptografických operáciách.

| Funkcia                          | Popis                                                                                         |
|----------------------------------|-----------------------------------------------------------------------------------------------|
| Ukladanie kľúčov pre AES128      | Implementácia funkcií na ukladanie a čítanie kľúčov pre šifrovanie a dešifrovanie pomocou algoritmu AES128. |
| Identifikácia zariadenia         | Rozšírené funkcie na identifikáciu zariadenia založené nielen na jednoduchom identifikátore. |
| Jedinečný identifikátor (ID)     | Využitie jedinečného identifikátora zariadenia na rôzne účely, ako je napríklad priradenie sieťových adries alebo autentifikácia. |
| Hashovanie                       | Implementácia funkcií na vytváranie hashov z dát získaných zo zariadenia, čo môže byť užitočné pri overovaní autenticity dát. |
| Generovanie náhodných čísel     | Funkcie na generovanie náhodných čísel pomocou kryptografického obvodu ATSHA204a, ktoré môžu byť využité pri rôznych kryptografických operáciách. |


Podrobné špecifikácie a technické údaje vychádzajú z [Datasheet ATSHA204a](https://ww1.microchip.com/downloads/en/DeviceDoc/ATSHA204A-Data-Sheet-40002025A.pdf).

## Prehľad

Hlavný program `main.c` demonštruje funkcie umožnené čipom ATSHA204a, ako je bezpečné ukladanie kľúčov, generovanie náhodných čísel a kryptografické výpočty. Kód inicializuje hardvér dosky, konfiguruje komunikačné nastavenia a vykonáva sériovú komunikáciu s ATSHA204a.

## Vlastné knižnice pre ATSHA204a

- `sha204_comm.h`: Zaoberá sa nižšie úrovňovými komunikačnými príkazmi s ATSHA204a.
- `sha204_comm_marshaling.h`: Uľahčuje maršálovanie príkazov a odpovedí medzi hostiteľom a ATSHA204a.
- `sha204_examples.h`: Obsahuje príklady použitia funkcií ATSHA204a.
- `sha204_i2c.h`: Riadi špecifiká komunikácie I2C s ATSHA204a.
- `sha204_return_codes.h`: Definuje návratové kódy pre operácie ATSHA204a.

Tieto knižnice sú súčasťou softvérovej knižnice ATSHA204a poskytovanej výrobcom čipu.

### Knižnica UART

- `uart/uart.h`: Riadi sériovú komunikáciu, ktorá je nevyhnutná na odosielanie a prijímanie údajov z čipu ATSHA204a a na účely ladenia.

# Kľúčové funkcie

### Inicializácia sériovej komunikácie

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
### main()
- Inicializuje dosku volaním `board_init()`.
  
```c
int main(void)
{
    // Inicializácia dosky
    board_init();

    // Odoslanie úvodných príkazov pre nastavenie sériovej komunikácie
    UART_SendChar(27);                // escape
    UART_SendString("[2J");           // clear and home
    UART_SendChar(27);                // escape
    UART_SendString("[0;32;40m");     // farba pozadia a textu

    // Inicializácia ATSHA204
    sha204p_init();

    // Deklarácia a inicializácia premenných pre odpoveď
    uint8_t response[SHA204_RSP_SIZE_MAX];
    uint8_t wakeup_status = sha204c_wakeup(response);
```

### Kontrola stavu prebudenia kryptografického obvodu
- Po inicializácii komunikácie s ATSHA204 sa pošle príkaz na jeho prebudenie.
- Návratový kód tohto príkazu sa uloží do premennej `wakeup_status`.
- Ak je `wakeup_status` rôzny od `SHA204_SUCCESS`, znamená to, že prebudenie zlyhalo.
- V takom prípade sa vypíše chybová hláška s konkrétnym návratovým kódom.
- Ak je `wakeup_status` rovnaký ako `SHA204_SUCCESS`, znamená to, že prebudenie bolo úspešné.
- V takom prípade sa vypíše správa o úspešnom prebudení zariadenia.

### Odeslanie príkazu kryptografickému obvodu
V tejto časti kódu je definovaný príkaz `my_command`, ktorý sa odosiela ATSHA204. Príkaz je reprezentovaný polom bajtov.

Funkcia `sha204p_send_command()` sa používa na odoslanie príkazu ATSHA204. Parametre funkcie sú veľkosť príkazu a samotný príkaz uložený v premennej `my_command`.

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

V tejto časti kódu je vykonané čítanie sériového čísla z obvodu ATSHA204 pomocou I2C komunikácie, ktorá je implementovaná vo vytvorenej knižnici `sha204_i2c`. Po inicializácii a zobudení obvodu z režimu spánku je možné posielať príkazy a prijímať odpovede zo zariadenia. Komunikácia prebieha cez definované funkcie, ktoré zabezpečujú odosielanie adries, správ a riadenie stavov zariadenia.
 
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
```

### Generovanie náhodného čísla

V tejto časti kódu je generované náhodné číslo pomocou kryptografického obvodu ATSHA204.

- Premenné `command`, `response_random` a ukazovateľ `random_number` sú definované na uchovanie príkazu a odpovede z ATSHA204. 

- Funkcia `sha204m_random` implementovaná v knižnici `sha204_comm_marshalling` slúži na generovanie náhodných čísel pomocou SHA204a. Pri jej použití sa poskytne buffer pre odoslanie (`tx_buffer`) a buffer pre prijatie (`rx_buffer`) dát, spolu s módom generovania náhodných čísel, určeným parametrom `mode`. 

Funkcia najskôr overí, či nie sú parametre `tx_buffer` alebo `rx_buffer` nulové ukazovatele a či je hodnota `mode` v rozsahu povolených módov. Následne nastaví príslušné hodnoty v bufferoch `tx_buffer` a `rx_buffer` pre vykonanie príkazu na generovanie náhodných čísel. 

Potom odosiela príkaz a prijíma odpoveď od obvodu SHA204a. Ak sa operácia úspešne vykoná, vráti návratový kód `SHA204_SUCCESS`, inak vráti chybový kód `SHA204_BAD_PARAM`.

```c
// Definícia premenných pre príkaz, odpoveď a ukazovateľ na náhodné číslo
uint8_t command[SHA204_CMD_SIZE_MAX];
uint8_t response_random[RANDOM_RSP_SIZE];
uint8_t *random_number = &response_random[SHA204_BUFFER_POS_DATA];

// Generovanie náhodného čísla pomocou kryptografického obvodu ATSHA204
volatile uint8_t rand_gen_status = sha204m_random(command, response_random, RANDOM_NO_SEED_UPDATE);

// Kontrola úspešnosti generovania náhodného čísla
if (rand_gen_status == SHA204_SUCCESS)
{
    // Ak bolo generovanie úspešné, vypíše sa náhodné číslo
    printf("Random number generated successfully: ");
    for (int i = 0; i < RANDOM_RSP_SIZE - SHA204_BUFFER_POS_DATA; i++)
    {
        printf("%02X ", random_number[i]);
    }

    // Prevedenie kryptografického obvodu ATSHA204 do spánkového režimu
    sha204e_sleep();
}
else
{
    // Ak generovanie zlyhalo, vypíše sa chybová hláška s návratovým kódom
    printf("Error generating random number! %d", rand_gen_status);
}

printf("\n\r");
```

### CRC výpočet
Funkcia `sha204c_calculate_crc` iteruje cez všetky byty vstupných údajov a aplikuje na ne polynomiálne delenie. Výsledkom je dvojbajtové CRC, ktoré je uložené v poli crc. Následne je možné vypočítané CRC vypísať pomocou printf funkcie.

### Čítanie konfiguračnej zóny

V tejto časti kódu je vykonané čítanie konfiguračnej zóny z ATSHA204.

- Premenné `config_data`, `device_id` a `read_config_status` sú definované na uchovanie údajov konfiguračnej zóny, identifikátoru zariadenia a stavu čítania konfiguračnej zóny.
- Funkcia `sha204e_read_config_zone()` sa používa na čítanie konfiguračnej zóny z ATSHA204. Parametre funkcie zahŕňajú identifikátor zariadenia a pole pre ukladanie údajov konfiguračnej zóny.
- Návratový kód funkcie sa ukladá do premennej `read_config_status`.
- Ak je `read_config_status` rovný `SHA204_SUCCESS`, vypíšu sa údaje konfiguračnej zóny vo formáte bajtov.
- V prípade, že `read_config_status` nie je `SHA204_SUCCESS`, vypíše sa chybová hláška s konkrétnym návratovým kódom.

```c
// Definícia premenných pre údaje konfiguračnej zóny, identifikátor zariadenia a stav čítania konfiguračnej zóny
uint8_t config_data[88];
uint8_t device_id = 0xC8;
uint8_t read_config_status = sha204e_read_config_zone(device_id, config_data);

// Výpis hlavičky pre konfiguračnú zónu
printf("Config zone:\n\r");

// Kontrola úspešnosti čítania konfiguračnej zóny
if (read_config_status == SHA204_SUCCESS)
{
    // Ak bolo čítanie úspešné, vypíšu sa údaje konfiguračnej zóny
    for (int i = 0; i < 88; i++)
    {
        printf("Byte %d: 0x%02X\n\r", i, config_data[i]);
    }
}
else
{
    // Ak čítanie zlyhalo, vypíše sa chybová hláška s návratovým kódom
    printf("Error reading config zone! %d", read_config_status);
}
```

### R/W dát

- V tejto časti kódu sú definované premenné pre buffer na odoslanie (`write_tx_buffer`), buffer na prijatie (`write_rx_buffer`), zónu zápisu (`write_zone`), adresu zápisu (`write_address`) a MAC (`mac`).
- Funkcia `sha204m_write()` sa používa na zápis dát do kryptografického obvodu ATSHA204. Parametre funkcie zahŕňajú buffery na odoslanie a prijatie, zónu zápisu, adresu zápisu, náhodné číslo a MAC.
- Návratový kód funkcie `sha204m_write()` sa ukladá do premennej `write_data_status`.
- Ak je `write_data_status` rovný `SHA204_SUCCESS`, vypíše sa správa o úspechu zápisu dát.
- V opačnom prípade, ak zápis dát zlyhal, vypíše sa chybová hláška s konkrétnym návratovým kódom.
- Ďalej je realizované čítanie dát z kryptografického obvodu ATSHA204 pomocou funkcie `sha204m_read()`. Parametre funkcie zahŕňajú buffery na odoslanie a prijatie, zónu čítania a adresu čítania.
- Návratový kód funkcie `sha204m_read()` sa ukladá do premennej `read_data_status`.
- Ak je `read_data_status` rovný `SHA204_SUCCESS`, vypíšu sa prečítané dáta vo formáte hexadecimálnej reprezentácie.
- V opačnom prípade, ak čítanie dát zlyhalo, vypíše sa chybová hláška s konkrétnym návratovým kódom.


```c
// Definícia premenných pre buffer na odoslanie, buffer na prijatie, zónu zápisu, adresu zápisu a MAC
uint8_t write_tx_buffer[SHA204_COUNT_IDX + SHA204_ZONE_ACCESS_32 +
	WRITE_MAC_SIZE + SHA204_CRC_SIZE];
uint8_t write_rx_buffer[WRITE_RSP_SIZE];
uint8_t write_zone = SHA204_ZONE_DATA;
uint16_t write_address = 0x0000;
uint8_t mac[WRITE_MAC_SIZE] = { 0 };

// Zápis dát do kryptografického obvodu ATSHA204
uint8_t write_data_status =
	sha204m_write(write_tx_buffer, write_rx_buffer, write_zone, write_address,
		random_number, mac);

// Kontrola úspešnosti zápisu dát
if (write_data_status == SHA204_SUCCESS)
{
    // Ak bol zápis úspešný, vypíše sa správa o úspechu
    printf("Success writing data!");
}
else
{
    // Ak zápis zlyhal, vypíše sa chybová hláška s návratovým kódom
    printf("Error writing data zone! %d\n\r", write_data_status);
}

// *********************Čítanie dát *********************

// Definícia premenných pre buffer na odoslanie, buffer na prijatie, zónu čítania a adresu čítania
uint8_t read_tx_buffer[5];
uint8_t read_rx_buffer[READ_32_RSP_SIZE];
uint8_t read_zone = SHA204_ZONE_DATA;
uint16_t read_address = 0x0000;

// Čítanie dát z kryptografického obvodu ATSHA204
uint8_t read_data_status =
	sha204m_read(read_tx_buffer, read_rx_buffer, read_zone, read_address);

// Kontrola úspešnosti čítania dát
if (read_data_status == SHA204_SUCCESS)
{
    // Ak bolo čítanie úspešné, vypíšu sa prečítané dáta
    for (int i = 0; i < READ_32_RSP_SIZE; i++)
        printf("Byte %d: 0x%02X\n", i, read_rx_buffer[i]);
}
else
{
    // Ak čítanie zlyhalo, vypíše sa chybová hláška s návratovým kódom
    printf("Error reading data zone! %d\n\r", read_data_status);
}

return 0;
```

