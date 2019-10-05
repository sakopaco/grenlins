#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/io.h"

// DEFINIR VARIABLES GLOBALES Y CONSTANTES
// CONSTANTES ENSAMBLADOR
#define HALT __asm halt __endasm   //wait for the next interrupt

// CONSTANTES GENERICAS
#define VERDADERO 1
#define FALSO 0

// CONTANTES MSX
#define NOSEPULSATECLA 0 // la funcion Inkey devuelve 0 si no se pulsa ninguna tecla
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
/* COLORES MSX
0  - 0000 TRANSPARENTE
1  - 0001 NEGRO
2  - 0010 VERDE
3  - 0011 VERDE CLARO
4  - 0100 AZUL OSCURO
5  - 0101 AZUL CLARO
6  - 0110 ROJO OSCURO
7  - 0111 CELESTE
8  - 1000 ROJO
9  - 1001 ROJO CLARO
10 - 1010 AMARILLO OSCURO
11 - 1011 AMARILLO CLARO
12 - 1100 VERDE OSCURO
13 - 1101 VIOLETA
14 - 1110 GRIS
15 - 1111 BLANCO
*/
#define COLORTRASPARENTE 0
#define COLORNEGRO 1
#define COLORVERDE 2
#define COLORVERDECLARO 2
#define COLORAZULOSCURO 4
#define COLORAZULCLARO 5
#define COLORROJOOSCURO 6
#define COLORCELESTE 7
#define COLORROJO 8
#define COLORROJOCLARO 9
#define COLORAMARILLOOSCURO 10
#define COLORAMARILLO 11
#define COLORVERDEOSCURO 12
#define COLORMAGENTA 13
#define COLORGRIS 14
#define COLORBLANCO 15
// COLORES TILES
#define TILENEGRONEGRO 17
#define TILENEGROBLANCO 240
#define TILENEGROGRIS 224


// CONSTANTES TILES
#define LETRA_SP 32
#define LETRA_GU 45
#define NUMER_0 48
#define NUMER_1 49
#define NUMER_2 50
#define NUMER_3 51
#define NUMER_4 52
#define NUMER_5 53
#define NUMER_6 54
#define NUMER_7 55
#define NUMER_8 56
#define NUMER_9 57
#define LETRA_DP 58
#define LETRA_A 65
#define LETRA_B 66
#define LETRA_C 67
#define LETRA_D 68
#define LETRA_E 69
#define LETRA_F 70
#define LETRA_G 71
#define LETRA_H 72
#define LETRA_I 73
#define LETRA_J 74
#define LETRA_K 75
#define LETRA_L 76
#define LETRA_M 77
#define LETRA_N 78
#define LETRA_O 79
#define LETRA_P 80
#define LETRA_Q 81
#define LETRA_R 82
#define LETRA_S 83
#define LETRA_T 84
#define LETRA_U 85
#define LETRA_V 86
#define LETRA_W 87
#define LETRA_X 88
#define LETRA_Y 89
#define LETRA_Z 90

// CONTANTES JUEGO
#define RECORD 500 // PUNTOS INICIALES DE RECORD
#define VIDAS 3 // VIDAS INICIALES DEL PROTA DEL JUEGO

#define FASE1 1
#define FASE2 2
#define FASE3 3
#define FASE4 4
#define FASE5 5
#define FASE6 6

#define ESPERAINTROFASE 10 //600 POR DEFECTO

// VARIABLES JUEGO
unsigned int record; // VALOR MÁXIMO DE PUNTOS ALCANZADOS DURANTE LA SESIÓN (SE INICIALIZA CON "RECORD")
unsigned int puntos; // PUNTOS ALCANZADOS DURANTE LA SESIÓN (SE INICIALIZA A 0)
char cadena[8]; // CADENA PARA PONER LOS PUNTOS / VIDAS / NIVEL Y LO QUE HAYA QUE CONVERTIR CON itoa
char fase; // CADA STAGE DEL JUEGO 1-> 6
char nivel; // EL NIVEL INCREMENTA LA DIFICULTAD DE LAS FASES Y SE INCREMENTA CUANDO SE SUPERAN TODAS LAS FASES. VA DE 1 EN ADELANTE

// VARIABLES DE FUNCIONES GENERICAS
static FCB file; // VARIABLE PARA LEER UN FICHERO


/*
// ESTRUCTURAS
struct escena // ESTRUCTURA PARA ALMACENAR LAS PANTALLAS DE CADA ESCENA
{
	char BP0[256]; // PATRONES
	char BP1[256];
	char BP2[256];
	char BC0[256]; // COLORES
	char BC1[256];
	char BC2[256];
} fondo;
*/

// SETUP / INICIALIZACIÓN GLOBAL
// FUNCIONES JUEGO
void PintarPantallaInicialJuego (void);
void PreparaTilesTexto 			(char tercio);
void LimpiaTilesTexto 			(char tercio);
void PonerColorTileLetra		(void);
void PonerTileLocate 			(unsigned int mapt, char fila, char col, char* texto);
void PintarIntroFase			(void);
void CargaFondoJuego			(void);

