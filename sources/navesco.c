#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_graph1.h"
#include "fusion-c/header/vdp_sprites.h"
#include "fusion-c/header/vdp_circle.h"
#include "fusion-c/header/psg.h"

#define HALT __asm halt __endasm   //wait for the next interrupt

#define DESAPARECE 209
#define LIMINFENEM 150
#define LIMSUPBOMNOD 50
#define LIMINFBOMNOD 166
#define NUMENEMIGOS 9
#define NOQUEDANNAVES 0
#define NOMASNIVELES 10 // equivale al último nivel + 1 (en principio hay 5 niveles)
#define CIERTO 1
#define FALSO 0
#define ENEMABIERTA 4
#define SWAPENEMIGOS 15

#define DESPLAZAENEMIGOSN1 -1
#define DESPLAZAENEMIGOSN2 -1
#define DESPLAZAENEMIGOSN3 -2
#define DESPLAZAENEMIGOSN4 -2
#define DESPLAZAENEMIGOSN5 -3
#define DESPLAZAENEMIGOSN6 -3
#define DESPLAZAENEMIGOSN7 -3
#define DESPLAZAENEMIGOSN8 -3
#define DESPLAZAENEMIGOSN9 -3

#define UMBRALCOLISION 7
#define UMBRALCOLISIONNODRIZA 14
#define UMBRALCOLISIONBOMBA 7

#define SALENAVENODNIVEL1 1000 // para pruebas se pone 15 pero es 1300 (menor más prob)
#define SALENAVENODNIVEL2 800
#define SALENAVENODNIVEL3 400
#define SALENAVENODNIVEL4 200
#define SALENAVENODNIVEL5 100
#define SALENAVENODNIVEL6 100
#define SALENAVENODNIVEL7 100
#define SALENAVENODNIVEL8 100
#define SALENAVENODNIVEL9 100

#define PROBBOMBANODNIVEL1 25 // para pruebas se pone un 100 pero es 15 (mayor más prob)
#define PROBBOMBANODNIVEL2 50
#define PROBBOMBANODNIVEL3 70
#define PROBBOMBANODNIVEL4 80
#define PROBBOMBANODNIVEL5 100
#define PROBBOMBANODNIVEL6 100
#define PROBBOMBANODNIVEL7 100
#define PROBBOMBANODNIVEL8 100
#define PROBBOMBANODNIVEL9 100

#define VELBOMBANODRIZAN1 1
#define VELBOMBANODRIZAN2 2
#define VELBOMBANODRIZAN3 3
#define VELBOMBANODRIZAN4 4
#define VELBOMBANODRIZAN5 5
#define VELBOMBANODRIZAN6 5
#define VELBOMBANODRIZAN7 5
#define VELBOMBANODRIZAN8 5
#define VELBOMBANODRIZAN9 5

// valores inciales de la partida
#define RECORDINICIAL 1000
#define VIDASINICIALES 2
#define NIVELINICIAL 1 // 1 por defecto por si quiero depurar el final y empezar en la 5
#define MARCADORINICIAL 0
#define PUNTOSNAVEMUERTA 30
#define PUNTOSNODRIZAMUERTA 100

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

// variables para captura de teclado y joystick
#define TECLADO 0
#define ESPACIO 255

// SCREEN 2 -> 256 x 192
#define LIMIZQ 0
#define LIMDER 256
#define LIMIZQJU 16 // limite izq para el jugador
#define LIMDERJU 224 // limite der para el jugador
#define LIMIZQENE 8 // limite izq para enemigos
#define LIMDERENE 239 // limite der para enemigos
#define LIMARRDIS 24 // limite arriba para disparo (8 px de caracter * 3 lineas)
#define LIMABADIS 156 // limite abajo para disparo

#define VELDISJU 3 // prites que sube a la vez el disparo
#define VELENEMIGOSDES 5 // prites que se desplazan los enemigos a la vez hacia abajo
#define VELJUGADORDES 2 // prites que se desplaza el jugado der/izp

#define ESPERABUCLE 500 // tiempo que se espera en el bucle para que no vaya demasiado rapido

#define NOSEPULSATECLA 0 // la funcion Inkey devuelve 0 si no se pulsa ninguna tecla

