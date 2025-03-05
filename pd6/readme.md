## Praktiskais darbs #6 - atmiņas rezervācija
### Programma

Uzrakstīt programmu, kas rezervē atmiņu (drīkst to pēc tam neatbrīvot). Programmu būs jāizmanto zemak aprakstītajos eksperimentos. Par pamatu var izmantot šo skeletu:

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  printf("allocating...\n");

  // ...

  printf("done\n");
  return 0;
}
```

### Eksperimenti
Eksperimentāli noskaidrot, cik daudz atmiņas iespējams rezervēt heap segmentā (var lietot malloc()). Salīdziniet - vai un kā šis apjoms mainās, ja:

*  Atmiņa tiek rezervēta "vienā gabalā" (one chunk)
*  Atmiņa tiek rezervēta pa 1MB lieliem gabaliem (many chunks)

Kādi ir secinājumi?

### Novērtējums

Salīdziniet programmu ātrdarbību:

*  1. variants - atmiņa tiek rezervēta, bet netiek izmantota.
*  2. variants - atmiņa tiek rezervēta un tiek izmantota (piemēram, aizpildīta ar 0, izmantojot memset() funkciju)
*  Salīdziniet izpildes laiku, ja tiek veikta 100MB atmiņas rezervāciju.

Padoms. Laiku ērti ir mērīt ar programmu time. (Lai korekti interpretētu programmas dotos rezultātus, jāsaskaita kopā "system" un "user" laiks; pirmais atbilst laikam, kas pavadīts sistēmas izsaukumos, otrais: laikam, ko programma patērējusi izpildot pati savu kodu).

Izmantojot proc failusistēmu vai ps programmu, novērtējiet (1) kopējās rezervētās virtuālās atmiņas (VmSize), (2) resident set size izmērus (VmRSS) abos gadījumos. Secinājumi?

### Steka atmiņa

Eksperimentāli noskaidrot, cik daudz atmiņas iespējams rezervēt stekā. Padoms - atmiņas rezervācijai stekā izmantot funkciju alloca()!

*  Kas notiks, ja programmas darbības laikā tiks izsaukta šāda funkcija:
```
void f() {
     char array[100 * 1024 * 1024];
     memset(array, 0, sizeof(array));
}
```
*  Izskaidrot, kāpēc. Kas mainītos, ja mainīgais array būtu deklarēts kā "static"?

### Iesūtīšana

Risinājumus ievietot estudijas.lu.lv Atbildes raksta atvēlētajā teksta laukā, Programmas pirmkodu pievienot pielikumā ar nosaukumu "LSP_PD6_Vards_Uzvards.c". 