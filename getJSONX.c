
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
    operations = str_split(expr, ".");
    n = operations.length;
    //
    token = (jsmntok_t*)tokens;
    i = n;
    for (i = 0; i < n; i++) {
        char *pp;
        int ii, nn;
        //
        opr = str_split(operations.items[i], "[]");
        nn = opr.length;
        //
        switch (nn) {
        case 2:
            pp = opr.items[1];
            if (token && token->type == JSMN_OBJECT && isdigit(*pp)) {
                ii = atoi(pp);
                token = jsmn_get_sub_object(js, token, opr.items[0]);
                if (!token) { goto UNKNOWN; }
                token++;
                if (token->type != JSMN_ARRAY) { goto UNKNOWN; }
                token = jsmn_get_array_item(js, token, ii);
            }
            else { goto UNKNOWN; }
            break;
        case 1:
            if (token && token->type == JSMN_OBJECT) {
                if ((char)*operations.items[i] != '.') {
                    token = jsmn_get_sub_object(js, token, operations.items[i]);
                    if (!token) { goto UNKNOWN; }
                    token++;
                }
            }
            else { goto UNKNOWN; }
            break;
        default:
UNKNOWN:
            printf("Err: %s\n", operations.items[i]);
            i = n;
            break;
        }
    }
    //"." = root
    return token;
}

//
void help(const char *app) {
    printf("Get information from json. v0.2.0 by YX Hao\n");
    printf("Usage: %s <operations> [json-file]\n", app);
    printf("operation examples:\n");
    printf("    [.]a.b.c[0].d\n");
    printf("    .\n");
    printf("examples:\n");
    printf("    type test.json | %s .\n", app);
    printf("    %s . < test.json\n", app);
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
            printf("File not found: %s\n", filename);
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