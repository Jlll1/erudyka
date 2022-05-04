#define _GNU_SOURCE
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Function Declarations */
char *  getErudykaDbPath();
int     handleGet(int id);
int     handleLink(int id1, int id2);
int     handleNewCard(const char *content);
int     handleSearch(const char *predicate);
void    printUsage();
int     string_contains_invariant(const char *str1, const char *str2);
void    string_trimTrailing(char *str);

/* Globals */
char *erudykaMainDbPath;
char *erudykaLinksDbPath;

char *
getErudykaDbPath(char *dbName)
{
    char *result;
    asprintf(&result, "%s/.erudyka/%s.edk", getenv("HOME"), dbName);

    return result;
}

int
handleGet(int id)
{
    if (id < 1) return -1;

    FILE *db = fopen(erudykaMainDbPath, "r");
    if (db == NULL) return -1;

    char card[500];
    fseek(db, 501 * (id - 1), SEEK_SET);
    fgets(card, 500, db);

    string_trimTrailing(card);
    printf("%s\n", card);
    return 0;
}

int
handleLink(int id1, int id2)
{
    if (id1 < 1 || id2 < 1) return -1;

    FILE *links = fopen(erudykaLinksDbPath, "a");
    if (links == NULL) return -1;

    fprintf(links, "%d-%d\n", id1, id2);

    return 0;
}

int
handleNewCard(const char *content)
{
    if (strlen(content) > 500 - 2) return -1;

    FILE *db = fopen(erudykaMainDbPath, "a");
    if (db == NULL) return -1;

    fprintf(db, "%-500s\n", content);

    return 0;
}

int
handleSearch(const char *predicate)
{
    FILE *db = fopen(erudykaMainDbPath, "r");
    if (db == NULL) return -1;

    int i = 1;
    char card[500];
    while (fread(card, 501, 1, db)) {
        if (string_contains_invariant(card, predicate)) {
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

int
string_contains_invariant(const char *str1, const char *str2)
{
    for(; *str1 != '\0'; str1++) {
        for(int i = 0;; i++) {
            if(str2[i] == '\0') return 1;
            if(tolower((unsigned char)str1[i]) != tolower((unsigned char)str2[i]))
                break;
        }
    }

    return 0;
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
    erudykaMainDbPath = getErudykaDbPath("main");
    erudykaLinksDbPath = getErudykaDbPath("links");
    if (argc == 1) {
        printUsage();
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        /*
         * 1 parameter commands
         */
        if(!strcmp(argv[i], "get")) {           /* Retrieve a card by id */
            return handleGet(atoi(argv[++i]));
        } else if(!strcmp(argv[i], "new")) {    /* Add a new card */
            return handleNewCard(argv[++i]);
        } else if(!strcmp(argv[i], "search")) { /* Search for cards that match the parameter */
            return handleSearch(argv[++i]);
        }
        /*
         * 2 parameter commands
         */
          else if(!strcmp(argv[i], "link")) {   /* Link card1 to card2 */
              return handleLink(atoi(argv[++i]), atoi(argv[++i]));
        } else {
            printUsage();
            return 0;
        }
    }

    return 0;
}