/*
colores MSX
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
#define COLNEGRO 1
#define COLVERDE 2
#define COLAZULOSC 5
#define COLROJOOSC 6
#define COLROVIOLETA 13
#define COLGRIS 14
#define COLBLANCO 15
#define COLNEGRONEGRO 17
#define FILABLANCA 240
#define FILAGRIS 224

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

	//sprite nave a
static const unsigned char navea[] = {
	0x00,0x01,0x01,0x03,0x06,0x03,0x02,0x07,0x4C,0x4B,0x4C,0x3F,0x66,0x59,0x66,0x3F,0x00,0x80,0x80,0xC0,0x60,0xC0,0x40,0xE0,0x32,0xD2,0x32,0xFC,0x66,0x9A,0x66,0xFC
};
	//sprite nave b
static const unsigned char naveb[] = {
	0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x03,0x04,0x03,0x00,0x19,0x26,0x19,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x80,0x00,0xC0,0x20,0xC0,0x00,0x98,0x64,0x98,0x00
};
	//sprite enemigo1 abierto
static const unsigned char enemigo1a[] = {
	0x00,0x20,0x38,0x08,0x3F,0x23,0x2A,0x3E,0x1C,0x1F,0x15,0xF0,0x15,0x1F,0x06,0x00,0x00,0x04,0x1C,0x18,0xFC,0xC4,0x54,0x7C,0x38,0xF8,0xA8,0x0F,0xA8,0xF8,0x60,0x00
};
	//sprite enemigo1 cerrado
static const unsigned char enemigo1c[] = {
	0x00,0x00,0x78,0x48,0x3F,0x23,0x2A,0x3E,0x1C,0x9F,0x5F,0x34,0x1F,0x1F,0x06,0x00,0x00,0x00,0x1E,0x1A,0xFC,0xC4,0x54,0x7C,0x38,0xF9,0xFA,0x4C,0xF8,0xF8,0x60,0x00
};
	//sprite enemigo2 abierto
static const unsigned char enemigo2a[] = {
	0x04,0xE2,0x11,0x09,0x7D,0x55,0x27,0x1A,0x22,0x23,0x2D,0x38,0x37,0x30,0x23,0x1F,0x20,0x47,0x88,0x90,0xBE,0xAA,0xE4,0x58,0x44,0xC4,0xB4,0x1C,0xEC,0x0C,0xC4,0xF8
};
	//sprite enemigo2 cerrado
static const unsigned char enemigo2c[] = {
	0x00,0x02,0x35,0x49,0xFD,0x55,0x27,0x1A,0x22,0x43,0x4D,0x58,0x53,0x54,0x23,0x1F,0x00,0x40,0xAC,0x92,0xBF,0xAA,0xE4,0x58,0x44,0xC2,0xB2,0x1A,0xCA,0x2A,0xC4,0xF8
};
	//sprite enemigo3 abierto
static const unsigned char enemigo3a[] = {
	0x80,0x41,0x36,0x08,0x14,0x9E,0x91,0xF5,0x1F,0x14,0x3F,0x54,0x94,0x24,0x48,0x10,0x01,0x82,0x6C,0x10,0x28,0x78,0x88,0xAF,0xF9,0x29,0xFC,0x2A,0x29,0x24,0x12,0x08
};
	//sprite enemigo3 cerrado
static const unsigned char enemigo3c[] = {
	0x20,0x21,0x36,0x08,0x14,0x1E,0x11,0xF5,0x9F,0x95,0xBF,0x54,0x94,0x24,0x48,0x90,0x04,0x84,0x6C,0x10,0x28,0x79,0x89,0xAF,0xF8,0xA8,0xFC,0x2A,0x29,0x24,0x12,0x09
};
	//sprite disparo
static const unsigned char disparo[] = {
	0x00,0x00,0x00,0x00,0x01,0x03,0x01,0x01,0x01,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x80,0x80,0x80,0x80,0x40,0x00,0x00,0x00,0x00,0x00
};
	//sprite explosion
static const unsigned char explosion[] = {
	0x20,0x51,0x08,0x65,0x15,0x57,0xBF,0x2E,0x7F,0xBB,0xFF,0xD7,0x7F,0x7E,0x77,0x3F,0x88,0x08,0x92,0x25,0xC8,0xB2,0xFD,0xFC,0xEE,0xBE,0xFD,0xFD,0xBD,0xFE,0xCC,0xF8
};
	//sprite nave nodrizaizq
static const unsigned char nodrizaizq[] = {
	0x00,0x00,0x00,0x00,0x01,0x0F,0x3F,0x7F,0x52,0x3F,0x1B,0x0D,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xE1,0xE5,0xFF,0xFF,0x52,0xBF,0x55,0x55,0xFF,0x00,0x00,0x00
};
	//sprite nave nodrizader
static const unsigned char nodrizader[] = {
	0x00,0x00,0x00,0xF8,0x87,0xA7,0xFF,0xFF,0x4A,0xFF,0xAA,0xAA,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xF0,0xFC,0xFE,0x4A,0xFC,0xD8,0xB0,0xE0,0x00,0x00,0x00
};
	// sprite bomba nave nodriza
static const unsigned char nodrizabom[] = {
	0x03,0x01,0x01,0x03,0x01,0x01,0x03,0x03,0x03,0x07,0x0D,0x0D,0x06,0x03,0x01,0x00,0xC0,0x80,0x80,0xC0,0x80,0x80,0xC0,0xC0,0xC0,0xE0,0xF0,0xF0,0xE0,0xC0,0x80,0x00
};


	//matriz pos enemigos
static unsigned char menemigosx[9];
static unsigned char menemigosy[9];
	//matriz pos nave
static unsigned char mnave[2]; // dos posicione
	//matriz pos disparo (se inicia a invisible y 209 y es lo que se comprobará para ver si está disparada o no)
static unsigned char mdisparo[2];
	//matriz pos nave nodriza
static unsigned char mnavenod[2][2];
	// matriz pos bomba nave nodriza
static unsigned char mbomnod[2];

	// si la nave está abierta o cerrada
static unsigned char enemabierta; // 4 abierta 0 cerrada
static unsigned char swapenemigos; // contador que dice cuando cambiar enemigos abiertos por cerrados

	//marcador
int  marcador;
	//record
int  record;
	//vidas
char vidas;
	//nivel
char nivel;
	// cadena auxiliar para poner los puntos / vidas / nivel y lo que haya que convertir con itoa
char cadena[8];
	// número de naves que quedan vivas
char quedannaves;
	// variable que, según valor aleatorio, indica si debe salir nave nodriza
int  salenavenod;
	// variable que dice probabilidad de que nodriza tire bomba (según nivel)
char probbombanodriza;
	// varible que controla la velocidad con la que cae las bombas lanzadas por nodriza
char velbombanodriza;


	// sonidos
FX   fx01;
FX   fx02;
FX   fx03;


void  InicializaTiles       (void);
void  PintaIntroInicio      (void);
void  PintaFondoNivel1      (void);
void  PintaFondoNivel2      (void);
void  PintaFondoNivel3      (void);
void  PintaFondoNivel4      (void);
void  PintaFondoNivel5      (void);
void  PintaFondoNivel6      (void);
void  PintaFondoNivel7      (void);
void  PintaFondoNivel8      (void);
void  PintaFondoNivel9      (void);
void  IncializaPosiciones   (void);
void  PintaSprites          (char enemabierta);
void  PintaTextos           (void);
void  PintaTileTexto        (int mapt, char fila, char col, char* texto);
void  PintaPuntos           (void);
void  PintaVidas            (void);
void  PintaNivel            (void);
int   Aleatorio             (int limiteInf, int limiteSup);
char* itoa                  (int i, char b[]);
void  Espera                (int contador);
void  ExplosionaNave        (void);
char  HayColisionDisparoNave(char numnave);
char  HayColisionDisparoNaveNodriza(void);
char  HayColisionBombaNaveNodriza(void);
void  LimpiaPantallaCompleta(void);
void  PintaEstrellas        (void);
void  LimpiaFondo 			(void);
void  GameOver 				(void);
void  EscenaFinJuego		(void);
void  PulsaEspacio 			(void);
int   Absoluto				(int valor);
void  WAIT 					(int cicles);
void  PonNodriza 			(char mostrar);
void  SumaVidaExtra			(void);
void  PintaInicioNivel		(char nivel);
void  ActualizaPuntos		(void);
char  TiraBombaNodriza      (void);
void  Play                  (char fx);

//BEGIN
void main() 
{
	//Setup/Initialization
	//Define variables
	char contador;
	char contador2;
	
	//desplazamiento (cuanto se mueve la nave. si llega a un limite se multiplica por -1 para que cambien los enemigos de dirección)
	int  desplazamientox;  // como puede variar de nivel a nivel es una variable y no una const
	// si nave muerta porque llegan abajo
	char navemuerta;

	//Load graphics and patterns
		Screen(2);
		SetColors(15,1,1);
		KeySound(0);

		// configura sprites
		SpriteReset();
		Sprite16();
		SpriteDouble();	

		// poner patrones de sprites
		SetSpritePattern(0,enemigo1a,32);
		SetSpritePattern(4,enemigo1c,32);
		SetSpritePattern(8,enemigo2a,32);
		SetSpritePattern(12,enemigo2c,32);
		SetSpritePattern(16,enemigo3a,32);
		SetSpritePattern(20,enemigo3c,32);
		SetSpritePattern(24,navea,32);  //*****************************************************************************************
		SetSpritePattern(28,explosion,32);
		SetSpritePattern(32,disparo,32);
		SetSpritePattern(36,nodrizaizq,32);
		SetSpritePattern(40,nodrizader,32);
		SetSpritePattern(44,nodrizabom,32);
		SetSpritePattern(48,naveb,32);

		// configura tiles
		InicializaTiles();


		// sonidos
		// disparo
  		fx01.isTone=true;
  		fx01.isNoise=false;
  		fx01.Tone=0;
  		fx01.Noise=0;
  		fx01.Period=10000;
  		fx01.Shape=1;
  		// explosión
		fx02.isTone=false;
  		fx02.isNoise=true;
		fx02.Tone=0;
  		fx02.Noise=24;
  		fx02.Period=10000;
  		fx02.Shape=1;
  		// reocrrido nodriza
  		fx03.isTone=false;
  		fx03.isNoise=true;
		fx03.Tone=0;
  		fx03.Noise=12;
  		fx03.Period=10000;
  		fx03.Shape=5;

	//Start Game
	// initial game values
		record = RECORDINICIAL;	
		IncializaPosiciones();

	//Loop Game
	do {
		//Title/Intro Screen
		PintaIntroInicio();
		//FIN Title/Intro Screen

		//Setup level variables
		//Player selects from one or more game modes
		//Setup game variables
		nivel = NIVELINICIAL;
		marcador = MARCADORINICIAL;
		vidas = VIDASINICIALES;
		enemabierta = ENEMABIERTA;

		//Presentación del nivel o mensaje o pulsa una tecla para empezar nivel
		// no se usa aqui

		//New Level/stage
		do {
			navemuerta = FALSO;

			switch(nivel) {
				case 1: {
					desplazamientox = DESPLAZAENEMIGOSN1; // como es menos empiezan desplazandose a la izq
					velbombanodriza = VELBOMBANODRIZAN1;
					PintaFondoNivel1();
					break;
				}
				case 2: {
					desplazamientox = DESPLAZAENEMIGOSN2; // como es menos empiezan desplazandose a la izq
					velbombanodriza = VELBOMBANODRIZAN2;
					PintaInicioNivel(2);
					PintaFondoNivel2();
					break;
				}
				case 3: {
					desplazamientox = DESPLAZAENEMIGOSN3; // como es menos empiezan desplazandose a la izq
					velbombanodriza = VELBOMBANODRIZAN3;
					SumaVidaExtra();
					PintaInicioNivel(3);
					PintaFondoNivel3();
					break;
				}
				case 4: {
					desplazamientox = DESPLAZAENEMIGOSN4; // como es menos empiezan desplazandose a la izq
					velbombanodriza = VELBOMBANODRIZAN4;
					PintaInicioNivel(4);
					PintaFondoNivel4();
					break;
				}
				case 5: {
					desplazamientox = DESPLAZAENEMIGOSN5; // como es menos empiezan desplazandose a la izq
					velbombanodriza = VELBOMBANODRIZAN5;
					PintaInicioNivel(5);
					PintaFondoNivel5();
					break;
				}
				case 6: {
					desplazamientox = DESPLAZAENEMIGOSN6; // como es menos empiezan desplazandose a la izq
					velbombanodriza = VELBOMBANODRIZAN6;
					PintaInicioNivel(6);
					PintaFondoNivel6();
					break;
				}
				case 7: {
					desplazamientox = DESPLAZAENEMIGOSN7; // como es menos empiezan desplazandose a la izq
					velbombanodriza = VELBOMBANODRIZAN7;
					PintaInicioNivel(7);
					PintaFondoNivel7();
					break;
				}
				case 8: {
					desplazamientox = DESPLAZAENEMIGOSN8; // como es menos empiezan desplazandose a la izq
					velbombanodriza = VELBOMBANODRIZAN8;
					PintaInicioNivel(8);
					PintaFondoNivel8();
					break;
				}
				case 9: {
					desplazamientox = DESPLAZAENEMIGOSN9; // como es menos empiezan desplazandose a la izq
					velbombanodriza = VELBOMBANODRIZAN9;
					PintaInicioNivel(9);
					PintaFondoNivel9();
					break;
				}
			} // fin switch pintar fondo nivel

			//Paint texts
			//pintar puntos + record
			// pintar vidas

			//Paint elements
			//Paint accesories
			//Paint enemies

			IncializaPosiciones ();

			PintaSprites (enemabierta);
		
			KillKeyBuffer(); // por si se ha quedado un espacio en el bufer o cursor que no haga nada

			//Game Loop
			//Loop
			do {
				//Get player input
				switch(JoystickRead(TECLADO)) { // moviemiento
					case 3: // derecha
						mnave[0] = mnave[0] + VELJUGADORDES;
						break;
					case 7: // izquierda
						mnave[0] = mnave[0] - VELJUGADORDES;
						break;
				}

				if (TriggerRead(TECLADO) == ESPACIO) {
					if (mdisparo[1] == DESAPARECE) {
						mdisparo[0] = mnave[0];
						mdisparo[1] = LIMABADIS;
						
						Play(0);
					}
				}
				
				//Enemy spawn/initialization
				//(como aún no se pone la nave de la zona superior se deja limpio)
				// Mueve enemigos
				for (contador = 0; contador < NUMENEMIGOS; contador++)
					if (menemigosy[contador] != DESAPARECE)
						menemigosx[contador] += desplazamientox;

				// Mueve nodriza
				if (mnavenod[0][1] != DESAPARECE) {
					mnavenod[0][0] += 2;
					mnavenod[1][0] += 2;
				}
				
				// Mueve bomba nodriza
				if (mbomnod[1] != DESAPARECE) {
					mbomnod[1] += velbombanodriza;
				}

				//deteccion limites
					// limite lateral enemigos
					for (contador = 0; contador < NUMENEMIGOS; contador++) 
						if (menemigosx[contador] < LIMIZQENE || menemigosx[contador] > LIMDERENE)  {
							for (contador2 = 0; contador2 < NUMENEMIGOS; contador2++)
								if (menemigosy[contador2] != DESAPARECE) // si no está muerto desciende
									menemigosy[contador2] += VELENEMIGOSDES;

							desplazamientox *= -1; // cambio de sentido
							break;
						}

					// limite vertical enemigos
					for (contador = 0; contador < NUMENEMIGOS; contador++)
						if (menemigosy[contador] > LIMINFENEM && menemigosy[contador] != DESAPARECE) {
							vidas--;
							navemuerta = CIERTO;
							break;
						}

					// limite nave
					if (mnave[0] < LIMIZQJU) mnave[0] = LIMIZQJU;
					else if (mnave[0] > LIMDERJU) mnave[0] = LIMDERJU;

					// limite disparo
					if (mdisparo[1] > LIMARRDIS && mdisparo[1] != DESAPARECE) mdisparo[1] -= VELDISJU;
					else mdisparo[1] = DESAPARECE;

					// limite nave nodriza
					if (mnavenod[0][0] == 240) PonNodriza (FALSO);

					// limite nave nodriza
					if (mbomnod[1] >= LIMINFBOMNOD) mbomnod[1] = DESAPARECE;

				//Collision detection
					// comprobando colisión con naves enemigas
					for (contador = 0; contador < NUMENEMIGOS; contador++) {
						if (HayColisionDisparoNave(contador) == CIERTO && mdisparo[1] != DESAPARECE) {
							mdisparo[1] = DESAPARECE;
							menemigosy[contador] = DESAPARECE;
							quedannaves--; // restamos una nave a las que quedan vivas
							// si muere un enemigo se actualizn los puntos y record
							marcador += PUNTOSNAVEMUERTA;
							ActualizaPuntos();

							Play(1);
							
							break; // si muere un enemigo (ejemplo el 0) no hay necesidad de seguir exmainando
						}
					}

					// comprobando colisión con nave nodriza
					if (HayColisionDisparoNaveNodriza() == CIERTO && mdisparo[1] != DESAPARECE) {
						mdisparo[1] = DESAPARECE;
						PonNodriza(FALSO);
						marcador += PUNTOSNODRIZAMUERTA;
						ActualizaPuntos();

						Play(1);
					}

					// comprobando colisión de bomba de nave nodriza con nave
					if (HayColisionBombaNaveNodriza() == CIERTO) {
						vidas--;
						navemuerta = CIERTO;
						mbomnod[1] = DESAPARECE;
					}

				//Animaciones
					swapenemigos++;
					if (swapenemigos == SWAPENEMIGOS) {
						swapenemigos = 0;
						if (enemabierta == 0) enemabierta = 4;
						else enemabierta = 0;
					} 

				//Draw changes to screen
					// pinta puntos + record: (para no hacer comprobaciones/pintar innecesarias se pone antes)

					if (navemuerta == CIERTO) {
						navemuerta = FALSO;
						// muestra explosión
						ExplosionaNave();
						// repone posición enemigos y nave
						IncializaPosiciones ();

						PintaVidas();

						if (vidas == 0) break; // para agilizar salgo así en vez de una comprobación por iteracion
					}

					PintaSprites (enemabierta);

				// Retardo tras pintar para controlar la velocidad del programa
				Espera (ESPERABUCLE);

				//Otras Variable update/Checks
				// ver si tiene que aparecer la nodriza
				// y que probabiliad tiene de tirar bomba
				switch(nivel) {
					case 1: {
						salenavenod = SALENAVENODNIVEL1;
						probbombanodriza = PROBBOMBANODNIVEL1;
						break;
					}
					case 2: {
						salenavenod = SALENAVENODNIVEL2;
						probbombanodriza = PROBBOMBANODNIVEL2;
						break;
					}
					case 3: {
						salenavenod = SALENAVENODNIVEL3;
						probbombanodriza = PROBBOMBANODNIVEL3;
						break;
					}
					case 4: {
						salenavenod = SALENAVENODNIVEL4;
						probbombanodriza = PROBBOMBANODNIVEL4;
						break;
					}
					case 5: {
						salenavenod = SALENAVENODNIVEL5;
						probbombanodriza = PROBBOMBANODNIVEL5;
						break;
					}
					case 6: {
						salenavenod = SALENAVENODNIVEL6;
						probbombanodriza = PROBBOMBANODNIVEL6;
						break;
					}
					case 7: {
						salenavenod = SALENAVENODNIVEL7;
						probbombanodriza = PROBBOMBANODNIVEL7;
						break;
					}
					case 8: {
						salenavenod = SALENAVENODNIVEL8;
						probbombanodriza = PROBBOMBANODNIVEL8;
						break;
					}
					case 9: {
						salenavenod = SALENAVENODNIVEL9;
						probbombanodriza = PROBBOMBANODNIVEL9;
						break;
					}
				}
				if (Aleatorio(0,salenavenod) == 0 && mnavenod[0][1] == DESAPARECE) PonNodriza(CIERTO);

				// ver si tiene que disparar nodriza
				if (mnavenod[0][1] != DESAPARECE) // sólo tira bombas si nave nodriza está en pantalla
					if (TiraBombaNodriza() == CIERTO) {
						mbomnod[0] = mnavenod[1][0];
						mbomnod[1] = LIMSUPBOMNOD; 
					}

			//END Game Loop
			} while (quedannaves != NOQUEDANNAVES && vidas != 0); // vidas y nivel //Until NO LIVES / LEVEL/STAGE FINISHED

			//Otras Variable update/Checks
			if (quedannaves == NOQUEDANNAVES) nivel++; // si no quedan naves es porque pasamos de nivel
			
		//FIN New Level/stage
		} while (nivel < NOMASNIVELES && vidas != 0);//

		if (vidas == 0) //IF NO LIVES
			GameOver();
		else // IF NO MORE LEVELS
			EscenaFinJuego();		

	} while (1); //FIN Loop Game total del juego
} //END PROGRAM


void InicializaTiles () {
	char contador;
	char contador2;
	int  direccioncarac;
	
	HideDisplay();
	for (contador=0; contador < 2; contador++) {
		direccioncarac = (32 * 8);
		if (contador == 0)
			direccioncarac += TPB0;
		else
			direccioncarac += TPB1;
		VpokeFirst(direccioncarac);

		// CARACTER SP / 32
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);

		direccioncarac = (45 * 8);
		if (contador == 0)
			direccioncarac += TPB0;
		else
			direccioncarac += TPB1;
		VpokeFirst(direccioncarac);
		// CARACTER - / 45
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		VpokeNext(0b01111111);
		VpokeNext(0b00111111);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
		
		direccioncarac = (58 * 8);
		if (contador == 0)
			direccioncarac += TPB0;
		else
			direccioncarac += TPB1;
		VpokeFirst(direccioncarac);
		// CARACTER : / 58
		VpokeNext(0b00000000);
		VpokeNext(0b00011000);
		VpokeNext(0b00011000);
		VpokeNext(0b00000000);
		VpokeNext(0b00011000);
		VpokeNext(0b00011000);
		VpokeNext(0b00000000);
		VpokeNext(0b00000000);
	}

	for (contador=0; contador < 2; contador++) {
		direccioncarac = (48 * 8);
		if (contador == 0)
			direccioncarac += TPB0;
		else
			direccioncarac += TPB1;
		VpokeFirst(direccioncarac);
	
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
	}

	for (contador=0; contador < 2; contador++) {
		direccioncarac = (65 * 8);
		if (contador == 0)
			direccioncarac += TPB0;
		else
			direccioncarac += TPB1;
		VpokeFirst(direccioncarac);

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
	}


/**********************************************
	FORMA/PATRON ESTRELLAS
************************************************/
	for (contador = 0; contador < 3; contador++) {
		if (contador == 0)
			VpokeFirst(TPB0);
		else if (contador == 1)
			VpokeFirst(TPB1);
		else 
			VpokeFirst(TPB2);

		for (contador2 = 0; contador2 < 2; contador2 ++) {
			VpokeNext(0b00000000);
			VpokeNext(0b01000100);
			VpokeNext(0b00000000);
			VpokeNext(0b00010000);
			VpokeNext(0b00000000);
			VpokeNext(0b01000100);
			VpokeNext(0b00000000);
			VpokeNext(0b00000000);

			VpokeNext(0b00000000);
			VpokeNext(0b00001000);
			VpokeNext(0b00000000);
			VpokeNext(0b00100010);
			VpokeNext(0b00000000);
			VpokeNext(0b00001000);
			VpokeNext(0b00000000);
			VpokeNext(0b00000000);

			VpokeNext(0b00000000);
			VpokeNext(0b00010000);
			VpokeNext(0b00010000);
			VpokeNext(0b01111100);
			VpokeNext(0b00010000);
			VpokeNext(0b00010000);
			VpokeNext(0b00000000);
			VpokeNext(0b00000000);

			VpokeNext(0b00000000);
			VpokeNext(0b00000000);
			VpokeNext(0b00010000);
			VpokeNext(0b00111000);
			VpokeNext(0b00010000);
			VpokeNext(0b00000000);
			VpokeNext(0b00000000);
			VpokeNext(0b00000000);

			VpokeNext(0b00000000);
			VpokeNext(0b00000000);
			VpokeNext(0b00011000);
			VpokeNext(0b00111100);
			VpokeNext(0b00011000);
			VpokeNext(0b00000000);
			VpokeNext(0b00000000);
			VpokeNext(0b00000000);
		}
	}