// FUNCIONES GENERICAS
void  PulsaEspacio 		(void);
char* itoa 				(int i, char b[]);
void  WAIT 				(int cicles);
char  FT_RandomNumber 	(char a, char b);
void  FT_SetName		( FCB *p_fcb, const char *p_name );

// INICIO PROGRAMA
void main(void) 
{
	// DEFINIR VARIABLES NO GLOBALES
	struct escena fondo; // ESTRUCTURA PARA ALMACENAR LAS PANTALLAS DE CADA ESCENA

	char finpartida; // INDICADOR VERDADERO 1 O FALSO 0 PARA VER SI NOS HAN MATADO TODAS LAS VIDAS
	char vidas; // VIDAS DEL PROTA DE LA PARTIDA

	// SETUP / INICIALIZACIÓN (INICIAL) ENTORNO Y VARIABLES DE JUEGO
	Screen(2);
	SetColors(COLORNEGRO,COLORNEGRO,COLORNEGRO);
	KeySound(FALSO);

	record = RECORD;
	puntos = 0;

	// CONFIGURA SPRITES
	SpriteReset();	
	Sprite16();
	SpriteDouble();	

	// CARGAR GRÁFICOS Y PATRONES COMUNES

	// LOOP JUEGO
	do { 
		// PINTAR PANTALLA INICIAL DE JUEGO
		PintarPantallaInicialJuego();

		// SELECCIONAR SI APLICA MODOS DE JUEGO
		// SETUP / INICIALIZACIÓN (NO INICIAL, DE CADA PARTIDA) VARIABLES DE JUEGO
		finpartida = FALSO;
		fase = 1; 
		nivel = 1; 
		vidas = VIDAS;

		// LOOP JUEGO FASE
		do {
			// PINTAR INTRO DE FASE
			PintarIntroFase();

			switch(fase) {
				case FASE1: {
					// SETUP / INICIALIZACIÓN VARIABLES DE FASE


					// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE FASE)
					CargaFondoJuego();
					do {} while(VERDADERO);

					// PINTAR PANTALLA DE FASE
					// PINTAR TEXTOS
					// PINTAR PUNTOS + RECORD????
					// PINTAR VIDAS ?????

					// PINTAR ENEMIGOS, SI APLICA
					// PINTAR ELEMENTOS SI APLICA
					// PINTAR ACCESORIOS, SIAPLICA

					//LOOP DENTRO FASE

						// OBTENER ENTRADAS DEL JUGADOR (MOVIMIENTOS, DISPAROS, ETC)
				
						// INICIALIZA/SPAWNEA ENEMIGOS				

						// DETECCIÓN DE LIMITES DE OBJETOS MÓVILES
					
						// DETECCIÓN DE COLISIONES
					
						// ACTUALIZA VARIABLES DE ANIMACIONES/ ACCESORIOS
					
						// DIBUJAR CAMBIOS EN PANTALLA
							// PINTAR/ACTUALIZAR ENEMIGOS, SI APLICA
							// PINTAR/ACTUALIZAR ELEMENTOS SI APLICA
							// PINTAR/ACTUALIZAR ACCESORIOS, SIAPLICA
							// ACTUALIZA ANIMACIONES/ ACCESORIOS

						// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES

					// FIN LOOP DENTRO FASE
					break;
				}

				case FASE2: {

			// SETUP / INICIALIZACIÓN VARIABLES DE FASE

			// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE FASE)

			// PINTAR PANTALLA DE FASE
			// PINTAR TEXTOS
			// PINTAR PUNTOS + RECORD????
			// PINTAR VIDAS ?????

			// PINTAR ENEMIGOS, SI APLICA
			// PINTAR ELEMENTOS SI APLICA
			// PINTAR ACCESORIOS, SIAPLICA

			//LOOP DENTRO FASE

				// OBTENER ENTRADAS DEL JUGADOR (MOVIMIENTOS, DISPAROS, ETC)
				
				// INICIALIZA/SPAWNEA ENEMIGOS				

				// DETECCIÓN DE LIMITES DE OBJETOS MÓVILES
					
				// DETECCIÓN DE COLISIONES
					
				// ACTUALIZA VARIABLES DE ANIMACIONES/ ACCESORIOS
					
				// DIBUJAR CAMBIOS EN PANTALLA
					// PINTAR/ACTUALIZAR ENEMIGOS, SI APLICA
					// PINTAR/ACTUALIZAR ELEMENTOS SI APLICA
					// PINTAR/ACTUALIZAR ACCESORIOS, SIAPLICA
					// ACTUALIZA ANIMACIONES/ ACCESORIOS

				// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES

			// FIN LOOP DENTRO FASE
					break;
				}

				case FASE3: {
			// SETUP / INICIALIZACIÓN VARIABLES DE FASE

			// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE FASE)

			// PINTAR PANTALLA DE FASE
			// PINTAR TEXTOS
			// PINTAR PUNTOS + RECORD????
			// PINTAR VIDAS ?????

			// PINTAR ENEMIGOS, SI APLICA
			// PINTAR ELEMENTOS SI APLICA
			// PINTAR ACCESORIOS, SIAPLICA

			//LOOP DENTRO FASE

				// OBTENER ENTRADAS DEL JUGADOR (MOVIMIENTOS, DISPAROS, ETC)
				
				// INICIALIZA/SPAWNEA ENEMIGOS				

				// DETECCIÓN DE LIMITES DE OBJETOS MÓVILES
					
				// DETECCIÓN DE COLISIONES
					
				// ACTUALIZA VARIABLES DE ANIMACIONES/ ACCESORIOS
					
				// DIBUJAR CAMBIOS EN PANTALLA
					// PINTAR/ACTUALIZAR ENEMIGOS, SI APLICA
					// PINTAR/ACTUALIZAR ELEMENTOS SI APLICA
					// PINTAR/ACTUALIZAR ACCESORIOS, SIAPLICA
					// ACTUALIZA ANIMACIONES/ ACCESORIOS

				// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES

			// FIN LOOP DENTRO FASE
					break;
				}

				case FASE4: {
			// SETUP / INICIALIZACIÓN VARIABLES DE FASE

			// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE FASE)

			// PINTAR PANTALLA DE FASE
			// PINTAR TEXTOS
			// PINTAR PUNTOS + RECORD????
			// PINTAR VIDAS ?????

			// PINTAR ENEMIGOS, SI APLICA
			// PINTAR ELEMENTOS SI APLICA
			// PINTAR ACCESORIOS, SIAPLICA

			//LOOP DENTRO FASE

				// OBTENER ENTRADAS DEL JUGADOR (MOVIMIENTOS, DISPAROS, ETC)
				
				// INICIALIZA/SPAWNEA ENEMIGOS				

				// DETECCIÓN DE LIMITES DE OBJETOS MÓVILES
					
				// DETECCIÓN DE COLISIONES
					
				// ACTUALIZA VARIABLES DE ANIMACIONES/ ACCESORIOS
					
				// DIBUJAR CAMBIOS EN PANTALLA
					// PINTAR/ACTUALIZAR ENEMIGOS, SI APLICA
					// PINTAR/ACTUALIZAR ELEMENTOS SI APLICA
					// PINTAR/ACTUALIZAR ACCESORIOS, SIAPLICA
					// ACTUALIZA ANIMACIONES/ ACCESORIOS

				// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES

			// FIN LOOP DENTRO FASE
					break;
				}

				case FASE5: {
			// SETUP / INICIALIZACIÓN VARIABLES DE FASE

			// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE FASE)

			// PINTAR PANTALLA DE FASE
			// PINTAR TEXTOS
			// PINTAR PUNTOS + RECORD????
			// PINTAR VIDAS ?????

			// PINTAR ENEMIGOS, SI APLICA
			// PINTAR ELEMENTOS SI APLICA
			// PINTAR ACCESORIOS, SIAPLICA

			//LOOP DENTRO FASE

				// OBTENER ENTRADAS DEL JUGADOR (MOVIMIENTOS, DISPAROS, ETC)
				
				// INICIALIZA/SPAWNEA ENEMIGOS				

				// DETECCIÓN DE LIMITES DE OBJETOS MÓVILES
					
				// DETECCIÓN DE COLISIONES
					
				// ACTUALIZA VARIABLES DE ANIMACIONES/ ACCESORIOS
					
				// DIBUJAR CAMBIOS EN PANTALLA
					// PINTAR/ACTUALIZAR ENEMIGOS, SI APLICA
					// PINTAR/ACTUALIZAR ELEMENTOS SI APLICA
					// PINTAR/ACTUALIZAR ACCESORIOS, SIAPLICA
					// ACTUALIZA ANIMACIONES/ ACCESORIOS

				// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES

			// FIN LOOP DENTRO FASE
					break;
				}

				case FASE6: {
					// SETUP / INICIALIZACIÓN VARIABLES DE FASE

					// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE FASE)

					// PINTAR PANTALLA DE FASE
					// PINTAR TEXTOS
					// PINTAR PUNTOS + RECORD????
					// PINTAR VIDAS ?????

					// PINTAR ENEMIGOS, SI APLICA
					// PINTAR ELEMENTOS SI APLICA
					// PINTAR ACCESORIOS, SIAPLICA

					//LOOP DENTRO FASE

						// OBTENER ENTRADAS DEL JUGADOR (MOVIMIENTOS, DISPAROS, ETC)
				
						// INICIALIZA/SPAWNEA ENEMIGOS				

						// DETECCIÓN DE LIMITES DE OBJETOS MÓVILES
					
						// DETECCIÓN DE COLISIONES
					
						// ACTUALIZA VARIABLES DE ANIMACIONES/ ACCESORIOS
					
						// DIBUJAR CAMBIOS EN PANTALLA
							// PINTAR/ACTUALIZAR ENEMIGOS, SI APLICA
							// PINTAR/ACTUALIZAR ELEMENTOS SI APLICA
							// PINTAR/ACTUALIZAR ACCESORIOS, SIAPLICA
							// ACTUALIZA ANIMACIONES/ ACCESORIOS

						// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES

					// FIN LOOP DENTRO FASE
					break;
				}
				default: {

				}
			}

			// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES
				// SI QUEDAN MÁS VIDAS INCREMENTA FASE
				// SI FASE > 5 NIVEL++ FASE = 1

		// FIN LOOP JUEGO FASE
		} while (!finpartida);
		
		// PINTAR PANTALLA/MENSAJE FIN PARTIDA
			// SI JUEGO DE LOOPS SÓLO GAME OVER
			// SI NO JUEGO DE LOOPS GAME OVER O FIN PARTIDA

	// FIN LOOP JUEGO
	} while (VERDADERO);

// FIN PROGRAMA
}


