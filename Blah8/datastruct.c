#include "datastruct.h"

#include <stdlib.h>
#include <stdio.h>

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

