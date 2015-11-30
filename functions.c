#include "functions.h"

SDL_Texture* createTexture(char nameImage[], SDL_Renderer* renderer)
{
	// takes an image address and a renderer as input and converts everything to a texture
	SDL_Surface* img = IMG_Load(nameImage);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, img);
	SDL_FreeSurface(img);
	return texture;
}

SDL_Rect updateCharacterPosition(enum DIRECTION keyPressed, SDL_Rect destPos) // even if all it does is moving a texture
{
	switch(keyPressed)
	{
		//case UP: destPos.y -= srcPos.h; break;
		//case DOWN: destPos.y += srcPos.h; break;
		case RIGHT: destPos.x += 4; break;
		case LEFT: destPos.x -= 4; break;
		default: destPos.x +=0; break;
	}
	if (destPos.x > WIDTH - 20)
	{
		destPos.x = 0;
	}
	else if (destPos.x < 0)
	{
		destPos.x = WIDTH - 20;
	}
	return destPos;
}


Player initializePlayer(char nameImageIn[], SDL_Renderer* rendererIn)
{
	Player player;
	// basically the constructor of the player
	player.src.w = SPRITEBLOCK_W;
	player.src.h = SPRITEBLOCK_H;

	player.dst.x = WIDTH/2;
	player.dst.y = HEIGHT - (HEIGHT/10);
	player.dst.w = 30;//playerIn.src.w;
	player.dst.h = 30;//playerIn.src.h;

	player.texture = createTexture(nameImageIn, rendererIn);

	return player;
}

Bullet initializeBullet(char nameImageIn[], SDL_Renderer* rendererIn,int  speed)
{
	// initializes the bullet, basically a constructor
	Bullet bullet;
	bullet.srcPos.x = 0;
	bullet.srcPos.y = 0;
	bullet.srcPos.w = SPRITEBLOCK_W;
	bullet.srcPos.h = SPRITEBLOCK_H;

	bullet.dstPos.w = 15;
	bullet.dstPos.h = 15;

	bullet.active = 0;
	bullet.speed = speed;
	bullet.texture = createTexture(nameImageIn, rendererIn);
	return bullet;
}

SDL_Rect updateBulletPosition(Bullet bullet, int direction)
{
	// updates the position of the bullet once shot
	if (direction == 1)
		bullet.dstPos.y -= bullet.speed;
	else
		bullet.dstPos.y += bullet.speed;
	return bullet.dstPos;
}

int checkCollision(Bullet bullet, Invader invaders[ROWS][COLS])
{
	/* this is the function that checks for collision between the bullet and the Invader
	 * if there is a collision the invader is set inactive
	 * the function will return 1 if there is a collision and 0 if there isn't one
	 * the return values will be used in the main to deactivate the bullet
	 */
	for(int r=0; r < ROWS; ++r)
	{
		for(int c=0; c < COLS; ++c)
		{
			if (SDL_HasIntersection(&invaders[r][c].pos, &bullet.dstPos) == SDL_TRUE && invaders[r][c].active == 1)
			{
				invaders[r][c].active = 0;
				return 1;
			}
		}
	}
	return 0;
}

int DestroyBullet(Bullet bullet)
{
	if (bullet.dstPos.y < 0 || bullet.dstPos.y > HEIGHT )
		return 0;
	else
		return 1;
}

int getLeftMostInvaderColumn(Invader invaders[ROWS][COLS])
{
	/*
	* returns the biggest index of the invaders matrix
	* this is useful for collision with the screen
	*/
	int min = COLS;
	for(int r = 0; r < ROWS; ++r)
	{
		for(int c = 0; c < COLS; ++c)
		{
			if (invaders[r][c].active == 1 && c < min)
					min = c;
		}
	}
	return min;
}

int getRightMostInvaderColumn(Invader invaders[ROWS][COLS])
{
	/*
	 * returns the smallest index of the invaders matrix
	 * this is useful for collision with the screen
	 */

	int max = 0;
	for(int r = 0; r < ROWS; ++r)
	{
		for(int c = 0; c < COLS; ++c)
		{
			if (invaders[r][c].active == 1 && c > max)
				max = c;
		}
	}
	return max;
}

// this function needs to be merged with Delay()
int UpdateSprite()
{
	static int block;
	static int frame = 0;
	int speed = 2;

	if (block % (SPRITEBLOCK_H/speed) == 0)
	{
		frame = (block*speed);
	}
	block++;
	if (block == ((SPRITEBLOCK_H)*SPRITEBLOCKS)/speed)
		block = 0;
	return frame;
}


