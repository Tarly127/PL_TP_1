#include "aux.h"

struct comment{
	char* id;
	char* name;
	char* comBody;
	int   upvote;
	int   hasReplies;
	char* date;
	char* timestamp;
	int   numberOfReplies;
	struct comment** replies;
};

struct Stack {
	int top;
	int capacity;
	int bottom;
	Comment* cms;
};



//Funções relacionadas com COMMENT

Comment initComment(){
	Comment c = (struct comment*)malloc(sizeof(struct comment));
	c->id = NULL;
	c->name = NULL;
	c->comBody = NULL;
	c->upvote = 0;
	c->hasReplies = 0;
	c->date = NULL;
	c->timestamp = NULL;
	c->numberOfReplies = 0;
	c->replies = (struct comment**)malloc(sizeof(struct comment*));

	return c;

}

Comment newComment(char* id, char* name, char* comBody, int likes, char* date, char* time){
	Comment c = (struct comment*)malloc(sizeof(struct comment));
	c->id = id;
	c->name = name;
	c->comBody = comBody;
	c->upvote = likes;
	c->hasReplies = 0;
	c->date = date;
	c->timestamp = time;
	c->numberOfReplies = 0;
	c->replies = (struct comment**)malloc(sizeof(struct comment*));

	return c;
}

void addReply(Comment cP, Comment cF){
	(cP->numberOfReplies)++;
	(cP->hasReplies) = 1;
  	(cP->replies) = (struct comment**)realloc((cP->replies), ((cP->numberOfReplies) ) * sizeof(struct comment*));
	(cP->replies)[(cP->numberOfReplies) - 1] = cF;
}

//Funções para esvaziar as estruturas

void freeComment(Comment c){
	free(c->id);
	free(c->comBody);
	free(c->date);
	free(c->timestamp);
  for(int i = 0; i < c->numberOfReplies; i++){
		freeComment((c->replies)[i]);
	}
  free(c->replies);
  free(c);
}

void freeStack(stack s){
	int n = 0;
	for(int i = 0; i<s->bottom; i++){
		freeComment((s->cms)[i]);
	}

	free(s);
}

//Funções relacionadas com a estrutura STACK

struct Stack* createStack(int capacity){
	struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->bottom = 0;
	(stack->cms) = (struct comment**)malloc(stack->capacity * sizeof(struct comment*));
	return stack;
}

int isFull(struct Stack* stack){
	return stack->top == stack->capacity - 1;
}

int isEmpty(struct Stack* stack){
	return stack->top < (stack->bottom);
}

void push(struct Stack* stack, Comment cm){
	if (!isFull(stack)){
	  	(stack->cms)[++(stack->top)] = cm;
	}
}

Comment pop(struct Stack* stack){
	if (isEmpty(stack)) return NULL;
	return (stack->cms)[(stack->top)--];
}

Comment peek(struct Stack* stack){
	if (isEmpty(stack)) return NULL;
	return (stack->cms)[(stack->top)];
}

int checkTop(struct Stack* stack){
	return stack->top;
}

int incBottom(struct Stack* stack){
	if(stack->bottom == (stack->capacity) - 1) return -1;
	else return ++(stack->bottom);
}

int checkBottom(struct Stack* stack){
	return stack->bottom;
}

//Função para impressão no formato JSON

void parseJSONAux(Comment c, FILE* fp, int depth){
	for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	fprintf(fp, "\"id\":\"%s\",\n", c->id);
	for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	fprintf(fp, "\"user\":\"%s\",\n", c->name);
	for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	fprintf(fp, "\"date\":%s,\n", c->date);
	for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	fprintf(fp, "\"timestamp\":%s,\n", c->timestamp);
	for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	fprintf(fp, "\"commentText\":\"%s\",\n", c->comBody);
	for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	fprintf(fp, "\"likes\":%d,\n", c->upvote);
	for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	if(c->hasReplies == 0) fprintf(fp, "\"hasReplies\":TRUE,\n");
	else fprintf(fp, "\"hasReplies\":FALSE,\n");
	for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	fprintf(fp, "\"numberOfReplies\":%d,\n", c->numberOfReplies);
	for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	fprintf(fp, "\"replies\":[", c->id);
	for(int i = 0; i < c->numberOfReplies; i++){
		fprintf(fp, "\n");
		for(int i = 0; i < depth+2; i++) fprintf(fp, "\t");
		fprintf(fp, "{\n");
		parseJSONAux((c->replies)[i], fp, depth+1);
		fprintf(fp, "\n");
		for(int i = 0; i < depth+2; i++) fprintf(fp, "\t");
		if(i == (c->numberOfReplies)-1) fprintf(fp, "}\n");
		else fprintf(fp, "},\n");
	}
	if(c->numberOfReplies != 0) for(int i = 0; i < depth+1; i++) fprintf(fp, "\t");
	fprintf(fp, "]");
}

void parseJSON(stack st){
	FILE* fp = fopen("sol_comments.json", "w");
	if(fp != NULL){
		fprintf(fp, "\"commentThread\":[");
		for(int i = 0; i < st->bottom; i++){
			fprintf(fp, "\n\t{\n");
			parseJSONAux((st->cms)[i], fp, 1);
			if(!st->cms[i+1]) fprintf(fp, "\n\t}\n");
			else fprintf(fp, "\n\t},\n");
		}
		fprintf(fp, "]");
		fclose(fp);
	}
}

//Função para obter o TIMESTAMP de uma data

char* getTimestamp(char* date){
		struct tm temp = {0};
		char* target = (char*)malloc(sizeof(char*));
    char* token = strtok(date, " ");

    token = strtok(NULL, ", "); //mês
    if(!strcmp(token, "January")) temp.tm_mon = 0;
    if(!strcmp(token, "February")) temp.tm_mon = 1;
    if(!strcmp(token, "March")) temp.tm_mon = 2;
    if(!strcmp(token, "April")) temp.tm_mon = 3;
    if(!strcmp(token, "May")) temp.tm_mon = 4;
    if(!strcmp(token, "June")) temp.tm_mon = 5;
    if(!strcmp(token, "July")) temp.tm_mon = 6;
    if(!strcmp(token, "August")) temp.tm_mon = 7;
    if(!strcmp(token, "September")) temp.tm_mon = 8;
    if(!strcmp(token, "October")) temp.tm_mon = 9;
    if(!strcmp(token, "November")) temp.tm_mon = 10;
    if(!strcmp(token, "December")) temp.tm_mon = 11;
    token = strtok(NULL, ", ");//dia
    temp.tm_mday = atoi(token);
    token = strtok(NULL, ", ");//ano
    temp.tm_year = atoi(token)-1900;
    token = strtok(NULL, ":");//hora
    temp.tm_hour = atoi(token);
    token = strtok(NULL, " ");//minuto
    temp.tm_min = atoi(token);
    token = strtok(NULL, " \""); //AM or PM
    if(!strcmp(token, "PM")) temp.tm_hour = (temp.tm_hour+12)%24;

		sprintf(target, "%u", (unsigned)mktime(&temp));
		return target;
}
