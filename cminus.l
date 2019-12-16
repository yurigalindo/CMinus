%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
char nextToken[MAXTOKENLEN+1];

FILE *f_out;


%}

DIGITO [0-9]
LETRA [a-zA-Z]


%x comment
%x erro

%%

"/""*" 			BEGIN(comment);
<comment>.		
<comment>\n		lineno++;
<comment>"*/"   BEGIN(INITIAL);
		
else 			return ELSE;
if				return IF ;
int				return INT;
return 			return RETURN;
void			return VOID;
while			return WHILE;
{DIGITO}+		return NUM;
{LETRA}+     	return ID;
"<"=            return MENIG;
"<"   			return MENOR;
">"=			return MAIIG;
">"				return MAIOR;
==				return IGLIGL;
!=				return DIF ;
=				return ATT ;
"+"				return SOM ;
"-"				return SUB ;
"*"				return MUL ;
"/"				return DIV ;
","				return VIRG ;
"{"				return ABCH ;
"}"				return FECCH;
"["				return ABCO;
"]"				return FCCO;
;				return PEV;
"("				return ABP;
")"				return FCP;	
\t	
" "
\n				lineno++;
.               printf("ERRO léxico na linha %d, lexema não reconhecido:",lineno); ECHO;BEGIN(erro);
<erro>^[" "\n\t] ECHO;
<erro>\n		lineno++;printf("\n");BEGIN(INITIAL); return ERROR;
<erro>[" "|\t] printf("\n");BEGIN(INITIAL); return ERROR;
%%



TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno=0;
    lineno++;
    yyin = source;
    yyout = listing;
    currentToken=yylex();
    strncpy(nextToken,yytext,MAXTOKENLEN);
    strncpy(tokenString,nextToken,MAXTOKENLEN);
  }
  else{
    strncpy(tokenString,nextToken,MAXTOKENLEN);
    currentToken = yylex();
    strncpy(nextToken,yytext,MAXTOKENLEN);
  }
  
  if (TraceScan) {
    fprintf(listing,"\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

/*
yywrap()
{
	return 1;
}*/

/*
TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}
*/
/*int main(int argc, char *argv[])
{
	/*
OUTRO ^[0-9a-zA-Z;/=\-"+""*""("")"\n" "><"{""}""[""]"","\t"!"]
{OUTRO}+		printf("Erro Lexico na lina %d, lexema:",lineno); ECHO;printf("\n");
*/
/*
  int token, i, buf[100];
  FILE *f_in;
  f_out = fopen("TOKENS","w");
  for (i=0;i<=99;i++) buf[i] = 0;
 

  if (argc == 2)
  {
    if(f_in = fopen(argv[1],"r"))  yyin = f_in;
    else  perror(argv[0]);
  }
  else  yyin = stdin;
  
  i = 0;
  while ((token=yylex())!=FIM)
  {}


  return 0;

}
*/

