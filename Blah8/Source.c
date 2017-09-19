#include <SDL.h>
#include "chip8.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640

typedef enum  Keypad
{
	KEY_1, KEY_2, KEY_3, KEY_4,
	KEY_Q, KEY_W, KEY_E, KEY_R,
	KEY_A, KEY_S, KEY_D, KEY_F,
	KEY_Z, KEY_X, KEY_C, KEY_V
}Keypad;

SDL_Window  *gWindow = NULL;
SDL_Surface *gSurface = NULL;
SDL_Renderer *gRenderer = NULL;

int quit = 0;

void loadGame(char* path);

int initScreen();
void pressedKeys(SDL_Event *e);
void renderScreenNow();

int main(int argc, char* args[]) 
{
	initChip8();

	loadGame(args[1]);

	

	//Start up SDL and create window
	if (!initScreen())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		SDL_Event e;

		while (!quit)
		{
			emulateCycle();

			pressedKeys(&e);
			if (drawFlag == 1)
			{ 
				renderScreenNow();
			}
			drawFlag = 0;			
		}
	}
	return 0;
}

int initScreen()
{
	//Initialization flag
	int success = 1;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = 0;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Blah8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
									SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = 0;
		}
		else
		{
			//Get window surface 
			gSurface = SDL_GetWindowSurface( gWindow ); //Fill the surface white 
			SDL_FillRect( gSurface, NULL, SDL_MapRGB( gSurface->format, 128, 64, 32 ) ); 
			SDL_UpdateWindowSurface( gWindow ); 

			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = 0;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 1);
			}
		}
	}

	return success;
}

void pressedKeys(SDL_Event *e)
{
	//Handle events on queue
	while (SDL_PollEvent(e) != 0)
	{
		//User requests quit
		if (e->type == SDL_QUIT)
		{
			quit = 1;
		}
		else if ((e->type == SDL_KEYDOWN) || (e->type == SDL_KEYUP))
		{
			int changer = 0;
			if (e->type == SDL_KEYDOWN)
				changer = 1;

			switch (e->key.keysym.sym)
			{
				case SDLK_1:
				{
					key[KEY_1] = changer;
					break;
				}
				case SDLK_2:
				{
					key[KEY_2] = changer;
					break;
				}
				case SDLK_3:
				{
					key[KEY_3] = changer;
					break;
				}
				case SDLK_4:
				{
					key[KEY_4] = changer;
					break;
				}
				case SDLK_q:
				{
					key[KEY_Q] = changer;
					break;
				}
				case SDLK_w:
				{
					key[KEY_W] = changer;
					break;
				}
				case SDLK_e:
				{
					key[KEY_E] = changer;
					break;
				}
				case SDLK_r:
				{
					key[KEY_R] = changer;
					break;
				}
				case SDLK_a:
				{
					key[KEY_A] = changer;
					break;
				}
				case SDLK_s:
				{
					key[KEY_S] = changer;
					break;
				}
				case SDLK_d:
				{
					key[KEY_D] = changer;
					break;
				}
				case SDLK_f:
				{
					key[KEY_F] = changer;
					break;
				}
				case SDLK_z:
				{
					key[KEY_Z] = changer;
					break;
				}
				case SDLK_x:
				{
					key[KEY_X] = changer;
					break;
				}
				case SDLK_c:
				{
					key[KEY_C] = changer;
					break;
				}
				case SDLK_v:
				{
					key[KEY_V] = changer;
					break;
				}
			}

		}
	}
	
}

void renderScreenNow()
{
	int i, j;
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 64; j++)
		{
			if (gfx[i][j] == 1)
			{
				SDL_Rect fillRect = { j * 20, i * 20, 20, 20 };
				SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 0xFF); 
				SDL_RenderFillRect(gRenderer, &fillRect);
				
			}
		}
	}
	SDL_RenderPresent(gRenderer);
}
