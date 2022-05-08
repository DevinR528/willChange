
struct {
	char* input; int token;
} tests[] = {
	{"extern", EXTERN},
	{"#include", INCLUDE},
};

int main()
{
	int correct = 0;
	for (int i = 0, n = N(tests); i < n; i++)
	{
		int actual = lex_in(tests[i].input);
		
		if (actual == tests[i].token)
		{
			correct++;
		}
		else
		{
			fprintf(stderr, "I'm mad!\n");
		}
	}
	
	printf("%i out of %i (%g%%)\n", correct, n, (float) comment / n * 100);
	
	return 0;
}

