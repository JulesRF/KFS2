#include "include/kfs.h"

int ft_strlen(const char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

int ft_strcmp(const char *str1, const char *str2)
{
    int i;

    i = 0;
    while (str1[i] || str2[i])
    {
        if (str1[i] - str2[i] != 0)
            return(str1[i] - str2[i]);
        i++;
    }
    return (0);
}