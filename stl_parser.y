%{
#include <cstdio>
#include "node.h"
#include "ast.h"

extern "C" {
    int yyparse();
}

//extern char *yytext;
//extern int yylineno;
int yylex(void);
void yyerror(const char *msg);	
AST ast; /* the top level root node of our final AST */
%}

%union {
    //class Bound *bnd;
	//class StlItem *node;
	//class BooleanExpression *boolean_exp;
	//class AnalogExpression *analog_exp;
	
    Bound *bnd;
    BooleanExpression *boolean_exp;
    AnalogExpression *analog_exp;
    char *str;
    double dbl;
    int integer;
}

%token TKDEFINE
%token <str>TKIDENTIFIER 
%token <str>TKAIDENTIFIER 
%token <str>TKBIDENTIFIER 
%token TKAIDENTIFIER
%token TKVPROP TKASSERT
%token TKNOT TKFELL TKROSE TKOR TKAND TKXOR TKIMPLIES TKIFF
%token TKG_WEAK TKG_STRONG TKF_WEAK TKF_STRONG TKU_WEAK TKU_STRONG
%token TKS TKB TKONCE TKHIST
%token TKDDT TKSHIFT
%token TKEQUAL
%token TKINTEGER 
%token <dbl> TKREAL
%token TKLEQ TKGEQ TKGREATER TKLESS TKEQ
%token TKMODFIL TKMODMASK
%token TKNEXTOCC
%token TKNEXTOCC_DUMMY
%token <integer> TKINTEGER

/* Dummy tokens for precedence rules */
%token TKG_STRONG_T TKF_WEAK_T TKU_WEAK_T
/* End of dummy tokens */

%token '{' '}' ';' ':' '(' ')' '[' ']' ',' '='

%start vprop_specification

%right TKNEXTOCC_S TKNEXTOCC_W
%right TKF_STRONG TKG_STRONG_T TKF_WEAK_T TKU_WEAK_T
%right TKS TKB TKU_STRONG TKU_WEAK
%right TKONCE TKHIST
%right TKIMPLIES TKIFF
%right TKG_WEAK
%left TKXOR TKAND TKOR
%left TKNOT
%left TKFELL
%left TKROSE
%right TKDDT TKSHIFT TKABS TKDISTANCE TKFREEZE
%left TKMINUS TKPLUS TKTIMES

%type <boolean_exp> property
%type <boolean_exp> stl_property
%type <boolean_exp> boolean_atom
%type <analog_exp> analog_atom
%type <boolean_exp> boolean_expression
%type <analog_exp> analog_expression
%type <bnd> range
%type <integer> occurence


%%

vprop_specification:
    TKVPROP TKIDENTIFIER '{' 
        define_directive_list 
        property_declaration_list '}' { printf("create vprop_specification1\n"); }

define_directive_list:
    | define_directive_list define_directive { printf("create define_directive_list1\n"); }

define_directive:
    TKDEFINE TKAIDENTIFIER TKEQUAL analog_expression ';' { printf("create define_directive1: %s\n", $2); }
    | TKDEFINE TKBIDENTIFIER TKEQUAL stl_property ';' { ast.addDefine($2, $4); printf("create define_directive2: %s\n", $2); }

property_declaration_list:
    property_declaration { printf("create property_declaration_list1\n"); }
    | property_declaration_list property_declaration { printf("create property_declaration_list2\n"); }
    ;

property_declaration:
    TKIDENTIFIER TKASSERT ':' property ';' { ast.setAST($4); printf("create property_declaration1\n"); }

property:
    stl_property { $$ = $1; printf("create property1\n"); }

stl_property:
    boolean_atom { $$ = $1; printf("create stl_property1\n"); }
    | '(' stl_property ')' { $$ = $2; printf("create stl_property2\n"); }
    | TKNOT stl_property { printf("create stl_property3\n"); }
	| TKFELL stl_property { printf("create stl_property4\n"); }
	| TKROSE stl_property { $$ = $2; printf("create stl_property5\n"); }
    | stl_property TKAND stl_property { $$ = new NAnd($1, $3); printf("create stl_property6 and\n"); }
    | stl_property TKOR stl_property { printf("create stl_property7\n"); }
    | stl_property TKXOR stl_property { printf("create stl_property8\n"); }
    | stl_property TKIMPLIES stl_property {$$ = new NImply($1, $3); printf("create stl_property9 ->\n"); }
    | stl_property TKIFF stl_property { printf("create stl_property10\n"); }
    | TKG_WEAK stl_property { $$ = new NAlways($2, NULL, NULL); printf("create stl_property11 always\n"); }
    | TKG_WEAK range stl_property { $$ = new NAlways($3, NULL, $2); printf("create stl_property12 always - with range\n"); } %prec TKG_STRONG_T
    | TKG_STRONG range stl_property { printf("create stl_property13 always! - with range\n"); } %prec TKG_STRONG_T
    | TKF_STRONG stl_property { printf("create stl_property14 eventually!\n"); }
	| TKONCE stl_property { printf("create stl_property15\n"); }
	| TKHIST stl_property { printf("create stl_property16\n"); }
	| TKONCE range stl_property { printf("create stl_property17 - with range\n"); }
	| TKHIST range stl_property { printf("create stl_property18 - with range\n"); }
    | TKF_WEAK range stl_property { $$ = new NEventually($3, NULL, $2); printf("create stl_property19 eventually - with range\n"); } %prec TKF_WEAK_T
    | TKF_STRONG range stl_property { printf("create stl_property20 eventually! - with range\n"); } %prec TKF_WEAK_T
    | stl_property TKU_STRONG stl_property { printf("create stl_property21\n"); }
    | stl_property TKS stl_property { printf("create stl_property22\n"); }
    | stl_property TKB stl_property { printf("create stl_property23\n"); }
    | stl_property TKB range stl_property { printf("create stl_property24\n"); } %prec TKU_WEAK_T
    | stl_property TKU_WEAK stl_property { printf("create stl_property25\n"); }
    | stl_property TKU_WEAK range stl_property { printf("create stl_property26\n"); } %prec TKU_WEAK_T
    | stl_property TKU_STRONG range stl_property { printf("create stl_property27\n"); } %prec TKU_WEAK_T
	| stl_property TKS range stl_property { printf("create stl_property28\n"); }
	| TKNEXTOCC_S occurence stl_property { printf("create stl_property29\n"); }
	| TKNEXTOCC_S occurence range stl_property { printf("create stl_property30\n"); }
	| TKNEXTOCC_W occurence range stl_property { printf("create stl_property31\n"); }
    ;

