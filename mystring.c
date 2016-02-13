#include <string.h>
#include "mystring.h"

// Strip trailing characters
char *strstrip(char *str, char *trail)
{
    // Dont operate on a bad string
    if (!str || !str[0])
        return str;

    size_t j_max = strlen(trail);

    for (int i = strlen(str); i >= 0; --i)
    {
        for (int j = 0; j < j_max; j++)
            if (str[i] == trail[j])
                str[i] = '\0';

        if (str[i] != '\0')
            break;
    }

    return str;
}

// Chomp trailing CR/LF
char *strchomp(char *str)
{
    return strstrip(str, "\r\n");
}

// Return string as a file name
char *strtofile(const char *str, const char *ext)
{

}


