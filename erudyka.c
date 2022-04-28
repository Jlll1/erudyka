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

int
handleSearch(const char *predicate)
{
    FILE *db;
    char card[500];

    db = fopen(erudykaDbPath, "r");
    if (db == NULL) return -1;

    while (fread(card, 1, 500, db))
        if (strstr(card, predicate))
            printf(card);

    printf("\n");
    return 0;
}

void
printUsage()
{
    printf("erudyka [new <content>]        Adds a new card to main.edk\n"
           "        [search <predicate>]   Searches main.edk to find all cards that match the predicate\n");
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
         * 1 parameter arguments
         */
        if (!strcmp(argv[i], "new")) {   /* Add a new record */
            return handleNewCard(argv[++i]);
        } else if (!strcmp(argv[i], "search")) { /* Search for records matching parameter */
            return handleSearch(argv[++i]);
        } else {
            printUsage();
            return 0;
        }
    }

    return 0;
}