occurence:
	TKINTEGER { printf("create occurence1\n"); }

boolean_atom:
    TKBIDENTIFIER { $$ = new NBoolAtom(NULL, NULL, $1); printf("create boolean_atom1: %s %s\n", $$, $1); }
    | boolean_expression { $$ = $1; printf("create boolean_atom2: %s %s\n", $$, $1); }
    ;

boolean_expression:
    analog_expression TKLEQ TKREAL { printf("create boolean_expression1 TKLEQ: %s %f\n", $1, $3); }
    | analog_expression TKLESS TKREAL { $$ = new NPredicate(NULL, NULL, "Gg"); printf("create boolean_expression2 TKLESS: %f\n", $3); }
    | analog_expression TKGREATER TKREAL { printf("create boolean_expression3 TKGREATER: %f\n", $3); }
    | analog_expression TKGEQ TKREAL { printf("create boolean_expression4 TKGEQ: %f\n", $3); }
    | analog_expression TKEQ TKREAL { $$ = new NPredicate((BooleanExpression*)$1, NULL, "Gg"); printf("create boolean_expression5 TKEQ: %f\n", $3); }
    | TKDISTANCE '(' analog_expression ',' analog_expression ',' TKREAL ')' { printf("create boolean_expression6\n"); }
    | TKDISTANCE '(' analog_expression ',' analog_expression ',' TKREAL ',' { printf("create boolean_expression7\n"); }
      TKREAL ',' TKREAL ')' { printf("create boolean_expression8\n"); }
    | TKDISTANCE '(' boolean_expression ',' boolean_expression ',' TKREAL { printf("create boolean_expression9\n"); }
      ',' TKREAL ')' { printf("create boolean_expression10\n"); }
    ;
	
analog_expression:
    analog_atom { $$ = $1; printf("create analog_expression1: %s %s\n", $$, $1); }
    | '(' analog_expression ')' { $$ = $2; printf("create analog_expression2: %s %s\n", $$, $2); }
    | TKSHIFT '(' analog_expression ',' TKREAL ')' { printf("create analog_expression3\n"); }
    | TKABS '(' analog_expression ')' { $$ = new NAnalog($3, NULL, "x"); printf("create analog_expression4: abs\n"); }
    | analog_expression TKMINUS analog_expression { printf("create analog_expression5\n"); }
    | analog_expression TKPLUS analog_expression { printf("create analog_expression6\n"); }
    | analog_expression TKTIMES analog_expression { printf("create analog_expression7\n"); }
    | analog_expression TKPLUS TKREAL { printf("create analog_expression8\n"); }
    | analog_expression TKMINUS TKREAL { printf("create analog_expression9\n"); }
    | analog_expression TKTIMES TKREAL { printf("create analog_expression10\n"); }
    ;

analog_atom:
    TKAIDENTIFIER { /*$$ = new AnalogExpression($1);*/ printf("create analog_atom1: %s\n", $1); }
    ;

range:
    '[' TKREAL ':' TKREAL ']' { $$ = new Bound($2, $4); printf("create range1: %f:%f\n", $2, $4); }
    | '(' TKREAL ':' TKREAL ']' { printf("create range2\n"); }
    | '[' TKREAL ':' TKREAL ')' { printf("create range3\n"); }
    | '(' TKREAL ':' TKREAL ')' { printf("create range4\n"); }
    | '[' TKGEQ TKREAL ']' { printf("create range5\n"); }
    | '[' TKGREATER TKREAL ']' { printf("create range6\n"); }
    ;

%%

void yyerror(const char *msg) {
	extern int yylineno;  // defined and maintained in lex
	//extern char *yytext;  // defined and maintained in lex
	extern char yytext[];   /* Correct for traditional Lex */
	
	fprintf(stderr, "ERROR: %s at symbol '%s' on line %d\n", msg, yytext, yylineno);
	exit(1);
}