#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Function Declarations */
char *  getErudykaDbPath();
int     handleNewCard(const char *content);
int     handleSearch(const char *predicate);
void    printUsage();
void    string_trimTrailing(char *str);

/* Globals */
char *erudykaDbPath;

char *
getErudykaDbPath()
{
    char *result= getenv("HOME");
    strcat(result, "/.erudyka/main.edk");

    return result;
}

int
handleNewCard(const char *content)
{
    FILE *db;

    if (strlen(content) > 500 - 2) return -1;

    db = fopen(erudykaDbPath, "a");
    if (db == NULL) return -1;

    fprintf(db, "%-500s\n", content);

    return 0;
}

int
handleSearch(const char *predicate)
{
    FILE *db = fopen(erudykaDbPath, "r");
    if (db == NULL) return -1;

    int i = 0;
    char card[500];
    while (fread(card, 501, 1, db)) {
        if (strstr(card, predicate)) {
            string_trimTrailing(card);
            printf("%d: %s\n\n", i, card);
        }

        i++;
    }

    return 0;
}

void
printUsage()
{
    printf("erudyka [new <content>]        Adds a new card to main.edk\n"
           "        [search <predicate>]   Searches main.edk to find all cards that match the predicate\n");
}

void
string_trimTrailing(char *str)
{
    int i = 0;
    int lastNonWhitespaceCharacter = -1;

    while(str[i] != '\0') {
        if(str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
            lastNonWhitespaceCharacter = i;

        i++;
    }

    str[lastNonWhitespaceCharacter + 1] = '\0';
}

int
main(int argc, char const *argv[])
{
    erudykaDbPath = getErudykaDbPath();
    if (argc == 1) {
        printUsage();
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        /*
         * 1 parameter commands
         */
        if (!strcmp(argv[i], "get")) {           /* Retrieve a card by id */
            return handleGet(atoi(argv[++i]));
        } else if(!strcmp(argv[i], "new")) {     /* Add a new card */
            return handleNewCard(argv[++i]);
        } else if (!strcmp(argv[i], "search")) { /* Search for cards that match the parameter */
            return handleSearch(argv[++i]);
        } else {
            printUsage();
            return 0;
        }
    }

    return 0;
}
