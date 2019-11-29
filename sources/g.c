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
	BYTE tipo; // SE UTILIZA SOBRE TODO CON LOS PERSONAJES QUE NO SON EL PROTA U OBJETO QUE SE MUEVEN (1 GREM BUENO)
	BYTE activo; // SE UTILIZA SOBRE TODO CON LOS PERSONAJES QUE NO SON EL PROTA U OBJETO QUE SE MUEVEN
	BYTE direccionMira; // LA DIRECCIÓN A LA QUE MIRA EL ESPRITE (EN PRINCIPIO SÓLO VALE PROTA): 0 IZQ - 1 DER
	BYTE x; // POS X
	BYTE y; // POS Y
	BYTE destinoX; // CUANDO TIENE QUE LLEGAR A UNA COORDENADA X DESDE DONDE ESTÉ EL SPRITE
	BYTE destinoY; // CUANDO TIENE QUE LLEGAR A UNA COORDENADA Y DESDE DONDE ESTÉ EL SPRITE
	BYTE velocidadX; // PIXELS QUE MOVERÁ EN X EN CADA ITERACIÓN
	BYTE velocidadY; // PIXELS QUE MOVERÁ EN Y EN CADA ITERACIÓN
	BYTE contSiguienteEscena; // DE N A 0 (CUANDO LLEGA A 0 CAMBIO DE ESCENA)
	BYTE resetContador; // N PARA EL CONTADOR
	BYTE plano; // PARA ASIGNAR UN PLANO AL SPRITE
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
static Sprites_STR* sprites_otros; // LISTA CON LOS OTROS SPRITES DE UNA ESCENA (GLOBAL PARA NO TENER QUE PASARLA ENTRE FUNCIONES) // SE EMPLEARÁN LISTAS PARA CONTROLARLOS


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
void inicializaSpriteProta			(BYTE escena);
void inicializaSpriteEnemigo		(BYTE escena, Sprites_STR* enemigo);
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
						// DEFINE PATRONES DE SPRITES
						SetSpritePattern((char)0,  prota1_izq_1,   	(char)32);
						SetSpritePattern((char)4,  prota1_izq_2,   	(char)32);
						SetSpritePattern((char)8,  prota1_izq_3,   	(char)32);
						SetSpritePattern((char)12, prota1_izq_41,  	(char)32);
						SetSpritePattern((char)16, prota1_izq_42,  	(char)32);
						SetSpritePattern((char)20, prota1_der_1,   	(char)32);
						SetSpritePattern((char)24, prota1_der_21,  	(char)32);
						SetSpritePattern((char)28, prota1_der_22,  	(char)32);
						SetSpritePattern((char)32, prota1_der_3,   	(char)32);
						SetSpritePattern((char)36, prota1_der_4,   	(char)32);				
						SetSpritePattern((char)40, grenbueno_izq_1,	(char)32);
						SetSpritePattern((char)44, grenbueno_izq_2,	(char)32);
						SetSpritePattern((char)48, grenbueno_der_1,	(char)32);
						SetSpritePattern((char)52, grenbueno_der_2,	(char)32);
						SetSpritePattern((char)56, grenbueno_cae,  	(char)32);
						SetSpritePattern((char)60, grenbueno_tiraobj,(char)32);

						// INICIOALIZA ESTRUCTURA DE SPRITE DE PROTA						
						inicializaSpriteProta((BYTE)escena);
  
						// RESERVA MEMORIA PARA ESTRUCTURAS DE SPRITES (NO PROTA)
						sprites_otros = (Sprites_STR *) malloc(1); // LA MISMA ESTRUCTURA NOS VALE PARA GREM ANDANDO A IZQ, DER, CAER O MACETA QUE CAE

						inicializaSpriteEnemigo((BYTE)escena, sprites_otros); 

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

						// INICIALIZA/SPAWMEA/MUEVE ENEMIGOS
						if (sprites_otros[0].activo == FALSO) { // INICIALIZA/SPAWMEA
							sprites_otros[0].activo = VERDADERO;
							sprites_otros[0].destinoX = (BYTE)FT_RandomNumber((BYTE)BASESPRITEGRENHORIZQ, (BYTE)BASESPRITEGRENHORDER);
							if(FT_RandomNumber((BYTE)0, (BYTE)2) == (BYTE)1) {
								sprites_otros[0].direccionMira = MIRADER;
								sprites_otros[0].x = BASESPRITEGRENHORDER;
								sprites_otros[0].escena1 = sprites_otros[0].escena1d;
							} else {
								sprites_otros[0].direccionMira = MIRAIZQ;
								sprites_otros[0].x = BASESPRITEGRENHORIZQ;
								sprites_otros[0].escena1 = sprites_otros[0].escena1i;
							}
							sprites_otros[0].y = BASESPRITEGRENVER;
						} else { // MUEVE ENEMIGOS
							if (sprites_otros[0].direccionMira == MIRADER) {
								sprites_otros[0].x += sprites_otros[0].velocidadX;
							} else {
								sprites_otros[0].x += -sprites_otros[0].velocidadX;
							}
						}

						// DETECCIÓN DE LIMITES DE OBJETOS MÓVILES
					
						// DETECCIÓN DE COLISIONES
					
						// ACTUALIZA VARIABLES DE ANIMACIONES/ ACCESORIOS
					
						// DIBUJAR CAMBIOS EN PANTALLA
							// PINTAR/ACTUALIZAR ENEMIGOS, SI APLICA
							if (sprites_otros[0].activo == VERDADERO) {
								PutSprite(sprites_otros[0].plano, sprites_otros[0].escena1, sprites_otros[0].x, sprites_otros[0].y, COLORROJOOSCURO);


								if (sprites_otros[0].direccionMira == MIRADER) {
									if (sprites_otros[0].x > sprites_otros[0].destinoX)
										sprites_otros[0].activo = FALSO;
								} else {
									if (sprites_otros[0].x < sprites_otros[0].destinoX)
										sprites_otros[0].activo = FALSO;
								}


							}





