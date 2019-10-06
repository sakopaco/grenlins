#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_graph2.h"
#include "fusion-c/header/vars_msxSystem.h"
#include "fusion-c/header/pt3replayer.h"

//posiciones memoria para tiles SCREEN 2 (32 ancho x 24 alto)
#define TPB0 0
#define TPB1 2048
#define TPB2 4096
#define MAPT1 6144
#define MAPT2 6400
#define MAPT3 6656
#define SPRITEATR 6912
#define TCB0 8192
#define TCB1 10240
#define TCB2 12288
#define SPRITEPAT 14336

void FT_SetName( FCB *p_fcb, const char *p_name );
void CargaFicheroVRAM (char *nombreFichero, int dirInicio);

static FCB file; 

void main() {
	Screen(2);
	SetColors(15,4,4);

	CargaFicheroVRAM ("ST1SC2.CHR", TPB0);
	CargaFicheroVRAM ("ST1SC2.CLR", TCB0);
} /////////////////////////

void FT_SetName( FCB *p_fcb, const char *p_name ) {
  char i, j;
  memset( p_fcb, 0, sizeof(FCB) );
  for( i = 0; i < 11; i++ ) {
    p_fcb->name[i] = ' ';
  }
  for( i = 0; (i < 8) && (p_name[i] != 0) && (p_name[i] != '.'); i++ ) {
    p_fcb->name[i] =  p_name[i];
  }
  if( p_name[i] == '.' ) {
    i++;
    for( j = 0; (j < 3) && (p_name[i + j] != 0) && (p_name[i + j] != '.'); j++ ) {
      p_fcb->ext[j] =  p_name[i + j] ;
    }
  }
}

void CargaFicheroVRAM (char *nombreFichero, int dirInicio) {
	int fila, contador;
	unsigned char bufferFichero[256];

	FT_SetName( &file, nombreFichero );
	fcb_open( &file );
	VpokeFirst(dirInicio);
	for(fila = 0; fila < 24; fila ++) {
		fcb_read( &file, bufferFichero, 256 );
		for (contador = 0; contador < 256 ; contador++) VpokeNext(bufferFichero[contador]);
	}
	fcb_close( &file );
}