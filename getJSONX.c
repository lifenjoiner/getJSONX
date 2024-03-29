
#include <stdio.h>
#include <stdlib.h>

#include "file.h"

#include "jsmnx.h"
#include "vector.h"

//
void show_token(const char* js, jsmntok_t *token) {
	if (token != NULL) {
		printf("%.*s\n", token->end - token->start, js + token->start);
	}
}

//parse js flavor expression
jsmntok_t* jsmn_parse_expr_on_ws(const char* js, const jsmntok_t *tokens, const char *expr) {
    Vector operations, opr;
    size_t i, n;
    jsmntok_t *token;
    //
    operations = _tcs_split(expr, ".");
    n = operations.length;
    //
    token = (jsmntok_t*)tokens;
    for (i = 0; i < n; i++) {
        char *pp;
        int nn;
        //
        if (!token) { goto UNKNOWN; }
        opr = _tcs_split(operations.items[i], "[]");
        nn = opr.length;
        //
        if (nn == 1) {
            pp = opr.items[0];
            if (token->type == JSMN_OBJECT) {
                if ((char)*pp != '.') {
                    token = jsmn_get_sub_object(js, token, pp);
                    if (!token) { goto UNKNOWN; }
                    token++;
                }
            }
            else if (token->type == JSMN_ARRAY) {
                if (!strcmp(pp, "length()")) {
                    printf("%d\n", token->size);
                    return NULL;
                }
                else if (isdigit(*pp)) { // top level array
                    token = jsmn_get_array_item(js, token, atoi(pp));
                }
            }
            else { goto UNKNOWN; }
        }
        else if (nn > 1) {
            int j;
            for (j = 0; j < nn - 1; j++) {
                pp = opr.items[j + 1];
                if (!isdigit(*pp)) { goto UNKNOWN; }
                if (j == 0 && token->type == JSMN_OBJECT) {
                    token = jsmn_get_sub_object(js, token, opr.items[j]);
                    if (!token) { goto UNKNOWN; }
                    token++;
                }
                if (token->type != JSMN_ARRAY) { goto UNKNOWN; }
                token = jsmn_get_array_item(js, token, atoi(pp));
            }
        }
        else {
UNKNOWN:
            fprintf(stderr, "Err: %s\n", operations.items[i]);
            i = n;
            token = NULL;
        }
        vector_free(&opr);
    }
    vector_free(&operations);
    //"." = root
    return token;
}

//
void help(const char *app) {
    fprintf(stderr, "Get information from json. v0.2.3 by YX Hao\n");
    fprintf(stderr, "Usage: %s <operations> [json-file]\n", app);
    fprintf(stderr, "operation examples:\n");
    fprintf(stderr, "    [.]a.b.c[0].d\n");
    fprintf(stderr, "    [.]a.b.c.length()\n");
    fprintf(stderr, "    .\n");
    fprintf(stderr, "examples:\n");
    fprintf(stderr, "    type test.json | %s .\n", app);
    fprintf(stderr, "    %s . < test.json\n", app);
    exit(EXIT_FAILURE);
}

//
int main(int argc, const char** argv) {
    const char* filename;
    FILE* fp;
    char* input;
    size_t input_length;
    jsmntok_t *tokens, *token;
    int ret;
    //
    if (argc == 2) { fp = stdin; }
    else if (argc == 3) {
        //
        filename = argv[2];
        fp = fopen(filename, "r");
        if (!fp) {
            fprintf(stderr, "File not found: %s\n", filename);
            exit(EXIT_FAILURE);
        }
    }
    else { help(argv[0]); }
    //
    input_length = read_file(fp, &input);
    if (fp != stdin) { fclose(fp); }
    //
    if (input_length == 0) {return 0;}
    //
    tokens = calloc(1, sizeof(jsmntok_t));
    ret = jsmn_tokenise(input, &tokens);
    //
    if (ret < 0) {return 1;}
    token = jsmn_parse_expr_on_ws(input, tokens, argv[1]);
    //
    show_token(input, token);
    //
    jsmn_free_tokens(&tokens);
    //
    return 0;
}