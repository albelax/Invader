#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "Invader.h"
#include "functions.h"
#include "player.h"

void initializeInvaders(Invader invaders[ROWS][COLS]);
void updateInvaders(Invader invaders[ROWS][COLS]);

void drawInvaders(SDL_Renderer *ren,SDL_Texture *tex,Invader invaders[ROWS][COLS]);

int main()
{
	/*//////////////////////////////////////////// Initialization SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		printf("%s\n",SDL_GetError());
		return EXIT_FAILURE;
	}
	// we are now going to create an SDL window

	SDL_Window *win = 0;
	win = SDL_CreateWindow("Invaders", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (win == 0)
	{
		printf("%s\n",SDL_GetError());
		return EXIT_FAILURE;
	}
	// the renderer is the core element we need to draw, each call to SDL for drawing will need the
	// renderer pointer
	SDL_Renderer *ren = 0;
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// check to see if this worked
	if (ren == 0)
	{
		printf("%s\n",SDL_GetError() );
		return EXIT_FAILURE;
	}

	// this will set the background colour to white.
	// however we will overdraw all of this so only for reference
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	/*//////////////////////////////////////////// End Initialization SDL */

	Invader invaders[ROWS][COLS];
	initializeInvaders(invaders);
	// initialise SDL and check that it worked otherwise exit
	// see here for details http://wiki.libsdl.org/CategoryAPI

  char image[] = "/home/i7434091/Lectures/PPP/Invaders/invaders.png"; //InvaderA2.bmp";
	SDL_Texture *tex = 0;
	tex = createTexture(image, ren);

  char playerImageName[] = "/home/i7434091/Lectures/PPP/Invaders/Player.bmp";
	Player player = initializePlayer(playerImageName, ren);
  char bulletImageName[] = "/home/i7434091/Lectures/PPP/Invaders/Bullet.png";
	Bullet playerBullet = initializeBullet(bulletImageName, ren, 30);
	Bullet invadersBullet = initializeBullet(bulletImageName, ren,30);

	if(tex == 0 || player.texture == 0 || playerBullet.texture == 0)
	{
		printf("IMG_Load: %s\n", IMG_GetError());
    return EXIT_FAILURE;
	}


	int quit=0;
	enum DIRECTION playerDirection = NONE;
	// now we are going to loop forever, process the keys then draw
	while (quit !=1)
	{
		if (countActiveInvaders(invaders) == 0)
			quit = 1;
		SDL_Event event;
		// grab the SDL even (this will be keys etc)
		while (SDL_PollEvent(&event))
		{
			// look for the x of the window being clicked and exit
			if (event.type == SDL_QUIT || countActiveInvaders(invaders) == 0)
				quit = 1;

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				// if we have an escape quit
					case SDLK_ESCAPE : quit=1; break;
					case SDLK_LEFT : playerDirection = LEFT; break;
					case SDLK_RIGHT : playerDirection = RIGHT; break;
					default: break;
				}

				if (event.key.keysym.sym == SDLK_SPACE && playerBullet.active == 0)
				{
					playerBullet.dstPos.x = playerBullet.startingPos.x;
					playerBullet.dstPos.y = playerBullet.startingPos.y;
					playerBullet.active = 1;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				if(playerDirection == LEFT && event.key.keysym.sym == SDLK_LEFT)
					playerDirection = NONE;
				if(playerDirection == RIGHT && event.key.keysym.sym == SDLK_RIGHT)
					playerDirection = NONE;
			}
		}
		// now we clear the screen (will use the clear colour set previously)
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);

		playerBullet.startingPos.x = player.dst.x + (player.src.w/10); // WARNING: magic numbers here
		playerBullet.startingPos.y = player.dst.y - (player.src.h/4);  // WARNING: magic numbers here

		if(playerBullet.active == 1)
		{
			SDL_RenderCopy(ren, playerBullet.texture, &playerBullet.srcPos, &playerBullet.dstPos);
			playerBullet.dstPos = updateBulletPosition(playerBullet,1);

			if (checkCollision(playerBullet, invaders) || playerBullet.dstPos.y < 0)
				playerBullet.active = 0;
			else
				playerBullet.active = 1;
		}

		if (Delay() == 0 && invadersBullet.active == 0)
		{
				invadersBullet.dstPos.x = invaderShoot(invaders).x;
				invadersBullet.dstPos.y = invaderShoot(invaders).y;
				invadersBullet.active = 1;
		}


		if(invadersBullet.active == 1)
		{
			SDL_RenderCopy(ren, invadersBullet.texture, &invadersBullet.srcPos, &invadersBullet.dstPos);
			invadersBullet.dstPos = updateBulletPosition(invadersBullet,0);
			invadersBullet.active = DestroyBullet(invadersBullet);
		}

		updateInvaders(invaders);
		drawInvaders(ren,tex,invaders);
		// Up until now everything was drawn behind the scenes.
		// This will show the new, red contents of the window.

		player.dst = updateCharacterPosition(playerDirection, player.dst);
		SDL_RenderCopy(ren, player.texture, &player.src, &player.dst);
		SDL_RenderPresent(ren);
	}
	SDL_Quit();
	return 0;
}


