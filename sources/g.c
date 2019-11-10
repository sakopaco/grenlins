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

typedef unsigned char BYTE;

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
#define TILENEGRO 17
#define TILEBLANCO 240
#define TILEGRIS 224
#define TILEVERDEOSCURO 192
#define TILEVERDECLARO 32


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

// COLORES LETRAS PARA FUNCION PORNER COLORES EN LETRAS
#define COLORLETRABLANCO 0
#define COLORLETRAVERDE 1 

// CONTANTES JUEGO
#define RECORD 500 // PUNTOS INICIALES DE RECORD
#define VIDASINICIO 3 // VIDAS INICIALES DEL PROTA DEL JUEGO
#define PUNTOSINICIO 0 // PUNTOS INICIALES DEL PROTA DEL JUEGO
#define ESCENAINICIO 1 // ESCENA INICIO (PARA DEPURAR Y COMENZAR EN UNA ESCENA EN CONCRETO)
#define NIVELINICIO 1 // ESCENA INICIO (PARA DEPURAR Y COMENZAR EN UNA ESCENA EN CONCRETO)

#define ESCENA1 1
#define VALINICIOCONTADOR1 40 // VALOR INICIO CONTADOR PARA FIN DE ESCENA 1
#define ESCENA2 2
#define VALINICIOCONTADOR2 40 // VALOR INICIO CONTADOR PARA FIN DE ESCENA 2
#define ESCENA3 3
#define VALINICIOCONTADOR3 40 // VALOR INICIO CONTADOR PARA FIN DE ESCENA 3
#define ESCENA4 4
#define VALINICIOCONTADOR4 40 // VALOR INICIO CONTADOR PARA FIN DE ESCENA 4
#define ESCENA5 5
#define VALINICIOCONTADOR5 40 // VALOR INICIO CONTADOR PARA FIN DE ESCENA 5
#define ESCENA6 6
#define VALINICIOCONTADOR6 40 // VALOR INICIO CONTADOR PARA FIN DE ESCENA 6

#define ESCENAINICIO 1 // PARA CUANDO HAYA QUE DEPURAR, PODER EMPEZAR POR LA ESCENA QUE SEAN

#define ESPERAINTROESCENA 10 //600 POR DEFECTO


// ESTRUCTURAS GENERICAS
// EL PROTA ES UN CONJUNTO DE 3 o 4 SPRITES, LOS MALOS SON SÓLO 1 (A VECES AGRUPADOS EN ARRAY)
typedef struct {
	BYTE activo;
	BYTE tipo; // IDENTIFICADOR DEL SPRITE (ENEMIGO, PROTA PARTE SUPERIOR, PROTA PARTE INFERIOR, BALA, ETC)
	BYTE x; // POS X
	BYTE y; // POS Y
	BYTE velocidadx; // PIXELS QUE MOVERÁ EN X EN CADA ITERACIÓN
	BYTE velocidady; // PIXELS QUE MOVERÁ EN Y EN CADA ITERACIÓN
	BYTE cont_siguiente_escena; // DE N A 0 (CUANDO LLEGA A 0 CAMBIO DE ESCENA)
	BYTE reset_contador; // N PARA EL CONTADOR
	BYTE numero_escenas; // ES POR SI UTILIZO 3 O MAS ESCENAS SABER CUANTAS USO
	BYTE escenas_actual; // ES POR SI UTILIZO 3 O MAS ESCENAS SABER EN CUAL ESTOY
	// EL ESPRITE MÁS COMPLEJO LLEVARÁ 3 ESCENAS PARA IZQ Y 3 PARA DER
	BYTE *escena1; // EL PATRON DE SPRITES A MOSTRAR
	BYTE *escena2; // EL PATRON DE SPRITES A MOSTRAR
} Sprites_STR;

typedef struct {
	Sprites_STR *sprite1; // SPRITE SUP IZQ
	Sprites_STR *sprite2; // SPRITE INF IZQ
	Sprites_STR *sprite3; // SPRITE SUP DER
	Sprites_STR *sprite4; // SPRITE INF DER
	BYTE direccion; // 0 NO CAMBIA NADA, 1 MODIFICA CONJUNTO MIRAR IZQ, 2 MODIFICA CONJUNTO MIRAR DER
} Sprite_PROTA;


