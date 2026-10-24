#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "lexer.h"

static FILE *fp = NULL;

static const char *keywords[MAX_KEYWORDS] =
{
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

static const char operators[] = "+-*/%=!<>|&";
static const char specialCharacters[] = ",;{}()[]";

void initializeLexer(const char *filename)
{
    fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Error opening file\n");
        exit(1);
    }
}

Token getNextToken()
{
    Token token;
    token.type = UNKNOWN;
    token.lexeme[0] = '\0';

    int ch;

    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;

    if (ch == EOF) {
        token.type = END_OF_FILE;
        return token;
    }

    int i = 0;

    if (ch == '/')
    {
        int next = fgetc(fp);

        if (next == '/')
        {
            while ((ch = fgetc(fp)) != '\n' && ch != EOF);
            return getNextToken();
        }
        else if (next == '*')
        {
            int prev = 0;
            while ((ch = fgetc(fp)) != EOF)
            {
                if (prev == '*' && ch == '/')
                    break;
                prev = ch;
            }
            return getNextToken();
        }
        else
        {
            ungetc(next, fp);
            token.lexeme[0] = '/';
            token.lexeme[1] = '\0';
            token.type = OPERATOR;
            return token;
        }
    }

    if (isalpha(ch) || ch == '_')
    {
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_'))
        {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0';

        if (ch != EOF)
            ungetc(ch, fp);

        categorizeToken(&token);
        return token;
    }

    if (isdigit(ch))
    {
        token.lexeme[i++] = ch;
        int hasDot = 0;

        while ((ch = fgetc(fp)) != EOF &&
              (isdigit(ch) || (!hasDot && ch == '.')))
        {
            if (ch == '.')
                hasDot = 1;

            token.lexeme[i++] = ch;
        }

        token.lexeme[i] = '\0';

        if (ch != EOF)
            ungetc(ch, fp);

        token.type = CONSTANT;
        return token;
    }

    if (ch == '"')
    {
        token.lexeme[i++] = ch;

        while ((ch = fgetc(fp)) != EOF && ch != '"')
        {
            token.lexeme[i++] = ch;
        }

        token.lexeme[i++] = '"';
        token.lexeme[i] = '\0';

        token.type = CONSTANT;
        return token;
    }

    if (ch == '\'')
    {
        token.lexeme[i++] = ch;

        while ((ch = fgetc(fp)) != EOF && ch != '\'')
        {
            token.lexeme[i++] = ch;
        }

        token.lexeme[i++] = '\'';
        token.lexeme[i] = '\0';

        token.type = CONSTANT;
        return token;
    }

    if (strchr("=<>!&|+-", ch))
    {
        int next = fgetc(fp);

        if ((ch == '=' && next == '=') ||
            (ch == '!' && next == '=') ||
            (ch == '<' && next == '=') ||
            (ch == '>' && next == '=') ||
            (ch == '&' && next == '&') ||
            (ch == '|' && next == '|') ||
            (ch == '+' && next == '+') ||
            (ch == '-' && next == '-'))
        {
            token.lexeme[0] = ch;
            token.lexeme[1] = next;
            token.lexeme[2] = '\0';
        }
        else
        {
            ungetc(next, fp);
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
        }

        token.type = OPERATOR;
        return token;
    }

    if (isSpecialCharacter(ch))
    {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = SPECIAL_CHARACTER;
        return token;
    }
    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';
    token.type = UNKNOWN;
    return token;
}

void categorizeToken(Token *token)
{
    if (isKeyword(token->lexeme))
        token->type = KEYWORD;
    else if (isConstant(token->lexeme))
        token->type = CONSTANT;
    else if (isIdentifier(token->lexeme))
        token->type = IDENTIFIER;
    else
        token->type = UNKNOWN;
}

int isKeyword(const char *str)
{
    for (int i = 0; i < MAX_KEYWORDS; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isOperator(char ch)
{
    return strchr(operators, ch) != NULL;
}

int isSpecialCharacter(char ch)
{
    return strchr(specialCharacters, ch) != NULL;
}

int isConstant(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int isIdentifier(const char *str)
{
    if (!isalpha(str[0]) && str[0] != '_')
        return 0;

    for (int i = 1; str[i] != '\0'; i++)
    {
        if (!isalnum(str[i]) && str[i] != '_')
            return 0;
    }
    return 1;
}