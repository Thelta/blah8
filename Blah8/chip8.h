#pragma once
#include "datastruct.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define local_persist static 
#define global_variable static
#define internal static

extern int drawFlag;
extern uint8_t gfx[32][64];
extern uint8_t key[16];

void initChip8();
void emulateCycle();
void loadGame(char* path);
internal void somethingWrong(uint16_t opcode);
internal void ii0(uint16_t opcode);
internal void ii00e0(uint16_t opcode);
internal void ii00ee(uint16_t opcode);
internal void ii1nnn(uint16_t opcode);
internal void ii2nnn(uint16_t opcode);
internal void ii3xkk(uint16_t opcode);
internal void ii4xkk(uint16_t opcode);
internal void ii5xy0(uint16_t opcode);
internal void ii6xkk(uint16_t opcode);
internal void ii7xkk(uint16_t opcode);
internal void ii8(uint16_t opcode);
internal void ii8xy0(uint16_t opcode);
internal void ii8xy1(uint16_t opcode);
internal void ii8xy2(uint16_t opcode);
internal void ii8xy3(uint16_t opcode);
internal void ii8xy4(uint16_t opcode);
internal void ii8xy5(uint16_t opcode);
internal void ii8xy6(uint16_t opcode);
internal void ii8xy7(uint16_t opcode);
internal void ii8xye(uint16_t opcode);
internal void ii9xy0(uint16_t opcode);
internal void iiannn(uint16_t opcode);
internal void iibnnn(uint16_t opcode);
internal void iicxkk(uint16_t opcode);
internal void iidxyn(uint16_t opcode);
internal void iie(uint16_t opcode);
internal void iiex9e(uint16_t opcode);
internal void iiexa1(uint16_t opcode);
internal void iif(uint16_t opcode);
internal void iifx07(uint16_t opcode);
internal void iifx0a(uint16_t opcode);
internal void iifx15(uint16_t opcode);
internal void iifx18(uint16_t opcode);
internal void iifx1e(uint16_t opcode);
internal void iifx29(uint16_t opcode);
internal void ifx33(uint16_t opcode);
internal void iifx55(uint16_t opcode);
internal void iifx65(uint16_t opcode);
