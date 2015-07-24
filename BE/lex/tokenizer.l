%{
  #include <stdio.h>
  #include <strcmp.h>

  enum {
    LOOKUP = 0,
    NOUN,
    VERB,
    ADJ,
    ADV,
    PRON,
    CONJ
  };

  int state;

  int insert_word(int type, char* word);
  int lookup_word(char* word);
%}

%%
\n { state = LOOKUP; }

^noun { state = NOUN; }
^verb { state = VERB; }
^adj  { state = ADJ; }
^adv { state = ADV; }
^pron { state = PRON; }
^conj { state = CONJ; }

. {}
%%

int
insert_word(int type, char* word)
{

}

int
lookup_word(char* word)
{

}

int yywrap() { return 1; }
int
main()
{
  return 0;
}