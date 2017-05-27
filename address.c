#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int fun(void *src, unsigned long srclen)
{
	unsigned char *lDst = (unsigned char*)src;
    unsigned char *lSrc = (unsigned char*)src;
    printf("lDst = %p\n", lDst);
    printf("lSrc = %p\n", lSrc);
    strcpy(lDst, "adbcd");
    printf("lDstr = %s\n", lDst);
    strcpy(lSrc, "cdef");
    printf("lSrc = %s\n", lSrc);
    printf("lDstr = %s\n", lDst);
}
int main()
{
    unsigned char *p = (unsigned char *)malloc(sizeof(unsigned char));
    fun(p, sizeof(unsigned char));
}
