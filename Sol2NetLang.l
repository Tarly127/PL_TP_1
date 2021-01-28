%{
#include "aux.h"

char* id_curr = NULL;
char* name_curr = NULL;
char* date_curr = NULL;
char* time_curr = NULL;
char* combody_curr = NULL;
char* likes_curr = NULL;

Comment curr_comment;

stack stackCms;
int num_coments = -1;

%}

%x html sid susername scomBody sdatetime slike

ada \<a[ ]+data\-action\=
ac \<a[ ]+class\=
lc \<li[ ]+class\=

spec_chars \xc3[\x80-\xbf]
letter [a-zA-Z]|{spec_chars}
name ({letter}+[ ,]*)+

%%
  BEGIN INITIAL;
<INITIAL>[0-9]+                          { num_coments = atoi(yytext);
                                           stackCms = createStack(num_coments);
                                           combody_curr = strdup("");
                                           name_curr = strdup("");
                                           BEGIN html; }
<*>[ \n\t\r]                             ;
<html>{lc}\"post\"                       { BEGIN sid; }
<html>{ada}\"profile\"                   { BEGIN susername; }
<html>{ac}\"time\-ago\"                  { BEGIN sdatetime; }
<html>\<div\>                            { BEGIN scomBody; }
<html>{ac}\"vote\-up                     { BEGIN slike; }
<sdatetime>title\=\"[^\"]+\"             { date_curr = strdup(yytext+6);
                                           time_curr = getTimestamp(yytext+6);
                                           BEGIN html; }
<susername>\>{name}                      { name_curr = strdup(yytext+1); }
<susername>\<\/a\>                       { BEGIN html; }
<sid>[0-9]+                              { combody_curr = strdup("");
                                           id_curr = strdup(yytext);
                                           BEGIN html; }
<scomBody>\<\/\div\>                     { BEGIN html; }
<scomBody>\<(\/?)([a-z]{1,2})(\/?)\>     ;
<scomBody>[^\<\n\t\r]+                   { combody_curr = (char*)realloc(combody_curr, strlen(combody_curr) + strlen(yytext) + 1);
                                           char tmp[strlen(combody_curr) + strlen(yytext) + 1];
                                           strcpy(tmp, combody_curr); strcat(tmp, yytext);
                                           combody_curr = strdup(tmp); }
<slike>[0-9]+                            { likes_curr=strdup(yytext);
                                           BEGIN html; }
<html>\-role\=\"children\"\>             { curr_comment = newComment(id_curr, name_curr, combody_curr, atoi(likes_curr), date_curr, time_curr);
                                           if(!(isEmpty(stackCms))) {addReply(peek(stackCms), curr_comment);}
                                           push(stackCms, curr_comment);
                                           curr_comment = NULL;
                                           name_curr = NULL;
                                         }
<html>Show\ more\ replies                { if(checkTop(stackCms) == checkBottom(stackCms)) incBottom(stackCms);
                                         pop(stackCms); }
<*>.                                     ;
%%

int yywrap(){
  parseJSON(stackCms);
  freeStack(stackCms);
  exit(0);
}
