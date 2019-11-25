#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/io.h"
#include "fusion-c/header/vdp_sprites.h"

#include "defconstantes.inc"

// ESTRUCTURAS GENERICAS
// EL PROTA ES UN CONJUNTO DE 4 SPRITES (LAS PRIERNAS SON DOS), EL RESTO 1 (PUEDEN CAMBIAR SI SON ESCENAS)
typedef struct {
	BYTE activo;
	BYTE direccion_mira; // LA DIRECCIÓN A LA QUE MIRA EL ESPRITE (EN PRINCIPIO SÓLO VALE PROTA): 0 IZQ - 1 DER
	BYTE x; // POS X
	BYTE y; // POS Y
	BYTE velocidadx; // PIXELS QUE MOVERÁ EN X EN CADA ITERACIÓN
	BYTE velocidady; // PIXELS QUE MOVERÁ EN Y EN CADA ITERACIÓN
	BYTE cont_siguiente_escena; // DE N A 0 (CUANDO LLEGA A 0 CAMBIO DE ESCENA)
	BYTE reset_contador; // N PARA EL CONTADOR
	// EL ESPRITE MÁS COMPLEJO LLEVARÁ 3 ESCENAS PARA IZQ Y 3 PARA DER
	BYTE escena1; // EL PATRON DE SPRITES A MOSTRAR ARRIBA IZQ
	BYTE escena2; // EL PATRON DE SPRITES A MOSTRAR ABAJO IZQ
	BYTE escena3; // EL PATRON DE SPRITES A MOSTRAR ARRIBA DER
	BYTE escena4; // EL PATRON DE SPRITES A MOSTRAR ABAJO IZQ
	BYTE escena1i; // EL PATRON DE SPRITES A MOSTRAR CUANDO ANDA A LA IZQ
	BYTE escena2i; // EL PATRON DE SPRITES A MOSTRAR CUANDO ANDA A LA IZQ
	BYTE escena3i; // EL PATRON DE SPRITES A MOSTRAR CUANDO ANDA A LA IZQ
	BYTE escena4i; // EL PATRON DE SPRITES A MOSTRAR CUANDO ANDA A LA IZQ
	BYTE escena5i; // PARA INTERCALAR CON escena4i  CUANDO ANDA A LA IZQ
	BYTE escena6i; // PARA INTERCALAR CON escena4i  CUANDO ANDA A LA IZQ
	BYTE escena1d; // EL PATRON DE SPRITES A MOSTRAR CUANDO ANDA A LA DER
	BYTE escena2d; // EL PATRON DE SPRITES A MOSTRAR CUANDO ANDA A LA DER
	BYTE escena3d; // EL PATRON DE SPRITES A MOSTRAR CUANDO ANDA A LA DER
	BYTE escena4d; // EL PATRON DE SPRITES A MOSTRAR CUANDO ANDA A LA DER
	BYTE escena5d; // PARA INTERCALAR CON escena2d  CUANDO ANDA A LA DER
	BYTE escena6d; // PARA INTERCALAR CON escena2d  CUANDO ANDA A LA DER
} Sprites_STR;

// VARIABLES JUEGO
static Sprites_STR sprites_prota; // LISTA CON TODOS LOS SPRITES DE UNA ESCENA (GLOBAL PARA NO TENER QUE PASARLA ENTRE FUNCIONES)
static Sprites_STR sprites_malos; // LISTA CON TODOS LOS SPRITES DE UNA ESCENA (GLOBAL PARA NO TENER QUE PASARLA ENTRE FUNCIONES)
static Sprites_STR sprites_otros; // LISTA CON TODOS LOS SPRITES DE UNA ESCENA (GLOBAL PARA NO TENER QUE PASARLA ENTRE FUNCIONES)



// VARIABLES JUEGO
unsigned int record; // VALOR MÁXIMO DE PUNTOS ALCANZADOS DURANTE LA SESIÓN (SE INICIALIZA CON "RECORD")
unsigned int puntos; // PUNTOS ALCANZADOS DURANTE LA SESIÓN (SE INICIALIZA A 0)
BYTE cadena[8]; // CADENA PARA PONER LOS PUNTOS / VIDAS / NIVEL Y LO QUE HAYA QUE CONVERTIR CON itoa
BYTE nivel; // EL NIVEL INCREMENTA LA DIFICULTAD DE LAS ESCENAS Y SE INCREMENTA CUANDO SE SUPERAN TODAS LAS ESCENAS. VA DE 1 EN ADELANTE
BYTE vidas; // VIDAS DEL PROTA DE LA PARTIDA
BYTE escena; // ESCENA DEL JUEGO (1->6)
BYTE nivel; // LOOP DEL JUEGO (1->n)
BYTE contadorEscena; // EL VALOR QUE VA DECRECIENDO PARA MARCAR FIN DE LAS ESCENAS