// fin formas estrellas

	// FORMAS
	// patron tile 0 tierra plana
	VpokeFirst(TPB2 + 32 * 8);
	for (contador = 0; contador < 8; contador++) 
		VpokeNext(0); 

	// patron tile 1 tierra superior
	VpokeNext(42);
	VpokeNext(85);
	VpokeNext(170);
	VpokeNext(85);
	VpokeNext(0);
	VpokeNext(0);
	VpokeNext(0);
	VpokeNext(0);

	// patron tile 2 tierra plana nivel 2
	for (contador = 0; contador < 8; contador++) 
		VpokeNext(0); 

	// patron tile 3 tierra superior
	VpokeNext(42);
	VpokeNext(85);
	VpokeNext(170);
	VpokeNext(85);
	VpokeNext(0);
	VpokeNext(0);
	VpokeNext(0);
	VpokeNext(0);
	
	// pintar patrones vacios para limpiar pantalla
	for (contador = 0; contador < 8; contador++) {
		Vpoke(TPB0 + (255 * 8) + contador, 0); // último tile tileset 0
		Vpoke(TPB1 + (255 * 8) + contador, 0); // último tile tileset 1
		Vpoke(TPB2 + (255 * 8) + contador, 0); // último tile tileset 2
	}

	
	// COLORES
	// colores letras (realmente todos los caracteres blanco/blanco/blanco/blanco/gris/blanco/gris/gris fondo negro)
	// lo pongo todo así y luego coloreo los especiales
	for (contador = 0; contador < 255; contador++) {
		VpokeFirst(TCB0 + (contador * 8));
		VpokeNext(FILAGRIS);
		VpokeNext(FILAGRIS);
		VpokeNext(FILABLANCA);
		VpokeNext(FILABLANCA);
		VpokeNext(FILABLANCA);
		VpokeNext(FILABLANCA);
		VpokeNext(FILAGRIS);
		VpokeNext(FILAGRIS);
	}
	VpokeFirst(TCB1);
	for (contador = 0; contador < 255; contador++) {
		VpokeNext(FILAGRIS);
		VpokeNext(FILAGRIS);
		VpokeNext(FILABLANCA);
		VpokeNext(FILABLANCA);
		VpokeNext(FILABLANCA);
		VpokeNext(FILABLANCA);
		VpokeNext(FILAGRIS);
		VpokeNext(FILAGRIS);
	}

	
	// coloreando tile vacio
	VpokeFirst(14328);
	for (contador = 0; contador < 8; contador++) 
		VpokeNext(COLNEGRO);

	// coloreando tiles tierra
	for (contador = 0; contador < 8; contador++) {
		Vpoke(12288 + contador + 32 * 8, COLROJOOSC);	
		Vpoke(12296 + contador + 32 * 8, COLROJOOSC);
		Vpoke(12304 + contador + 32 * 8, COLVERDE);	
		Vpoke(12312 + contador + 32 * 8, COLVERDE);
	}

