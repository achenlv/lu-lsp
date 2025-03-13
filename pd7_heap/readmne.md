# LU-LSP-b:L07


## Uzdevums

Realizēt vienkāršu dinamiskās atmiņas izdalīšanas risinājumu.

## Jārealizē:

```
void * myalloc(size_t size);
int myfree(void * ptr);
```

Dots statisks atmiņas buferis ierobežotai atmiņai, ko var izmantot myalloc():

```
#define MY_BUFFER_SIZE 4096

unsigned char mybuffer[MY_BUFFER_SIZE];
```

Kļūdas vai nepietiekamas atmiņas gadījumā:
* myalloc() atgriež NULL, 
* myfree() atgriež (-1);

Izmantot `NextFit` algoritmu brīvo atmiņas bloku meklēšanai un piešķiršanai.

Testēt dažādus scenārijus un novērtēt veiktspēju. Komentēt ari kādu metriku jūs izvēlaties veiktspējas novērtējumam, un kādus testus.

Risinājumu iesūtīt e-studijās. 