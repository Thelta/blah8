#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640

typedef struct Stack
{
	unsigned short stack[16];
	unsigned short sp;
}Stack;

typedef enum  Keypad
{
	KEY_1, KEY_2, KEY_3, KEY_4,
	KEY_Q, KEY_W, KEY_E, KEY_R,
	KEY_A, KEY_S, KEY_D, KEY_F,
	KEY_Z, KEY_X, KEY_C, KEY_V
}Keypad;

unsigned char memory[4096] = { 0 };
unsigned short opcode = 0;
unsigned char V[16]; //registers
FILE *fli = NULL;

unsigned char gfx[32][64] = { 0 };

unsigned char key[16] = { 0 };

unsigned short I = 0; //index register
unsigned short pc = 0x200;	//program register

unsigned char delayTimer = 0;
unsigned char soundTimer = 0;

unsigned char chip8_fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

int drawFlag = 0 ;

SDL_Window  *gWindow = NULL;
SDL_Surface *gSurface = NULL;
SDL_Renderer *gRenderer = NULL;

int quit = 0;


Stack *stackInit();
unsigned short stackPop(Stack *st);
void stackPush(Stack *st, unsigned short v);

void loadGame();
void emulateCycle(Stack *st);

int initScreen();
void pressedKeys(SDL_Event *e);
void renderScreenNow();
void renderer();
void debug();

