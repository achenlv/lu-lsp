**LU DN Kurss: Linux sistēmu programmēšana**  
**Vidus-semestra kontroldarbs**   
 

## Iesniegšanas termiņš:
  23:59  13.03.2025  
Risinājums tiks savākts no servera divas reizes:  
12:00, kad sagaidāms ka A daļa ir izpildīta pilnībā;  
23:59, kad sagaidāms ka arī B daļa ir izpildīta.

## Noteikumi

* Noteikumu neievērošana var novest pie darba anulēšanas.  
* Maksimāli kontroldarbā iespējams iegūt 100% (100 punktus).  
* Risinājumu izklāstā esiet īsi un kodolīgi, tomēr neaizmirstiet uzrakstīt pieņēmumus, risinājumu, un pamatojumu kur nepieciešams. Komentējiet pirmkodu kur tas nepieciešams.  
* Darbs pie kontroldarba ir individuāls. Vienīgās pieļaujamās konsultācijas ir ar pasniedzēju. Ja studentu darbi liecinās par plaģiātismu rezultāts tiks anulēts.


  
## Iesniegšana 
Uzdevums ir risināms un rezultāti iesniedzami kursa serverī  asm1.df.lu.lv   
Risinājums iesniedzams jūsu direktorijā  `~/kd1`

Iesniedzamie faili:

* kd1.c  		– Programmas pirmkods vienā failā  
* Makefile 	– Nodrošina programmmas kompilāciju  
* one		– Vienības translēšanas fails  
* zero		– Nulles translēšanas fails  
* tran		– Patvaļīgs translēšanas fails  
* chip		– Patvaļīgs šifrēšanas fails  
* test.in		– Ievaddatu testa fails  
* analysis.pdf 	– Komentāri un analīze

Visiem failiem jābūt iesniegtiem attiecīgajā direktorijā  līdz iesniegšanas termiņam, kad tie tiks savākti.

## Uzdevumi

### A: (80%)
Izstrādāt programmu **kd1**, kas pārveido ieejas datus lietojot **translācijas** failu (translation-file) vai **šifrēšanas** tabulu (cypher-table), un ir izsaucama sekojoši:

		kd1 [-t translation-file] [-s cypher-table]   
			[-o output-file] [input-file]

		-t nosaka translācijas failu  
		-s nosaka šifrēšanas tabulas failu   
		-o nosaka izvada failu

* **Ieejas dati** jālasa no *stdin* ja faila vārds nav norādīts parametros (input-file).   
* **Izejas dati** jāraksta uz *stdout* ja faila vārds nav norādīts parametros (output-file).  
* Parametrus \-t un \-s nav paredzēts lietot vienlaicīgi.

* Ja nav uzrādīti ne šifrēšanas tabula ne translācijas fails, tad lieto iebūvēto šifrēšanu, kas darbojas sekojoši: ar katru ievada-simbolu izpilda operāciju XOR 0x80, tātad, nomainot katra baita kreiso bitu uz pretējo.

**Šifrēšanas fails** (cypher-table) ir binārs fails ar 256 baitiem, kas definē šifrēšanas tabulu.

Ievada-simbols tiek šifrēts atrodot atbilstošo baitu tabulā pēc simbola koda. Piemēram, ja simbola 'A' kods ir 65, tad no tabulas tiek paņemts 65\. baits (indeksējot no 0\) un ar to tiek aizstāti visi ‘A’ kas ir ievaddatos.

**Translācijas fails** (translation-file) ir fails, kurā ir divas simbolu virknes, kas norādīs kuri simboli ir tulkojami uz citiem. 

Faila pirmais baits norāda, ar kādu simbolu tiks atdalītas abas simbolu virknes turpmāk failā. Tas var būt jebkuršs simbols, piemēram, jaunas rindas simbols.

Pirmā simbolu virkne satur simbolus kurus jātulko, bet otrā satur jaunos simbolus – uz kuriem tulkot.  
Ja jātulko arī pats atdalītājsimbols uz ko citu, tad tā jauno simbolu pievieno pašās faila beigās.

**Piemēram**, translācijas fails:

    XabcXABCY

nomainītu visus mazos burtus a, b, un c uz attiecīgajiem lielajiem burtiem. Pats atdalītājsimbols X tiks tulkots par Y.

Piezīme: par atdalītājsimbolu varētu būt ērti lietot “newline” (apzīmēts ar ↵), 0x0A simbolu, tad translācijas fails būtu sekojošs: pirmā rinda tukša, tad seko pirmā virkne jaunā rindā, un tad jaunā rindā otrā virkne:

    ↵  
    abc↵
    ABC↵

#### Uzdevums
Izstrādāt un testēt programmu, kas realizē šo specifikāciju.   
Izveidot un pievienot arī **testa failus, tai skaitā:**

* (5%) vienības translācijas fails (katrs simbols uz sevi),  
* (5%) nulles translācijas fails (visi simboli uz nulli ’\\0’) un   
* (5%) patvaļīga šifrēšanas tabula,   
* (5%) patvaļīgs translācijas fails,   
* (5%) ievaddati testam.

Vēlams realizēt pēc iespējas optimālu programmas darbību. 

Komentējiet kodu pēc būtības.

Veiciet kļūdu pārbaudi visos pēc jūsu domām kritiskajos gadījumos. Kodam jābūt pēc iespējas noturīgam pret ievada, parametru un sistēmas kļūmēm.

#### Vērtējums

* 25% \- Pievienotie testa faili, kā aprakstīts iepriekš  
* 5% \- Saturīgs un komentēts pirmkods kas risina problēmu  
* 5% \- Makefile programmas kompilēšanai un testēšanai (darbinot *make* un *make test*)  
* 5% \- Pirmkods kompilējas bez kļūdām  
* 40% \- Atbilst pārbaudes testiem.

### B: (20%)
Analīze un  novērtējums optimālai programmas kd1a darbībai.

Veikt jūsu programmas **kd1** darbības analīzi attiecībā uz tās veiktspēju (ātrumu) ar buferizētu un nebuferizētu ievadu un izvadu. Novērtēt kāds būtu optimāls bufera izmērs lai uzlabotu ātrdarbību. Ņemiet vērā, ka šim risinājumam tas var atšķirties no praktisko darbu rezultātiem. Jums empīriski vai analītiski jāparada, ka risinājuma bufera izmērs ir optimāls.

Programmas ierobežojumi ir sekojoši:

* Patērētā datu atmiņa nedrīkst pārsniegt 64KB.   
* Buferim jāatrodas datu (nevis steka vai *heap*) atmiņā.

Aprakstīt savus testu rezultātus, novērojumus un lēmumus attiecībā uz kd1a arhitektūru un veiktspēju. Aprakstu noformēt pdf failā 

**analysis.pdf**

un pievienot risinājumam. Fails var saturēt gan tekstu, gan grafisku informāciju, piemēram, veiktspējas analīzes grafiks atkarībā no bufera izmēra.