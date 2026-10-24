
#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    initializeLexer(argv[1]);

    Token token;

    while ((token = getNextToken()).type != END_OF_FILE) {
        printf("Token: %s, Type: %d\n",
               token.lexeme, token.type);
    }

    return 0;
}
