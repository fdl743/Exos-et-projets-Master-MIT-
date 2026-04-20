#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─────────────────────────────────────────────
   CONSTANTES & TYPES
───────────────────────────────────────────── */
#define FICHIER_CSV   "personnes.csv"
#define MAX_LIGNE     256
#define MAX_CHAMP     64
#define SEP           ","

typedef struct {
    int  id;
    char nom[MAX_CHAMP];
    int  age;
    char email[MAX_CHAMP];
} Personne;

/* ─────────────────────────────────────────────
   UTILITAIRES
───────────────────────────────────────────── */

/* Supprime le \n en fin de chaîne */
void strip_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

/* Parse une ligne CSV → Personne. Retourne 1 si OK, 0 sinon */
int parse_ligne(const char *ligne, Personne *p) {
    char copie[MAX_LIGNE];
    strncpy(copie, ligne, MAX_LIGNE - 1);
    copie[MAX_LIGNE - 1] = '\0';
    strip_newline(copie);

    char *token = strtok(copie, SEP);
    if (!token) return 0;
    p->id = atoi(token);

    token = strtok(NULL, SEP);
    if (!token) return 0;
    strncpy(p->nom, token, MAX_CHAMP - 1);

    token = strtok(NULL, SEP);
    if (!token) return 0;
    p->age = atoi(token);

    token = strtok(NULL, SEP);
    if (!token) return 0;
    strncpy(p->email, token, MAX_CHAMP - 1);

    return 1;
}

/* Affiche une Personne de façon formatée */
void afficher_personne(const Personne *p) {
    printf("  [%d] %-20s | Age: %3d | %s\n",
           p->id, p->nom, p->age, p->email);
}

/* Affiche l'en-tête du tableau */
void afficher_entete() {
    printf("\n  %-4s %-20s   %-5s   %s\n", "ID", "NOM", "AGE", "EMAIL");
    printf("  %s\n", "─────────────────────────────────────────────────────");
}

/* Génère un nouvel ID unique (max existant + 1) */
int nouvel_id() {
    FILE *f = fopen(FICHIER_CSV, "r");
    if (!f) return 1;

    char ligne[MAX_LIGNE];
    Personne p;
    int max_id = 0;

    fgets(ligne, MAX_LIGNE, f); /* sauter l'en-tête */
    while (fgets(ligne, MAX_LIGNE, f)) {
        if (parse_ligne(ligne, &p) && p.id > max_id)
            max_id = p.id;
    }
    fclose(f);
    return max_id + 1;
}

/* Initialise le fichier CSV avec l'en-tête si inexistant */
void init_fichier() {
    FILE *f = fopen(FICHIER_CSV, "r");
    if (f) { fclose(f); return; }

    f = fopen(FICHIER_CSV, "w");
    if (!f) { perror("Impossible de créer le fichier"); exit(1); }
    fprintf(f, "id,nom,age,email\n");
    fclose(f);
    printf("  → Fichier '%s' créé.\n", FICHIER_CSV);
}

/* ─────────────────────────────────────────────
   OPÉRATIONS CRUD
───────────────────────────────────────────── */

/* 1. AFFICHER toutes les entrées */
void afficher_tout() {
    FILE *f = fopen(FICHIER_CSV, "r");
    if (!f) { perror("fopen"); return; }

    char ligne[MAX_LIGNE];
    Personne p;
    int nb = 0;

    fgets(ligne, MAX_LIGNE, f); /* sauter l'en-tête */
    afficher_entete();
    while (fgets(ligne, MAX_LIGNE, f)) {
        if (parse_ligne(ligne, &p)) {
            afficher_personne(&p);
            nb++;
        }
    }
    printf("\n  Total : %d enregistrement(s).\n\n", nb);
    fclose(f);
}

/* 2. AJOUTER une entrée */
void ajouter(const char *nom, int age, const char *email) {
    FILE *f = fopen(FICHIER_CSV, "a");
    if (!f) { perror("fopen"); return; }

    int id = nouvel_id();
    fprintf(f, "%d,%s,%d,%s\n", id, nom, age, email);
    fclose(f);
    printf("  → Ajouté : [%d] %s, %d ans, %s\n", id, nom, age, email);
}

