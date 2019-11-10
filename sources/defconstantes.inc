
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