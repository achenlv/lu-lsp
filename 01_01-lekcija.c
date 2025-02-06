#include <stdio.h>
// v1
//#define max(x,y) x > y ? x : y 
// v2, liai izvairītos no a >> b ? a: b
#define max(x,y) (x) > (y) ? (x) : (y) 

//v1
// #define inc(x) (x) = (x) + 1
//v2
#define inc(x) (x) = (x) + 1; printf('Done');


typedef struct {
  int x;
  int y;
  int z;
} vector_t; // Nav labais tonis _t.
// izsauc ar:
// vector_t v1,v2;






int main() {
  int a = 1;
  int b, c;
  b=c=0;
  //v1
  // printf("%d\n", inc(a));
  //v2
  // printf("%d\n%s\n", inc(a));
  // inc(a);
  if (a > b) {
    inc(a);
  }
  return 0;
}


int main_max() {
  int a,b,x;

  b=3;

	a = max('a',b);
	printf("%d\n", a);



  x = a > b ? a: b;
  printf("%d\n", x);



  printf("%d\n", max(a+b, x));

  // printf("%d\n", max(fun(a), b));

	return 0;
}




// GOOGLE: The lost art of structure packing. (piemēram, par to pašu struct faila sākumā)
// http://www.catb.org/esr/structure-packing/
// GOOGLE: Raytracing visit-card Pixar 
// https://fabiensanglard.net/postcard_pathtracer/