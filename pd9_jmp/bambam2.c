#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Lēcienu buferis starp programmas daļām */
static jmp_buf jump_buffer;

static int signal_count = 0;


/* Signāla apstrādātāji */
void sigalrm_handler(int sig) {
  printf("\n3 sekundes pagājušas.\nSākam iznīcību.\n");
  longjmp(jump_buffer, 1); /* beidz bezgalīgo ciklu */
}
void sigint_handler(int sig) {
  signal_count++;
  printf("Ctrl+C nav palīdzējis.\n");
}

int main(void)
{
  struct sigaction sa_alrm, sa_int;

  /* Inicializē SIGALRM apstrādes struktūru */
  sa_alrm.sa_handler = sigalrm_handler; 
  sigemptyset(&sa_alrm.sa_mask);     
  sa_alrm.sa_flags = 0;

  /* Inicializē SIGINT apstrādes struktūru */
  sa_int.sa_handler = sigint_handler; 
  sigemptyset(&sa_int.sa_mask);     
  sa_int.sa_flags = 0;

  /* Reģistrējam apstrādātājus katram signālam */
  sigaction(SIGALRM, &sa_alrm, NULL);
  sigaction(SIGINT, &sa_int, NULL); 
  
  
  printf("Programmas sākums \n\n");

  if (setjmp(jump_buffer) == 0) {
    printf("Gaidam 3 sekundes līdz iznīcībai.\n");
  } 
  else {
    printf("Iznīcība veiksmīgi sasniegta.\n");
    if (signal_count > 0) {
       printf("Iznīcību gaidot Ctrl+C nepalīdzēja %d %s.\n", signal_count, signal_count == 1 ? "reizi" : "reizes");  
    }
    return 0; 
  }

    // taimeris uz 3 sekundeem
    alarm(3);

    // bezgalīgs cikls
    while (1);

    // atgriež kaut kādu nenulles kodu, lai kompilators nesūdzētos
    return -1;
}