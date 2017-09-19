#pragma once

typedef struct Stack
{
	unsigned short stack[16];
	unsigned short sp;
}Stack;


Stack *stackInit();
unsigned short stackPop(Stack *st);
void stackPush(Stack *st, unsigned short v);
