#include <stdio.h>

int main(int argc, char **argv)
{
    int a = 0;
    int *p = &a;
    printf("%p\n", p);
    if(argc>1)
	printf("first arg=%s\n", argv[1]);
    return 42;
}