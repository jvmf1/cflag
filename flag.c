#include "flag.h"
#include "cmap.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

static flag_t* flag_create(const char *name, const char *fmt, void *data, const size_t size, const size_t nmemb) {
	flag_t *flag = malloc(sizeof(flag_t));
	if (flag == NULL) return NULL;
	flag->data=data;
	flag->name=name;
	flag->fmt=fmt;
	flag->size=size;
	flag->nmemb=nmemb;
	return flag;
}

int parser_init(parser_t *parser, const size_t cap, const int flags) {
	parser->flags = flags;
	parser->map = cmap_create(cap);
	parser->map->free_data_function=free;
	if (parser->map == NULL) return 1;
	return 0;
}

int parser_add_flag_ex(parser_t *parser, const char *name, const char *fmt, void *data, const size_t size, const size_t nmemb) {
	flag_t *flag = flag_create(name, fmt, data, size, nmemb);
	if (flag == NULL) return 1;
	if (cmap_insert(parser->map, name, flag) != 0) return 1;
	return 0;
}

void parser_free(parser_t *parser) {
	cmap_free(parser->map);
}

static void parse(flag_t *flag, char *arg, int offset) {
	if (flag->nmemb == 0)
		*(bool*)flag->data = true;
	else if (strcmp(flag->fmt, "%s") == 0) {
		*(char**)(flag->data+flag->size*offset) = arg;
	}
	else if (sscanf(arg, flag->fmt, flag->data+flag->size*offset) == 0) {
		fprintf(stderr, "error: '%s' is invalid for flag %s\n", arg, flag->name);
		exit(1);
	}
}

static void unknow_flag(char *arg) {
	fprintf(stderr, "error: flag %s is unknown\n", arg);
	exit(1);
}

static void missing_value(flag_t *flag) {
	if (strlen(flag->name) == 1)
		fprintf(stderr, "error: flag -%s is missing a value\n", flag->name);
	else
		fprintf(stderr, "error: flag --%s is missing a value\n", flag->name);
	exit(1);
}

void parser_parse(parser_t *parser, int argc, char **argv) {
	int i = 1;
	if (parser->flags & PARSER_DONT_SKIP_FIRST)
		i = 0;
	for (; i < argc; i++) {
		int arg_len = strlen(argv[i]);
		if (argv[i][0] == '-' && argv[i][1] != '-') {
			for (int j = 1; j < arg_len; j++) {
				char tmp[2] = {0};
				tmp[0] = argv[i][j];
				flag_t *flag = cmap_get(parser->map, tmp);
				if (flag == NULL)
					unknow_flag(tmp);
				if (j != arg_len -1 && flag->nmemb != 0) {
					fprintf(stderr, "error: cant parse flag -%s value\n", tmp);
					exit(1);
				}
				if (i + flag->nmemb >= argc)
					missing_value(flag);
				if (flag->nmemb == 0)
					parse(flag, "", 0);
				else
					for (int k = 0; k < flag->nmemb; k++)
						parse(flag, argv[i+1+k], k);
				i+=flag->nmemb;
			}
		} else {

			if (arg_len < 3)
				unknow_flag(argv[i]);
			flag_t *flag = cmap_get(parser->map, argv[i]+2);
			if (flag == NULL)
				unknow_flag(argv[i]);
			if (i + flag->nmemb >= argc)
				missing_value(flag);
			if (flag->nmemb == 0)
				parse(flag, "", 0);
			else
				for (int k = 0; k < flag->nmemb; k++)
					parse(flag, argv[i+1+k], k);
			i+=flag->nmemb;

		}
	}
}
