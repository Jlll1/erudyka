#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *erudykaDbPath;

char *
getErudykaDbPath(void)
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

int handleSearch(const char *predicate)
{
    FILE *db;
    char card[500];

    db = fopen(erudykaDbPath, "r");
    if (db == NULL) return -1;

    while (fread(card, 1, 500, db))
        printf(card);

    printf("\n");
    return 0;
}

int
main(int argc, char const *argv[])
{
    erudykaDbPath = getErudykaDbPath();

    for (int i = 1; i < argc; i++) {
        /*
         * 1 parameter arguments
         */
        if (!strcmp(argv[i], "-n")) {   /* Add a new record */
            return handleNewCard(argv[++i]);
        } else if (!strcmp(argv[i], "-s")) { /* Search for records matching parameter */
            return handleSearch(argv[++i]);
        }
    }

    return 0;
}
