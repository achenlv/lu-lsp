Praktiskais darbs #5

Realizēt programmu, kas beidz darbību, notiekot taimera signālam.

Darbības beigšanu realizēt caur longjmp uz main() funkciju, no kuras tad notiek return 0;.

Izmantojamās funkcijas:

* setjmp()
* longjmp()
* sigaction()  [piemērs](http://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html)

Par signāliem kārtīgi mācīsimies vēlāk! Neliels ieskats, kas var noderēt: [1](https://www.gnu.org/software/libc/manual/html_node/Handler-Returns.html#Handler-Returns)

Uzmanību, konkrētais praktiskais darbs nestrādās ar striktu c90/c99 standartu, jo tam nepieciešami POSIX signāli, kas standartā nav iekļauti. Šajā PD atļauts kompilēt ar jaunāko standartu, vai korektāk - izmantot kompilatora slēdzi "-D_POSIX_C_SOURCE"

Koda šablons:

```
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// TODO: jūsu kods te!

int main(void)
{
    // TODO: jūsu kods te!


    // tālāko kodu nemainīt!

    // taimeris uz 3 sekundeem
    alarm(3);

    // bezgalīgs cikls
    while (1);

    // atgriež kaut kādu nenulles kodu, lai kompilators nesūdzētos
    return -1;
}
```