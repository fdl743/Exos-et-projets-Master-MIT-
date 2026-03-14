#include<stdio.h>
#include<stdlib.h>
int main()
{
    int i, n=3;
    int vect_1[3]={1, 2, 3}, vect_2[3]={4, 0, 1}, res[3];

   
    printf("1.La somme des deux vecteurs V_1 = (1, 2, 3) et V_2 = (4, 0, 1) est :\n");
    for(i=0; i<n; i++)
    {
        res[i] = vect_1[i] + vect_2[i];
        printf("%d\n", res[i]);
    }

    printf("2.Le produit scalaire des deux vecteurs entrés est :\n");
    int produit_scalaire = 0;
    for(i=0; i<n; i++)
    {
        produit_scalaire += vect_1[i] * vect_2[i];
    }
    printf("%d\n", produit_scalaire);
    
    return 0;
}