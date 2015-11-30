#ifndef BULLET_H
#define BULLET_H

typedef struct
{
	SDL_Rect startingPos;
	SDL_Rect dstPos;
	SDL_Rect srcPos;
	int active;
	int speed;
	SDL_Texture* texture;
}Bullet;

#endif // BULLET_H