int main(int argc, char* args[]) 
{
	Stack *st = stackInit();

	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	loadGame();

	srand(time(NULL));

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
			if (opcode == 0x8070)
				printf("come here");
			emulateCycle(st);

			pressedKeys(&e);
			if (drawFlag == 1)
				renderScreenNow();
			drawFlag = 0;
			/*if (opcode != 0xf00a)
				debug();*/
			
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



Stack* stackInit()
{
	Stack *st;
	st = (Stack*)malloc(sizeof(Stack));

	st->sp = 0;

	return st;
}

unsigned short stackPop(Stack *st)
{
		return st->stack[--(st->sp)];
}

void stackPush(Stack *st, unsigned short v)
{
		st->stack[(st->sp)++] = v;
}

void loadGame()
{
	FILE *fl = fopen("game.c8", "rb");
	int i;
	if (fl == NULL)
		printf("uuuuuuuuuuuck");

	fseek(fl, 0, SEEK_END);
	long bufferSize = ftell(fl);
	fseek(fl, 0, SEEK_SET);

	for (i = 0; i < bufferSize; i++)
	{
		memory[512 + i] = fgetc(fl);
	}

	fclose(fl);
}

void emulateCycle(Stack *st)
{
	opcode = memory[pc] << 8 ;
	opcode = opcode | memory[pc + 1];

	switch (opcode & 0xF000)
	{
		case 0x0000:
		{
			switch (opcode & 0x000F)
			{
				case 0x0000:
				{
					for (int i = 0; i < 32; i++)
					{
						for (int j = 0; j < 64; j++)
						{
							gfx[i][j] = 0;
						}
					}
					pc += 2;
					break;
				}
				case 0x000E:
				{
					pc = stackPop(st) + 2;
					break;
				}
				default:
				{
					printf("something went wong %x", opcode);
					break;
				}
			}
			break;
		}
		case 0x1000:
		{
			pc = opcode & 0x0FFF;
			break;
		}
		case 0x2000:
		{
			stackPush(st, pc);
			pc = opcode & 0x0FFF;
			break;
		}
		case 0x3000:
		{
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
			break;
		}
		case 0x4000:
		{
			if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
			break;
		}
		case 0x5000:
		{
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
			break;
		}
		case 0x6000:
		{
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
			break;
		}
		case 0x7000:
		{
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			pc += 2;
			break;
		}
		case 0x8000:
		{
			switch (opcode & 0x000F)
			{
				case 0x0000:
				{
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				}
				case 0x0001:
				{
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				}
				case 0x0002:
				{
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				}
				case 0x0003:
				{
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				}
				case 0x0004:
				{
					if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				}
				case 0x0005:
				{
					if (V[(opcode & 0x0F00) >> 8] >= V[(opcode & 0x00F0) >> 4])
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				}
				case 0x0006:
				{
					if (V[(opcode & 0x0F00) >> 8] & 1)
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
					pc += 2;
					break;
				}
				case 0x0007:
				{
					if (V[(opcode & 0x0F00) >> 8] <= V[(opcode & 0x00F0) >> 4])
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				}
				case 0x000E:
				{
					if (V[(opcode & 0x0F00) >> 8] >> 7)
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
					pc += 2;
					break;
				}
				default:
				{
					printf("something went wong %x", opcode);
					int kk;
					scanf("%d", &kk);

					break;
				}
			}
			break;
		}
		case 0x9000:
		{
			if (V[opcode & 0x0F00 >> 8] != V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
			break;
		}
		case 0xA000:
		{
			I = opcode & 0x0FFF;
			pc += 2;
			break;
		}
		case 0xB000:
		{
			pc = V[0] + (opcode & 0x0FFF);
			break;
		}
		case 0xC000:
		{
			V[(opcode & 0x0F00) >> 8] = (unsigned char (rand() % 256)) & (opcode & 0x00FF);
			pc += 2;
			break;
		}
		case 0xD000:
		{
			unsigned short x = V[(opcode & 0x0f00) >> 8];
			unsigned short y = V[(opcode & 0x00f0) >> 4];
			unsigned short height = (opcode & 0x000f);
			unsigned short pixel;

			V[0xF] = 0;

			for (int i = 0; i < height; i++)
			{
				pixel = memory[I + i];

				for (int j = 0; j < 8; j++)
				{
					if (pixel & (0x80 >> j))
					{
						if (gfx[(y + i) % 32][(x + j) % 64] == 1)
							V[0xF] = 1;
						gfx[(y + i) % 32][(x + j) % 64] ^= 1;
					}
				}
			}

			drawFlag = 1;
			pc += 2;
			break;
		}
		case 0xE000:
		{
			switch (opcode & 0x00f0)
			{
				case 0x0090:
				{
					if (key[V[(opcode & 0x0F00) >> 8]] != 0)
						pc += 4;
					else
						pc += 2;
					break;
				}
				case 0x00A0:
				{
					if (key[V[(opcode & 0x0F00) >> 8]] == 0)
						pc += 4;
					else
						pc += 2;
					break;
				}
				
			}
			break;
		}
		case 0xF000:
		{
			switch (opcode & 0x00FF)
			{
				case 0x0007:
				{
					V[(opcode & 0x0F00) >> 8] = delayTimer;
					pc += 2;
					break;
				}
				case 0x000A:
				{
					int pressed = 0;
					for (int i = 0; i < 16; i++)
					{
						if (key[i] == 1)
						{
							V[(opcode & 0x0F00) >> 8] = i;
							pressed = 1;
						}
					}
					
					if (pressed != 1)
						return;

					pc += 2;
					break;
				}
				case 0x0015:
				{
					delayTimer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				}
				case 0x0018:
				{
					soundTimer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				}
				case 0x001E:
				{
					if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF)	
						V[0xF] = 1;
					else
						V[0xF] = 0;
					I += V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				}
				case 0x0029:
				{
					I = V[(opcode % 0x0F00) >> 8] * 5;
					pc += 2;
					break;
				}
				case 0x0033:
				{
					memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;

					break;
				}
				case 0x0055:
				{
					int a = (opcode & 0x0F00) >> 8;

					for (int i = 0; i < a + 1; i++)
						memory[I + i] = V[i];
					I += a + 1;
					pc += 2;
					break;
				}
				case 0x0065:
				{
					int a = (opcode & 0x0F00) >> 8;

					for (int i = 0; i < a + 1; i++)
						V[i] = memory[I + i];
					I += a + 1;
					pc += 2;
					break;
				}

			}
			break;
		}
		default:
		{
			printf("something went wong %x", opcode);
			break;
		}
	}
	if (delayTimer > 0)
		delayTimer--;
	if (soundTimer > 0)
	{
		if (soundTimer == 1)
			printf("%c", 7);
		soundTimer--;
	}
	printf("%x\n", opcode);
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

void debug()
{
	if (fli == NULL)
		fli = fopen("debug.txt", "wb");

	fprintf(fli, "\n opcode :%x, I : %d \n\n", opcode, I);

	for (int i = 0; i < 16; i++)
	{
		fprintf(fli, "V%d: %x\t", i, V[i]);
	}
	fprintf(fli, "\n\n");

	if ((opcode & 0xF000) == 0xD000)
	{
		for (int i = 0; i < 32; i++)
		{
			for (int j = 0; j < 64; j++)
				fprintf(fli, "%d", gfx[i][j]);
			fprintf(fli, "\n");
		}
	}

}