/* 3. SUPPRIMER par ID */
void supprimer(int cible_id) {
    FILE *f = fopen(FICHIER_CSV, "r");
    if (!f) { perror("fopen"); return; }

    char tmp_path[] = "tmp_csv.tmp";
    FILE *tmp = fopen(tmp_path, "w");
    if (!tmp) { perror("fopen tmp"); fclose(f); return; }

    char ligne[MAX_LIGNE];
    Personne p;
    int trouve = 0;

    /* Copier l'en-tête */
    fgets(ligne, MAX_LIGNE, f);
    fputs(ligne, tmp);

    while (fgets(ligne, MAX_LIGNE, f)) {
        if (parse_ligne(ligne, &p)) {
            if (p.id == cible_id) {
                trouve = 1; /* on saute cette ligne */
            } else {
                fprintf(tmp, "%d,%s,%d,%s\n", p.id, p.nom, p.age, p.email);
            }
        }
    }

    fclose(f);
    fclose(tmp);

    if (trouve) {
        remove(FICHIER_CSV);
        rename(tmp_path, FICHIER_CSV);
        printf("  → Entrée [%d] supprimée.\n", cible_id);
    } else {
        remove(tmp_path);
        printf("  → ID %d introuvable.\n", cible_id);
    }
}

/* 4. MODIFIER par ID */
void modifier(int cible_id, const char *nom, int age, const char *email) {
    FILE *f = fopen(FICHIER_CSV, "r");
    if (!f) { perror("fopen"); return; }

    char tmp_path[] = "tmp_csv.tmp";
    FILE *tmp = fopen(tmp_path, "w");
    if (!tmp) { perror("fopen tmp"); fclose(f); return; }

    char ligne[MAX_LIGNE];
    Personne p;
    int trouve = 0;

    fgets(ligne, MAX_LIGNE, f);
    fputs(ligne, tmp);

    while (fgets(ligne, MAX_LIGNE, f)) {
        if (parse_ligne(ligne, &p)) {
            if (p.id == cible_id) {
                /* Appliquer uniquement les champs non vides */
                if (strlen(nom)   > 0) strncpy(p.nom,   nom,   MAX_CHAMP - 1);
                if (age > 0)           p.age = age;
                if (strlen(email) > 0) strncpy(p.email, email, MAX_CHAMP - 1);
                trouve = 1;
            }
            fprintf(tmp, "%d,%s,%d,%s\n", p.id, p.nom, p.age, p.email);
        }
    }

    fclose(f);
    fclose(tmp);

    if (trouve) {
        remove(FICHIER_CSV);
        rename(tmp_path, FICHIER_CSV);
        printf("  → Entrée [%d] modifiée.\n", cible_id);
    } else {
        remove(tmp_path);
        printf("  → ID %d introuvable.\n", cible_id);
    }
}

/* 5. RECHERCHER par nom ou email (insensible à la casse) */
void rechercher(const char *terme) {
    FILE *f = fopen(FICHIER_CSV, "r");
    if (!f) { perror("fopen"); return; }

    char ligne[MAX_LIGNE];
    Personne p;
    int nb = 0;

    /* Version simple : recherche dans nom et email */
    fgets(ligne, MAX_LIGNE, f);
    afficher_entete();

    while (fgets(ligne, MAX_LIGNE, f)) {
        if (parse_ligne(ligne, &p)) {
            if (strstr(p.nom,   terme) != NULL ||
                strstr(p.email, terme) != NULL) {
                afficher_personne(&p);
                nb++;
            }
        }
    }

    printf("\n  %d résultat(s) pour \"%s\".\n\n", nb, terme);
    fclose(f);
}

/* ─────────────────────────────────────────────
   MENU INTERACTIF
───────────────────────────────────────────── */

void menu_ajouter() {
    char nom[MAX_CHAMP], email[MAX_CHAMP];
    int  age;

    printf("  Nom   : "); scanf("%63s", nom);
    printf("  Age   : "); scanf("%d",   &age);
    printf("  Email : "); scanf("%63s", email);
    ajouter(nom, age, email);
}

void menu_supprimer() {
    int id;
    printf("  ID à supprimer : ");
    scanf("%d", &id);
    supprimer(id);
}

