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
  int garums = sv_garums(virkne);
  if (garums <= 1) return;  // Pārbaudam īsos gadījumus
  
  // Atrodam pirmo atstarpi
  char *pirmais_vards = virkne;
  char *p = virkne;
  while (*p != ' ' && *p != '\0') p++;
  
  // Ja nav atstarpes, nav ko mainīt
  if (*p == '\0') return;
  
  // Saglabājam pirmā vārda garumu
  int pirma_varda_garums = p - pirmais_vards;
  
  // Saglabājam pirmā vārda saturu pagaidu buferī
  char temp;
  int i;
  
  // Pārvietojam atlikušo tekstu pa kreisi
  char *src = p + 1;  // Izlaižam atstarpi
  char *dst = virkne;
  while (*src != '\0') {
    *dst++ = *src++;
  }
  *dst = '\0';
  
  // Pievienojam atstarpi
  *dst++ = ' ';
  
  // Kopējam pirmo vārdu beigās
  src = pirmais_vards;
  for (i = 0; i < pirma_varda_garums; i++) {
    *dst++ = src[i];
  }
  *dst = '\0';
}


int main(void) {
    char buferis[100];

    printf("Tests uzdevumam PD1.1:\n");
    printf("%d\n", sv_garums("hello world"));
    printf("%d\n", sv_garums("123"));
    printf("%d\n", sv_garums(""));

    printf("Tests uzdevumam PD1.2:\n");
    sv_kopet("hello world", buferis);
    printf("%s\n", buferis);
    sv_kopet("123", buferis);
    printf("%s\n", buferis);
    sv_kopet("", buferis);
    printf("%s\n", buferis);

    printf("Tests uzdevumam PD1.3:\n");
    printf("%d\n", sv_meklet("hello world", "wo"));
    printf("%d\n", sv_meklet("hello world", "123"));
    printf("%d\n", sv_meklet("hello world", ""));

    printf("Tests uzdevumam PD1.4:\n");
    sv_kopet("hello world", buferis);
    sv_apgriezt(buferis);
    printf("%s\n", buferis);
    sv_kopet("123", buferis);
    sv_apgriezt(buferis);
    printf("%s\n", buferis);

    return 0;
}