/**********************************************
	COLOR ESTRELLAS
************************************************/
	for (contador = 0; contador < 3; contador++) {
		if (contador == 0)
			VpokeFirst(TCB0);
		else if (contador == 1)
			VpokeFirst(TCB1);
		else 
			VpokeFirst(TCB2);

		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b01000001);
		
		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b01010001);

		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b01100001);
		
		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b01110001);
		
		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b10010001);
		
		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b10100001);
		
		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b10110001);
		
		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b11000001);
		
		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b11100001);
		
		for (contador2 = 0; contador2 < 8; contador2 ++) 
			VpokeNext(0b11110001);
	}
	
	// fin colores estrellas
	// colorear patrones vacios para limpiar pantalla
	for (contador = 0; contador < 8; contador++) {
		Vpoke(TCB0 + (255 * 8) + contador, COLNEGRONEGRO); // último tile tileset 0
		Vpoke(TCB1 + (255 * 8) + contador, COLNEGRONEGRO); // último tile tileset 1
		Vpoke(TCB2 + (255 * 8) + contador, COLNEGRONEGRO); // último tile tileset 2
	}

	ShowDisplay();
}


void PintaIntroInicio () {
	char contador;

	HideDisplay();

	LimpiaPantallaCompleta();

	PintaTileTexto (MAPT1, 3, 8, "HI-SCORE:");
	itoa(record, cadena);
	PintaTileTexto (MAPT1, 3, 18, cadena);
	PintaTileTexto (MAPT1, 7, 8, "SPACE INVADERS");
	PintaTileTexto (MAPT2, 5, 5, "A GAME FROM PACOSOFT");
	PintaTileTexto (MAPT2, 7, 6, "-- PRESS A KEY --");

	ShowDisplay();

	PulsaEspacio();

	for (contador = 0; contador < 20; contador++) {
		PintaTileTexto (MAPT2, 7, 6, "-- PRESS A KEY --");
		WAIT(8);
		PintaTileTexto (MAPT2, 7, 6, "                 ");
		WAIT(8);
	} 
}


