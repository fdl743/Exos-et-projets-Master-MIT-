#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* Ce programme a pour but de lire un tableau d'un fichier csv et de transférer dans un fichier HTML, qu'il aura générer.
Le plan est le suivant :
- Ouvrir le fichier csv contenant les données 
- Lire les données à exporter 
- Ouvrir le fichier html de destination et y transférer ligne par ligne les données
- Fermer le fichier html
- Fermer le fichier csv
*/
int main()
{
 FILE *csv = fopen("fichierINT.csv", "r" );
 FILE *html = fopen("tableau.html", "w");

 char ligne[500];

 fprintf(html, "<html>\n <head><style> table { \n border-collapse: collapse \n; width: 60% \n; margin: auto\n;}\n th, td { \n border: 1px solid \n ; padding: 10px;\n} \n p{\n font-weight:bold; font-size: 40px; \n text-align: center \n} \n span{ \n color: red;\n} \n tr:hover{\n background-color: #444; \n color: white;\n transition: 0.2s;\n }\n</style>\n<\head> \n<body>\n <p> Voici le tableau (html) des données recueillies dans le <span> fichierINT.csv</span> :</p><table>\n");
 while(fgets(ligne, sizeof(ligne), csv))
 {
   fprintf(html, "<tr>");
   char *token = strtok(ligne, ",");
   while(token != NULL)
   {
     fprintf(html, "<td>%s</td>", token);
     token = strtok(NULL, ",");
   }
   fprintf(html, "</tr>\n");
 }
 fprintf(html, "</table>\n </body>\n </html>");
 fclose(html);
 fclose(csv);

return 0;
}


