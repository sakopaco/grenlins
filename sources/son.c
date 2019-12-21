#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_graph2.h"
#include "fusion-c/header/vars_msxSystem.h"
#include "fusion-c/header/pt3replayer.h"

#define SONG_BUFFER 9000


void FT_SetName( FCB *p_fcb, const char *p_name );
void FT_errorHandler(char n, char *name);
int FT_LoadData(char *file_name, char *buffer, int size, int skip);



static FCB file; 


// dudas PT3Init, PT3Rout, PT3Play


void main (void) {
	char music;
unsigned char song[SONG_BUFFER];  
	char *file[1]={"Music1.pt3"}; // lista de ficheros
	int size[1]={8417}; // tamaño de ficheros

	music=0; // equivale a fichero 0

	InitPSG(); // para musica
  
	FT_LoadData(file[music], song, size[music], 0);
	PT3Init (song+99, 0);
	Cls();

//while(1){
    Halt();
    DisableInterupt();
    PT3Rout();
    PT3Play();
    EnableInterupt();  
//}
  /*
	while (1) { // al pulsar escape termina la ejecución
		Halt();
		k=Inkey();
		if (k==27) {
			EnableInterupt(); 
			InitPSG();
			Screen(0);
			Exit(0);
		}

		DisableInterupt();
		PT3Rout();
		PT3Play();
		EnableInterupt();
	}
*/

}



/* ---------------------------------
          FT_LoadData
  Load Data to a specific pointer
  size is the size of data to read
  skip represent the number of Bytes
  you want to skip from the begining of the file
  Example: skip=7 to skip 7 bytes of a MSX bin
-----------------------------------*/ 
int FT_LoadData(char *file_name, char *buffer, int size, int skip)
{
    
    FT_SetName( &file, file_name );
    if(fcb_open( &file ) != FCB_SUCCESS) 
    {
          FT_errorHandler(1, file_name);
          return (0);
    }
    if (skip>0)
    {
        fcb_read( &file, buffer, skip );
    }
   
    
    fcb_read( &file, buffer, size );
    
    if( fcb_close( &file ) != FCB_SUCCESS ) 
    {
      FT_errorHandler(2, file_name);
      return (0);
    }
    return(0);
}



/* ---------------------------------
            FT_errorHandler

          In case of Error
-----------------------------------*/ 
void FT_errorHandler(char n, char *name)
{
  InitPSG();
  Screen(0);
  SetColors(15,6,6);
  switch (n)
  {
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

/* ---------------------------------
                FT_SetName

    Set the name of a file to load
                (MSX DOS)
-----------------------------------*/ 
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