// IMPLEMENTACIÓN DE FUNCIONES
// FUNCION: PINTA PANTALLA CON RECORD, NOMBRE JUEGO, ETC... Y ESPERA A PULSAR TECLA PARA EMPEZAR A JUGAR
// ENTRADAS: -
// SALIDAS: -
void PintarPantallaInicialJuego () {
	char contador;

	HideDisplay(); // OCULTAMOS PORQUE AL PINTAR LOS TILES SE VEN EN PANTALLA
	// PREPARAMOS LOS TILES DE LETRAS EN CADA TERCIO DE PANTALLA Y SE LIMPIA PARA USO
	PreparaTilesTexto ((char)1);
	LimpiaTilesTexto ((char)1);
	PreparaTilesTexto ((char)2);
	LimpiaTilesTexto ((char)2);
	PreparaTilesTexto ((char)3);
	LimpiaTilesTexto ((char)3);
	
	PonerTileLocate (MAPT1, 3, 8, "RECORD:");
	itoa(record, cadena);
	PonerTileLocate (MAPT1, 3, 18, cadena);
	PonerTileLocate (MAPT1, 7, 8, "LOS GREMMINS");
	PonerTileLocate (MAPT2, 5, 5, "UN JUEGO DE PACOSOFT");
	PonerTileLocate (MAPT2, 7, 6, "-- PULSA ESPACIO --");
	ShowDisplay(); // MOSTRAMOS RESULTADO

	PulsaEspacio();

	for (contador = 0; contador < 20; contador++) {
		PonerTileLocate (MAPT2, 7, 6, "-- PULSA ESPACIO --");
		WAIT(8);
		PonerTileLocate (MAPT2, 7, 6, "                   ");
		WAIT(8);
	}
} // FIN PintarPantallaInicialJuego

