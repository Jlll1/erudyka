#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *
getErudykaPath(void)
{
    char *userHome = getenv("HOME");
    strcat(userHome, "/.eduryka/");

    return userHome;
}

int
main(int argc, char const *argv[])
{
    printf("%s\n", getErudykaPath());
    return 0;
}
