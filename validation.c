
/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/* 
 * Le but principal de ce code est de s'assurer que les messages envoyés par le client sont
 * valides, c'est-à-dire que les messages respectent le format JSON et
 * respectent le protocole spécifié par le côté serveur.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>

// Function to test if a string resembles JSON
int isJSONValid(const char *chaine) {
    regex_t regex;
    int reti;

    // Expression régulière pour vérifier la structure JSON
    const char *pattern = "\\{\\s*\"code\"\\s*:\\s*\"[^\"]+\"\\s*,\\s*\"valeurs\"\\s*:\\s*\\[.*\\]\\s*\\}";

    // Compiler l'expression régulière
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Erreur lors de la compilation de l'expression régulière\n");
        return 0;  // Retourner 0 en cas d'erreur
    }

    // Exécuter la correspondance
    reti = regexec(&regex, chaine, 0, NULL, 0);
    regfree(&regex);  // Libérer la mémoire utilisée par la structure regex

    if (reti) {
        return 0;  // La chaîne ne correspond pas à l'expression régulière, ce n'est pas un JSON valide
    }

    // Recherche du début du tableau de valeurs
    char *start = strstr(chaine, "\"valeurs\" : [");
    if (start == NULL) {
        printf("Erreur : Impossible de trouver le tableau de valeurs.\n");
        return 0;
    }

    // Déplacement au début des valeurs
    start += strlen("\"valeurs\" : [");

    // Recherche de la fin du tableau de valeurs
    char *end = strchr(start, ']');
    if (end == NULL) {
        printf("Erreur : Impossible de trouver la fin du tableau de valeurs.\n");
        return 0;
    }

    // Copie des valeurs dans une nouvelle chaîne de caractères
    char *values = (char *)malloc(end - start + 1);
    strncpy(values, start, end - start);

    values[end - start] = '\0';

    // supprime les espaces en début de chaîne
    while (values[0] == ' ') {
        values++;
    }

    // supprime les espaces en fin de chaîne
    while (values[strlen(values) - 1] == ' ') {
        values[strlen(values) - 1] = '\0';
    }

//    printf("values %s\n", values);

    // On transforme la chaîne en tableau de chaînes, on compte le nombre de valeurs et on enlève les espaces
    // On ne prend en compte que les valeurs qui ne sont pas entre guillemets
    int nbValeurs = 0;
    for (int i = 0; i < strlen(values); i++) {
        if (values[i] == ',') {
            nbValeurs++;
        }
    }
    nbValeurs++;
//    printf("nbValeurs %d\n", nbValeurs);

    char **valeurs = (char **)malloc(nbValeurs * sizeof(char *));
    char *valeur = strtok(values, ",");
    int i = 0;
    while (valeur != NULL) {
        valeurs[i] = valeur;
        valeur = strtok(NULL, ",");
        i++;
    }

    for (int i = 0; i < nbValeurs; i++) {
        // supprime les espaces en début de chaîne
        while (valeurs[i][0] == ' ') {
            valeurs[i]++;
        }

        // supprime les espaces en fin de chaîne
        while (valeurs[i][strlen(valeurs[i]) - 1] == ' ') {
            valeurs[i][strlen(valeurs[i]) - 1] = '\0';
        }
    }

//    // affichage des valeurs
//    for (i = 0; i < nbValeurs; i++) {
//        printf("valeur %d: %s\n", i, valeurs[i]);
//    }

    // Vérifier que les valeurs sont des nombres avec un regex
    // On ne vérifie que les valeurs qui n'ont pas de guillemets
    const char *pattern2 = "^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)$";
    const char *pattern3 = "^\"[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)\"$";
    if (reti) {
        fprintf(stderr, "Erreur lors de la compilation de l'expression régulière\n");
        return 0;  // Retourner 0 en cas d'erreur
    }

    for (i = 0; i < nbValeurs; i++) {
        if (valeurs[i][0] != '"') {
            reti = regcomp(&regex, pattern2, REG_EXTENDED);
            reti = regexec(&regex, valeurs[i], 0, NULL, 0);
            if (reti) {
                return 0;  // La chaîne ne correspond pas à l'expression régulière, ce n'est pas un JSON valide
            }
        }
        else {
            reti = regcomp(&regex, pattern3, REG_EXTENDED);
            reti = regexec(&regex, valeurs[i], 0, NULL, 0);
            if (!reti) {
                return 0;  // La chaîne correspond à l'expression régulière, ce n'est pas un JSON valide
            }
        }
    }

    return 1;  // La chaîne correspond à la structure JSON et respecte la condition sur les guillemets
}