void PintaFondoNivel1 () {
	char contador;

	HideDisplay();

	LimpiaPantallaCompleta();

	PintaTextos();
	PintaPuntos();
	PintaEstrellas();

	// pintar pantalla tierra
	VpokeFirst(MAPT3 + 32 * 6); // MAPT 3 fila 6 tile 1
	for(contador = 0; contador < 32; contador++) 
		VpokeNext(33);
	VpokeFirst(MAPT3 + 32 * 7); // MAPT 3 fila 7 tile 0
	for(contador = 0; contador < 32; contador++) 
		VpokeNext(32);

	ShowDisplay();
}


void PintaFondoNivel2 () {
	char contador;

	HideDisplay();

	LimpiaPantallaCompleta();

	PintaTextos();
	PintaPuntos();
	PintaEstrellas();

	// pintar pantalla tierra
	VpokeFirst(MAPT3 + 32 * 6); // MAPT 3 fila 6 tile 1
	for(contador = 0; contador < 32; contador++) 
		VpokeNext(35);
	VpokeFirst(MAPT3 + 32 * 7); // MAPT 3 fila 7 tile 0
	for(contador = 0; contador < 32; contador++) 
		VpokeNext(34);
	
	ShowDisplay();	
}


void PintaFondoNivel3 () {
	PintaFondoNivel1();
}