// VARIABLES JUEGO
unsigned int record; // VALOR MÁXIMO DE PUNTOS ALCANZADOS DURANTE LA SESIÓN (SE INICIALIZA CON "RECORD")
unsigned int puntos; // PUNTOS ALCANZADOS DURANTE LA SESIÓN (SE INICIALIZA A 0)
BYTE cadena[8]; // CADENA PARA PONER LOS PUNTOS / VIDAS / NIVEL Y LO QUE HAYA QUE CONVERTIR CON itoa
BYTE nivel; // EL NIVEL INCREMENTA LA DIFICULTAD DE LAS ESCENAS Y SE INCREMENTA CUANDO SE SUPERAN TODAS LAS ESCENAS. VA DE 1 EN ADELANTE
BYTE vidas; // VIDAS DEL PROTA DE LA PARTIDA
BYTE escena; // ESCENA DEL JUEGO (1->6)
BYTE nivel; // LOOP DEL JUEGO (1->n)
BYTE valContadorDec; // EL VALOR QUE VA DECRECIENDO PARA MARCAR FIN DE ALGUNAS ESCENAS

Sprites_STR *lista_sprites; // LISTA CON TODOS LOS SPRITES DE UNA ESCENA (GLOBAL PARA NO TENER QUE PASARLA ENTRE FUNCIONES)
/*
ESCENA 1 TIPOS:
0: PROTA SUPERIOR
1: PROTA INFERIOR A
2: PROTA INFERIOR B
*/

// VARIABLES DE FUNCIONES GENERICAS
static FCB file; // VARIABLE PARA LEER UN FICHERO

