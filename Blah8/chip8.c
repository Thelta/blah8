#include "chip8.h"

global_variable Stack *st;

global_variable uint8_t memory[4096] = { 0 };
global_variable uint16_t opcode = 0;
global_variable uint8_t V[16]; //registers

uint8_t gfx[32][64] = { 0 };

uint8_t key[16] = { 0 };

global_variable uint16_t I = 0; //index register
global_variable uint16_t pc = 0x200;	//program register

global_variable uint8_t delayTimer = 0;
global_variable uint8_t soundTimer = 0;

global_variable uint8_t chip8_fontset[80] =
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

int drawFlag = 0;

void (*instructionIdentifier[])(uint16_t) = { ii0, ii1nnn, ii2nnn, ii3xkk, ii4xkk, ii5xy0, ii6xkk, ii7xkk, ii8, ii9xy0, iiannn, iibnnn, iicxkk, iidxyn, iie, iif };
void (*type0[])(uint16_t) = { ii00e0, ii00ee };
void (*type8[])(uint16_t) = {ii8xy0, ii8xy1, ii8xy2, ii8xy3, ii8xy4, ii8xy5, ii8xy6, ii8xy7, 
							somethingWrong, somethingWrong, somethingWrong, somethingWrong, somethingWrong, somethingWrong,  ii8xye};
void (*typee[])(uint16_t) = { iiex9e, iiexa1 };
void (*typef[])(uint16_t) = { iifx07, iifx15, somethingWrong, iifx0a, iifx18, ifx33, somethingWrong, somethingWrong,
							  iifx29, somethingWrong, iifx1e, somethingWrong, somethingWrong, iifx55, somethingWrong,
							  somethingWrong, iifx65 };


void initChip8()
{
	st = stackInit();

	for (int i = 0; i < 80; ++i)
	{
		memory[i] = chip8_fontset[i];
	}

	srand(time(NULL));
}

void emulateCycle()
{
	//fprintf(stderr, "pc: %d\n", pc);
	opcode = memory[pc] << 8;
	opcode = opcode | memory[pc + 1];
	//fprintf(stderr, "opcode: %x\n", opcode);
	instructionIdentifier[(opcode & 0xf000) >> 12](opcode);

	if (delayTimer > 0)
	{
		delayTimer--;
	}
	if (soundTimer > 0)
	{
		if (soundTimer == 1)
		{
			printf("%c", 7);
		}
			
		soundTimer--;
	}

}

void loadGame(char* path)
{
	FILE *fl = fopen(path, "rb");
	int i;
	if (fl == NULL)
	{
		printf("Game couldn't loaded");
		exit(0);
	}

	fseek(fl, 0, SEEK_END);
	long bufferSize = ftell(fl);
	fseek(fl, 0, SEEK_SET);

	for (i = 0; i < bufferSize; i++)
	{
		memory[512 + i] = fgetc(fl);
	}

	fclose(fl);
}


internal void somethingWrong(uint16_t opcode)
{
	printf("Something is wrong");
	exit(-1);
}

internal void ii0(uint16_t opcode)
{
	if (opcode != 0xee && opcode != 0xe0)
	{
		somethingWrong(opcode);
	}
	uint16_t identifier = (opcode & 0x000e);

	//there are only two instructions 00e0 and 00ee.by dividing 14 we end up with 0 and 1.
	type0[identifier / 14](opcode);
}

internal void ii00e0(uint16_t opcode)
{
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			gfx[i][j] = 0;
		}
	}
	pc += 2;
}

internal void ii00ee(uint16_t opcode)
{
	pc = stackPop(st) + 2;
}

internal void ii1nnn(uint16_t opcode)
{
	pc = opcode & 0x0FFF;
}

internal void ii2nnn(uint16_t opcode)
{
	stackPush(st, pc);
	pc = opcode & 0x0FFF;
}

internal void ii3xkk(uint16_t opcode)
{
	if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
		pc += 4;
	else
		pc += 2;
}

internal void ii4xkk(uint16_t opcode)
{
	if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
		pc += 4;
	else
		pc += 2;
}

internal void ii5xy0(uint16_t opcode)
{
	if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
		pc += 4;
	else
		pc += 2;
}

internal void ii6xkk(uint16_t opcode)
{
	V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
	pc += 2;
}

internal void ii7xkk(uint16_t opcode)
{
	V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
	pc += 2;
}

internal void ii8(uint16_t opcode)
{
	uint16_t identifier = (opcode & 0x000f);
	type8[identifier](opcode);
}

internal void ii8xy0(uint16_t opcode)
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
	pc += 2;
}

internal void ii8xy1(uint16_t opcode)
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
	pc += 2;
}

