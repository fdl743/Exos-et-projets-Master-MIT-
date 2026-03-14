#include<stdio.h>
#include<stdlib.h>

int main()
{
    char mot[1000];
    int longeur=0, i=0 ;

    printf("Ecris une phrase:\n");
    fgets(mot, 1000, stdin);

    while(mot[i] !='\0')
    {
    longeur +=1;
    i++;
    }
    printf("%d",longeur - 1);
    return 0;
}