void menu_modifier() {
    int  id, age;
    char nom[MAX_CHAMP], email[MAX_CHAMP];

    printf("  ID à modifier : ");
    scanf("%d", &id);
    printf("  Nouveau nom   (Enter pour garder) : "); scanf("%63s", nom);
    printf("  Nouvel age    (0 pour garder)     : "); scanf("%d",   &age);
    printf("  Nouvel email  (Enter pour garder) : "); scanf("%63s", email);

    /* Si l'utilisateur a juste tapé '-', on considère vide */
    modifier(id,
             strcmp(nom,   "-") == 0 ? "" : nom,
             age,
             strcmp(email, "-") == 0 ? "" : email);
}

void menu_rechercher() {
    char terme[MAX_CHAMP];
    printf("  Terme de recherche : ");
    scanf("%63s", terme);
    rechercher(terme);
}

void afficher_aide_menu() {
    printf("\n");
    printf("  ╔══════════════════════════════════════╗\n");
    printf("  ║                MENU                  ║\n");
    printf("  ╠══════════════════════════════════════╣\n");
    printf("  ║  1. Afficher toutes les entrées      ║\n");
    printf("  ║  2. Ajouter une entrée               ║\n");
    printf("  ║  3. Supprimer une entrée (par ID)    ║\n");
    printf("  ║  4. Modifier une entrée  (par ID)    ║\n");
    printf("  ║  5. Rechercher                       ║\n");
    printf("  ║  0. Quitter                          ║\n");
    printf("  ╚══════════════════════════════════════╝\n");
    printf("  Veuillez choisir : ");
}

void lancer_menu() {
    int choix;
    do {
        afficher_aide_menu();
        if (scanf("%d", &choix) != 1) { choix = -1; }

        switch (choix) {
            case 1: afficher_tout();    break;
            case 2: menu_ajouter();     break;
            case 3: menu_supprimer();   break;
            case 4: menu_modifier();    break;
            case 5: menu_rechercher();  break;
            case 0: printf("  À bientôt !\n\n"); break;
            default: printf("  Choix invalide, réessayez!\n"); break;
        }
    } while (choix != 0);
}

/* ─────────────────────────────────────────────
   USAGE EN LIGNE DE COMMANDE
───────────────────────────────────────────── */

void afficher_aide_cli(const char *prog) {
    printf("\nUsage :\n");
    printf("  %s                          → menu interactif\n", prog);
    printf("  %s list                     → afficher tout\n", prog);
    printf("  %s add <nom> <age> <email>  → ajouter\n", prog);
    printf("  %s del <id>                 → supprimer\n", prog);
    printf("  %s set <id> <nom> <age> <email> → modifier\n", prog);
    printf("  %s find <terme>             → rechercher\n\n", prog);
    printf("  Pour garder un champ lors de 'set', utilisez '-'\n\n");
}

/* ─────────────────────────────────────────────
   MAIN
───────────────────────────────────────────── */

int main(int argc, char *argv[]) {

    init_fichier();

    /* ── Mode interactif (aucun argument) ── */
    if (argc == 1) {
        lancer_menu();
        return 0;
    }

    /* ── Mode ligne de commande ── */
    const char *cmd = argv[1];

    if (strcmp(cmd, "list") == 0) {
        afficher_tout();

    } else if (strcmp(cmd, "add") == 0) {
        if (argc < 5) {
            printf("  Usage : %s add <nom> <age> <email>\n", argv[0]);
            return 1;
        }
        ajouter(argv[2], atoi(argv[3]), argv[4]);

    } else if (strcmp(cmd, "del") == 0) {
        if (argc < 3) {
            printf("  Usage : %s del <id>\n", argv[0]);
            return 1;
        }
        supprimer(atoi(argv[2]));

    } else if (strcmp(cmd, "set") == 0) {
        if (argc < 6) {
            printf("  Usage : %s set <id> <nom> <age> <email>\n", argv[0]);
            return 1;
        }
        modifier(atoi(argv[2]),
                 strcmp(argv[3], "-") == 0 ? "" : argv[3],
                 atoi(argv[4]),
                 strcmp(argv[5], "-") == 0 ? "" : argv[5]);

    } else if (strcmp(cmd, "find") == 0) {
        if (argc < 3) {
            printf("  Usage : %s find <terme>\n", argv[0]);
            return 1;
        }
        rechercher(argv[2]);

    } else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "--help") == 0) {
        afficher_aide_cli(argv[0]);

    } else {
        printf("  Commande inconnue : '%s'\n", cmd);
        afficher_aide_cli(argv[0]);
        return 1;
    }

    return 0;
}