// FUNCION: PONE LOS TILES DE LETRAS PARA ESCRIBIR EN PANTALLA
// ENTRADAS: EN QUE TERCIO PONE LOS TILES DE LAS LETRAS
// SALIDAS: -
void PreparaTilesTexto (char tercio) {
	int comienzo;
	int contadorPTT;

	if (tercio == 1) {
		comienzo = TPB0;
	} else {
		if (tercio == 2) {
			comienzo = TPB1;
		} else {
			comienzo = TPB2;
		}
	}

	// DEFINIMOS LOS TILES
	VpokeFirst(comienzo + 32 * 8);
	// CARACTER SP / 32
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
		
	VpokeFirst(comienzo + 45 * 8);
	// CARACTER - / 45
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	VpokeNext(0b01111111);
	VpokeNext(0b00111111);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);
	
	VpokeFirst(comienzo + 58 * 8);
	// CARACTER : / 58
	VpokeNext(0b00000000);
	VpokeNext(0b00011000);
	VpokeNext(0b00011000);
	VpokeNext(0b00000000);
	VpokeNext(0b00011000);
	VpokeNext(0b00011000);
	VpokeNext(0b00000000);
	VpokeNext(0b00000000);

	VpokeFirst(comienzo + 48 * 8);
	// CARACTER 0 / 48
	VpokeNext(0b01111111);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER 1 / 49
	VpokeNext(0b00111000);
	VpokeNext(0b00011000);
	VpokeNext(0b00011000);
	VpokeNext(0b00011000);
	VpokeNext(0b00111110);
	VpokeNext(0b00111110);
	VpokeNext(0b00111110);
	VpokeNext(0b00000000);
	
	// CARACTER 2 / 50
	VpokeNext(0b01111111);
	VpokeNext(0b00000011);
	VpokeNext(0b00000011);
	VpokeNext(0b01111111);
	VpokeNext(0b01100000);
	VpokeNext(0b01100000);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER 3 / 51
	VpokeNext(0b01111110);
	VpokeNext(0b00000110);
	VpokeNext(0b00000110);
	VpokeNext(0b01111111);
	VpokeNext(0b00000111);
	VpokeNext(0b00000111);
	VpokeNext(0b11111111);
	VpokeNext(0b00000000);

	// CARACTER 4 / 52
	VpokeNext(0b01110000);
	VpokeNext(0b01110000);
	VpokeNext(0b01110000);
	VpokeNext(0b01110111);
	VpokeNext(0b01110111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000111);
	VpokeNext(0b00000000);

	// CARACTER 5 / 53
	VpokeNext(0b01111111);
	VpokeNext(0b01100000);
	VpokeNext(0b01100000);
	VpokeNext(0b01111111);
	VpokeNext(0b00000111);
	VpokeNext(0b00000111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);

	// CARACTER 6 / 54
	VpokeNext(0b01111100);
	VpokeNext(0b01101100);
	VpokeNext(0b01100000);
	VpokeNext(0b01111111);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);

	// CARACTER 7 / 55
	VpokeNext(0b01111111);
	VpokeNext(0b00000011);
	VpokeNext(0b00000011);
	VpokeNext(0b00011111);
	VpokeNext(0b00011000);
	VpokeNext(0b00011000);
	VpokeNext(0b00011000);
	VpokeNext(0b00000000);

	// CARACTER 8 / 56
	VpokeNext(0b00111110);
	VpokeNext(0b00110110);
	VpokeNext(0b00110110);
	VpokeNext(0b01111111);
	VpokeNext(0b01110111);
	VpokeNext(0b01110111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);

	// CARACTER 9 / 57
	VpokeNext(0b01111111);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01111111);
	VpokeNext(0b00000111);
	VpokeNext(0b00000111);
	VpokeNext(0b00000111);
	VpokeNext(0b00000000);

	VpokeFirst(comienzo + 65 * 8);
	// CARACTER A / 65
	VpokeNext(0b00111111);
	VpokeNext(0b00110011);
	VpokeNext(0b00110011);
	VpokeNext(0b01111111);
	VpokeNext(0b01110011);
	VpokeNext(0b01110011);
	VpokeNext(0b01110011);
	VpokeNext(0b00000000);
	
	// CARACTER B / 66
	VpokeNext(0b01111110);
	VpokeNext(0b01100110);
	VpokeNext(0b01100110);
	VpokeNext(0b01111111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER C / 67
	VpokeNext(0b01111111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100000);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER D / 68
	VpokeNext(0b01111110);
	VpokeNext(0b01100110);
	VpokeNext(0b01100110);
	VpokeNext(0b01110111);
	VpokeNext(0b01110111);
	VpokeNext(0b01110111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER E / 69
	VpokeNext(0b01111111);
	VpokeNext(0b01100000);
	VpokeNext(0b01100000);
	VpokeNext(0b01111111);
	VpokeNext(0b01110000);
	VpokeNext(0b01110000);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER F / 70
	VpokeNext(0b01111111);
	VpokeNext(0b01100000);
	VpokeNext(0b01100000);
	VpokeNext(0b01111111);
	VpokeNext(0b01110000);
	VpokeNext(0b01110000);
	VpokeNext(0b01110000);
	VpokeNext(0b00000000);
	
	// CARACTER G / 71
	VpokeNext(0b01111111);
	VpokeNext(0b01100011);
	VpokeNext(0b01100000);
	VpokeNext(0b01101111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER H / 72
	VpokeNext(0b01110011);
	VpokeNext(0b01110011);
	VpokeNext(0b01110011);
	VpokeNext(0b01111111);
	VpokeNext(0b01110011);
	VpokeNext(0b01110011);
	VpokeNext(0b01110011);
	VpokeNext(0b00000000);
	
	// CARACTER I / 73
	VpokeNext(0b00001100);
	VpokeNext(0b00001100);
	VpokeNext(0b00001100);
	VpokeNext(0b00001100);
	VpokeNext(0b00111100);
	VpokeNext(0b00111100);
	VpokeNext(0b00111100);
	VpokeNext(0b00000000);
	
	// CARACTER J / 74
	VpokeNext(0b00001100);
	VpokeNext(0b00001100);
	VpokeNext(0b00001100);
	VpokeNext(0b00001110);
	VpokeNext(0b00001110);
	VpokeNext(0b01101110);
	VpokeNext(0b01111110);
	VpokeNext(0b00000000);
	
	// CARACTER K / 75
	VpokeNext(0b01100110);
	VpokeNext(0b01100110);
	VpokeNext(0b01101100);
	VpokeNext(0b01111111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b00000000);
	
	// CARACTER L / 76
	VpokeNext(0b00110000);
	VpokeNext(0b00110000);
	VpokeNext(0b00110000);
	VpokeNext(0b01110000);
	VpokeNext(0b01110000);
	VpokeNext(0b01110000);
	VpokeNext(0b01111110);
	VpokeNext(0b00000000);
	
	// CARACTER M / 77
	VpokeNext(0b01100111);
	VpokeNext(0b01111111);
	VpokeNext(0b01111111);
	VpokeNext(0b01110111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b00000000);
	
	// CARACTER N / 78
	VpokeNext(0b01100111);
	VpokeNext(0b01110111);
	VpokeNext(0b01111111);
	VpokeNext(0b01101111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b00000000);	
	
	// CARACTER O / 79
	VpokeNext(0b01111111);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER P / 80
	VpokeNext(0b01111111);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01111111);
	VpokeNext(0b01110000);
	VpokeNext(0b01110000);
	VpokeNext(0b01110000);
	VpokeNext(0b00000000);
	
	// CARACTER Q / 81
	VpokeNext(0b01111111);
	VpokeNext(0b01100011);
	VpokeNext(0b01100011);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER R / 82
	VpokeNext(0b01111110);
	VpokeNext(0b01100110);
	VpokeNext(0b01100110);
	VpokeNext(0b01111111);
	VpokeNext(0b01110111);
	VpokeNext(0b01110111);
	VpokeNext(0b01110111);
	VpokeNext(0b00000000);
	
	// CARACTER S / 83
	VpokeNext(0b01111111);
	VpokeNext(0b01100000);
	VpokeNext(0b01111111);
	VpokeNext(0b00000011);
	VpokeNext(0b01110011);
	VpokeNext(0b01110011);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);
	
	// CARACTER T / 84
	VpokeNext(0b01111111);
	VpokeNext(0b00011100);
	VpokeNext(0b00011100);
	VpokeNext(0b00011100);
	VpokeNext(0b00011100);
	VpokeNext(0b00011100);
	VpokeNext(0b00011100);
	VpokeNext(0b00000000);
	
	// CARACTER U / 85
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000); 
	
	// CARACTER V / 86
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01101111);
	VpokeNext(0b00111110);
	VpokeNext(0b00011100);
	VpokeNext(0b00000000);
	
	// CARACTER W / 87
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01101111);
	VpokeNext(0b01111111);
	VpokeNext(0b01111111);
	VpokeNext(0b01100111);
	VpokeNext(0b00000000);
	
	// CARACTER X / 88
	VpokeNext(0b01110011);
	VpokeNext(0b01110011);
	VpokeNext(0b01110011);
	VpokeNext(0b00111110);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b00000000);
	
	// CARACTER Y / 89
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01100111);
	VpokeNext(0b01111111);
	VpokeNext(0b00011100);
	VpokeNext(0b00011100);
	VpokeNext(0b00011100);
	VpokeNext(0b00000000);
	
	// CARACTER Z / 90
	VpokeNext(0b01111111);
	VpokeNext(0b01100110);
	VpokeNext(0b01101100);
	VpokeNext(0b00011000);
	VpokeNext(0b00110111);
	VpokeNext(0b01100111);
	VpokeNext(0b01111111);
	VpokeNext(0b00000000);

	// COLOREAMOS LOS TILES
	if (tercio == 1) {
		comienzo = TCB0;
	} else {
		if (tercio == 2) {
			comienzo = TCB1;
		} else {
			comienzo = TCB2;
		}
	}

	VpokeFirst(TCB0);
	for (contadorPTT = 0; contadorPTT < 768; contadorPTT++) // 768 es 256 (un tercio) x 3 = nº tiles posible
		PonerColorTileLetra();
} // FIN PreparaTilesTexto


