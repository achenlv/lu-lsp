#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GARUMS 64

typedef struct Cilveks {
    char vards[MAX_GARUMS];
    struct Cilveks* mate;
    struct Cilveks* tevs;
    struct Cilveks* next;
    int paaudze;
    int apmeklets;
} Cilveks;

Cilveks* sakums = NULL;

// Funkcija jauna cilvēka izveidošanai
Cilveks* jauns_cilveks(const char* vards) {
    Cilveks* c = (Cilveks*)malloc(sizeof(Cilveks));
    if (c == NULL) {
        fprintf(stderr, "Kļūda: Nevar atvēlēt atmiņu\n");
        exit(1);
    }
    strcpy(c->vards, vards);
    c->mate = NULL;
    c->tevs = NULL;
    c->next = NULL;
    c->paaudze = 0;
    c->apmeklets = 0;
    return c;
}

// Meklēt cilvēku pēc vārda
Cilveks* atrast_cilveku(const char* vards) {
    Cilveks* temp = sakums;
    while (temp != NULL) {
        if (strcmp(temp->vards, vards) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Pievienot cilvēku sarakstam
void pievienot_sarakstam(Cilveks* cilveks) {
    if (sakums == NULL) {
        sakums = cilveks;
    } else {
        cilveks->next = sakums;
        sakums = cilveks;
    }
}

// Pārbaudīt ciklus, izmantojot rekursīvu DFS
int meklet_ciklus(Cilveks* cilveks, Cilveks** cela, int garums) {
    if (cilveks == NULL) return 0;
    
    // Pārbauda vai cilvēks jau ir ceļā
    for (int i = 0; i < garums; i++) {
        if (cela[i] == cilveks) return 1;
    }
    
    cela[garums] = cilveks;
    
    // Pārbauda ceļu caur māti
    if (cilveks->mate && meklet_ciklus(cilveks->mate, cela, garums + 1)) return 1;
    
    // Pārbauda ceļu caur tēvu
    if (cilveks->tevs && meklet_ciklus(cilveks->tevs, cela, garums + 1)) return 1;
    
    return 0;
}

// Pārbaudīt ciklu esamību visam kokam
int ir_cikli() {
    int cilveku_skaits = 0;
    Cilveks* temp = sakums;
    while (temp != NULL) {
        cilveku_skaits++;
        temp = temp->next;
    }
    
    #ifdef DEBUG
        printf("Sāku ciklu pārbaudi. Cilvēku skaits: %d\n", cilveku_skaits);
    #endif
    
    Cilveks** cels = malloc(cilveku_skaits * sizeof(Cilveks*));
    temp = sakums;
    while (temp != NULL) {
        if (meklet_ciklus(temp, cels, 0)) {
            free(cels);
            return 1;
        }
        temp = temp->next;
    }
    free(cels);
    return 0;
}

// Pārbaudīt vai cilvēkam nav vairāku māšu/tēvu
int ir_dubulti_vecaki() {
    Cilveks* berns = sakums;
    while (berns != NULL) {
        int masu_skaits = 0;
        int tevu_skaits = 0;
        
        Cilveks* potencialais_vecaks = sakums;
        while (potencialais_vecaks != NULL) {
            if (potencialais_vecaks->mate == berns) {
                masu_skaits++;
            }
            if (potencialais_vecaks->tevs == berns) {
                tevu_skaits++;
            }
            if (masu_skaits > 1 || tevu_skaits > 1) {
                return 1;
            }
            potencialais_vecaks = potencialais_vecaks->next;
        }
        berns = berns->next;
    }
    return 0;
}

// Aprēķināt paaudzes
void aprekinat_paaudzes(Cilveks* cilveks, int paaudze) {
    if (cilveks == NULL || cilveks->apmeklets) return;
    
    cilveks->apmeklets = 1;
    cilveks->paaudze = paaudze;
    
    // Rekursīvi aprēķina bērnu paaudzes
    Cilveks* temp = sakums;
    while (temp != NULL) {
        if ((temp->mate == cilveks || temp->tevs == cilveks) && !temp->apmeklets) {
            aprekinat_paaudzes(temp, paaudze - 1);
        }
        temp = temp->next;
    }
}

// Atrast cilvēkus bez vecākiem (saknes)
void atrast_un_aprekinat_saknes() {
    #ifdef DEBUG
        printf("Meklēju saknes un aprēķinu paaudzes...\n");
    #endif
    
    // Atjaunojam apmeklējuma statusu
    Cilveks* temp = sakums;
    while (temp != NULL) {
        temp->apmeklets = 0;
        temp = temp->next;
    }
    
    // Atrodam saknes un aprēķinam paaudzes
    temp = sakums;
    while (temp != NULL) {
        if (temp->mate == NULL && temp->tevs == NULL) {
            #ifdef DEBUG
                printf("Atrasta sakne: %s\n", temp->vards);
            #endif
            aprekinat_paaudzes(temp, 1000);
        }
        temp = temp->next;
    }
}

// Kārtot masīvu pēc paaudzēm
void kartot_paaudzes(Cilveks** mas, int garums) {
    for (int i = 0; i < garums - 1; i++) {
        for (int j = 0; j < garums - i - 1; j++) {
            if (mas[j]->paaudze < mas[j + 1]->paaudze) {
                Cilveks* temp = mas[j];
                mas[j] = mas[j + 1];
                mas[j + 1] = temp;
            }
        }
    }
}

int main() {
    #ifdef DEBUG
        freopen("in.txt", "r", stdin);
        printf("Debug režīms: Lasu datus no in.txt\n");
    #endif

    char rinda[MAX_GARUMS];
    Cilveks* tekosais = NULL;
    int cilveku_skaits = 0;
    
    // Ielasīt datus
    while (fgets(rinda, sizeof(rinda), stdin)) {
        #ifdef DEBUG
            printf("Lasīta rinda: %s", rinda);
        #endif

        if (rinda[0] == '\n') continue;
        
        char vards[MAX_GARUMS];
        if (strncmp(rinda, "VARDS ", 6) == 0) {
            sscanf(rinda + 6, "%s", vards);
            #ifdef DEBUG
                printf("Apstrādāju VARDS: %s\n", vards);
            #endif
            
            tekosais = atrast_cilveku(vards);
            if (tekosais == NULL) {
                tekosais = jauns_cilveks(vards);
                pievienot_sarakstam(tekosais);
                cilveku_skaits++;
            }
        }
        else if (strncmp(rinda, "MATE ", 5) == 0 && tekosais != NULL) {
            sscanf(rinda + 5, "%s", vards);
            #ifdef DEBUG
                printf("Apstrādāju MATE: %s priekš %s\n", vards, tekosais->vards);
            #endif
            
            Cilveks* mate = atrast_cilveku(vards);
            if (mate == NULL) {
                mate = jauns_cilveks(vards);
                pievienot_sarakstam(mate);
                cilveku_skaits++;
            }
            tekosais->mate = mate;
        }
        else if (strncmp(rinda, "TEVS ", 5) == 0 && tekosais != NULL) {
            sscanf(rinda + 5, "%s", vards);
            #ifdef DEBUG
                printf("Apstrādāju TEVS: %s priekš %s\n", vards, tekosais->vards);
            #endif
            
            Cilveks* tevs = atrast_cilveku(vards);
            if (tevs == NULL) {
                tevs = jauns_cilveks(vards);
                pievienot_sarakstam(tevs);
                cilveku_skaits++;
            }
            tekosais->tevs = tevs;
        }
    }

    #ifdef DEBUG
        printf("\nKopējais cilvēku skaits: %d\n", cilveku_skaits);
    #endif
    
    // Pārbauda kļūdas
    if (ir_cikli()) {
        fprintf(stderr, "Kļūda: Atrasts cikls ģimenes kokā\n");
        return 1;
    }
    
    if (ir_dubulti_vecaki()) {
        fprintf(stderr, "Kļūda: Atrasts cilvēks ar vairākām mātēm vai tēviem\n");
        return 1;
    }
    
    // Aprēķina paaudzes
    atrast_un_aprekinat_saknes();
    
    // Sagatavo masīvu kārtošanai
    Cilveks** kartosanai = malloc(cilveku_skaits * sizeof(Cilveks*));
    Cilveks* temp = sakums;
    int indekss = 0;
    while (temp != NULL) {
        kartosanai[indekss++] = temp;
        temp = temp->next;
    }
    
    // Kārto un izvada rezultātu
    kartot_paaudzes(kartosanai, cilveku_skaits);
    
    #ifdef DEBUG
        printf("\nSakārtots rezultāts:\n");
    #endif
    
    for (int i = 0; i < cilveku_skaits; i++) {
        printf("%s\n", kartosanai[i]->vards);
    }
    
    // Atbrīvo atmiņu
    free(kartosanai);
    while (sakums != NULL) {
        Cilveks* dzest = sakums;
        sakums = sakums->next;
        free(dzest);
    }
    
    return 0;
}