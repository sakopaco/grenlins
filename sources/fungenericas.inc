
// FUNCION: BLOQUEA HASTA QUE SE PULSA TECLA
// ENTRADAS: -
// SALIDAS: -
void PulsaEspacio () {
	KillKeyBuffer();
	do {
	} while (Inkey() == NOSEPULSATECLA);
	KillKeyBuffer();
} // FIN PulsaEspacio


// FUNCION: COPIA EL ENTERO PASADO POR VALOR EN LA CADENA PASADA POR REFERENCIA
// ENTRADAS:
// i: ENTERO A CONVERTIR
// b[]: CADENA QUE TENDRÁ EL VALOR DE i EN MODO TEXTO
// SALIDAS: -
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    int shifter = i;

    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    
    do { //Move to where representation ends
        ++p;
        shifter = shifter/10;
    } while(shifter);
    *p = '\0';
    do { //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    } while(i);
    return b;
} // FIN itoa



// FUNCION: Generates a pause in the execution of n interruptions.
// ENTRADAS:
// cicles: ESPERA PAL: 50=1second. ; NTSC: 60=1second. 
// SALIDAS: -
void WAIT(int cicles) {
  unsigned int i;

  for(i=0;i<cicles;i++) HALT;
  return;
} // FIN WAIT


// FUNCION: DEVUELVE UN NUMERO ALEATORIO ENTRE A Y B-1 (EN PRINCIPIO CHAR CON LO QUE SE TRABAJA COMO MUCHO A 255)
// ENTRADAS:
// a: NÚMERO MINIMO
// b: NÚMERO MAXIMO
// SALIDAS: -
BYTE FT_RandomNumber (BYTE a, BYTE b)
{
    return(rand()%(b-a)+a); // 
} // FIN FT_RandomNumber


// FUNCION: PONE UN NOMBRE EN LA ESTRUCTURA FCB PARA PODERLA USAR CON FICHEROS
// ENTRADAS:
// a: p_fcb // PUNTERO A ESTRUCTURA DE FICHERO FCB
// b: p_name // TEXTO CON EL NOMBRE DEL FICHERO
// SALIDAS:
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
} // FT_SetName


// FUNCION: CARGA LOS TRES TERCIOS DE PANTALLA EN VRAM BIEN SEAN PATRONES O COLORES
// ENTRADAS:
// a: p_fcb // NOMBRE DEL FICHERO A CARGAR
// b: p_name // DIRECCIÓN INICIO A CARGAR EN VRAM 0 - PATRONES / 8192 - COLORES
// SALIDAS:
void CargaFicheroVRAM (char *nombreFichero, int dirInicio) {
	int fila, contador;
	unsigned char bufferFichero[256]; // TODOS LOS TILES DE UNA FILA. NO SÉ DE DONDE SALE EL 256 >8( DEBERÍA SER 32 PERO SÓLO FUNCIONA CON 256 :_(

	FT_SetName( &file, nombreFichero ); // INDICAR NOMBRE FICHERO A ESTRUCTURA FICHERO
	fcb_open( &file ); // ABRE FICHERO
	VpokeFirst(dirInicio); // INIDICAR POSICIÓN DE INICIO A ESCRIBIR EN VRAM
	for(fila = 0; fila < 24; fila ++) { // RECORRER LAS 24 FILAS DE LA PANTALLA
		fcb_read( &file, bufferFichero, 256 );
		for (contador = 0; contador < 256 ; contador++) VpokeNext(bufferFichero[contador]);
	}
	fcb_close( &file ); // CIERRA FICHERO
} // CargaFicheroVRAM
