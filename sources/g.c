#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/io.h"
#include "fusion-c/header/vdp_sprites.h"
#include "fusion-c/header/pt3replayer.h"
#include "fusion-c/header/ayfx_player.h"


#include "defconstantes.inc"


// SONIDO / MUSICA / EFECTOS SONOROS
BYTE song[SONG_BUFFER]; // ARRAY QUE ALMACENARÁ EL ARCHIVO DE MÚSICA A TOCAR. SONG_BUFFER VALE 9000
static FCB file; // FICHERO PARA ARCHIVO DE MÚSICA / Y DE EFECTOS SONOROS
char *nombresFicherosMusica[2]; // ARRAY QUE ALMACENA LOS NOMBRES DE LOS FICHEROS DE MUSICA
int tamanosFicherosMusica[2]; // ARRAY QUE ALMACENA LOS TAMAÑOS DE LOS FICHEROS DE MUSICA


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
BYTE fallosEscena; // EL VALOR QUE VA DECRECIENDO PARA MARCAR FIN DE VIDA POR DEMASIADOS FALLOS


int contadorSonidoAndaGremEsc1; // ###############



// VARIABLES DE FUNCIONES GENERICAS
static FCB file; // VARIABLE PARA LEER UN FICHERO

// DEFINICION DE SPRITES
#include "defsprites.inc"

// SETUP / INICIALIZACIÓN GLOBAL
// FUNCIONES JUEGO
void PintarPantallaInicialJuego (void);
void PreparaTilesTexto 			(BYTE tercio, BYTE color);
void LimpiaTilesTexto 			(BYTE tercio);
void PonerColorTileLetra		(int inicio, BYTE tipo);
void PonerTileLocate 			(unsigned int mapt, BYTE fila, BYTE col, BYTE* texto);
void PintarIntroEscena			(void);
void CargaFondoJuego			(void);
void PonerTextosFijosZonaInf 	(void);
void PonerTextosVidas			(void);
void PonerTextosPuntos			(void);
void PonerTextosNivelEscena		(void);
void PonerTextosContador		(void);
void PonerMarcoContador 		(void);
void inicializaSpriteProta		(BYTE escena);
void inicializaSpriteEnemigo	(BYTE escena, Sprites_STR* enemigo);
void MueveProta					(BYTE escena, BYTE direccion);
void MueveEnemigo				(BYTE escena, Sprites_STR* enemigo);
void FlipSpritesProta			(void);
BYTE SuperaLimitesProtaEsc1		(void);
void VerificaColisionesEsc1		(void);


// FUNCIONES GENERICAS
void  PulsaEspacio 		(void);
char* itoa 				(int i, BYTE b[]);
void  Espera 			(unsigned int ciclos);
char  IsSpriteCollision (void);
BYTE  RandomNumber 		(BYTE a, BYTE b);
void  FT_SetName		(FCB *p_fcb, const char *p_name);
void  CargaFicheroVRAM 	(BYTE *nombreFichero, int dirInicio);
void  FT_errorHandler	(char n, char *name);
int   FT_LoadData		(char *file_name, char *buffer, int size, int skip);
void  PT3PreparaPlayer 	(char* nCancion, BYTE* datosFichero, int tamano, BYTE modo);
void  PT3ParaPlayer		(void);
void  AYFXPreparaEfectos(void);


