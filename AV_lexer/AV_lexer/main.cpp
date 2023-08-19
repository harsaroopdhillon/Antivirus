#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "grammer.tab.h"

int yyparse(void);
void yyerror(const char *);
int yylex();
extern FILE *yyin;
extern int yylineno;

int main(int argc, char **argv)
{
	
	yyin = fopen("enter.code", "rb");
	if (!yyin) {
		puts("fopen issue.");
	}
	int result = 0;
	while (result == 0)
	{
		result = yyparse();
		if (result == 0);
		else if (result > 0)
			std::cout << "The input is invalid at line " << yylineno - 1 << "result:" << result<<'\n';
	}
	getchar();
	return 0;
}
