/* Name:  Sameer Deshmukh
 * Class: BE I
 * Roll:  405119
 * Title: Lex program for counting the number of lines containing white space, comments and code.
 */

%{
  int white_space = 0;
  int comments    = 0;
  int code        = 0;
%}

%s COMMENT

%%

^[ \t]*\n                 { white_space++; }
^[ \t]*"/*"               { BEGIN COMMENT; }
^[ \t]*"/*".*"*/"[ \t]*\n { comments ++; /* single line comment. ^ because code in that line would count as a line of code not comment. */ }

<COMMENT>"*/"[ \t]*\n { comments++; BEGIN 0; }
<COMMENT>"*/"         { BEGIN 0; }
<COMMENT>\n           { comments++; }
<COMMENT>.\n          { comments++; }

.+"/*".*"*/".*\n { code++; }
.*"/*".*"*/".+\n { code++; }
.+"/*".*\n       { code++; BEGIN COMMENT; }
.\n              { code++; }

.                ;

%%


int main()
{ 
  yyin = fopen("example.c","r");
  yyout = fopen("output.txt", "w");
  yylex();

  fprintf(yyout, "code: %d whitespace: %d comments: %d", code, white_space, code);

  fclose(yyin);
  fclose(yyout);

  return 0;
}

yywrap()
{
  printf("\nReading input from file is finished\n");
}

/***********************************OUTPUT***********************************
*
* ➜  comment_parsing git:(master) ✗ lex comments.l
* ➜  comment_parsing git:(master) ✗ gcc lex.yy.c  
* ➜  comment_parsing git:(master) ✗ ./a.out       
* 
* Reading input from file is finished
*/

/* FILE - example.c */

/* Simple single line comment */
#include <stdio.h>

int main()
{ 
  int i;
  float big_num = 0.3425;

  /* This is another single line comment with some code in front*/ i = 34;
  printf("This is /**/*i*** : %d", i);
  
  /*left side comment*/ i = 32; /*right side comment*/

  big_num = 934.34; /* This is a multi line comment.

     Multi-line comments make life interesting
  */

  /* This is little more 
   * interesting multi line
   * comment
   */
}

/* FILE - output.txt */
code: 10 whitespace: 6 comments: 10
