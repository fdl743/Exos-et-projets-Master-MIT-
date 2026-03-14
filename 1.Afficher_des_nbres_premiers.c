#include<stdio.h>
#include<stdlib.h>

int main()
{   
    int nbre=3 , i, n=0;
    int liste_prime_nbre[25]={2};

    printf("Les 25 premiers nombres premiers sont :\n");

    while(n<25)
    {
      for(i=2; i<nbre; i++)
      {
       if(nbre%i==0)
        {
         break ;
        }
        else if (nbre%i !=0)
        { 
         if (i<nbre-1)
         {
           i= i; 
         }
         else
         {
          liste_prime_nbre[n+1]=nbre;
          n++;
         }
        }
    
      }
      nbre++;
    } 

  for (i=0; i<25; i++)
  {
    printf("%d\n",liste_prime_nbre[i]);
  }
    return 0;
}