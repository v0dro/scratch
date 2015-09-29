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
