# Projekt ATSHA204a

# Features of ATSHA204a

Náš projekt využíva zabezpečený kryptografický prvok ATSHA204a, ktorý poskytuje pokročilé funkcie pre bezpečnú autentifikáciu a šifrovanie v zabudovaných systémoch. Nižšie sú podrobne opísané kľúčové funkcie, ktoré zahrňujeme vo vývoji našich zariadení.

## Zabezpečené Hardvérové Úložisko Kľúčov

- ATSHA204a poskytuje zabezpečené úložisko pre kryptografické kľúče, založené na hardvérovej báze, chrániace pred neoprávneným prístupom a únikom informácií.

## Autentifikácia

- Podporuje bezpečné symetrické autentifikačné operácie medzi hostiteľom a klientom, zaručujúc bezpečnú komunikáciu medzi zariadeniami.

## SHA-256 Hashovací Algoritmus

- Implementuje vynikajúci hashovací algoritmus SHA-256, ktorý poskytuje možnosti pre výpočet správy autentifikačného kódu (MAC) a hash-bazovaného správy autentifikačného kódu (HMAC).

## Dĺžka Kľúča a Úložisko

- Poskytuje špičkovú dĺžku kľúča 256-bitov a úložisko pre až 16 kľúčov, umožňujúc robustné zabezpečenie pre rôzne aplikácie.

## Unikátne Sériové Číslo

- Každé zariadenie má zaručené unikátne 72-bitové sériové číslo, ktoré zlepšuje možnosti sledovania a autentifikácie.

## Generátor Náhodných Čísel

- Obsahuje interný generátor náhodných čísel (RNG) vysoké kvality pre bezpečné kryptografické operácie.

## EEPROM

- Ponúka 4.5 kb EEPROM pre uloženie kľúčov a dát, a 512-bit OTP (One Time Programmable) pamäť pre fixné informácie.

## Viacnásobné Možnosti Vstupu/Výstupu

- Podporuje viacnásobné komunikačné rozhrania vrátane UART-kompatibilného vysokorýchlostného jedno-drôtového rozhrania a 1 MHz I2C rozhrania.

## Rozsah Napájacieho a Komunikačného Napätia

- Pracuje v rozsahu napájacích napätí 2.0V do 5.5V a komunikačných napätí 1.8V do 5.5V, čo umožňuje flexibilitu v rôznych aplikačných prostrediach.

## Energeticky Úsporný Režim

- Ponúka extrémne nízky spánkový prúd (<150 nA), čo zvyšuje energetickú efektívnosť zariadenia.

## Bezpečné Sťahovanie a Bootovanie

- Zabezpečuje kontrolu ekosystému, bezpečnosť správ a ochranu proti klonovaniu, čím zvyšuje celkovú bezpečnosť zariadenia pri jeho spúšťaní a aktualizácii.

## Dostupnosť v Rôznych Baleniach

- Dostupné v rôznych formátoch balenia vrátane 8-vývodového SOIC, 8-vývodového TSSOP, 3-vývodového SOT23, 8-políčkového UDFN a 3-vývodového CONTACT, čo poskytuje flexibilitu pri návrhu hardvéru.

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

