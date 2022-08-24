#pragma once

#include "cmap.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct {
	void *data;
	const char *name;
	const char *fmt;
	size_t size;
	size_t nmemb;
} flag_t;

typedef struct {
	int flags;
	cmap *map;
} parser_t;

#define CHOOSE_FMT(p) _Generic((*p),\
		int: "%d",\
		short int: "%hd",\
		long int: "%ld",\
		unsigned int: "%u",\
		short unsigned int: "%hu",\
		long unsigned int: "%lu",\
		float: "%f",\
		char: "%c",\
		char*: "%s",\
		double: "%lf")\

#define parser_add_flag(parser, name, var) parser_add_flag_ex(parser, name, CHOOSE_FMT(var), var, sizeof(*var), 1)

#define parser_add_flag_bool(parser, name, var) parser_add_flag_ex(parser, name, "", var, sizeof(*var), 0)

#define parser_add_flag_arr(parser, name, var) parser_add_flag_ex(parser, name, CHOOSE_FMT(*var), var, sizeof(*var[0]), sizeof(*var) / sizeof(*var[0]))

int parser_init(parser_t *parser, const size_t cap, const int flags);

int parser_add_flag_ex(parser_t *parser, const char *name, const char *fmt, void *data, const size_t size, const size_t nmemb);

void parser_free(parser_t *parser);

void parser_parse(parser_t *parser, int argc, char **argv);

#define PARSER_DONT_SKIP_FIRST 1 << 0