void PintaFondoNivel4 () {
	PintaFondoNivel2();
}


void PintaFondoNivel5 () {
	PintaFondoNivel1();
}


void PintaFondoNivel6 () {
	PintaFondoNivel2();
}


void PintaFondoNivel7 () {
	PintaFondoNivel1();
}

void PintaFondoNivel8 () {
	PintaFondoNivel2();
}

void PintaFondoNivel9 () {
	PintaFondoNivel1();
}


void IncializaPosiciones () {
	char contador;
	
	// como se pintan todas las naves se aprovecha para decir que están todas vivas	
	quedannaves = NUMENEMIGOS; // en principio todos los enemigos están vivos
	
	// enemigos
	for(contador = 0; contador < 3; contador++) {
		menemigosx[contador]     = (contador+1) * 35;
		menemigosx[contador + 3] = (contador+1) * 35;
		menemigosx[contador + 6] = (contador+1) * 35;
		menemigosy[contador]     = 40;
		menemigosy[contador + 3] = 64;
		menemigosy[contador + 6] = 88;
	}

	// nave
	mnave[0] = 70;
	mnave[1] = 160;
			
	// disparo
	mdisparo[0] = 70;
	mdisparo[1] = DESAPARECE;// equivale a disparo = falso (se pone el disparo en la matriz a oculto)

	// nodriza
	PonNodriza (FALSO);

	// bomba nodriza
	mbomnod[0] = 0;
	mbomnod[1] = DESAPARECE;// equivale a disparo = falso (se pone el disparo en la matriz a oculto)
}


