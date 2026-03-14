#include <stdio.h>

int main()
{
    int A[3][3]={{1, 2, 3},{0, 1, 2}}, B[3][3]={{1, 2, 3},{0, 1, 2}}, C[3][3]={{0, 0, 0},{0, 0, 0}};
    int i, j, k;
    int m=3, n=3, p=3;

    for(i=0;i<m;i++)
    {
    for(j=0;j<p;j++)
      {
          C[i][j] = 0;

           for(k=0;k<n;k++)
           {
                C[i][j] += A[i][k] * B[k][j];
           }
        }
    }
    printf("Matrice produit :\n");
    for(i=0;i<m;i++)
    {
        for(j=0;j<p;j++)
        {
            printf("%d ",C[i][j]);
        }
        printf("\n");
    }

    return 0;
}