// VARIABLES DE FUNCIONES GENERICAS
static FCB file; // VARIABLE PARA LEER UN FICHERO

// DEFINICION DE SPRITES
#include "defsprites.inc"

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
void PonerTextosContador			(void);
void PonerMarcoContador 			(void);
void MueveProta						(BYTE escena, BYTE direccion);
void FlipSpritesProta				(void);
BYTE SuperaLimitesProtaEsc1			(void);

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
	BYTE finPartida; // INDICADOR VERDADERO 1 O FALSO 0 PARA VER SI NOS HAN MATADO TODAS LAS VIDAS

	// SETUP / INICIALIZACIÓN (INICIAL) ENTORNO Y VARIABLES DE JUEGO
	Screen(SCREEN2);
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
	SpriteSmall();	

	// CARGAR GRÁFICOS Y PATRONES COMUNES

	// LOOP JUEGO
	do { 
		// PINTAR PANTALLA INICIAL DE JUEGO
		//#######  comentado para ahorrar tiempo
		//PintarPantallaInicialJuego();

		// SELECCIONAR SI APLICA MODOS DE JUEGO
		// SETUP / INICIALIZACIÓN (NO INICIAL, DE CADA PARTIDA) VARIABLES DE JUEGO
		finPartida = FALSO;
		nivel = NIVELINICIO; 
		vidas = VIDASINICIO;
		puntos = PUNTOSINICIO;
		escena = ESCENAINICIO;

		// LOOP JUEGO ESCENA
		do {
			// PINTAR INTRO DE ESCENA
			
			//#######  comentado para ahorrar tiempo
			//PintarIntroEscena();

			switch(escena) {
				case ESCENA1: {
					// SETUP / INICIALIZACIÓN VARIABLES DE ESCENA
					contadorEscena = VALINICIOCONTADOR1;

					// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE ESCENA)
					// PONER LOS PATRONES DE SPRITES DE ESTA ESCENA
					
						SetSpritePattern((char)0,  prota1_izq_1,   (char)32);
						SetSpritePattern((char)4,  prota1_izq_2,   (char)32);
						SetSpritePattern((char)8,  prota1_izq_3,   (char)32);
						SetSpritePattern((char)12, prota1_izq_41,  (char)32);
						SetSpritePattern((char)16, prota1_izq_42,  (char)32);
						SetSpritePattern((char)20, prota1_der_1,   (char)32);
						SetSpritePattern((char)24, prota1_der_21,  (char)32);
						SetSpritePattern((char)28, prota1_der_22,  (char)32);
						SetSpritePattern((char)32, prota1_der_3,   (char)32);
						SetSpritePattern((char)36, prota1_der_4,   (char)32);
				/*
						SetSpritePattern((char)40, grenbueno_izq_1,(char)32);
						SetSpritePattern((char)44, grenbueno_izq_2,(char)32);
						SetSpritePattern((char)48, grenbueno_der_1,(char)32);
						SetSpritePattern((char)52, grenbueno_der_2,(char)32);
						SetSpritePattern((char)56, grenbueno_cae,  (char)32);
*/

						sprites_prota.activo = (BYTE)1;
						sprites_prota.direccion_mira = (BYTE)PROTAMIRAIZQ;
						sprites_prota.x = (BYTE)100;
						sprites_prota.y = (BYTE)100;
						sprites_prota.velocidadx = (BYTE)1;
						sprites_prota.velocidady = (BYTE)0;
						sprites_prota.cont_siguiente_escena = (BYTE)2;
						sprites_prota.reset_contador = (BYTE)2;
						sprites_prota.escena1  = (BYTE)0;
						sprites_prota.escena2  = (BYTE)4;
						sprites_prota.escena3  = (BYTE)8;
						sprites_prota.escena4  = (BYTE)12;
						sprites_prota.escena1i = (BYTE)0;
						sprites_prota.escena2i = (BYTE)4;
						sprites_prota.escena3i = (BYTE)8;
						sprites_prota.escena4i = (BYTE)12;
						sprites_prota.escena5i = (BYTE)12;
						sprites_prota.escena6i = (BYTE)16;
						sprites_prota.escena1d = (BYTE)20;
						sprites_prota.escena2d = (BYTE)24;
						sprites_prota.escena3d = (BYTE)32;
						sprites_prota.escena4d = (BYTE)36;
						sprites_prota.escena5d = (BYTE)24;
						sprites_prota.escena6d = (BYTE)28;
  

					
					// PINTAR EN PANTALLA
  					HideDisplay(); // OCULTAMOS PORQUE AL PINTAR LOS TILES SE VEN EN PANTALLA
						// PINTAR PANTALLA DE ESCENA
						CargaFondoJuego();
						// PINTAR TEXTOS FIJOS, PUNTOS, PINTAR, VIDAS, ETC
						PonerTextosFijosZonaInf();
						// PINTAR ENEMIGOS INICIALES (SI APLICA)
						// PINTAR ELEMENTOS INICIALES (SI APLICA)
						// PINTAR ACCESORIOS INICIALES (SI APLICA)
						
						PutSprite(1, sprites_prota.escena1, BASESPRITEPROTA     , 100     , COLORBLANCO);
						PutSprite(2, sprites_prota.escena2, BASESPRITEPROTA     , 100 + 16, COLORBLANCO);
						PutSprite(3, sprites_prota.escena3, BASESPRITEPROTA + 16, 100     , COLORBLANCO);
						PutSprite(4, sprites_prota.escena4, BASESPRITEPROTA + 16, 100 + 16, COLORBLANCO);

					ShowDisplay(); // MOSTRAMOS UNA VEZ YA ESTÁ LA PANTALLA CONFIGURADA Y NO SE VE EL PROCESO
					do { //LOOP DENTRO ESCENA

						// OBTENER ENTRADAS DEL JUGADOR (MOVIMIENTOS, DISPAROS, ETC)
						// MOVIMIENTO
						MueveProta(escena,JoystickRead(LEETECLADO));

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

					} while(contadorEscena); // FIN LOOP DENTRO ESCENA (CONTADOR ESCENA TIENDE A 0 Y VERO ES FALSO)
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
				// SI QUEDAN MÁS VIDAS INCREMENTA ESCENA
				// SI ESCENA > 5 NIVEL++ ESCENA = 1

		// FIN LOOP JUEGO ESCENA
		} while (!finPartida);
		
		// PINTAR PANTALLA/MENSAJE FIN PARTIDA
			// SI JUEGO DE LOOPS SÓLO GAME OVER
			// SI NO JUEGO DE LOOPS GAME OVER O FIN PARTIDA

	// FIN LOOP JUEGO
	} while (VERDADERO);
} // FIN PROGRAMA


// FUNCION: ACTUALIZA LOS DATOS DE LOS SPRITES DEL PROTA Y MUESTRA AL PROTA POR PANTALLA
// ENTRADAS: 
// escena: LA ESCENA DEL JUEGO QUE SE TRATE (EL MOVIMIENTO DEL PROTA PUEDE VARIAR)
// direccion: DIRECCIONES DE LA FUNCION JoystickRead (1 arriba, 3 derecha ...)
// SALIDAS: -
void MueveProta (BYTE escena, BYTE direccion) {
	

	WAIT(1); // PRESCINDIBLE SEGÚN SE DESARROLLE EL PROGRAMA

	if (escena == 1) {
		switch(direccion) {
			case TIZQUIERDA: {
				if (sprites_prota.direccion_mira == PROTAMIRADER)
					FlipSpritesProta();

				if(SuperaLimitesProtaEsc1() == FALSO) // SI ES VERDADERO SUPERA LIMITES Y NO MOVEMOS SPRITE PROTA
					sprites_prota.x += -sprites_prota.velocidadx;
				else
					sprites_prota.x = LIMITEPROESC1IZQ;

				sprites_prota.cont_siguiente_escena--;

				if (sprites_prota.cont_siguiente_escena == 0) {
					sprites_prota.cont_siguiente_escena = sprites_prota.reset_contador;
				
					if (sprites_prota.escena4 == sprites_prota.escena5i)
						sprites_prota.escena4 = sprites_prota.escena6i;
					else
						sprites_prota.escena4 = sprites_prota.escena5i;
				}
				break;
			}

			case TDERECHA: {
				if (sprites_prota.direccion_mira == PROTAMIRAIZQ)
					FlipSpritesProta();
				
				if(SuperaLimitesProtaEsc1() == FALSO) // SI ES VERDADERO SUPERA LIMITES Y NO MOVEMOS SPRITE PROTA
					sprites_prota.x += sprites_prota.velocidadx;
				else
					sprites_prota.x = LIMITEPROESC1DER;

				sprites_prota.cont_siguiente_escena--;

				if (sprites_prota.cont_siguiente_escena == 0) {
					sprites_prota.cont_siguiente_escena = sprites_prota.reset_contador;
				
					if (sprites_prota.escena2 == sprites_prota.escena5d)
						sprites_prota.escena2 = sprites_prota.escena6d;
					else
						sprites_prota.escena2 = sprites_prota.escena5d;
				}
				break;
			}
		}
	}

	PutSprite(1, sprites_prota.escena1, sprites_prota.x     , sprites_prota.y     , COLORBLANCO);
	PutSprite(2, sprites_prota.escena2, sprites_prota.x     , sprites_prota.y + 16, COLORBLANCO);
	PutSprite(3, sprites_prota.escena3, sprites_prota.x + 16, sprites_prota.y     , COLORBLANCO);
	PutSprite(4, sprites_prota.escena4, sprites_prota.x + 16, sprites_prota.y + 16, COLORBLANCO);
} // FIN MueveProta


// FUNCION: EXAMINA LOS DATOS DE LOS SPRITES DEL PROTA. SI EL PROTA MIRA A LA DERECHA O LA IZQ (sprites_prota.direccion_mira = PROTAMIRAIZQ o PROTAMIRADER)
// LOS DATOS EN LA ESTRUCTURA PARA QUE MIRE A LA IZQUIERDA
// ENTRADAS: - (SE PODRÍA HABER PASADO sprites_prota.direccion_mira PERO APROVECHO QUE ES GLOBAL)
// SALIDAS: -
void FlipSpritesProta (void) {
	if (sprites_prota.direccion_mira == PROTAMIRAIZQ) { // INTERCAMBIAMOS POSICIONES CON LOS DE LA DERECHA
		sprites_prota.escena1 = sprites_prota.escena1d;
		sprites_prota.escena2 = sprites_prota.escena2d;
		sprites_prota.escena3 = sprites_prota.escena3d;
		sprites_prota.escena4 = sprites_prota.escena4d;
		sprites_prota.direccion_mira = PROTAMIRADER;
	} else {  // INTERCAMBIAMOS POSICIONES CON LOS DE LA IZQUIERDA	
		sprites_prota.escena1 = sprites_prota.escena1i;
		sprites_prota.escena2 = sprites_prota.escena2i;
		sprites_prota.escena3 = sprites_prota.escena3i;
		sprites_prota.escena4 = sprites_prota.escena4i;
		sprites_prota.direccion_mira = PROTAMIRAIZQ;
	}
} // FIN FlipSpritesProta

// FUNCION: EXAMINA LOS DATOS DE LOS SPRITES DEL PROTA. SI NO SUPERA LOS LIMITES MANDO FALSO (0) Y EN CASO CONTRARIO VERDADERO (0)
// ENTRADAS: - (SE PODRÍA HABER PASADO sprites_prota.direccion_mira PERO APROVECHO QUE ES GLOBAL)
// SALIDAS: SI NO SUPERA LOS LIMITES MANDO FALSO (0) Y EN CASO CONTRARIO VERDADERO (0)
BYTE SuperaLimitesProtaEsc1 (void) {
	if (sprites_prota.direccion_mira == PROTAMIRAIZQ) {
		if (sprites_prota.x <= LIMITEPROESC1IZQ)
			return VERDADERO;
	} else {
		if (sprites_prota.x >= LIMITEPROESC1DER)
			return VERDADERO;
	}

	return FALSO;
} // FIN SuperaLimitesProtaEsc1

#include "fungenericas.inc"

#include "funjuego.inc"

// TODO

// que mueva bien las piernas repasar lo de las escenas