// FUNCION: VACIA EN EL MAPA EL TERCIO QUE SE INDIQUE, PINTO EL ESPACIO EN BLANCO EN TODA LA PANTALLA
// ENTRADAS: TERCIO A LIMPIAR 1->3
// SALIDAS: -
void LimpiaTilesTexto (char tercio) {
	int  contadorPTT,comienzo;

	if (tercio == 1) {
		comienzo = MAPT1;
	} else {
		if (tercio == 2) {
			comienzo = MAPT2;
		} else {
			comienzo = MAPT3;
		}
	}
	VpokeFirst(comienzo);
	for (contadorPTT = 0; contadorPTT < 256; contadorPTT++) VpokeNext(LETRA_SP); // POSICIÓN DE ESPACIO Y COINCIDE CON SU CÓDIGO ASCII
} // FIN LimpiaTilesTexto


// FUNCION: PONE COLOR EN TILE LINEA A LINEA 8 LINEAS. NO SE PASA VPOKEFIRST PORQUE LO TIENE
//			YA DE ANTES (POR ALIGERAR)
// ENTRADAS: -
// SALIDAS: -
void PonerColorTileLetra () {
	VpokeNext(TILENEGROGRIS);
	VpokeNext(TILENEGROGRIS);
	VpokeNext(TILENEGROBLANCO);
	VpokeNext(TILENEGROBLANCO);
	VpokeNext(TILENEGROBLANCO);
	VpokeNext(TILENEGROBLANCO);
	VpokeNext(TILENEGROGRIS);
	VpokeNext(TILENEGROGRIS);
} // FIN PonerColorTileLetra


