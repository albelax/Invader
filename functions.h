#ifndef FUNCTIONS_H__
#define FUNCTIONS_H__
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "player.h"
#include "bullet.h"
#include "Invader.h"
#include <time.h>

#define WIDTH 800
#define HEIGHT 600
#define SPRITEBLOCK_W 88
#define SPRITEBLOCK_H 64
#define SPRITEBLOCKS 2

enum DIRECTION{UP,DOWN,LEFT,RIGHT,NONE};

SDL_Texture* createTexture(char nameImage[], SDL_Renderer* renderer);

SDL_Rect updateCharacterPosition(enum DIRECTION keyPressed, SDL_Rect destPos);


Player initializePlayer(char nameImageIn[], SDL_Renderer* rendererIn);

Bullet initializeBullet(char nameImageIn[], SDL_Renderer* rendererIn,int  speed);

SDL_Rect updateBulletPosition(Bullet bullet, int direction);

int checkCollision(Bullet bullet, Invader invaders[ROWS][COLS]);

int DestroyBullet(Bullet bullet);

int getLeftMostInvaderColumn(Invader invaders[ROWS][COLS]);

int getRightMostInvaderColumn(Invader invaders[ROWS][COLS]);

// this function needs to be merged with Delay()
int UpdateSprite();

int Delay();

int countActiveInvaders(Invader invaders[ROWS][COLS]);


// // // // // // // // no need to keep these two functions anymore
void activeInvaderList(Invader invaders[ROWS][COLS], Invader retList[]);

void closestInvaderList(Invader invaders[ROWS][COLS], Invader retList[]);
// // // // // // // //


int ShootingInvaders(Invader invaders[ROWS][COLS], Invader retList[]);

SDL_Rect invaderShoot(Invader invaders[ROWS][COLS]);



#endif
