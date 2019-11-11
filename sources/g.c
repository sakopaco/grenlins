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
	BYTE x; // POS X
	BYTE y; // POS Y
	BYTE velocidadx; // PIXELS QUE MOVERÁ EN X EN CADA ITERACIÓN
	BYTE velocidady; // PIXELS QUE MOVERÁ EN Y EN CADA ITERACIÓN
	BYTE cont_siguiente_escena; // DE N A 0 (CUANDO LLEGA A 0 CAMBIO DE ESCENA)
	BYTE reset_contador; // N PARA EL CONTADOR
	BYTE numero_escenas; // ES POR SI UTILIZO 3 O MAS ESCENAS SABER CUANTAS USO
	BYTE escenas_actual; // ES POR SI UTILIZO 3 O MAS ESCENAS SABER EN CUAL ESTOY
	// EL ESPRITE MÁS COMPLEJO LLEVARÁ 3 ESCENAS PARA IZQ Y 3 PARA DER
	BYTE *escena1i; // EL PATRON DE SPRITES A MOSTRAR
	BYTE *escena2i; // EL PATRON DE SPRITES A MOSTRAR
	BYTE *escena3i; // EL PATRON DE SPRITES A MOSTRAR
	BYTE *escena4i; // EL PATRON DE SPRITES A MOSTRAR
	BYTE *escena5i; // PARA INTERCALAR CON escena4i
	BYTE *escena6i; // PARA INTERCALAR CON escena4i
	BYTE *escena1d; // EL PATRON DE SPRITES A MOSTRAR
	BYTE *escena2d; // EL PATRON DE SPRITES A MOSTRAR
	BYTE *escena3d; // EL PATRON DE SPRITES A MOSTRAR
	BYTE *escena4d; // EL PATRON DE SPRITES A MOSTRAR
	BYTE *escena5d; // PARA INTERCALAR CON escena2d
	BYTE *escena6d; // PARA INTERCALAR CON escena2d
} Sprites_STR;

// VARIABLES JUEGO
Sprites_STR  sprites_prota; // LISTA CON TODOS LOS SPRITES DE UNA ESCENA (GLOBAL PARA NO TENER QUE PASARLA ENTRE FUNCIONES)
Sprites_STR  sprites_malos; // LISTA CON TODOS LOS SPRITES DE UNA ESCENA (GLOBAL PARA NO TENER QUE PASARLA ENTRE FUNCIONES)
Sprites_STR  sprites_otros; // LISTA CON TODOS LOS SPRITES DE UNA ESCENA (GLOBAL PARA NO TENER QUE PASARLA ENTRE FUNCIONES)

// VARIABLES JUEGO
unsigned int record; // VALOR MÁXIMO DE PUNTOS ALCANZADOS DURANTE LA SESIÓN (SE INICIALIZA CON "RECORD")
unsigned int puntos; // PUNTOS ALCANZADOS DURANTE LA SESIÓN (SE INICIALIZA A 0)
BYTE cadena[8]; // CADENA PARA PONER LOS PUNTOS / VIDAS / NIVEL Y LO QUE HAYA QUE CONVERTIR CON itoa
BYTE nivel; // EL NIVEL INCREMENTA LA DIFICULTAD DE LAS ESCENAS Y SE INCREMENTA CUANDO SE SUPERAN TODAS LAS ESCENAS. VA DE 1 EN ADELANTE
BYTE vidas; // VIDAS DEL PROTA DE LA PARTIDA
BYTE escena; // ESCENA DEL JUEGO (1->6)
BYTE nivel; // LOOP DEL JUEGO (1->n)
BYTE valContadorDec; // EL VALOR QUE VA DECRECIENDO PARA MARCAR FIN DE ALGUNAS ESCENAS



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

					// CARGAR GRÁFICOS Y PATRONES NO COMUNES (DE ESCENA)
					// PONER LOS PATRONES DE SPRITES DE ESTA ESCENA
						SetSpritePattern((char)0,  prota1_izq_1,   (char)32);
						SetSpritePattern((char)4,  prota1_izq_2,   (char)32);
						SetSpritePattern((char)8,  prota1_izq_3,  (char)32);
						SetSpritePattern((char)12, prota1_izq_41,  (char)32);
						SetSpritePattern((char)16, prota1_izq_42,   (char)32);
						SetSpritePattern((char)20, prota1_der_1,   (char)32);
						SetSpritePattern((char)24, prota1_der_21,  (char)32);
						SetSpritePattern((char)28, prota1_der_22,  (char)32);
						SetSpritePattern((char)32, prota1_der_3,   (char)32);
						SetSpritePattern((char)36, prota1_der_4,   (char)32);
						SetSpritePattern((char)40, grenbueno_izq_1,(char)32);
						SetSpritePattern((char)44, grenbueno_izq_2,(char)32);
						SetSpritePattern((char)48, grenbueno_der_1,(char)32);
						SetSpritePattern((char)52, grenbueno_der_2,(char)32);
						SetSpritePattern((char)56, grenbueno_cae,  (char)32);

						sprites_prota.activo = 1;
						sprites_prota.x = 100;
						sprites_prota.y = 100;
						sprites_prota.velocidadx = 2;
						sprites_prota.velocidady = 0;
						sprites_prota.cont_siguiente_escena = 2;
						sprites_prota.reset_contador = 2;
						sprites_prota.numero_escenas = 2;
						sprites_prota.escenas_actual = 0;
						sprites_prota.escena1i = prota1_izq_1;
						sprites_prota.escena2i = prota1_izq_2;
						sprites_prota.escena3i = prota1_izq_3;
						sprites_prota.escena4i = prota1_izq_41;
						sprites_prota.escena5i = prota1_izq_41;
						sprites_prota.escena6i = prota1_izq_42;
						sprites_prota.escena1d = prota1_der_1;
						sprites_prota.escena2d = prota1_der_21;
						sprites_prota.escena3d = prota1_der_3;
						sprites_prota.escena4d = prota1_der_4;
						sprites_prota.escena5d = prota1_der_21;
						sprites_prota.escena6d = prota1_der_22;


					
					// PINTAR EN PANTALLA
  						HideDisplay(); // OCULTAMOS PORQUE AL PINTAR LOS TILES SE VEN EN PANTALLA
						// PINTAR PANTALLA DE ESCENA
						CargaFondoJuego();
						// PINTAR TEXTOS FIJOS, PUNTOS, PINTAR, VIDAS, ETC
						PonerTextosFijosZonaInf();
						// PINTAR ENEMIGOS INICIALES (SI APLICA)
						// PINTAR ELEMENTOS INICIALES (SI APLICA)
						// PINTAR ACCESORIOS INICIALES (SI APLICA)
						ShowDisplay(); // MOSTRAMOS UNA VEZ YA ESTÁ LA PANTALLA CONFIGURADA Y NO SE VE EL PROCESO


						PutSprite(1, 0, 100, 100, COLORBLANCO);


					do {} while(VERDADERO);





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

#include "fungenericas.inc"

#include "funjuego.inc"

// TODO

//poner sprites en array + reserva memoria

