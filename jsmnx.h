/*	$: This module is used to extend jsmn getting substructure of a json node.
	@: YX Hao #: 7/24/2014
*/

#ifndef __JSMNX_H_
#define __JSMNX_H_

# ifdef __cplusplus
extern "C" {
# endif

#include <ctype.h>
#include <string.h>

#include "stringx.h"

#include "jsmn.h"

/* < Num(":") * 2 + Num("{") + Num("[") */
int jsmn_tokenise(const char *js, jsmntok_t **tokens) {
    jsmn_parser parser;
    int ret;
    int n, i;
    //
    n = strchrn(js, ':') * 2;
    n += strchrn(js, '{');
    n += strchrn(js, '[');
    n += strchrn(js, ',');
    //
    *tokens = realloc(*tokens, n * sizeof(jsmntok_t));
    jsmn_init(&parser);
    ret = jsmn_parse(&parser, js, strlen(js), *tokens, n);
    //
    if (ret < 0) {n = 0;}
    else {n = ret;}
    tokens = realloc(*tokens, n * sizeof(jsmntok_t));
    //
    return ret;
}

/* Could big block allocated */
void jsmn_free_tokens(jsmntok_t **tokens) {
    free(*tokens);
}

jsmntok_t* jsmn_get_sub_object(const char* js, jsmntok_t *token, const char* name) {
	if (token->type != JSMN_OBJECT) return NULL;
	int size = token->size; /* old version: 'size / 2' */
	int last_end = 0;
	/* only current namespace */
	int len = strlen(name);
	for (int i = 0; i < size; i++) {
		token++; /* over last one */
		for (; token->end <= last_end; token++) {} /* keep same level */
		const char* start = js + token->start;
		int length = token->end - token->start;
		/* not substr */
		if (length == len && strncmp(start, name, length) == 0) {
			return token;
		}
		token++; /* skip value */
		last_end = token->end; /* skip substructure */
	}
	return NULL;
}

jsmntok_t* jsmn_get_array_item(const char* js, jsmntok_t *token, int index) {
	if (token->type != JSMN_ARRAY) return NULL;
	int length = token->size;
	int last_end = 0;
	/* only current namespace */
	for (int i = 0; i < length; i++) {
		token++; /* over last one */
		for (; token->end <= last_end; token++) {}
		if (i == index) return token;
		last_end = token->end; /* skip substructure */
	}
	return NULL;
}

# ifdef __cplusplus
}
# endif

#endif /* __JSMNX_H_ */
