## Mājas darbs #4 - atmiņas rezervācija

1. Uzrakstīt programmu, kas rezervē atmiņu (drīkst pēc tam neatbrīvot). Uzdevums līdzīgs PD#6, drīkst izmantot tā kodu.

2. Salīdzināt trīs dažādus atmiņas rezervācijas veidus:

    Ar malloc() funkciju
    Ar mmap() funkciju (lietojot to bez konkrētas atmiņas adreses norādīšanas, bez "apakšā esoša" faila, un tā, lai varētu rezervēto atmiņu lietot gan lasīšanai, gan rakstīšanai)
    Ar sbrk() funkciju

Rezervēt atmiņu pa vienam megabaitam (= 1024*1024 baitu).

Katram veidam izpētīt:

    Kāds ir maksimālais atmiņas daudzums, ko tādā veidā var alocēt?
    Kāds programmas darbības laiks, ja tiek rezervēti 100 megabaiti atmiņas?

Kurš ir ātrākais veids šim konkrētajam lietojumam? Ar kuru veidu var rezervēt visvairāk atmiņas? Uzrakstīt 1-2 teikumu garu skaidrojumu - kāpēc tā?

Padoms. Laiku ērti ir mērīt ar programmu time. (Lai korekti interpretētu programmas dotos rezultātus, jāsaskaita kopā "system" un "user" laiks; pirmais atbilst laikam, kas pavadīts sistēmas izsaukumos, otrais: laikam, ko programma patērējusi izpildot pati savu kodu)


3. Izvēlēties vienu no rezervācijas veidiem, un izpētīt, kā uzrakstītajā programmā tiek izmantota statiskā atmiņa (tā atmiņa, kas ir rezervēta jau tās palaišanas brīdī). Ar programmu size vai objdump palīdzību noteikt:

    TEXT segmenta izmēru baitos
    DATA segmenta izmēru baitos
    BSS segmenta izmēru baitos

Jāiesniedz

Darbs iesūtāms kursa serverī, direktorijā md_mem.

Iesūtāmi sekojoši faili:

    md_mem.c programmas pirmkodu, kura mēra maksimālo pieejamo atmiņas daudzumu.
    md_mem_100.c programmas pirmkodu, kura rezervē 100MB atmiņas.
    apraksts.pdf īss apraksts un atbildes uz jautājumiem. nepieciešamības gadījumā dokumentā var iekļaut ekrānšāviņus.