void PintaSprites (char enemabierta) {
	char contador;

	// enemigos
	for (contador = 0; contador < 3; contador++) {
		PutSprite(contador + 1,  0 + enemabierta, menemigosx[contador]    , menemigosy[contador]    , 3);
		PutSprite(contador + 4,  8 + enemabierta, menemigosx[contador + 3], menemigosy[contador + 3], 5);
		PutSprite(contador + 7, 16 + enemabierta, menemigosx[contador + 6], menemigosy[contador + 6], 7);
	}
	// nave
	PutSprite(10, 24, mnave[0], mnave[1], COLGRIS); //***************************************************
	PutSprite(15, 48, mnave[0], mnave[1], COLAZULOSC);
	// disparo
	PutSprite(11, 32, mdisparo[0], mdisparo[1], COLBLANCO); 
	// nodriza
	PutSprite(12, 36, mnavenod[0][0], mnavenod[0][1], COLROVIOLETA);
	PutSprite(13, 40, mnavenod[1][0], mnavenod[1][1], COLROVIOLETA);
	// bomba nodriza
	PutSprite(14, 44, mbomnod[0], mbomnod[1], COLBLANCO);
}


void PintaTextos() {
	PintaTileTexto (MAPT1, 0, 0, "SCORE:           HI-SCORE: ");
	PintaTileTexto (MAPT1, 2, 0, "LIVES:           LEVEL: ");
	PintaPuntos();
	PintaVidas();
	PintaNivel();
}


void PintaTileTexto (int mapt, char fila, char col, char* texto) {
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
}


void PintaPuntos() {
	itoa(marcador, cadena);
	PintaTileTexto (MAPT1, 0, 7, cadena);

	itoa(record, cadena);
	PintaTileTexto (MAPT1, 0, 27, cadena);
}


void PintaVidas() {
	itoa(vidas, cadena);
	PintaTileTexto (MAPT1, 2, 7, cadena);
}


void PintaNivel() {
	itoa(nivel, cadena);
	PintaTileTexto (MAPT1, 2, 24, cadena);
}


int Aleatorio (int limiteInf, int limiteSup) { // Random number between a and b-1
	return(rand()%(limiteSup-limiteInf)+limiteInf);
}


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
}


void Espera (int tiempo) {
	int contador;
	
	for(contador = 0; contador < tiempo; contador++) {}
}


// muestra explosión y sonido
void ExplosionaNave () {
	char contador;

	Play(1);

	for (contador = 0; contador < 15; contador++) {
		PutSprite(10, 28, mnave[0], mnave[1], 6);
		WAIT(5);
		PutSprite(10, 28, mnave[0], mnave[1], 9);
		WAIT(5);
	}
}


char HayColisionDisparoNave (char numnave) {
	int poscolix;
	int poscoliy;

	poscolix = (menemigosx[numnave] + 8) - (mdisparo[0] + 8);
	poscoliy = (menemigosy[numnave] + 8) - (mdisparo[1] + 8);

	if (Absoluto(poscolix) < UMBRALCOLISION)
		if (Absoluto(poscoliy) < UMBRALCOLISION)
			return CIERTO;

	return FALSO;
}


char HayColisionDisparoNaveNodriza () {
	int poscolix;
	int poscoliy;

	poscolix = (mnavenod[0][0] + 8) - (mdisparo[0] + 8);
	poscoliy = (mnavenod[0][1] + 8) - (mdisparo[1] + 8);

	if (Absoluto(poscolix) < UMBRALCOLISIONNODRIZA)
		if (Absoluto(poscoliy) < UMBRALCOLISIONNODRIZA)
			return CIERTO;

	return FALSO;		
}


char HayColisionBombaNaveNodriza () {
	int poscolix;
	int poscoliy;

	poscolix = (mbomnod[0] + 8) - (mnave[0] + 8);
	poscoliy = (mbomnod[1] + 8) - (mnave[1] + 8);

	if (Absoluto(poscolix) < UMBRALCOLISIONBOMBA)
		if (Absoluto(poscoliy) < UMBRALCOLISIONBOMBA)
			return CIERTO;

	return FALSO;				
}


