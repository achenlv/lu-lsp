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