internal void ii8xy2(uint16_t opcode)
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
	pc += 2;
}

internal void ii8xy3(uint16_t opcode)
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
	pc += 2;
}

internal void ii8xy4(uint16_t opcode)
{
	if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
		V[0xF] = 1;
	else
		V[0xF] = 0;
	V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
	pc += 2;
}

internal void ii8xy5(uint16_t opcode)
{
	if (V[(opcode & 0x0F00) >> 8] >= V[(opcode & 0x00F0) >> 4])
		V[0xF] = 1;
	else
		V[0xF] = 0;
	V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
	pc += 2;
}

internal void ii8xy6(uint16_t opcode)
{
	if (V[(opcode & 0x0F00) >> 8] & 1)
		V[0xF] = 1;
	else
		V[0xF] = 0;
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
	pc += 2;
}

internal void ii8xy7(uint16_t opcode)
{
	if (V[(opcode & 0x0F00) >> 8] <= V[(opcode & 0x00F0) >> 4])
		V[0xF] = 1;
	else
		V[0xF] = 0;
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
	pc += 2;
}

internal void ii8xye(uint16_t opcode)
{
	if (V[(opcode & 0x0F00) >> 8] >> 7)
		V[0xF] = 1;
	else
		V[0xF] = 0;
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
	pc += 2;
}

internal void ii9xy0(uint16_t opcode)
{
	if (V[opcode & 0x0F00 >> 8] != V[(opcode & 0x00F0) >> 4])
		pc += 4;
	else
		pc += 2;
}

internal void iiannn(uint16_t opcode)
{
	I = opcode & 0x0FFF;
	pc += 2;
}

internal void iibnnn(uint16_t opcode)
{
	pc = V[0] + (opcode & 0x0FFF);
}

internal void iicxkk(uint16_t opcode)
{
	V[(opcode & 0x0F00) >> 8] = ((uint8_t)(rand() % 256)) & (opcode & 0x00FF);
	pc += 2;
}

internal void iidxyn(uint16_t opcode)
{
	uint16_t x = V[(opcode & 0x0f00) >> 8];
	uint16_t y = V[(opcode & 0x00f0) >> 4];
	uint16_t height = (opcode & 0x000f);
	uint16_t pixel;

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
}

internal void iie(uint16_t opcode)
{
	uint16_t identifier = ((opcode & 0x00ff) - 0x009e) / 3;
	typee[identifier](opcode);
}

internal void iiex9e(uint16_t opcode)
{
	if (key[V[(opcode & 0x0F00) >> 8]] != 0)
		pc += 4;
	else
		pc += 2;
}

internal void iiexa1(uint16_t opcode)
{
	if (key[V[(opcode & 0x0F00) >> 8]] == 0)
		pc += 4;
	else
		pc += 2;
}

internal void iif(uint16_t opcode)
{
	/*a simple hash.multiply tens digits to 3.then sum it with unit digits after that substract 7 smallest number in hash. */
	uint16_t identifier1 = ((opcode & 0x00f0) >> 4) * 3;
	uint16_t identifier2 = (opcode & 0x00f);
	uint16_t hash = (identifier1 + identifier2) - 7;

	typef[hash](opcode);
}

internal void iifx07(uint16_t opcode)
{
	V[(opcode & 0x0F00) >> 8] = delayTimer;
	pc += 2;
}

internal void iifx0a(uint16_t opcode)
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
}

internal void iifx15(uint16_t opcode)
{
	delayTimer = V[(opcode & 0x0F00) >> 8];
	pc += 2;
}

internal void iifx18(uint16_t opcode)
{
	soundTimer = V[(opcode & 0x0F00) >> 8];
	pc += 2;
}

internal void iifx1e(uint16_t opcode)
{
	if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF)
		V[0xF] = 1;
	else
		V[0xF] = 0;
	I += V[(opcode & 0x0F00) >> 8];
	pc += 2;
}

internal void iifx29(uint16_t opcode)
{
	I = V[(opcode % 0x0F00) >> 8] * 5;
	pc += 2;
}

internal void ifx33(uint16_t opcode)
{
	memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
	memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
	memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
	pc += 2;
}

internal void iifx55(uint16_t opcode)
{
	int a = (opcode & 0x0F00) >> 8;

	for (int i = 0; i < a + 1; i++)
	{
		memory[I + i] = V[i];
	}
	I += a + 1;
	pc += 2;
}

internal void iifx65(uint16_t opcode)
{
	int a = (opcode & 0x0F00) >> 8;

	for (int i = 0; i < a + 1; i++)
	{
		V[i] = memory[I + i];
	}
		
	I += a + 1;
	pc += 2;
}
