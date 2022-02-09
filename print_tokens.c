
#include <stdio.h>
#include <stdlib.h>

#include "file.h"

#include "jsmnx.h"

//
void show_token(const char* js, jsmntok_t *token) {
	if (token != NULL) {
		printf("%.*s\n", token->end - token->start, js + token->start);
	}
}

//
void help(const char *app) {
    printf("Usage: %s [json-file]\n", app);
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
    int i;
    //
    if (argc == 0) { fp = stdin; }
    else if (argc == 2) {
        //
        filename = argv[1];
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
    //
    for (i = 0; i < ret; i++) {
        token = &tokens[i];
		printf("%d: [%d, %d), type: %d, size: %d\n", i,
		token->start, token->end, token->type, token->size);
		if (token->type == JSMN_OBJECT || token->type == JSMN_ARRAY) {
			printf("[%d elems]\n", token->size);
		}
        show_token(input, token);
        printf("\n");
    }
    //
    jsmn_free_tokens(&tokens);
    //
    return 0;
}