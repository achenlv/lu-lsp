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
  
  while (*p != '\0') {
    if (*p == ' ' || *(p + 1) == '\0') {
      char *vards_beigas;
      
      if (*p == ' ') {
        vards_beigas = p - 1;
      }
      else {
        vards_beigas = p;
      }
      
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


int main(void) {
    char buferis[100];

    printf("Tests uzdevumam PD1.1:\n");
    printf("%d\n", sv_garums("Testa virkne"));
    printf("%d\n", sv_garums("3 vardi nav"));
    printf("%d\n", sv_garums(""));

    printf("Tests uzdevumam PD1.2:\n");
    sv_kopet("Testa virkne", buferis);
    printf("%s\n", buferis);
    sv_kopet("3 vardi nav", buferis);
    printf("%s\n", buferis);
    sv_kopet("", buferis);
    printf("%s\n", buferis);

    printf("Tests uzdevumam PD1.3:\n");
    printf("%d\n", sv_meklet("Testa virkne", "virkne"));
    printf("%d\n", sv_meklet("Testa virkne", "nav"));
    printf("%d\n", sv_meklet("Testa virkne", ""));

    printf("Tests uzdevumam PD1.4:\n");
    sv_kopet("Testa virkne", buferis);
    sv_apgriezt(buferis);
    printf("%s\n", buferis);
    sv_kopet("3 vardi nav", buferis);
    sv_apgriezt(buferis);
    printf("%s\n", buferis);

    printf("Tests uzdevumam PD1.5:\n");
    sv_kopet("Testa virkne", buferis);
    sv_vapgriezt(buferis);
    printf("%s\n", buferis);
    sv_kopet("Loti gara daudziem vardiem testa virkne", buferis);
    sv_vapgriezt(buferis);
    printf("%s\n", buferis);

    return 0;
}