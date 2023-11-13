/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
/*
 * Code pour le traitement des messages au format JSON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int parse_json(char *data)
{
// Recherche du début du tableau de valeurs
    char *start = strstr(data, "\"valeurs\" : [");
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

    // Remplacement des caractères indésirables
    for (int i = 0; i < strlen(values); i++) {
        if (values[i] == '"' || values[i] == '[' || values[i] == ']') {
            values[i] = ' ';
        }
    }
    // supprime les espaces en début de chaîne
    while (values[0] == ' ') {
        values++;
    }


    // Récupération du code
    char *codeStart = strstr(data, "\"code\" : \"") + strlen("\"code\" : \"");
    char *codeEnd = strchr(codeStart, '\"');
    char *code = (char *)malloc(codeEnd - codeStart + 1);
    strncpy(code, codeStart, codeEnd - codeStart);
    code[codeEnd - codeStart] = '\0';



    // Affichage du résultat
//    printf("code %s: values %s\n", code, values);

    char json_data[1024];
    sprintf(json_data, "%s: %s", code, values);
    strcpy(data, json_data);


    // Libération de la mémoire allouée
//    free(values);
//    free(code);

    return 0;
}