// FUNCION: PONE UN TEXTO EN LAS COORDENADAS QUE PASEMOS SIEMPRE QUE LOS TILES DE LAS LETRAS
//			ESTAN COLOCADAS Y EN SU POSICIÓN ASCII
// ENTRADAS: 
// mapt: tercio en el que se escribirá 1, 2, 3
// fila: fila comienzo 0 -> 7
// col: columna comienzo 0->31
// texto: cadena a escribir
// SALIDAS: -
void PonerTileLocate (unsigned int mapt, char fila, char col, char* texto) {
	char contador;
	char longitud;

	longitud = StrLen(texto);

	VpokeFirst(mapt + fila * 32 + col);

	for (contador = 0; contador < longitud; contador++) {
		switch (texto[contador]) {
			case LETRA_SP : {
				VpokeNext(LETRA_SP);
				break;
			}
			case LETRA_GU : {
				VpokeNext(LETRA_GU);
				break;
			}
			case NUMER_0 : {
				VpokeNext(NUMER_0);
				break;
			}
			case NUMER_1 : {
				VpokeNext(NUMER_1);
				break;
			}
			case NUMER_2 : {
				VpokeNext(NUMER_2);
				break;
			}
			case NUMER_3 : {
				VpokeNext(NUMER_3);
				break;
			}
			case NUMER_4 : {
				VpokeNext(NUMER_4);
				break;
			}
			case NUMER_5 : {
				VpokeNext(NUMER_5);
				break;
			}
			case NUMER_6 : {
				VpokeNext(NUMER_6);
				break;
			}
			case NUMER_7 : {
				VpokeNext(NUMER_7);
				break;
			}
			case NUMER_8 : {
				VpokeNext(NUMER_8);
				break;
			}
			case NUMER_9 : {
				VpokeNext(NUMER_9);
				break;
			}
			case LETRA_DP : {
				VpokeNext(LETRA_DP);
				break;
			}
			case LETRA_A : {
				VpokeNext(LETRA_A);
				break;
			}
			case LETRA_B : {
				VpokeNext(LETRA_B);
				break;
			}
			case LETRA_C : {
				VpokeNext(LETRA_C);
				break;
			}
			case LETRA_D : {
				VpokeNext(LETRA_D);
				break;
			}
			case LETRA_E : {
				VpokeNext(LETRA_E);
				break;
			}
			case LETRA_F : {
				VpokeNext(LETRA_F);
				break;
			}
			case LETRA_G : {
				VpokeNext(LETRA_G);
				break;
			}
			case LETRA_H : {
				VpokeNext(LETRA_H);
				break;
			}
			case LETRA_I : {
				VpokeNext(LETRA_I);
				break;
			}
			case LETRA_J : {
				VpokeNext(LETRA_J);
				break;
			}
			case LETRA_K : {
				VpokeNext(LETRA_K);
				break;
			}
			case LETRA_L : {
				VpokeNext(LETRA_L);
				break;
			}
			case LETRA_M : {
				VpokeNext(LETRA_M);
				break;
			}
			case LETRA_N : {
				VpokeNext(LETRA_N);
				break;
			}
			case LETRA_O : {
				VpokeNext(LETRA_O);
				break;
			}
			case LETRA_P : {
				VpokeNext(LETRA_P);
				break;
			}
			case LETRA_Q : {
				VpokeNext(LETRA_Q);
				break;
			}
			case LETRA_R : {
				VpokeNext(LETRA_R);
				break;
			}
			case LETRA_S : {
				VpokeNext(LETRA_S);
				break;
			}
			case LETRA_T : {
				VpokeNext(LETRA_T);
				break;
			}
			case LETRA_U : {
				VpokeNext(LETRA_U);
				break;
			}
			case LETRA_V : {
				VpokeNext(LETRA_V);
				break;
			}
			case LETRA_W : {
				VpokeNext(LETRA_W);
				break;
			}
			case LETRA_X : {
				VpokeNext(LETRA_X);
				break;
			}
			case LETRA_Y : {
				VpokeNext(LETRA_Y);
				break;
			}
			case LETRA_Z : {
				VpokeNext(LETRA_Z);
				break;
			}
		}
	}
} // FIN PintaTileTexto


