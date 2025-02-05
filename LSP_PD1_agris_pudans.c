#include <stdio.h>
#include <stdlib.h> 

// 1. Atrod simbolu virknes garumu
int sv_garums(char *virkne) {
  int garums = 0;
  while (virkne[garums] != '\0') garums++;

  return garums;
}

// 2. Nokopē simbolu virkni bez papildu mainīgajiem
void sv_kopet(char *no, char *uz) {
  while ((*uz++ = *no++) != '\0');
  // while (*no != '\0') {
  //   *uz = *no;
  //   no++;
  //   uz++;
  // }
  // *uz = '\0';
}

// 3. Meklē apakšvirkni
int sv_meklet(char *kur, char *ko) {
  int i = 0;
  int j;
  
  if (*ko == '\0') return 0;
  
  while (kur[i] != '\0') {
    j = 0;
    while (ko[j] != '\0' 
        && kur[i + j] != '\0' 
        && kur[i + j] == ko[j]
      ) {
          j++;
    }

    if (ko[j] == '\0') return i;
    
    i++;
  }

  return -1;
}

// 4. Apgriež simbolu virkni
void sv_apgriezt(char *virkne) {
  char *beigas = virkne;
  char temp;

  while (*beigas != '\0') beigas++;
  beigas--;
  
  while (virkne < beigas) {
    temp = *virkne;
    *virkne = *beigas;
    *beigas = temp;
    virkne++;
    beigas--;
  }
}

// 5. Apgirež vārdus
void sv_vapgriezt(char *virkne) {
  char *vards_sakums = virkne;
  char *p = virkne;
  
  // Tagad apgriežam katru vārdu atsevišķi
  while (*p != '\0') {
    if (*p == ' ' || *(p + 1) == '\0') {
      char *vards_beigas;
      
      if (*p == ' ') {
        vards_beigas = p - 1;
      } else {
        vards_beigas = p;
      }
      
      // Apgriežam individuālo vārdu
      while (vards_sakums < vards_beigas) {
        char temp = *vards_sakums;
        *vards_sakums = *vards_beigas;
        *vards_beigas = temp;
        vards_sakums++;
        vards_beigas--;
      }
      
      vards_sakums = p + 1;
    }
    p++;
  }
}


int main() {
  printf("LSP 1. praktiskais darbs\n");
  printf("Agris Pudāns\n");
  printf("St. apl. nr. ap08426\n");

  printf("\n--- TESTA PIEMĒRI ---\n");
  printf("-------------------------------\n");
  char virkne[] = "Simbolu virknes testa piemers";

  printf("1. Atrod simbolu virknes garumu\n");
  printf("Virknes '%s' garums: %d\n", virkne, sv_garums(virkne));

  printf("-------------------------------\n");
  printf("2. Nokopē simbolu virkni bez papildu mainīgajiem\n");
  char *kopija = (char *)malloc((sv_garums(virkne) + 1) * sizeof(char));
  sv_kopet(virkne, kopija);
  printf("Oriģināls: '%s' \nKopija: '%s'\n", virkne, kopija);
  free(kopija);

  printf("-------------------------------\n");
  printf("3. Meklē apakšvirkni\n");
  char meklet[] = "test";
  printf("Tekstā '%s' meklējam '%s'\n", virkne, meklet);
  printf("Atrašanās pozīcija: %d\n", sv_meklet(virkne, meklet));

  printf("-------------------------------\n");
  printf("4. Apgriež simbolu virkni\n");
  printf("Pirms apgriešanas: '%s'\n", virkne);
  sv_apgriezt(virkne);
  printf("Pēc apgriešanas: '%s'\n", virkne);

  printf("-------------------------------\n");
  printf("5. Apgirež vārdus\n");
  
  virkne[] = "Simbolu virknes testa piemers";
  sv_vapgriezt(virkne);
  printf("Pēc apgriešanas: '%s'\n", virkne);


  printf ("-------------------------------\n");
  return 0;
}