int Delay()
{
	/*
	* this function needs to be deleted and merged with the UPDATESPRITE function,
	* they really do the same thing, I just need to make it more general
	*/
	static int block;
	static int frame = 0;
	int speed = 200;

	if (block % (SPRITEBLOCK_H/speed) == 0)
	{
		frame = (block*speed);
	}
	block++;
	if (block == ((SPRITEBLOCK_H)*SPRITEBLOCKS)/speed)
		block = 0;
	return frame;
}



int countActiveInvaders(Invader invaders[ROWS][COLS])
{
	int count = 0;
	for(int r=0; r<ROWS; ++r)
	{
		for(int c=0; c<COLS; ++c)
		{
			if (invaders[r][c].active == 1)
			{
				++count;
			}
		}
	}
	return count;
}


// // // // // // // // no need to keep these two functions anymore
void activeInvaderList(Invader invaders[ROWS][COLS], Invader retList[])
{
	int count = 0;
	for(int r=0; r<ROWS; ++r)
	{
		for(int c=0; c<COLS; ++c)
		{
			if (invaders[r][c].active == 1)
			{
				retList[count] = invaders[r][c];
				count++;
			}
		}
	}
}


void closestInvaderList(Invader invaders[ROWS][COLS], Invader retList[])
{
	int count = 0;
	int closest[COLS];
	for (int i = 0; i < COLS; ++i)
	{
			closest[i] = 255;
	}

	for(int r = ROWS-1; r >= 0; --r)
	{
		for(int c = 0; c < COLS; ++c)
		{
			int isclosest = 0;

			if (invaders[r][c].active == 1 && r == ROWS-1)
			{
				invaders[r][c].closest = 1;
				isclosest = 1;
				closest[c] = r;
			}

			else if (invaders[r+1][c].active == 0 && invaders[r][c].active == 1 && closest[c] == 255)
			{
				invaders[r][c].closest = 1;
				isclosest = 1;
				closest[c] = r;
			}

			if (isclosest != 0)
			{
				++count;
			}
		}
	}

	for (int j = 0; j < COLS; ++j)
	{
		if(closest[j] != 255)
		{
			int x = (int)closest[j];
			retList[j] = invaders[x][j];
		}
	}
}

// // // // // // // //


int ShootingInvaders(Invader invaders[ROWS][COLS], Invader retList[])
{
	/*
	* this functions couts the closest invaders to the player
	* this function is useful because I only want the closest invaders to shoot at the player
	* the function controls returns the number of the "closest" invaders and set this ones to "closest"
	*/
	int count = 0;
	int closest[COLS];
	for (int i = 0; i < COLS; ++i)
	{
			closest[i] = 255;
	}
	// here I am doing a (backward) for loop to check the closest one
	// I am doing it backwards because it is easier that the closest one has the biggest index
	for(int r = ROWS-1; r >= 0; --r)
	{
		for(int c = 0; c < COLS; ++c)
		{
			int isclosest = 0;

			if (invaders[r][c].active == 1 && r == ROWS-1)
			{
				invaders[r][c].closest = 1;
				isclosest = 1;
				closest[c] = r;
			}

			else if (invaders[r+1][c].active == 0 && invaders[r][c].active == 1 && closest[c] == 255)
			{
				invaders[r][c].closest = 1;
				isclosest = 1;
				closest[c] = r;
			}

			if (isclosest != 0)
			{
				++count;
			}
		}
		int index = 0;
		for (int j = 0; j < COLS; ++j)
		{
			if(closest[j] != 255)
			{
				int x = (int)closest[j];
				retList[index] = invaders[x][j];
				++index;
			}
		}
	}
	if (count > 0)
		return count;
	else return 1; // I need to do this check in order to not make the program crash :(
}


SDL_Rect invaderShoot(Invader invaders[ROWS][COLS])
{
	srand((int)time(NULL));
	SDL_Rect ret;

	Invader activeInvaders[COLS];

	int count = ShootingInvaders(invaders, activeInvaders);

	int r = (rand()*rand()) %count;
	if (r >= 0 && r < count)
	{
		if (activeInvaders[r].active == 1)
			ret = activeInvaders[r].pos;
	}
	return ret;
}


