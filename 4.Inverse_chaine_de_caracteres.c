#include<stdio.h>
#include<stdlib.h>


int compte_chaine(char mot[])
{
    int i=0 ;
    while(mot[i] !='\0')
    {
    i++;
    }
    return i-1;
}

int main()
{
 char mot[2000];
 int len, i, n;

 printf("Écris une phrase:\n");
 fgets(mot, sizeof(mot), stdin);

 len = compte_chaine(mot);
 for(i=0; i<=len; i++)
 {
 n = len-i;
 printf("%c",mot[n]);
 }
 
    return 0;
}