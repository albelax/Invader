#ifndef PLAYER_H
#define PLAYER_H


typedef struct
{
	SDL_Rect src;
	SDL_Rect dst;
	int prevPos[2];
	SDL_Texture* texture;

}Player;


#endif // PLAYER_H

