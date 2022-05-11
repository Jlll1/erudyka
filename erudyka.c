#define _GNU_SOURCE
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Globals */
char *erudykaMainDbPath;
char *erudykaLinksDbPath;

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

char *
joinErudykaPath(char *path)
{
    char *result;
    asprintf(&result, "%s/.erudyka/%s", getenv("HOME"), path);

    return result;
}

int
printCard(int id)
{
    FILE *main = fopen(erudykaMainDbPath, "r");
    if (main == NULL) return -1;

    char card[500];
    fseek(main, 501 * (id - 1), SEEK_SET);
    fgets(card, 500, main);

    string_trimTrailing(card);
    printf("%s\n", card);

    fclose(main);
    return 0;
}

int
handleGet(int id)
{
    if (id < 1) return -1;

    FILE *links = fopen(erudykaLinksDbPath, "r");

    printCard(id);

    if (links == NULL) return 0;

    char link[30];
    while (fgets(link, 30, links)) {
        int result;
        int id1 = atoi(strtok(link, "-"));
        int id2 = atoi(strtok(NULL, "-"));
        if (id == id1) {
            result = printCard(id2);
        }

        if (result != 0) {
            return result;
        }
    }

    fclose(links);
    return 0;
}

int
handleLink(int id1, int id2)
{
    if (id1 < 1 || id2 < 1) return -1;

    FILE *links = fopen(erudykaLinksDbPath, "a");
    if (links == NULL) return -1;

    fprintf(links, "%d-%d\n", id2, id1);

    return 0;
}

int
handleNewCard(const char *content)
{
    if (strlen(content) > 500 - 2) return -1;

    FILE *main= fopen(erudykaMainDbPath, "a");
    if (main == NULL) return -1;

    fprintf(main, "%-500s\n", content);

    fclose(main);
    return 0;
}

int
handleSave(const char *content, const char *command)
{
    char *scriptPath;
    asprintf(
        &scriptPath,
        "sh %s/%s.sh %s",
        joinErudykaPath("scripts"), command, content);

    FILE *pipe = popen(scriptPath, "r");
    if (pipe == NULL) return -1;

    char card[498];
    fread(card, 498, 1, pipe);
    if (pclose(pipe)) return -1;

    handleNewCard(card);
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
printUsageAndExit()
{
    puts("erudyka <options> [command <args>]\n"
         "        --directory <dir>           Specifies the erudyka root directory, by default: ~/.erudyka/\n"
         "        [get <id>]                  Finds a card with specified id and prints it and all cards linked to it\n"
         "        [link <id1> <id2>]          Links two cards with specified ids together\n"
         "        [new <content>]             Adds a new card\n"
         "        [save <command> <content>]  Pipes <content> into sh <command> and uses result to create new card\n"
         "        [search <predicate>]        Prints all cards that match the predicate\n");

    exit(0);
}

int
main(int argc, char const *argv[])
{
    /* Options */
    if (argc < 3) printUsageAndExit();

    int i;
    for (i = 1; i < argc; i++) {
        if (strncmp("--", argv[i], 2)) break;

        if (!strcmp(argv[i], "--directory")) { /* Specify the erudyka root directory */
            i++;
            asprintf(&erudykaMainDbPath, "%smain.edk", argv[i]);
            asprintf(&erudykaLinksDbPath, "%slinks.edk", argv[i]);
            continue;
        }

        printUsageAndExit();
    }

    if (erudykaMainDbPath == NULL)
        erudykaMainDbPath = joinErudykaPath("main.edk");
    if (erudykaLinksDbPath == NULL)
        erudykaLinksDbPath = joinErudykaPath("links.edk");

    /* 1 parameter commands */
    if (argc < i + 1) printUsageAndExit();

    const char *command = argv[i];
    const char *parameter1 = argv[++i];

    if (!strcmp(command, "get")) {            /* Retrieve a card by id */
        return handleGet(atoi(parameter1));
    } else if (!strcmp(command, "new")) {     /* Add a new card */
        return handleNewCard(parameter1);
    } else if (!strcmp(command, "search")) {  /* Search for cards that match the parameter */
        return handleSearch(parameter1);
    }

    /* 2 parameter commands */
    if (argc < ++i) printUsageAndExit();

    const char *parameter2 = argv[i];

    if (!strcmp(command, "link")) {           /* Link card1 to card2 */
        return handleLink(atoi(parameter1), atoi(parameter2));
    } else if (!strcmp(command, "save")) {    /* Pass input to the specified script and save output as card */
        return handleSave(parameter1, parameter2);
    }

    printUsageAndExit();
}