void initializeInvaders(Invader invaders[ROWS][COLS])
{
	SDL_Rect pos;
	pos.w = SPRITEWIDTH;
	pos.h = SPRITEHEIGHT;
	int ypos = GAP;

	for(int r = 0; r < ROWS; ++r)
	{
		int xpos = GAP;
		for(int c = 0; c < COLS; ++c)
		{
			pos.x=xpos+SPRITEWIDTH;
			pos.y=ypos+SPRITEHEIGHT;
			xpos+=(GAP+SPRITEWIDTH);
			invaders[r][c].pos=pos;
			invaders[r][c].active=1;
			invaders[r][c].closest = 0;
			invaders[r][c].frame=0;
			if(r==0)
				invaders[r][c].type=TYPE1;
			else if(r==1 || r==2)
				invaders[r][c].type=TYPE2;
			else
				invaders[r][c].type=TYPE3;
		}
		ypos += (GAP + SPRITEHEIGHT);
	}
}

void drawInvaders(SDL_Renderer *ren, SDL_Texture *tex, Invader invaders[ROWS][COLS])
{
	SDL_Rect SrcR;
	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = SPRITEBLOCK_W;
	SrcR.h = SPRITEBLOCK_H;
	SrcR.y = UpdateSprite(2);

	for(int r = 0; r < ROWS; ++r)
	{
		for(int c = 0; c < COLS; ++c)
		{
			if(invaders[r][c].active)
			{
				switch(invaders[r][c].type)
				{
					case TYPE1 : SrcR.x = 0; break;
					case TYPE2 : SrcR.x = SrcR.w; break;
					case TYPE3 : SrcR.x = SrcR.w*2; break;
				}
				SDL_RenderFillRect(ren,&invaders[r][c].pos);
				SDL_RenderCopy(ren, tex,&SrcR,&invaders[r][c].pos);
			}
		}
	}
}

void updateInvaders(Invader invaders[ROWS][COLS])
{
	enum DIR{ FWD, BWD};
	static int DIRECTION = FWD;
	int yinc = 0;
	int biggestIndex = getRightMostInvaderColumn(invaders);
	int smallestIndex = getLeftMostInvaderColumn(invaders);
	//printf("%i \t %i \n", biggestIndex, COLS);
	if(invaders[0][COLS].pos.x >= (WIDTH-SPRITEWIDTH)-((biggestIndex+1)*(SPRITEWIDTH+GAP)))
	{
		DIRECTION = BWD;
		yinc = GAP;
	}
	else if(invaders[0][smallestIndex].pos.x <= SPRITEWIDTH)
	{
		DIRECTION = FWD;
		yinc = GAP;
	}

	for(int r = 0; r < ROWS; ++r)
	{
		for(int c = 0; c < COLS; ++c)
		{
			if(DIRECTION == FWD)
				invaders[r][c].pos.x += 1;
			else
				invaders[r][c].pos.x -= 1;
			invaders[r][c].pos.y += yinc;
		}
	}
}
