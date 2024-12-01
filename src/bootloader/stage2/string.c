#include <string.h>
#include <stdint.h>

const char* strchr(const char* str, char chr) 
{
    if (str == NULL)
        return NULL;

    while (*str)
    {
        if (*str == chr)
            return str;

        ++str;
    }

    return NULL;
}
char* strcpy(char* dst, char* src) 
{
    const char* origdst = dst;

    if (dst == NULL)
    {
        return NULL;
    }

    if (src == NULL)
    {
        return NULL; 
    }
    
    while (*src)
    {
        *dst = *src;
        *src++;
        *dst++;
    }
    
    
    *dst = '/0';
    return origdst;
}

unsigned strlen(const char* str)
{
    unsigned len = 0;
    while (*str)
    {
        ++len;
        ++str;
    }

    return len;
}