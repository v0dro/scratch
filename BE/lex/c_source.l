%{
	#include <stdio.h>
%}

%%
#include\ ?(<.*>|\".*\") { printf("Found header file %s", yytext); }
(int|float|char|void)    { printf("Found identifier %s", yytext); }
(=|>|<) { printf("Found operator %s", yytext);}
[a-zA-Z]+\ *\(.*\) { printf(); }
%%

int yywrap() { return 1; }

int main(int argc, char** argv)
{
  yyin = fopen(argv[1],"r");
  yylex();
  
  return 0;
}