/* // DEJO ESTO COMENTADO PORQUE AUNQUE NO SERÁ DE UTILIDAD EN LA ESCENA 1 ES LA BASE PARA EL RESTO DE ESCENAS CON MÁS DE UN ENEMIGO (PARA COPIARLO MÁS TARDE)
							for (contador = 0; contador < 3; contador++) {
								if (sprites_otros[contador].activo) {

								}
							}
*/
							// PINTAR/ACTUALIZAR ELEMENTOS SI APLICA
							// PINTAR/ACTUALIZAR ACCESORIOS, SIAPLICA
							// ACTUALIZA ANIMACIONES/ ACCESORIOS

						// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES



					} while(contadorEscena); // FIN LOOP DENTRO ESCENA (CONTADOR ESCENA TIENDE A 0 Y VERO ES FALSO)

					free(sprites_otros); // SE LIBERA LA MEMORIA RESERVADA EN ESTA ESCENA POR ESPRITES QUE NO SON EL PROTA
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


// FUNCION: INICIALIZA LOS VALORES DE LA ESTRUCTURA DEL SPRITE DEL PROTA
// ENTRADAS: 
// escena: LA ESCENA DEL JUEGO QUE SE TRATE YA QUE PUEDE VARIAR LOS PATRONES A USAR
// SALIDAS: -
void inicializaSpriteProta(BYTE escena) {
	switch(escena){
		case ESCENA1: {
			sprites_prota.direccionMira = (BYTE)MIRAIZQ;
			sprites_prota.x = (BYTE)100;
			sprites_prota.y = (BYTE)100;
			sprites_prota.velocidadX = (BYTE)1;
			sprites_prota.velocidadY = (BYTE)0;
			sprites_prota.contSiguienteEscena = (BYTE)2;
			sprites_prota.resetContador = (BYTE)2;
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
			break;
		}
	}
} // FIN inicializaSpriteProta


