
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
