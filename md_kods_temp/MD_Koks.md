# MD\_Koks

LSP kursa mājas darbs

## Mērķis

Darbs ar failiem un datu struktūrām programmēšnas valodā C.

## Uzdevums

Ģenerēt ģimenes koku. Ievaddatos ir informācija par personām. Jāizvada personu vārdi atkarība no to vecuma dilstošā kārtībā \- vecākas paaudzes cilvēki jāizdrukā vispirms.

Ievadā ir fails ar personu aprakstiem sekojošā formātā:

VARDS personas-vards  
MATE personas-mates-vards  
TEVS personas-tēva-vārds

Personu apraksts vienmēr sākas ar lauku VARDS. Pārējie divi lauki var nebūt norādīti, piemēram, kādai personai var nebūt zināms tēva vārds.

Personu skaits nav ierobežots, bet ir saprātīgs un ietilps pieejamajā operatīvajā atmiņā. Risinājums tiks pārbaudīts ar vismaz 1000 personu testa piemēriem.

Ja nav iespējams noteikt savstarpējo kārtību starp personām, tas nozīmē, ka pieļaujami vairāki korekti risinājumi. Piemēram, A ir tēvs B un A ir tēvs C \- visi sekojošie risinājumi ir korekti: ABC un ACB. Jādrukā tikai viens no tiem, pēc jūsu izvēles.

Var pieņemt, ka visas personas sadalās pa paaudzēm, un ka atšķirīgu paaudžu cilvēki savā starpā neprecēsies.

Vienam cilvēkam var būt bērni no dažādiem partneriem.

Ievaddati var saturēt divus vai vairāk nesaistītus ģimenes kokus. Tādā gadījumā tos vajag izvadīt atsevišķi: vispirms vienu koku, tukša rinda, tad nākamo koku.

Ievaddati var būt arī nekorekti. Piemēram, ja vienai personai ir divas mātes, vai saites veido ciklu. To jāspēj detektēt un izdot attiecīgu paziņojumu, kā arī programmai jābeidzas ar kodu 1\. Veiksmīgas izpildes gadījumā programma beidzas ar kodu 0\.

### Tehniska informācija:

Ievaddati nāk no **stdin**  
Izvads jādod uz **stdout**

Ievada rindas garums nepārsniedz 63 simbolus.  
Ievadā iespējamas tukšas rindas, kas jāignorē.  
Testēšana, pārbaude  
Programmai jāspēj pieņemt datu ievads no faila (caur "shell redirection").

Piemēram, programma var tikt testēta šādi:

./md\_koks \< in.txt \> out.txt

kur "in.txt" ir fails ar ievaddatiem, bet "out.txt" \- fails, kurā saglabāt programmas izvadu.

## Jāiesniedz

Visiem failiem jābūt uz **asm1.df.lu.lv** servera, jūsu kontā, direktorijā **md\_koks**

Iesniedzami sekojoši faili ar tieši šādiem nosaukumiem:

* md\_koks.c \- C programmas pirmkods.  
* Makefile \- lai jūsu projektu var kompilēt ar make sistēmu.  
* in.txt \- Viens testa ievada fails, kuru izmantojāt testēšanai.  
* README.txt \- ja nepieciešams dot komentārus vai atbildes uz jautājumiem.

Nevajag failus arhivēt.

## Vērtējums

Uzdevums tiks vērtēts sekojoši:

80% \- Korektu ievaddatu apstrāde un pareiza rezultātu izdruka.  
20% \- Nekorektu ievaddatu atpazīšana un kļūdas paziņojuma izdruka. Piemēram, A tēvs B un B tēvs A ir cikliskas attiecības.

## Piemēri

Ievada faila piemērs:

VARDS Dels  
TEVS Tevs  
MATE Mate

VARDS Tevs  
MATE Vecmate

VARDS CitsDels  
MATE CitaMate  
TEVS CitsTevs

Izvada piemērs:

Vecmate  
Mate  
Tevs  
Dels

CitsTevs  
CitaMate  
CitsDels  
