%{
  #include <stdio.h>
  #include <stdlib.h>
  int words = 0, chars = 0, lines = 0;
%}

%%
[^\ \n\t]+  { ++words; chars += yyleng;}
"\n" { ++lines; }
%%

int yywrap() { return 1; }

int main(int argc, char **argv)
{
  if (argc < 2) {
    yyin = stdin;
  }
  else {
    yyin = fopen(argv[1], "r");
  }
  
  yylex();
  printf("Word count :: %d ", words);
  printf("Char count :: %d ", chars);
  printf("Line count :: %d\n", lines);
  
  return 0;
}
