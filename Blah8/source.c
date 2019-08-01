#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "chip8.h"

#include <SDL.h>
#include <string.h>

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

typedef struct Config
{
	char gamePath[255];
	int resWidth;
	int resHeight;
	int fullscreen;
	uint8_t objectColor[3];	//0 R, 1 G, 2 B
	uint8_t backgroundColor[3];
	int keyCode[16];
} Config;



SDL_Window  *gWindow = NULL;
SDL_Surface *gSurface = NULL;
SDL_Renderer *gRenderer = NULL;

int quit = 0;

void loadGame(char* path);

int initScreen(Config config);
void pressedKeys(SDL_Event *e, int *scanCodes);
void renderScreenNow(Config config);
void splitRGB(uint8_t *configColor, uint32_t colorString);
void splitKeycodes(int *keycodes, char *keyCodeString);
Config readConfigFile();


int main(void)
{
	Config config = readConfigFile();

	initChip8();

	loadGame(config.gamePath);

	//Start up SDL and create window
	if (!initScreen(config))
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		SDL_Event e;

		while (!quit)
		{
			emulateCycle();

			pressedKeys(&e, config.keyCode);
			if (drawFlag == 1)
			{ 
				renderScreenNow(config);
			}
			drawFlag = 0;			
		}
	}
	return 0;
}


int initScreen(Config config)
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
									config.resWidth, config.resWidth / 2, SDL_WINDOW_SHOWN);

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
				//fullscreen
				SDL_SetWindowFullscreen(gWindow, config.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 1);
			}
		}
	}

	return success;
}

void pressedKeys(SDL_Event *e, int *scanCodes)
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

			e->key.keysym.sym;
			int i = 0;
			for (i = 0; i < 16; i++)
			{
				if (e->key.keysym.scancode == scanCodes[i])
				{
					key[i] = changer;
					break;
				}
			}

		}
	}
	
}

Config readConfigFile()
{
	Config config;

	FILE *fl = fopen("options.config", "r");
	if (fl == NULL)
	{
		printf("Config file couldn't be loaded");
		exit(0);
	}

	char keyCodeString[64];
	uint32_t objectColorString = 0, backgroundColorString = 0;

	fscanf(fl, "%[^\n]%*c%d\n%d\n%d\n%x\n%x\n%s", config.gamePath,
		   &(config.resWidth), &(config.resHeight), &(config.fullscreen),
		   &objectColorString, &backgroundColorString,
		   keyCodeString);

	splitRGB(&(config.objectColor), objectColorString);
	splitRGB(&(config.backgroundColor), backgroundColorString);
	splitKeycodes(&(config.keyCode), keyCodeString);

	return config;
}

void splitRGB(uint8_t *configColor, uint32_t colorString)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		configColor[i] = (colorString & (0xff000000 >> 8 * i)) >> (24 - 8 * i);
	}
}

void splitKeycodes(int *keycodes, char *keyCodeString)
{
	int i;
	char *oneKeyCodeString = strtok(keyCodeString,  ";");
	for (i = 0; i < 16; i++)
	{
		keycodes[i] = atoi(oneKeyCodeString);
		oneKeyCodeString = strtok(NULL, ";");
	}
}


void renderScreenNow(Config config)
{
	int y, x;
	SDL_SetRenderDrawColor(gRenderer, config.backgroundColor[0], config.backgroundColor[1], config.backgroundColor[2], 0xFF);
	SDL_RenderClear(gRenderer);

	for (y = 0; y < CHIP8_HEIGHT; y++)
	{
		for (x = 0; x < CHIP8_WIDTH;)
		{
			if (gfx[y][x] == 1)
			{
				int width, height;

				for (width = 1; width + x < CHIP8_WIDTH && gfx[y][width + x] == 1; width++);
				int done = 0;
				for (height = 1; y + height < CHIP8_HEIGHT; height++)
				{
					int widthControlForHeight;
					for (widthControlForHeight = 0; widthControlForHeight < width; widthControlForHeight++)
					{
						if (gfx[y + height][x + widthControlForHeight] == 0)
						{
							done = 1;
							break;
						}
					}

					if (done) { break; }
				}
				

				int pixelSize = round(config.resWidth / 64.0f);
				SDL_Rect fillRect = { x * pixelSize, y * pixelSize, pixelSize * width, pixelSize * height };
				SDL_SetRenderDrawColor(gRenderer, 
									   config.objectColor[0], config.objectColor[1], config.objectColor[2], 0xFF);
				SDL_RenderFillRect(gRenderer, &fillRect);

				x += width;

				
			}
			else
			{
				x++;
			}
		}
	}
	SDL_RenderPresent(gRenderer);
}
