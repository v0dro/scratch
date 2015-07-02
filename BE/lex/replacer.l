%{
  #include <stdio.h>
  char encrypted[500];
  int length = 0;
%}

%%
[a-wA-W] {encrypted[length] = yytext[0] + 3; ++length;}
[w-zW-Z] {encrypted[length]= yytext[0] - 23; ++length;}
. {encrypted[length]= yytext[0]; ++length;}
%%

int yywrap() { return 1; }

int main(int argc, char** argv)
{
  yyin = fopen(argv[1], "r");
  yylex();
  encrypted[length] = '\0';
  
  printf("\nENCRYPTING DATA....\n");
  printf("%s\n", encrypted);
  
  return 0;
}