// FUNCION: PONE UN TEXTO INICIAL ANTES DE EMPEZAR CADA FASE
// ENTRADAS: 
// fase: fase por la que vamos 1->6
// nivel: de 1 en adelante (el nivel incrementa la dificultad de las fases y se incremente cuando se superan todas las fases)
// SALIDAS: -
void PintarIntroFase () {
	HideDisplay(); // OCULTAMOS PORQUE AL PINTAR LOS TILES SE VEN EN PANTALLA
	// PREPARAMOS LOS TILES DE LETRAS EN CADA TERCIO DE PANTALLA Y SE LIMPIA PARA USO
	PreparaTilesTexto ((char)1);
	LimpiaTilesTexto ((char)1);
	PreparaTilesTexto ((char)2);
	LimpiaTilesTexto ((char)2);
	PreparaTilesTexto ((char)3);
	LimpiaTilesTexto ((char)3);

	PonerTileLocate (MAPT1, 3, 7, "FASE:");
	itoa(fase, cadena);
	PonerTileLocate (MAPT1, 3, 13, cadena);
	PonerTileLocate (MAPT1, 3, 18, "NIVEL:");
	itoa(nivel, cadena);
	PonerTileLocate (MAPT1, 3, 25, cadena);

	switch(fase) {
		case FASE1: {

			PonerTileLocate (MAPT2, 0, 1, "HAS IDO A UNA TIENDA ESPECIAL");
			PonerTileLocate (MAPT2, 1, 1, "A COMPRAR UNA MASCOTA MUY");
			PonerTileLocate (MAPT2, 2, 1, "EXTRAÑA Y NO SE DEJARÁ ATRAPAR");
			PonerTileLocate (MAPT2, 3, 1, "FACILMENTE HAZLO VARIAS VECES");
			PonerTileLocate (MAPT2, 4, 1, "HASTA QUE NO SE ESCAPE ANTES DE");
			PonerTileLocate (MAPT2, 5, 1, "QUE EL DUEÑO PIERDA LA PACIENCIA");
			PonerTileLocate (MAPT2, 6, 1, "Y TU UNA VIDA");
			break;
		}

		case FASE2: {
			break;
		}

		case FASE3: {
			break;
		}

		case FASE4: {
			break;
		}

		case FASE5: {
			break;
		}

		case FASE6: {
			break;
		}
	}

	PonerTileLocate (MAPT3, 3, 4, "PUNTOS:");
	itoa(puntos, cadena);
	PonerTileLocate (MAPT3, 3, 12, cadena);
	PonerTileLocate (MAPT3, 3, 15, "RECORD:");
	itoa(record, cadena);
	PonerTileLocate (MAPT3, 3, 23, cadena);

	ShowDisplay(); // MOSTRAMOS RESULTADO

	WAIT(ESPERAINTROFASE);
} // FIN PintaTileTexto


