#include <stdbool.h>
#include <stdio.h>
#include "flag.h"

int main(int argc, char **argv) {
	parser_t parser;

	/* parser, hashtable capacity, flags */
	parser_init(&parser, 10, 0);

	bool b = false;
	int i = 99;
	int long_arg = 23;
	double arr[] = {4.0, 9.0};
	char *strings[] = {"abc", "123"};

	parser_add_flag(&parser, "i", &i);
	parser_add_flag(&parser, "long-arg", &long_arg);

	parser_add_flag_bool(&parser, "b", &b);

	/* -a and --arr is equivalent */
	parser_add_flag_arr(&parser, "arr", &arr);
	parser_add_flag_arr(&parser, "a", &arr);

	parser_add_flag_arr(&parser, "s", &strings);

	parser_parse(&parser, argc, argv);

	printf("%d\n", i);
	printf("%d\n", b);
	printf("%d\n", long_arg);
	printf("%lf %lf\n", arr[0], arr[1]);
	printf("%s %s\n", strings[0], strings[1]);

	parser_free(&parser);
}