// DEFINICIÓN DE SPRITES
// SPRITE PROTA 1 SUP CUANDO ANDA A LA IZQ
static const BYTE prota1_izq_1[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE prota1_izq_21[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE prota1_izq_22[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE prota1_izq_3[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE prota1_izq_4[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
// SPRITE PROTA 1 SUP CUANDO ANDA A LA DER
static const BYTE prota1_der_1[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE prota1_der_21[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE prota1_der_22[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE prota1_der_3[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE prota1_der_4[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
// SPRITE GREMLIN BUENO CUANDO ANDA A LA IZQ
static const BYTE grenbueno_izq_1[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE grenbueno_izq_2[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
// SPRITE GREMLIN BUENO CUANDO ANDA A LA DER
static const BYTE grenbueno_der_1[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const BYTE grenbueno_der_2[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
// SPRITE GREMLIN BUENO CUANDO CAE
static const BYTE grenbueno_cae[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

// SETUP / INICIALIZACIÓN GLOBAL
// FUNCIONES JUEGO
void PintarPantallaInicialJuego 	(void);
void PreparaTilesTexto 				(BYTE tercio, BYTE color);
void LimpiaTilesTexto 				(BYTE tercio);
void PonerColorTileLetra			(int inicio, BYTE tipo);
void PonerTileLocate 				(unsigned int mapt, BYTE fila, BYTE col, BYTE* texto);
void PintarIntroEscena				(void);
void CargaFondoJuego				(void);
void PonerTextosFijosZonaInf 		(void);
void PonerTextosVidas				(void);
void PonerTextosPuntos				(void);
void PonerTextosNivel				(void);
void PonerTextosEscena				(void);

// FUNCIONES GENERICAS
void  PulsaEspacio 		(void);
char* itoa 				(int i, BYTE b[]);
void  WAIT 				(int cicles);
BYTE  FT_RandomNumber 	(BYTE a, BYTE b);
void  FT_SetName		(FCB *p_fcb, const char *p_name);
void  CargaFicheroVRAM 	(BYTE *nombreFichero, int dirInicio);


// INICIO PROGRAMA
void main(void) 
{
	// DEFINIR VARIABLES NO GLOBALES
//	struct escena fondo; // ESTRUCTURA PARA ALMACENAR LAS PANTALLAS DE CADA ESCENA

	BYTE finpartida; // INDICADOR VERDADERO 1 O FALSO 0 PARA VER SI NOS HAN MATADO TODAS LAS VIDAS

	// SETUP / INICIALIZACIÓN (INICIAL) ENTORNO Y VARIABLES DE JUEGO
	Screen(2);
	SetColors(COLORNEGRO,COLORNEGRO,COLORNEGRO);
	KeySound(FALSO);

	record = RECORD;
	puntos = VIDASINICIO;
	vidas = VIDASINICIO;
	nivel = NIVELINICIO;
	escena = ESCENAINICIO;


	// CONFIGURA SPRITES
	SpriteReset();	
	Sprite16();
	SpriteDouble();	

	// poner patrones de sprites
	/*
	SetSpritePattern(0, prota1_sup_i,  32);
	SetSpritePattern(4, prota1_sup_d,  32);
	SetSpritePattern(8, prota1_inf_i_1,32);
	SetSpritePattern(12,prota1_inf_i_2,32);
	SetSpritePattern(16,prota1_inf_i_3,32);
	SetSpritePattern(20,prota1_inf_d_1,32);
	SetSpritePattern(24,prota1_inf_d_2,32);
	SetSpritePattern(28,prota1_inf_d_3,32);
	SetSpritePattern(32,grenb1_inf_i,  32);
	SetSpritePattern(36,grenb1_inf_d,  32);
	*/


	// CARGAR GRÁFICOS Y PATRONES COMUNES

	// LOOP JUEGO
	do { 
		// PINTAR PANTALLA INICIAL DE JUEGO
		PintarPantallaInicialJuego();

		// SELECCIONAR SI APLICA MODOS DE JUEGO
		// SETUP / INICIALIZACIÓN (NO INICIAL, DE CADA PARTIDA) VARIABLES DE JUEGO
		finpartida = FALSO;
		nivel = NIVELINICIO; 
		vidas = VIDASINICIO;
		puntos = PUNTOSINICIO;
		escena = ESCENAINICIO;

		// LOOP JUEGO ESCENA
		do {
			// PINTAR INTRO DE ESCENA
			PintarIntroEscena();

			switch(escena) {
				case ESCENA1: {
					// SETUP / INICIALIZACIÓN VARIABLES DE ESCENA
					valContadorDec = VALINICIOCONTADOR1;
					//lista_sprites = (Sprites_STR *) malloc(15); *****

					// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE ESCENA)
					CargaFondoJuego();
					do {} while(VERDADERO);

					// PINTAR PANTALLA DE ESCENA
					// PINTAR TEXTOS
					// PINTAR PUNTOS + RECORD????
					// PINTAR VIDAS ?????

					// PINTAR ENEMIGOS, SI APLICA
					// PINTAR ELEMENTOS SI APLICA
					// PINTAR ACCESORIOS, SIAPLICA

					//LOOP DENTRO ESCENA

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

					// FIN LOOP DENTRO ESCENA
					break;
				}

				case ESCENA2: {

			// SETUP / INICIALIZACIÓN VARIABLES DE ESCENA

			// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE ESCENA)

			// PINTAR PANTALLA DE ESCENA
			// PINTAR TEXTOS
			// PINTAR PUNTOS + RECORD????
			// PINTAR VIDAS ?????

			// PINTAR ENEMIGOS, SI APLICA
			// PINTAR ELEMENTOS SI APLICA
			// PINTAR ACCESORIOS, SIAPLICA

			//LOOP DENTRO ESCENA

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

			// FIN LOOP DENTRO ESCENA
					break;
				}

				case ESCENA3: {
			// SETUP / INICIALIZACIÓN VARIABLES DE ESCENA

			// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE ESCENA)

			// PINTAR PANTALLA DE ESCENA
			// PINTAR TEXTOS
			// PINTAR PUNTOS + RECORD????
			// PINTAR VIDAS ?????

			// PINTAR ENEMIGOS, SI APLICA
			// PINTAR ELEMENTOS SI APLICA
			// PINTAR ACCESORIOS, SIAPLICA

			//LOOP DENTRO ESCENA

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

			// FIN LOOP DENTRO ESCENA
					break;
				}

				case ESCENA4: {
			// SETUP / INICIALIZACIÓN VARIABLES DE ESCENA

			// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE ESCENA)

			// PINTAR PANTALLA DE ESCENA
			// PINTAR TEXTOS
			// PINTAR PUNTOS + RECORD????
			// PINTAR VIDAS ?????

			// PINTAR ENEMIGOS, SI APLICA
			// PINTAR ELEMENTOS SI APLICA
			// PINTAR ACCESORIOS, SIAPLICA

			//LOOP DENTRO ESCENA

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

			// FIN LOOP DENTRO ESCENA
					break;
				}

				case ESCENA5: {
			// SETUP / INICIALIZACIÓN VARIABLES DE ESCENA

			// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE ESCENA)

			// PINTAR PANTALLA DE ESCENA
			// PINTAR TEXTOS
			// PINTAR PUNTOS + RECORD????
			// PINTAR VIDAS ?????

			// PINTAR ENEMIGOS, SI APLICA
			// PINTAR ELEMENTOS SI APLICA
			// PINTAR ACCESORIOS, SIAPLICA

			//LOOP DENTRO ESCENA

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

			// FIN LOOP DENTRO ESCENA
					break;
				}

				case ESCENA6: {
					// SETUP / INICIALIZACIÓN VARIABLES DE ESCENA

					// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE ESCENA)

					// PINTAR PANTALLA DE ESCENA
					// PINTAR TEXTOS
					// PINTAR PUNTOS + RECORD????
					// PINTAR VIDAS ?????

					// PINTAR ENEMIGOS, SI APLICA
					// PINTAR ELEMENTOS SI APLICA
					// PINTAR ACCESORIOS, SIAPLICA

					//LOOP DENTRO ESCENA

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

					// FIN LOOP DENTRO ESCENA
					break;
				}
				default: {

				}
			}

			// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES
				// LIBERA MEMORIA RESERVADA PARA SPRITES
				free (lista_sprites);
				// SI QUEDAN MÁS VIDAS INCREMENTA ESCENA
				// SI ESCENA > 5 NIVEL++ ESCENA = 1

		// FIN LOOP JUEGO ESCENA
		} while (!finpartida);
		
		// PINTAR PANTALLA/MENSAJE FIN PARTIDA
			// SI JUEGO DE LOOPS SÓLO GAME OVER
			// SI NO JUEGO DE LOOPS GAME OVER O FIN PARTIDA

	// FIN LOOP JUEGO
	} while (VERDADERO);
} // FIN PROGRAMA


// IMPLEMENTACIÓN DE FUNCIONES
// FUNCION: PINTA PANTALLA CON RECORD, NOMBRE JUEGO, ETC... Y ESPERA A PULSAR TECLA PARA EMPEZAR A JUGAR
// ENTRADAS: -
// SALIDAS: -
void PintarPantallaInicialJuego () {
	BYTE contador;

	HideDisplay(); // OCULTAMOS PORQUE AL PINTAR LOS TILES SE VEN EN PANTALLA
	// PREPARAMOS LOS TILES DE LETRAS EN CADA TERCIO DE PANTALLA Y SE LIMPIA PARA USO
	PreparaTilesTexto ((char)1, COLORLETRABLANCO);
	LimpiaTilesTexto ((char)1);
	PreparaTilesTexto ((char)2, COLORLETRABLANCO);
	LimpiaTilesTexto ((char)2);
	PreparaTilesTexto ((char)3, COLORLETRABLANCO);
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
// ENTRADAS: 
// tercio: EN QUE TERCIO PONE LOS TILES DE LAS LETRAS 1->3
// color: COLORE PREDEFINIDO POR LINEA PARA LETRAS (VER VARIABLES DEFINIDAS COLORLETRAVERDE, COLORLETRABLANCO)
// SALIDAS: -
void PreparaTilesTexto (BYTE tercio, BYTE color) {
	int comienzo, contador;

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
	
	PonerColorTileLetra(comienzo + (32 * 8), color); // ESPACIO
	PonerColorTileLetra(comienzo + (45 * 8), color); // -
	PonerColorTileLetra(comienzo + (58 * 8), color); // :
	for (contador = 0; contador < 10; contador++) PonerColorTileLetra(comienzo + (48 + contador) * 8, color); // NUMEROS
	for (contador = 0; contador < 27; contador++) PonerColorTileLetra(comienzo + (65 + contador) * 8, color); // LETRAS
} // FIN PreparaTilesTexto


// FUNCION: VACIA EN EL MAPA EL TERCIO QUE SE INDIQUE, PINTO EL ESPACIO EN BLANCO EN TODA LA PANTALLA
// ENTRADAS: TERCIO A LIMPIAR 1->3
// SALIDAS: -
void LimpiaTilesTexto (BYTE tercio) {
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
// ENTRADAS: 
// inicio: DIRECCIÓN DE INICIO DE CADA CARACTER A COLOREAR EN LA TABLA DE COLORES DE LA VRAM SCREEN 2
// tipo: NUMERO PARA (CON EL SWITCH) ELEGIR QUE COLOR LE PONEMOS A CADA CARACTER
// SALIDAS: -
void PonerColorTileLetra (int inicio, BYTE tipo) {
	VpokeFirst(inicio);
	switch(tipo) {
		case COLORLETRAVERDE: {
			VpokeNext(TILEVERDEOSCURO);
			VpokeNext(TILEVERDECLARO);
			VpokeNext(TILEVERDECLARO);
			VpokeNext(TILEBLANCO);
			VpokeNext(TILEBLANCO);
			VpokeNext(TILEVERDECLARO);
			VpokeNext(TILEVERDECLARO);
			VpokeNext(TILEVERDEOSCURO);
		}

		case COLORLETRABLANCO: {
			VpokeNext(TILEGRIS);
			VpokeNext(TILEGRIS);
			VpokeNext(TILEGRIS);
			VpokeNext(TILEBLANCO);
			VpokeNext(TILEBLANCO);
			VpokeNext(TILEGRIS);
			VpokeNext(TILEGRIS);
			VpokeNext(TILEGRIS);
		}
	}
} // FIN PonerColorTileLetra


// FUNCION: PONE UN TEXTO EN LAS COORDENADAS QUE PASEMOS SIEMPRE QUE LOS TILES DE LAS LETRAS
//			ESTAN COLOCADAS Y EN SU POSICIÓN ASCII
// ENTRADAS: 
// mapt: tercio en el que se escribirá 1, 2, 3
// fila: fila comienzo 0 -> 7
// col: columna comienzo 0->31
// texto: cadena a escribir
// SALIDAS: -
void PonerTileLocate (unsigned int mapt, BYTE fila, BYTE col, BYTE* texto) {
	BYTE contador;
	BYTE longitud;

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


// FUNCION: PONE UN TEXTO INICIAL ANTES DE EMPEZAR CADA ESCENA
// ENTRADAS: 
// escena: escena por la que vamos 1->6
// nivel: de 1 en adelante (el nivel incrementa la dificultad de las escenas y se incremente cuando se superan todas las escenas)
// SALIDAS: -
void PintarIntroEscena () {
	HideDisplay(); // OCULTAMOS PORQUE AL PINTAR LOS TILES SE VEN EN PANTALLA
	// PREPARAMOS LOS TILES DE LETRAS EN CADA TERCIO DE PANTALLA Y SE LIMPIA PARA USO
	PreparaTilesTexto ((BYTE)1, COLORLETRABLANCO);
	LimpiaTilesTexto ((BYTE)1);
	PreparaTilesTexto ((BYTE)2, COLORLETRABLANCO);
	LimpiaTilesTexto ((BYTE)2);
	PreparaTilesTexto ((BYTE)3, COLORLETRABLANCO);
	LimpiaTilesTexto ((BYTE)3);

	PonerTileLocate (MAPT1, 3, 7, "ESCENA:");
	itoa(escena, cadena);
	PonerTileLocate (MAPT1, 3, 13, cadena);
	PonerTileLocate (MAPT1, 3, 18, "NIVEL:");
	itoa(nivel, cadena);
	PonerTileLocate (MAPT1, 3, 25, cadena);

	switch(escena) {
		case ESCENA1: {
			PonerTileLocate (MAPT2, 0, 1, "LLEGASTE DE UNA FIESTA A CASA");
			PonerTileLocate (MAPT2, 1, 1, "Y TE QUEDASTE DORMIDO VIENDO");
			PonerTileLocate (MAPT2, 2, 1, "LOS GREMLINS TE HAS PUESTO A");
			PonerTileLocate (MAPT2, 3, 1, "SOÑAR QUE ESTÁS EN UNA TIENDA");
			PonerTileLocate (MAPT2, 4, 1, "MISTERIOSA COMPRANDO UNA");
			PonerTileLocate (MAPT2, 5, 1, "MASCOTA SE RAPIDO PARA QUE EL");
			PonerTileLocate (MAPT2, 6, 1, "DUEÑO NO PIERDA LA PACIENCIA");
			PonerTileLocate (MAPT2, 7, 1, "Y TU UNA VIDA");
			break;
		}

		case ESCENA2: {
			PonerTileLocate (MAPT2, 0, 1, "GIZMO SE HA COLADO EN EL BAÑO");
			PonerTileLocate (MAPT2, 1, 1, "DATE PRISA EN ATRAPARLO ANTES");
			PonerTileLocate (MAPT2, 2, 1, "DE QUE SE MOJE Y VIGILA LOS");
			PonerTileLocate (MAPT2, 3, 1, "CABLES Y ENCHUFES O LO PAGARAS");
			break;
		}

		case ESCENA3: {
			PonerTileLocate (MAPT2, 0, 1, "LAS CRIAS DE GIZMO SON MUY");
			PonerTileLocate (MAPT2, 1, 1, "JUGUETONAS. ATRAPALOS ANTES DE");
			PonerTileLocate (MAPT2, 2, 1, "QUE LLEGUEN A LA COCINA SON");
			PonerTileLocate (MAPT2, 3, 1, "MUY TRAVIESOS VIGILA TU CABEZA");
			break;
		}

		case ESCENA4: {
			PonerTileLocate (MAPT2, 0, 1, "HORROR LAS CRIAS HAN ENTRADO");
			PonerTileLocate (MAPT2, 1, 1, "EN LA COCINA. DEBES EVITAR QUE");
			PonerTileLocate (MAPT2, 2, 1, "COMAN LAS HAMBURGUESAS A TODA");
			PonerTileLocate (MAPT2, 3, 1, "COSTA ATRAPALOS");
			break;
		}

		case ESCENA5: {
			PonerTileLocate (MAPT2, 0, 1, "OCURRIO LO PEOR SE HAN VUELTO");
			PonerTileLocate (MAPT2, 1, 1, "MALOS Y QUIEREN ESCAPAR DE LA");
			PonerTileLocate (MAPT2, 2, 1, "CASA DESESPERADO NO DEBES");
			PonerTileLocate (MAPT2, 3, 1, "PERMITIR QUE OCURRA AUNQUE");
			PonerTileLocate (MAPT2, 4, 1, "DEBAS ACABAR CON ELLOS");
			break;
		}

		case ESCENA6: {
			PonerTileLocate (MAPT2, 0, 1, "EL MAS MALVADO E INTELIGENTE");
			PonerTileLocate (MAPT2, 1, 1, "SE HA QUEDADO ESCONDIDO EN TU");
			PonerTileLocate (MAPT2, 2, 1, "HABITACION QUIERE ACABAR");
			PonerTileLocate (MAPT2, 3, 1, "CONTIGO A MENOS QUE TU LO");
			PonerTileLocate (MAPT2, 4, 1, "HAGAS ANTES");
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

	WAIT(ESPERAINTROESCENA);
} // FIN PintaTileTexto


// FUNCION: CARGA EL FONDO DE PANTALLA, DEPENDE DE LA ESCENA QUÉ FONDO CARGA
// ENTRADAS: 
// SALIDAS: -
void CargaFondoJuego () {
	int contador;

	HideDisplay(); // OCULTAMOS PORQUE AL PINTAR LOS TILES SE VEN EN PANTALLA
	// CARGAR LOS PATRONES Y COLORES
	switch(escena) {
		case 1: {
			CargaFicheroVRAM ("ST1SC2.CHR", TPB0);
			CargaFicheroVRAM ("ST1SC2.CLR", TCB0);
			break;
		}
		case 2: {
			CargaFicheroVRAM ("ST1SC2.CHR", TPB0);
			CargaFicheroVRAM ("ST1SC2.CLR", TCB0);
			break;
		}
		case 3: {
			CargaFicheroVRAM ("ST1SC2.CHR", TPB0);
			CargaFicheroVRAM ("ST1SC2.CLR", TCB0);
			break;
		}
		case 4: {
			CargaFicheroVRAM ("ST1SC2.CHR", TPB0);
			CargaFicheroVRAM ("ST1SC2.CLR", TCB0);
			break;
		}
		case 5: {
			CargaFicheroVRAM ("ST1SC2.CHR", TPB0);
			CargaFicheroVRAM ("ST1SC2.CLR", TCB0);
			break;
		}
		case 6: {
			CargaFicheroVRAM ("ST1SC2.CHR", TPB0);
			CargaFicheroVRAM ("ST1SC2.CLR", TCB0);
			break;
		}
	}

	// PONER LOS TILES EN SUS POSICIONES
	VpokeFirst(MAPT1);
	for (contador = 0; contador < 255; contador++) VpokeNext(contador);
	VpokeFirst(MAPT2);
	for (contador = 0; contador < 255; contador++) VpokeNext(contador);
	VpokeFirst(MAPT2);
	for (contador = 0; contador < 255; contador++) VpokeNext(contador);
	

	PonerTextosFijosZonaInf();

	ShowDisplay(); // OCULTAMOS PORQUE AL PINTAR LOS TILES SE VEN EN PANTALLA Y AHORA MOSTRAMOS RESULTADO
}// FIN CargaFondoJuego


// FUNCION: ESCRIBE LOS TEXTOS FIJOS DE LA PARTE INFERIOR
// ENTRADAS: -
// SALIDAS: -
void PonerTextosFijosZonaInf () {
	// PONER TILES DE TEXTO EN 3º TERCIO Y EL TEXTO INICIAL
	LimpiaTilesTexto ((BYTE)3);
	PreparaTilesTexto ((BYTE)3, COLORLETRAVERDE);

	PonerTileLocate (MAPT3, 4,  2, "VIDAS");
	PonerTileLocate (MAPT3, 6,  2, "PUNTOS");
	PonerTileLocate (MAPT3, 4, 23, "NIVEL");
	PonerTileLocate (MAPT3, 6, 22, "ESCENA");

	PonerMarcoContador();
	PonerTextosPuntos();
	PonerTextosVidas();
	PonerTextosNivel();
	PonerTextosEscena();
	PonerTextosContador();
}// FIN PonerTextosFijosZonaInf


// FUNCION: PINTA EL MARCO DEL CONTADOR QUE DECREMENTA PARA FIN ESCENA
// ENTRADAS: -
// SALIDAS: -
void PonerMarcoContador () {
	PonerTileLocate (MAPT3, 3, 13, "OOOOOO");
	PonerTileLocate (MAPT3, 4, 13, "O    O");
	PonerTileLocate (MAPT3, 5, 13, "O    O");
	PonerTileLocate (MAPT3, 6, 13, "O    O");
	PonerTileLocate (MAPT3, 7, 13, "OOOOOO");
}// FIN PonerMarcoContador


// FUNCION: PINTA LOS PUNTOS EN LA ZONA INFERIOR
// ENTRADAS: -
// SALIDAS: -
void PonerTextosPuntos () {
	itoa(puntos, cadena);
	PonerTileLocate (MAPT3, 6, 9, cadena);
}// FIN PonerTextosPuntos


// FUNCION: PINTA LAS VIDAS EN LA ZONA INFERIOR
// ENTRADAS: -
// SALIDAS: -
void PonerTextosVidas () {
	itoa(vidas, cadena);
	PonerTileLocate (MAPT3, 4, 9, cadena);
}// FIN PonerTextosVidas


// FUNCION: PINTA EL NIVEL EN LA ZONA INFERIOR
// ENTRADAS: -
// SALIDAS: -
void PonerTextosNivel () {
	itoa(nivel, cadena);
	PonerTileLocate (MAPT3, 4, 29, cadena);
}// FIN PonerTextosNivel


// FUNCION: PINTA LAS ESCENA EN LA ZONA INFERIOR
// ENTRADAS: -
// SALIDAS: -
void PonerTextosEscena () {
	itoa(escena, cadena);
	PonerTileLocate (MAPT3, 6, 29, cadena);
}// FIN PonerTextosEscena


// FUNCION: PINTA EL CONTADOR A DECREMENTAR EN LA ZONA INFERIOR
// ENTRADAS: -
// SALIDAS: -
void PonerTextosContador () {
	itoa(valContadorDec, cadena);
	PonerTileLocate (MAPT3, 5, 15, cadena);
}// FIN PonerTextosEscena


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


// TODO


