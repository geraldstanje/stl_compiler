%{
#include "node.h"
#include "y.tab.hpp"
#include <cstdlib>

extern void yyerror(const char *message);
%}

/* Add this to get line numbers... */
%option yylineno

%x analog
%x boolean
%x comments
%x next_occurence

id              [A-Za-z_#][A-Za-z0-9_\$\./]*
digit           [0-9]
number          {digit}+
exponant        [eE][+-]?{number}
integer         [+-]?{number}
real            {integer}|{integer}("."{number})?|{exponant}?

%%

"vprop"                         return TKVPROP;
"assert"                        return TKASSERT;

"define"                        return TKDEFINE;

":="                            return TKEQUAL;

"not"                           return TKNOT;
"fall"                          return TKFELL;
"rise"                          return TKROSE;
"or"                            return TKOR;
"and"                           return TKAND;
"xor"                           return TKXOR;
"->"                            return TKIMPLIES;
"<->"                           return TKIFF;

"always"                        return TKG_WEAK;
"always!"                       return TKG_STRONG;
"eventually"                    return TKF_WEAK;
"eventually!"                   return TKF_STRONG;
"until"                         return TKU_WEAK;
"until!"                        return TKU_STRONG;

"once"                          return TKONCE;
"historically"                  return TKHIST;
"since"                         return TKS;
"backto"                        return TKB;

"ddt"                           return TKDDT;
"shift"                         return TKSHIFT;
"-"                             return TKMINUS;
"+"                             return TKPLUS;
"*"                             return TKTIMES;
"abs"                           return TKABS;

"distance"                      return TKDISTANCE;

"freeze"                        return TKFREEZE;

"modfil"                        return TKMODFIL;
"modmask"                       return TKMODMASK;

"next_rise"                     {BEGIN next_occurence; return TKNEXTOCC_W;}
"next_rise!"                    {BEGIN next_occurence; return TKNEXTOCC_S;}


">="                            return TKGEQ;
">"                             return TKGREATER;
"<="                            return TKLEQ;
"<"                             return TKLESS;
"=="                            return TKEQ;


"a:"                            { BEGIN analog; }
"b:"                            { BEGIN boolean; }
<analog>{id}                    {
                                  yylval.str = strdup (yytext);
                                  BEGIN 0;
                                  return TKAIDENTIFIER;
                                }
<analog>[ \t\n]*                ;
<boolean>{id}                   {
                                  yylval.str = strdup (yytext);
                                  BEGIN 0;
                                  return TKBIDENTIFIER;
                                }
<boolean>[ \t\n]*               ;
{id}                            {
                                  yylval.str = strdup (yytext);
                                  return TKIDENTIFIER;
                                }
{real}                          {
                                  yylval.dbl = atof (yytext);
                                  return TKREAL;
                                }
<next_occurence>{number}        {
                                  yylval.integer = atof (yytext);
                                  return TKINTEGER;
                                }
<next_occurence>[ \n\t\[]       ;
<next_occurence>"]"             { BEGIN 0; }
<next_occurence>.               { yyerror ("Invalid character"); }
[ \n\t\s\r]*                    /* ignore whitespace */;
[:;(){}\[\],]                   { return *yytext; }
.                               { yyerror ("Invalid character"); }

%%

int yywrap (void) {
    return 1;
}