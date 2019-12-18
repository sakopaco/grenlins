#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_graph1.h"
#include "fusion-c/header/vdp_sprites.h"


static unsigned char collision=0;


typedef unsigned char BYTE;

static const BYTE spritea[] = {
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xE0,0xBF,0xff,0x3F,0xE2,0x02,0x02
};
static const BYTE spriteb[] = {
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xE0,0xBF,0xff,0x3F,0xE2,0x02,0x02
};


void WAIT(int cicles) {
  unsigned int i;

  for(i=0;i<cicles;i++)
    if (SpriteCollision()==1) collision=1;
    while(IsVsync())
    {}
  return;
}



char SpriteCollisionZ(void)
{
  //return((readstatus(0) >> 5)  & 0x01);
	return((VDPstatus(0) >> 5)  & 0x01);
}

void main(void)
{
    int contador;
    int color;

    color = 15;

    Cls();
    Screen(2);
    SetColors(14,4,1);

    // CONFIGURA SPRITES
    SpriteReset();   
    Sprite16();
    SpriteSmall();   

    SetSpritePattern((char)0,  spritea,       (char)32);
    SetSpritePattern((char)4,  spriteb,       (char)32);

    PutSprite(1, 0, 50 , 100  , color);


    for (contador = 0; contador < 190; contador++) {
        PutSprite(2, 4, 55 , contador , color);
        WAIT(200);

        if (collision == 1) { // IT IS ALWAYS FALSE AND IT SEEMS NOT WORKING
            color = 6;
            PutSprite(2, 4, 55 , contador , color);
            collision=0;
            break;
        }
    }
    WaitKey();
    Screen(0);
    Exit(0);
}
