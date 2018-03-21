
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keyworddict.h"

int main()
{
	char * input = NULL;
	char * output = NULL;
	const char * keywords[3] = {"DFA", "¶ñÐÄ", "DA"};

	input = (char *)malloc(100);
	output = (char *)malloc(200);
	strcpy( input, "Hello DA World DFA, HaHa! ¶ñÐÄzzz" );

	KeywordDict * dict = new KeywordDict;

	for (int32_t i = 0; i < 3; ++i)
	{
		dict->add( (char *)keywords[i] );
	}

	printf("Content: %s\n", input );

	int32_t rc = dict->filter( input, strlen(input) );
	printf("rc:%d\nContent: %s\n", rc, input );

	strcpy( input, "Hello DA World DFA, HaHa! ¶ñÐÄzzz" );
	rc = dict->filter( input, strlen(input), output, 199, "judy", 4 );
	printf("rc:%d\nContent: %s\n", rc, output );

	free(input);
	free(output);

	delete dict;

	return 0;
}
