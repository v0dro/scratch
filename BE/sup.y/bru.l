%{
  // Declaration section 
  #include <stdio.h>
  #include <y.tab.h>
%}

%%
i|we|us  {   return PRONOUN;  }
homer|marge|lisa|bart|simpsons { return PROPER_NOUN; }
fly|walk|eat|burp|read|dance|sulk|am|are { return VERB; }
%%
