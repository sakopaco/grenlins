
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

  // HACE QUE SUENE EL PLAYER. PARECE QUE TIENE QUE ESTAR EN BUCLE TODO EL RATO (AUNQUE SEA EN EL BUCLE PRINCIPAL)
  // HASTA QUE SE PULSE UNA TECLA
  InitPSG(); // RESETEA EL PSG PARA SONIDO O MÚSICA
  KillKeyBuffer();
  do {
    Halt();
    DisableInterupt();
    PT3Rout(); // CARGA DATOS DE LA CANCIÓN EN PSG
    PT3Play(); // TOCA LOS DATOS DE LA CANCIÓN
    EnableInterupt();
  } while (Inkey() == NOSEPULSATECLA);
  KillKeyBuffer();
  PT3ParaPlayer(); // PARA LA CANCION DEL PLAYER
 

  for (contador = 0; contador < 20; contador++) {
    PonerTileLocate (MAPT2, 7, 6, "-- PULSA ESPACIO --");
    Espera (ESPINTROESCENA);
    PonerTileLocate (MAPT2, 7, 6, "                   ");
    Espera (ESPINTROESCENA);
  }
} // FIN PintarPantallaInicialJuego


// FUNCION: PONE LOS TILES DE LETRAS PARA ESCRIBIR EN PANTALLA
// ENTRADAS: 
// tercio: EN QUE TERCIO PONE LOS TILES DE LAS LETRAS 1->3
// color: COLORES PREDEFINIDO POR LINEA PARA LETRAS (VER VARIABLES DEFINIDAS COLORLETRAVERDE, COLORLETRABLANCO)
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
//      YA DE ANTES (POR ALIGERAR)
// ENTRADAS: 
// inicio: DIRECCIÓN DE INICIO DE CADA CARACTER A COLOREAR EN LA TABLA DE COLORES DE LA VRAM SCREEN 2
// tipo: NUMERO PARA (CON EL SWITCH) ELEGIR QUE COLOR LE PONEMOS A CADA CARACTER
// SALIDAS: -
void PonerColorTileLetra (int inicio, BYTE tipo) {
  VpokeFirst(inicio);
  switch(tipo) {
    case COLORLETRAVERDE: {
      VpokeNext(TILEVERDEOSCURO);
      VpokeNext(TILEVERDEOSCURO);
      VpokeNext(TILEVERDECLARO);
      VpokeNext(TILEVERDECLARO);
      VpokeNext(TILEVERDECLARO);
      VpokeNext(TILEVERDECLARO);
      VpokeNext(TILEVERDEOSCURO);
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
//      ESTAN COLOCADAS Y EN SU POSICIÓN ASCII
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
  BYTE contador;

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
      PonerTileLocate (MAPT2, 3, 1, "SONAR QUE ESTAS EN UNA TIENDA");
      PonerTileLocate (MAPT2, 4, 1, "MISTERIOSA COMPRANDO UNA");
      PonerTileLocate (MAPT2, 5, 1, "MASCOTA SE RAPIDO PARA QUE EL");
      PonerTileLocate (MAPT2, 6, 1, "DUENO NO PIERDA LA PACIENCIA");
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

  for(contador = 18; contador--; ) Espera (ESPTEXTOFASE);
} // FIN PintaTileTexto


// FUNCION: CARGA EL FONDO DE PANTALLA, DEPENDE DE LA ESCENA QUÉ FONDO CARGA
// ENTRADAS: 
// SALIDAS: -
void CargaFondoJuego () {
  int contador;

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
  PonerTextosNivelEscena();
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
void PonerTextosNivelEscena () {
  itoa(nivel, cadena);
  PonerTileLocate (MAPT3, 4, 29, cadena);

  itoa(escena, cadena);
  PonerTileLocate (MAPT3, 6, 29, cadena);
}// FIN PonerTextosNivelEscena


// FUNCION: PINTA EL CONTADOR A DECREMENTAR EN LA ZONA INFERIOR
// ENTRADAS: -
// SALIDAS: -
void PonerTextosContador () {
  itoa(contadorEscena, cadena);
  PonerTileLocate (MAPT3, 5, 15, cadena);
}// FIN PonerTextosEscena



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



// FUNCION: ACTUALIZA LOS DATOS DE LOS SPRITES DEL PROTA Y MUESTRA AL PROTA POR PANTALLA
// ENTRADAS: 
// escena: LA ESCENA DEL JUEGO QUE SE TRATE (EL MOVIMIENTO DEL PROTA PUEDE VARIAR)
// direccion: DIRECCIONES DE LA FUNCION JoystickRead (1 arriba, 3 derecha ...)
// SALIDAS: -
void MueveProta (BYTE escena, BYTE direccion) {
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



// FUNCION: INICIALIZA LOS VALORES DE LA ESTRUCTURA SE SPRITES NO PROTA SEGÚN ESCENA Y TIPO
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
      enemigo->velocidadX = (BYTE)VELGRENHOR1;
      enemigo->velocidadY = (BYTE)VELGRENVER1;
      enemigo->contSiguienteEscena = (BYTE)6;
      enemigo->resetContador = (BYTE)6;
      enemigo->plano = (BYTE)5;
      enemigo->escena1 = (BYTE)48;
      enemigo->escena1i = (BYTE)40;
      enemigo->escena2i = (BYTE)44;
      enemigo->escena1d = (BYTE)48;
      enemigo->escena2d = (BYTE)52;
      enemigo->escena5d = (BYTE)56; // grenbueno_cae
      enemigo->escena6d = (BYTE)60; // grenbueno_tiraobj
      break;
    }
  }
}//  FIN inicializaSpriteEnemigo