// FUNCION: INICIALIZA LOS VALORES DE LA ESTRUCTURA SE SPRITES NO PROTA SEGÚN ESCENA Y TIPO¿?
// ENTRADAS: 
// escena: LA ESCENA DEL JUEGO QUE SE TRATE YA QUE PUEDE VARIAR LOS PATRONES A USAR
// escena: ESTRUCTURA DE ESPRITE A INICIALIZAR, SE PASA COMO REFERENCIA PARA PODER MODIFICAR
// SALIDAS: -
void inicializaSpriteEnemigo (BYTE escena, Sprites_STR* enemigo) {
	switch(escena){
		case ESCENA1: {
			enemigo->tipo = (BYTE)1; // GREMBLIN BUENO
			enemigo->activo = (BYTE)FALSO;
			enemigo->direccionMira = (BYTE)MIRAIZQ;
			enemigo->velocidadX = (BYTE)VELPROTA1;
			enemigo->contSiguienteEscena = (BYTE)2;
			enemigo->resetContador = (BYTE)2;
			enemigo->plano = (BYTE)5;
			enemigo->escena1 = (BYTE)48;
			enemigo->escena1i = (BYTE)48;
			enemigo->escena2i = (BYTE)52;
			enemigo->escena1d = (BYTE)40;
			enemigo->escena2d = (BYTE)44;
			enemigo->escena5d = (BYTE)56; // grenbueno_cae
			enemigo->escena6d = (BYTE)60; // grenbueno_tiraobj
			break;
		}
	}
}


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
				if (sprites_prota.direccionMira == MIRADER)
					FlipSpritesProta();

				if(SuperaLimitesProtaEsc1() == FALSO) // SI ES VERDADERO SUPERA LIMITES Y NO MOVEMOS SPRITE PROTA
					sprites_prota.x += -sprites_prota.velocidadX;
				else
					sprites_prota.x = LIMITEPROESC1IZQ;

				sprites_prota.contSiguienteEscena--;

				if (sprites_prota.contSiguienteEscena == 0) {
					sprites_prota.contSiguienteEscena = sprites_prota.resetContador;
				
					if (sprites_prota.escena4 == sprites_prota.escena5i)
						sprites_prota.escena4 = sprites_prota.escena6i;
					else
						sprites_prota.escena4 = sprites_prota.escena5i;
				}
				break;
			}

			case TDERECHA: {
				if (sprites_prota.direccionMira == MIRAIZQ)
					FlipSpritesProta();
				
				if(SuperaLimitesProtaEsc1() == FALSO) // SI ES VERDADERO SUPERA LIMITES Y NO MOVEMOS SPRITE PROTA
					sprites_prota.x += sprites_prota.velocidadX;
				else
					sprites_prota.x = LIMITEPROESC1DER;

				sprites_prota.contSiguienteEscena--;

				if (sprites_prota.contSiguienteEscena == 0) {
					sprites_prota.contSiguienteEscena = sprites_prota.resetContador;
				
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


// FUNCION: EXAMINA LOS DATOS DE LOS SPRITES DEL PROTA. SI EL PROTA MIRA A LA DERECHA O LA IZQ (sprites_prota.direccionMira = PROTAMIRAIZQ o PROTAMIRADER)
// LOS DATOS EN LA ESTRUCTURA PARA QUE MIRE A LA IZQUIERDA
// ENTRADAS: - (SE PODRÍA HABER PASADO sprites_prota.direccionMira PERO APROVECHO QUE ES GLOBAL)
// SALIDAS: -
void FlipSpritesProta (void) {
	if (sprites_prota.direccionMira == MIRAIZQ) { // INTERCAMBIAMOS POSICIONES CON LOS DE LA DERECHA
		sprites_prota.escena1 = sprites_prota.escena1d;
		sprites_prota.escena2 = sprites_prota.escena2d;
		sprites_prota.escena3 = sprites_prota.escena3d;
		sprites_prota.escena4 = sprites_prota.escena4d;
		sprites_prota.direccionMira = MIRADER;
	} else {  // INTERCAMBIAMOS POSICIONES CON LOS DE LA IZQUIERDA	
		sprites_prota.escena1 = sprites_prota.escena1i;
		sprites_prota.escena2 = sprites_prota.escena2i;
		sprites_prota.escena3 = sprites_prota.escena3i;
		sprites_prota.escena4 = sprites_prota.escena4i;
		sprites_prota.direccionMira = MIRAIZQ;
	}
} // FIN FlipSpritesProta

// FUNCION: EXAMINA LOS DATOS DE LOS SPRITES DEL PROTA. SI NO SUPERA LOS LIMITES MANDO FALSO (0) Y EN CASO CONTRARIO VERDADERO (0)
// ENTRADAS: - (SE PODRÍA HABER PASADO sprites_prota.direccionMira PERO APROVECHO QUE ES GLOBAL)
// SALIDAS: SI NO SUPERA LOS LIMITES MANDO FALSO (0) Y EN CASO CONTRARIO VERDADERO (0)
BYTE SuperaLimitesProtaEsc1 (void) {
	if (sprites_prota.direccionMira == MIRAIZQ) {
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

