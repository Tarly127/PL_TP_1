#ifndef __AUX_H__
#define __AUX_H__

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct comment *Comment;
typedef struct Stack *stack;

Comment initComment();
Comment newComment(char* id, char* name, char* comBody, int likes, char* date, char* time);
void addReply(Comment cP, Comment cF);

struct Stack* createStack(int capacity);
int isFull(struct Stack* stack);
int isEmpty(struct Stack* stack);
void push(struct Stack* stack, Comment cm);
Comment pop(struct Stack* stack);
Comment peek(struct Stack* stack);
int checkTop(struct Stack* stack);
int incBottom(struct Stack* stack);
int checkBottom(struct Stack* stack);

void freeComment(Comment c);
void freeStack(stack s);

void parseJSONAux(Comment c, FILE* fp, int depth);
void parseJSON(stack st);

char* getTimestamp(char* date);

#endif