void LimpiaPantallaCompleta () { // pone todos los tiles con el negro
	int contador;
	
	// saca los sprites de la pantalla (en teoría los enemigos no es necesario pero ...)
	for(contador = 0; contador < 3; contador++) {
		menemigosy[contador]     = DESAPARECE;
		menemigosy[contador + 3] = DESAPARECE;
		menemigosy[contador + 6] = DESAPARECE;
	}
	mnave[1] = DESAPARECE;
	mdisparo[1] = DESAPARECE;
	PonNodriza(FALSO);
	mbomnod[1] = DESAPARECE;

	PintaSprites (ENEMABIERTA);

	// pinta toda la pantalla con el tile 255 negro completo
	for(contador = 0; contador < 256; contador++) {
		Vpoke(MAPT1 + contador, 255); // limpiar 1º tercio
		Vpoke(MAPT2 + contador, 255); // limpiar 2º tercio
		Vpoke(MAPT3 + contador, 255); // limpiar 3º tercio
	}
}


void LimpiaFondo () { // limpia sólo la parte de las estrellas (no marcador ni suelo)
	char contador;

	// limpiando 1º tercio
	for (contador = 95; contador < 255; contador++) 
		Vpoke(MAPT1 + contador, 255); // filas 3 a 8
	
	// limpiando 2º tercio
	for (contador = 0; contador < 255; contador++) 
		Vpoke(MAPT2 + contador, 255);

	// limpiando 3º tercio
	for (contador = 0; contador < 192; contador++) 
		Vpoke(MAPT3 + contador, 255); // filas 3 a 8
}


void PintaEstrellas () {
	char contador;

	// pintando estrellas 1º y 3º tercio
	for (contador = 0; contador < 8; contador++) {
		Vpoke(MAPT1 + Aleatorio(95,255), Aleatorio(0,9)); // filas 3 a 8
		Vpoke(MAPT3 + Aleatorio(0,191), Aleatorio(0,9)); // fila 1 a 6
	}
	// pintando estrellas 2º tercio
	for (contador = 0; contador < 15; contador++) 
		Vpoke(MAPT2 + Aleatorio(0,255),Aleatorio(0,9));
}


void GameOver () {
	HideDisplay();
	LimpiaPantallaCompleta();
	PintaTileTexto (MAPT2, 3, 10, "GAME OVER");
	ShowDisplay();

	WAIT(300);
}


void EscenaFinJuego () {
	HideDisplay();
	LimpiaPantallaCompleta();
	PintaTileTexto (MAPT2, 1, 8, "CONGRATULATIONS");
	PintaTileTexto (MAPT2, 3, 6, "YOU BEAT THE ENEMIES");
	ShowDisplay();

	WAIT(300);
}


void PulsaEspacio () {
	KillKeyBuffer();
	do {
	} while (Inkey() == NOSEPULSATECLA);
	KillKeyBuffer();
}


int Absoluto (int valor) {
	if (valor < 0) 
		return valor * -1;

	return valor;
}


// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second. 
void WAIT(int cicles) {
  int i;

  for(i=0;i<cicles;i++) HALT;
  return;
}


void PonNodriza (char mostrar) {
	mnavenod[0][0] = 0;
	mnavenod[1][0] = 16;
	
	if (mostrar == CIERTO) {
		mnavenod[0][1] = 30;
		mnavenod[1][1] = 30;

Play(2);

//***************************************************************************************


	} else {
		mnavenod[0][1] = DESAPARECE;
		mnavenod[1][1] = DESAPARECE;
	}
}

// se comienza con dos vidas (no hay vida 0)
// la vida extra se ganará en el nivel 3
void SumaVidaExtra (void) {
	LimpiaPantallaCompleta();

	// felicidades has ganado una vida extra
	PintaTileTexto (MAPT2, 2, 5, "YOU WON AN EXTRA LIFE");
	vidas++;

	WAIT(250);
}


void PintaInicioNivel (char nivel) {
	LimpiaPantallaCompleta();

	// Alcanzado nivel "nivel"
	PintaTileTexto (MAPT2, 2, 6, "YOU REACHED LEVEL: ");
	itoa(nivel, cadena);
	PintaTileTexto (MAPT2, 2, 25, cadena);

	WAIT(250);
}


void ActualizaPuntos () {
	// si el marcador supera los puntos del record se actualiza el record
	if (marcador > record) record = marcador;

	// pongo aquí pintar puntos porque es posible que en una iteración
	// no sea necesario actualizar puntos o añadir vidas
	PintaPuntos();
	PintaVidas();
}

/*si la nave nodriza está en pantalla (se comprueba antes) y no hay ya una bomba en pantalla
tirará bomba cuando pase por la vertical de la nave prota
(pero referencia 240) probabilidad (probbombanodriza) varia con la fase
 devuelve CIERTO o FALSO*/
char TiraBombaNodriza () {
	if (mbomnod[1] == DESAPARECE) // si no hay más bombas
		if (mnavenod[1][0] == mnave[0]) // 
			if (Aleatorio(0, 100) < probbombanodriza) // si da la casualidad
				return CIERTO;

	return FALSO;
}


void Play(char fx)
{
  char contadorsonidos;
  char contadorsonidos2;

  switch (fx) 
  {
    case 0: // disparo nave
      // opcion 1 de sonido
      SoundFX(0,&fx01);
      
      // opcion 2 de sonido
    /*
      for(contadorsonidos = 0; contadorsonidos < 240; contadorsonidos += 10) {
    	Sound(8,15);
    	Sound(0,contadorsonidos);
    	for(contadorsonidos2=0;contadorsonidos2<100;contadorsonidos2++);
      }
      Sound(0,0);
	*/

      break;
      
    case 1: // explosion
      SoundFX(1,&fx02);
      break;

    case 2: // nodriza
      SoundFX(1,&fx03);
      break;
  }
}


// TODO
// pintar niveles diferentes
// efectos de sonidos
// musica
// guardar en fichero aparte datos sprites y tiles

