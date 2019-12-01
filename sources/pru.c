#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_graph1.h"
#include "fusion-c/header/vdp_sprites.h"
#include "fusion-c/header/vdp_circle.h"

typedef unsigned char BYTE;

BYTE RandomNumber (BYTE a, BYTE b);

void main(void) 
{
	int contador;
	int resultado;


	Cls();

	for(contador=0; contador<100; contador++) {
		resultado = FT_RandomNumber(0,100);
		if (resultado < 50)
			printf("%d, ", resultado);
	}
	printf("lala3\n");

}

BYTE RandomNumber (BYTE a, BYTE b) {


return (BYTE)(rand () % (b-a+1) + a);

    //return (BYTE)(rand()%(b-a)+a); // 
} // FIN FT_RandomNumber