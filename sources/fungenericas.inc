
// FUNCION: BLOQUEA HASTA QUE SE PULSA TECLA
// ENTRADAS: -
// SALIDAS: -
void PulsaTecla () {
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



// FUNCION: CREA UNA PAUSA
// ENTRADAS:
// cicles: ENTERO CON UN CONTADOR PARA ESPERAR
// SALIDAS: -
void Espera (unsigned int ciclos) {
  unsigned int contador;
  for(contador = ciclos; contador--; ) {};
} // FIN Espera



// FUNCION: INDICA SI HAY UNA COLISIÓN DE SPRITES
// ENTRADAS: 
// SALIDAS: 1 - SI DOS SPRITES SOLISIONAN EN PANTALLA O 0 EN CASO CONTRARIO
char IsSpriteCollision(void)
{
  return((VDPstatus(0) >> 5)  & 0x01);
} // FIN IsSpriteCollision



// FUNCION: DEVUELVE UN NUMERO ALEATORIO ENTRE A Y B-1 (EN PRINCIPIO CHAR CON LO QUE SE TRABAJA COMO MUCHO A 255)
// ENTRADAS:
// a: NÚMERO MINIMO
// b: NÚMERO MAXIMO
// SALIDAS: -
BYTE RandomNumber (BYTE a, BYTE b) {
    return (BYTE)(rand () % (b-a+1) + a);
} // FIN RandomNumber


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
//          ESTA LECTURA ES MÁS RÁPIDA QUE ABRIR Y CERRAR 256 VECES UN FICHERO
// ENTRADAS:
// a: p_fcb // NOMBRE DEL FICHERO A CARGAR
// b: p_name // DIRECCIÓN INICIO A CARGAR EN VRAM 0 - PATRONES / 8192 - COLORES
// SALIDAS:
void CargaFicheroVRAM (char *nombreFichero, int dirInicio) {
  int fila, contador;
    unsigned char bufferFichero[256]; // TODOS LOS TILES DE UNA FILA. (32 tiles * 8 filas cada tile = 256 lecturas de unsigned int o char)

    FT_SetName( &file, nombreFichero ); // INDICAR NOMBRE FICHERO A ESTRUCTURA FICHERO
  fcb_open( &file ); // ABRE FICHERO
  VpokeFirst(dirInicio); // INIDICAR POSICIÓN DE INICIO A ESCRIBIR EN VRAM
  for(fila = 0; fila < 24; fila ++) { // RECORRER LAS 24 FILAS DE LA PANTALLA
    fcb_read( &file, bufferFichero, 256 );

      for (contador = 0; contador < 256 ; contador++) 
        VpokeNext(bufferFichero[contador]);
  }
  fcb_close( &file ); // CIERRA FICHERO
} // CargaFicheroVRAM







// FUNCION: CARGA LOS DATOS DE UNA MELODÍA EN EL PLAYER
// ENTRADAS: 
// nCancion: NUMERO DE CANCIÓN EN EL ARRAY DE MELODÍAS
// datosFichero: ARRAY CON LOS DATOS DE LA MELODÍA
// tamano: NUMERO DE BYTES DEL ARRAY CON LA MELODÍA
// modo: SI 0 LA MELODÍA SUENA EN LOOP
// SALIDAS: -
void PT3PreparaPlayer (char* nCancion, BYTE* datosFichero, int tamano, BYTE modo) {
  FT_LoadData(nCancion, datosFichero, tamano, 0); // CARGA LOS DATOS DEL FICHERO DE MÚSICA INDICADO (file[music]) Y SU TAMAÑO
  PT3Init (datosFichero + 99, modo); // INICIALIZA EL PLAYER INDICANDO: DONDE ESTÁ EL FICHERO EN MEMORIA Y 0 QUE FUNCIONE EN MODO LOOP
} // FIN PreparaPlayer


// FUNCION: PARA QUE SIGA SONANDO LO ÚLTIMO QUE EL PLAYER ENVIÓ AL PSG
// ENTRADAS: -
// SALIDAS: -
void PT3ParaPlayer (void) {
  // PARA EL PLAYER DE MÚSICA
    EnableInterupt(); 
    InitPSG();
} // FIN ParaPlayer


/* ---------------------------------
          FT_LoadData
  Load Data to a specific pointer
  size is the size of data to read
  skip represent the number of Bytes
  you want to skip from the begining of the file
  Example: skip=7 to skip 7 bytes of a MSX bin
-----------------------------------*/ 
int FT_LoadData(char *file_name, char *buffer, int size, int skip) {
    FT_SetName( &file, file_name );
    if(fcb_open( &file ) != FCB_SUCCESS) {
          FT_errorHandler(1, file_name);
          return (0);
    }

    if (skip>0) {
        fcb_read( &file, buffer, skip );
    }
   
    fcb_read( &file, buffer, size );
    
    if( fcb_close( &file ) != FCB_SUCCESS ) {
      FT_errorHandler(2, file_name);
      return (0);
    }

    return(0);
}// FIN FT_LoadData



/* ---------------------------------
            FT_errorHandler

          In case of Error
-----------------------------------*/ 
void FT_errorHandler(char n, char *name) {
    InitPSG();
    Screen(0);
    SetColors(15,6,6);
    switch (n) {
      case 1:
            printf("\n\rFAILED: fcb_open(): %s ",name);
          break;

        case 2:
          printf("\n\rFAILED: fcb_close(): %s",name);
          break;  

        case 3:
          printf("\n\rSORRY: this game does not work on %s",name);
          break; 
    }
  Exit(0);
}