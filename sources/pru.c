#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_graph1.h"
#include "fusion-c/header/vdp_sprites.h"
#include "fusion-c/header/vdp_circle.h"

typedef unsigned char BYTE;

BYTE FT_RandomNumber (BYTE a, BYTE b);

void main(void) 
{
	int contador;
	BYTE resultado;


	Cls();

	for(contador=0; contador<10; contador++) {
		resultado = FT_RandomNumber((BYTE)0,(BYTE)2);
		printf("%d\n", resultado);
	}
	printf("lalalalalal\n");

}

BYTE FT_RandomNumber (BYTE a, BYTE b) {
    return (BYTE)(rand()%(b-a)+a); // 
} // FIN FT_RandomNumber