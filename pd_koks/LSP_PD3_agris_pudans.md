ārējais resurss: http://andromeda.df.lu.lv/wiki/index.php/LU-LSP-b13:L03

LU-LSP-b13:L03
Praktiskais darbs #3

Realizēt programmu, kas meklē failus direktoriju struktūrā.

Programma saņem divus komandrindas argumentus: meklējamā faila vārdu un direktorijas nosaukumu.

Izdrukāt visus gadījumus, kuros norādītajā direktorijā (iekļaujot apakšdirektorijas) atrodas fails ar norādīto nosaukumu. Izvaddatu formāts: ceļš uz failu relatīvi norādītajai direktorijai.

Ņemt vērā, ka meklējamā faila tips var būt patvaļīgs! Tas var būt, piemēram, vājā saite, vai pat direktorija. Svarīgs ir tikai nosaukums.

Piemēram:

$ ./pd3 gcc /usr/bin

Izvada:

/usr/bin/gcc

Cits piemērs:

$ touch test.txt
$ ./pd3 test.txt .

Izvada:

./test.txt

vai arī

 test.txt

Beigt darbu ar kodu -1, ja ievadargumenti nav padoti, vai arī otrais arguments nav direktorija. Citos gadījumos beigt darbu ar kodu 0.

Precizējums: drīkst pieņemt, ka maksimālais ceļa garums (faila vārds + visas direktorijas pa ceļam uz to) nepārsniedz 255 simbolus.

Precizējums #2: vājajām saitēm nevajag sekot.

Risinājumus iesniegt e-studijas.lu.lv. Iesniedzamais fails jānosauc šādi: LSP_PD3_vards_uzvards.c. Tas nav jāarhivē. 