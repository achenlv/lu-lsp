#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// Buffers "lēcieniem" starp programmas daļām
static jmp_buf jump_buffer;

// Mainīgais, kas skaita, cik reizes saņemam signālu
static int signal_count = 0;

// Krāsu kodi terminālam
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

// Funkcija animācijas attēlošanai
void show_animation(int seconds) {
    char symbols[] = {'|', '/', '-', '\\'};
    int i;
    time_t start_time = time(NULL);
    
    printf(CYAN "\nGaidam %d sekundes: " RESET, seconds);
    fflush(stdout);
    
    while (difftime(time(NULL), start_time) < seconds) {
        for (i = 0; i < 4; i++) {
            printf(YELLOW "%c\b" RESET, symbols[i]);
            fflush(stdout);
            usleep(100000); // 0.1 sekundes
        }
    }
    printf("\n");
}

// Signāla apstrādātājs - tiek izsaukts, kad saņemam signālu
void signal_handler(int sig) {
    signal_count++;
    
    if (sig == SIGALRM) {
        printf(RED "\n🔔 SIGNĀLS: Saņemts SIGALRM (taimera signāls) #%d!\n" RESET, signal_count);
    } else if (sig == SIGINT) {
        printf(RED "\n⚠️ SIGNĀLS: Saņemts SIGINT (Ctrl+C) #%d!\n" RESET, signal_count);
    }
    
    // Ja saņemam signālu 3 reizes, beidzam programmu
    if (signal_count >= 3) {
        printf(RED "Saņemti %d signāli. Beidzam programmu!\n" RESET, signal_count);
        longjmp(jump_buffer, signal_count);
    } else {
        // Citādi turpinām gaidīt
        printf(YELLOW "Vēl varam saņemt %d signālus pirms beigām.\n" RESET, 3 - signal_count);
        alarm(2); // Iestatām jaunu taimeri
    }
}

int main(void) {
    struct sigaction sa;
    int jump_result;
    
    // Iniciējam struktūru signālu apstrādei
    sa.sa_handler = signal_handler; 
    sigemptyset(&sa.sa_mask);     
    sa.sa_flags = 0;
    
    // Reģistrējam apstrādātāju diviem signāliem
    sigaction(SIGALRM, &sa, NULL);  // Taimera signāls
    sigaction(SIGINT, &sa, NULL);   // Ctrl+C signāls
    
    printf(GREEN "🚀 Signālu demonstrācijas programma 🚀\n" RESET);
    printf("----------------------------------------\n");
    
    // Saglabājam pašreizējo izpildes stāvokli, lai vēlāk varētu atgriezties
    jump_result = setjmp(jump_buffer);
    
    // Pārbaudam, vai esam tikko uzsākuši vai atgriezušies ar longjmp
    if (jump_result == 0) {
        printf(BLUE "📝 IZPILDE: Programmas stāvoklis saglabāts (setjmp atgriež %d)\n" RESET, jump_result);
        printf(BLUE "📝 IZPILDE: Iestatām taimeri un gaidām signālus...\n" RESET);
        printf(MAGENTA "❗ INFO: Vari nospiest Ctrl+C, lai sūtītu SIGINT signālu manuāli!\n" RESET);
        
        // Iestatām taimeri uz 3 sekundēm
        alarm(3);
        
        // Rādām animāciju, kamēr gaidām
        while (1) {
            show_animation(1);
            printf(CYAN "⏳ GAIDA: Programma joprojām gaida signālus... (#%d)\n" RESET, signal_count);
        }
    } else {
        // Šeit nonākam, kad longjmp ir pārtraucis galveno ciklu
        printf(GREEN "🏁 BEIGAS: Programma beidzas pēc %d signāliem!\n" RESET, jump_result);
        printf("----------------------------------------\n");
        printf(YELLOW "Paldies par programmas izmēģināšanu! 👋\n" RESET);
        return 0;
    }
    
    // Šis kods nekad netiek izpildīts
    return -1;
}