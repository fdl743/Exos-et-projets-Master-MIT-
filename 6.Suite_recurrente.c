#include<stdio.h>
#include<stdlib.h>

int main()
{
    int tab[70];
  int n=30, i, somme=0;
  tab[0]=3; tab[1]=-4;
  for(i=2; i<70; i++)
  {
  tab[i]=2*tab[i-1]+3*tab[i-2];
  }
  
  for (i=0; i<30; i++)
  {
  somme = somme + tab[i];
  }
  printf("1.La somme des 30 premiers termes est :%d \n", somme);

  printf("2.Le 70 ième terme de la suite U est:%d", tab[69]);
  return 0;
}