// FUNCION: CARGA EL FONDO DE PANTALLA, DEPENDE DE LA FASE QUÉ FONDO CARGA
// ENTRADAS: 
// SALIDAS: -
void CargaFondoJuego () {

	int estadofichero;

	int salto;
	int contador;

	char bufferFichero[32]; // Set a 32 bytes buffer
	


	switch(fase) {
		case 1: {
			FT_SetName( &file, "ST1SC2.CHR" );
/*			
Screen(0);
SetColors(15,1,1);
Print("dentro de 1");
Print(file.name);
exit();
*/

			break;
		}
		case 2: {
			FT_SetName( &file, "STAGE2.SC2.CHR" );
			break;
		}
		case 3: {
			FT_SetName( &file, "STAGE3.SC2.CHR" );
			break;
		}
		case 4: {
			FT_SetName( &file, "STAGE4.SC2.CHR" );
			break;
		}
		case 5: {
			FT_SetName( &file, "STAGE5.SC2.CHR" );
			break;
		}
		case 6: {
			FT_SetName( &file, "STAGE6.SC2.CHR" );
			break;
		}
	}

	// LEYENDO FICHERO DE PATRONES DE TILES
	/*
	    if(fcb_open( &file ) != FCB_SUCCESS) 
        {
          Screen(0);
          SetColors(15,1,1);
          Print("se produjo un error");
          exit();
        }
	*/
	fcb_open( &file )
		VpokeFirst(TPB0);
		for (salto = 0; salto < 736; salto++) { // LIMITE salto 32 * 23 (NO 24 SE EMPIEZA POR 0)
			fcb_read( &file, bufferFichero, salto );
			for (contador = 0; contador < 32; contador++) {
				VpokeNext(bufferFichero[contador]);
			}
		}
	fcb_close( &file );

	FT_SetName( &file, "ST1SC2.CLR" );
	fcb_open( &file )
		VpokeFirst(TCB0);
		for (salto = 0; salto < 736; salto++) { // LIMITE salto 32 * 23 (NO 24 SE EMPIEZA POR 0)
			fcb_read( &file, bufferFichero, salto );
			for (contador = 0; contador < 32; contador++) {
				VpokeNext(bufferFichero[contador]);
			}
		}
	fcb_close( &file );

	VpokeFirst(MAPT1);
	for (contador = 0; contador < 736; contador++) {
		VpokeNext(contador);
	}
}// FIN CargaFondoJuego



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
// a:  NÚMERO MINIMO
// b:  NÚMERO MAXIMO
// SALIDAS: -
char FT_RandomNumber (char a, char b)
{
    return(rand()%(b-a)+a); // 
}


// FUNCION: PONE UN NOMBRE EN LA ESTRUCTURA FCB PARA PODERLA USAR CON FICHEROS
// ENTRADAS:
// a:  p_fcb // PUNTERO A ESTRUCTURA DE FICHERO FCB
// b:  p_name // TEXTO CON EL NOMBRE DEL FICHERO
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
}

// TODO


