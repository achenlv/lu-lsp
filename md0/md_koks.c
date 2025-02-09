/**********************************************
 * Agris Pudāns
 * st. apl. nr. ap08426
 * LSP MD0 Ģimenes koks
 * 
 * Uzdevumā būtu jāizmanto:
 * (o) struct - saglabāt informāciju par elementu
 * (o) hash table - ātrai piekļuvei pie elementa pēc id
 * (o) graph - ģimenes koka datu struktūra, kur mezgli ir elementi un malas ir elementu attiecības
 * 
 * Datus ielasa no stdin ievada un saglabā hash tabulā.
 * Datus izvada uz stdout izvadi.
 * 
 * Izmanto šādus grafa algoritmus:
 * (o) topological sorting - lai sakārtotu grafu pēc vecākā elementa
 * (o) depth-first search (DFS) - lai atrastu iespējamo ciklu 
 * (!) varbūt vēl kaut ko citu kļūdu noteikšanai
 * 
 * TODO:
 * - Ielasīt datus no ievada un izveidot grafu
 * - Izdrukāt grafu (pēc tam arī secībā)
 * - Sakārtot grafu
 * - Noteikt ciklus
 * - Noteikt dublējumus (>1 mātes/tēvi)
 * - 
 * 
 * 
 ***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GARUMS 64


typedef struct Cilveks {
  char pers_vards[MAX_GARUMS];
  char pers_mate[MAX_GARUMS];
  char pers_tevs[MAX_GARUMS];
  struct Cilveks* next;
} Cilveks;

Cilveks* pievienot(const char* vards) {
  Cilveks* cilveks;
  
  cilveks = (Cilveks*)malloc(sizeof(Cilveks));
  sv_kopet(vards,cilveks->pers_vards);
  cilveks->pers_mate[0] = '\0';
  cilveks->pers_tevs[0] = '\0';
  cilveks->next = NULL;
  
  return cilveks;
}

void sv_kopet(char *no, char *uz) {
  while ((*uz++ = *no++) != '\0');
}

void ielasit() {
  char ievads[MAX_GARUMS];
  Cilveks* jauns_cilveks = NULL;

  while (fgets(ievads, sizeof(ievads), stdin)) {
    if (ievads[0] == '\n') continue;

    // TODO: izmantot savu no parauga sv_meklet
    if (strncmp(ievads, "VARDS ", 6) == 0) {
      char pers_vards[MAX_GARUMS];
      sscanf(ievads + 6, "%s", pers_vards);
      jauns_cilveks = pievienot(pers_vards);
    } 
    else if (strncmp(ievads, "MATE ", 5) == 0) {
      char pers_mate[MAX_GARUMS];
      sscanf(ievads + 5, "%s", pers_mate);
      sv_kopet(pers_mate,jauns_cilveks->pers_mate);
      // TODO: izveidot jaunu vai labot esošo funkciju pievienojot cilveku, kuram pieivenot radinieku - jauns_cilveks
      pievienot(pers_mate);
    }
    else if (strncmp(ievads, "TEVS ", 5) == 0) {
      char pers_tevs[MAX_GARUMS];
      sscanf(ievads + 5, "%s", pers_tevs);
      sv_kopet(pers_tevs,jauns_cilveks->pers_tevs);
      pievienot(pers_tevs);
    }
  }
}

void izdrukat(Cilveks** cilveki, int skaits) {
  for (int i = skaits - 1; i >= 0 ; i--) {
    printf("%s\n", cilveki[i]->pers_vards);
  }
}

int main() {
  ielasit();

  // TODO: salabot izvadi atbilstoši algoritmam.
  izdrukat(cilveki, skaits);

  return 0;
}