// INICIO PROGRAMA
void main(void) 
{
	// DEFINIR VARIABLES NO GLOBALES
	BYTE finPartida; // INDICADOR VERDADERO 1 O FALSO 0 PARA VER SI NOS HAN MATADO TODAS LAS VIDAS
	int contadorEsperas; // PARA ESPERAS EN BUCLES DE CADA ESCENA

	// VARIABLES PARA MUSICA
	// LISTA DE FICHEROS
	nombresFicherosMusica[0] = "Music1.pt3";
	nombresFicherosMusica[1] = "Music2.pt3";
	// TAMAÑO DE LOS FICHEROS DENTRO DEL ARRAY *file (8417 <- Music1.pt3)
	tamanosFicherosMusica[0] = 8417; 
	tamanosFicherosMusica[0] = 2501;

	// SETUP / INICIALIZACIÓN (INICIAL) ENTORNO Y VARIABLES DE JUEGO
	Screen(SCREEN2);
	SetColors(COLORNEGRO,COLORNEGRO,COLORNEGRO);
	KeySound(FALSO);

	record = RECORD;
	puntos = VIDASINICIO;
	vidas = VIDASINICIO;
	nivel = NIVELINICIO;
	escena = ESCENAINICIO;


// ###############################
	contadorSonidoAndaGremEsc1 = 1;



	// PREPARA EFECTOS SONOROS
	afbdata=MMalloc(AFB_SIZE);  
  	AYFXPreparaEfectos();
  	FT_LoadData("test.afb", afbdata, AFB_SIZE, 0);

	// PREPARA PLAYER DE MELODIAS	
	PT3PreparaPlayer (nombresFicherosMusica[MELODIA1], song, tamanosFicherosMusica[MELODIA1], 0); // MELODIA1 VARIABLE QUE INDICA LA CANCIÓN A TOCAR DENTRO DEL ARRAY *file

	// CONFIGURA SPRITES
	SpriteReset();	
	Sprite16();
	SpriteSmall();	

	// CARGAR GRÁFICOS Y PATRONES COMUNES

	// LOOP JUEGO
	do { 
		// PINTAR PANTALLA INICIAL DE JUEGO
		//PintarPantallaInicialJuego();//#######  comentado para ahorrar tiempo
		

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
			PintarIntroEscena();//#######  comentado para ahorrar tiempo

			switch(escena) {
				case ESCENA1: {
					// SETUP / INICIALIZACIÓN VARIABLES DE ESCENA
					contadorEscena = VALINICIOCONTADOR1;
					fallosEscena = LIMITEDALLOSESC1;

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

						if (sprites_otros[0].activo == VERDADERO) {
							MueveEnemigo(escena, sprites_otros);
						} else {
							// INICIALIZA/SPAWMEA/MUEVE ENEMIGOS
							sprites_otros[0].tipo = (BYTE)1; // SON TODOS DE TIPO 1 (GREM ANDANDO CORNISA)
							sprites_otros[0].activo = (BYTE)VERDADERO;
							sprites_otros[0].escena1 = (BYTE)48; // RESETEO SPRITE
							sprites_otros[0].destinoX = (BYTE)RandomNumber((BYTE)BASESPRITEGRENHORIZQ, (BYTE)BASESPRITEGRENHORDER);
							if(RandomNumber((BYTE)0, (BYTE)2) == (BYTE)1) {
								sprites_otros[0].direccionMira = (BYTE)MIRADER;
								sprites_otros[0].x = (BYTE)BASESPRITEGRENHORIZQ;
								sprites_otros[0].escena1 = sprites_otros[0].escena1d;
							} else {
								sprites_otros[0].direccionMira = (BYTE)MIRAIZQ;
								sprites_otros[0].x = (BYTE)BASESPRITEGRENHORDER;
								sprites_otros[0].escena1 = sprites_otros[0].escena1i;
							}
							sprites_otros[0].y = BASESPRITEGRENVER;
						}


/* // DEJO ESTO COMENTADO PORQUE AUNQUE NO SERÁ DE UTILIDAD EN LA ESCENA 1 ES LA BASE PARA EL RESTO DE ESCENAS CON MÁS DE UN ENEMIGO (PARA COPIARLO MÁS TARDE)
							for (contador = 0; contador < 3; contador++) {
								if (sprites_otros[contador].activo) {

								}
							}
*/
							

						// DETECCIÓN DE COLISIONES
					
						// ACTUALIZA VARIABLES DE ANIMACIONES/ ACCESORIOS
					
						// DIBUJAR CAMBIOS EN PANTALLA
							// PINTAR/ACTUALIZAR ELEMENTOS SI APLICA
							// PINTAR/ACTUALIZAR ACCESORIOS, SIAPLICA
							// ACTUALIZA ANIMACIONES/ ACCESORIOS
						VerificaColisionesEsc1();

						// OTROS CHEQUEOS Y ACTUALIZCIONES DE VARIABLES

							// ESPERA PARA QUE EL MOVIMIENTO DE SPRITES NO SEA DEMASIADO RÁPIDO
						/*
							for(contadorEsperas = 900; contadorEsperas--; ) {
								// SONIDO DEL GREMLIN ANADANDO	
								if (sprites_otros[0].tipo == 1) {
									FT_CheckFX();
									if (contadorSonidoAndaGremEsc1 == 0) {
										contadorSonidoAndaGremEsc1 = 1000;
										PlayFX(3);
									} else {
										contadorSonidoAndaGremEsc1--;	
									}
								}
    						}
*/
						/*
							for(contadorEsperas = 200; contadorEsperas--; ) {
								// ACTUALIZA EFECTOS DE SONIDO
								FT_CheckFX();
								// SONIDO DEL GREMLIN ANADANDO	
								if (sprites_otros[0].tipo == 1 && contadorSonidoAndaGremEsc1 == 1) {
									if (contadorEsperas == 0) {
										AYFXPreparaEfectos();
										PlayFX(3);
										contadorSonidoAndaGremEsc1 = 0;
									} 
								}
    						}
    						*/
    						// PARA QUE LOS GRAFICOS SE MUESTREN BIEN
	    					while(IsVsync()) {}
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



// FUNCION: ACTUALIZA LOS DATOS DE LOS SPRITES DEL ENEMIGO Y MUESTRA AL ENEMIGO POR PANTALLA
// ENTRADAS: 
// escena: LA ESCENA DEL JUEGO QUE SE TRATE (EL MOVIMIENTO DEL PROTA PUEDE VARIAR)
// enemigo: ESTRUCTURA DEL SPRITE DEL ENEMIGO
// SALIDAS: -
void MueveEnemigo (BYTE escena, Sprites_STR* enemigo) {
	switch(escena) {
		case 1: {
			switch(enemigo->tipo) {
				case 1: { // GREMBLIN BUENO	ANDANDO IZQ O DER
					if (enemigo->direccionMira == (BYTE)MIRADER) {
						enemigo->x += enemigo->velocidadX;

						if (enemigo->x >= enemigo->destinoX) {
							enemigo->tipo = (BYTE)2; // GREMLIN CAYENDO O MACETA
							if(RandomNumber((BYTE)0, (BYTE)100) <= (BYTE)(PROBOBJETOESC1 + (nivel * 5))) {
								enemigo->escena1 = enemigo->escena5d;
							} else {
								enemigo->escena1 = enemigo->escena6d;
							}
						} else
							if (enemigo->escena1 == enemigo->escena1d)
								enemigo->escena1 = enemigo->escena2d;
							else
								enemigo->escena1 = enemigo->escena1d;
					} else {
						enemigo->x -= enemigo->velocidadX;

						if (enemigo->x <= enemigo->destinoX) {
							enemigo->tipo = (BYTE)2; // GREMLIN CAYENDO O MACETA
							if(RandomNumber((BYTE)0, (BYTE)100) <= (BYTE)(PROBOBJETOESC1 + (nivel * 5))) {
								enemigo->escena1 = enemigo->escena5d;
							} else {
								enemigo->escena1 = enemigo->escena6d;
							}
						} else
							if (enemigo->escena1 == enemigo->escena1i)
								enemigo->escena1 = enemigo->escena2i;
							else
								enemigo->escena1 = enemigo->escena1i;
					}

					break;
				}

				case 2: { // GREMLIN BUENO O MACETA CAYENDO
					enemigo->y += enemigo->velocidadY;
					if (enemigo->y >= LIMITEGREMESC1) { // VERIFICAMOS LIMITE HASTA DONDE CAE
						enemigo->activo = (BYTE)FALSO; // DESACTIVO SPRITE (NO ES NECESARIO PERO ES PARA SEGUIR LA MISMA TÓNICA CUANDO SEAN MUCHOS)
						fallosEscena--;
					}

					break;
				}
			}

			break;
		}
	}

	PutSprite(enemigo->plano, enemigo->escena1, enemigo->x, enemigo->y, COLORROJOOSCURO);
} // FIN MueveEnemigo


// FUNCION: 
// ENTRADAS: - 
// SALIDAS: -
void VerificaColisionesEsc1 (void) {
	if (IsSpriteCollision() == VERDADERO) {
		Screen(0);
		Cls();
		Exit(0);
	}
} // FIN VerificaColisionesEsc1



// FUNCION: INICIALIZA PSG PARA EFECTOS SONOROS
// ENTRADAS: -
// SALIDAS: -
void AYFXPreparaEfectos (void) {
  InitPSG();
  InitFX();
} // FIN AYFXPreparaEfectos






#include "fungenericas.inc"

#include "funjuego.inc"

// TODO

// que mueva bien las piernas repasar lo de las escenas

