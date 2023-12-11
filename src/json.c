/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
/*
 * Code pour le traitement des messages au format JSON
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>
#include "validation.h"


int parse_json(char *data)
{

    // Vérification de la validité du JSON
    if (!isJSONValid(data)) {
        printf("Erreur : Le JSON n'est pas valide.\n");
        return 0;
    }

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

    // supprime les espaces en fin de chaîne
    while (values[strlen(values) - 1] == ' ') {
        values[strlen(values) - 1] = '\0';
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

int envoie_json(int socketfd, char *data)
{
    int size = 0;
    char *data2[1024];
    char *token, *rest, code[1024];


    token = strtok_r(data, ":", &rest);
    strcpy(code, token);
//    rest = strtok(rest, " ");
//    printf("token :%s\n", rest);
    token = strtok_r(rest, ",", &rest);

    while (token != NULL)
    {
//        printf("token :%s\n", token);
        while (token[0] == ' ')
            token++;
        data2[size] = token;
        size++;
        token = strtok_r(rest, ",", &rest);
    }


//    printf("code :%s\n", code);
//    printf("token :%s\n", token);
//    printf("rest :%s\n", rest);
//    printf("size :%d\n", size);
//    for (int i = 0; i < size; ++i) {
//        printf("data2[%d] :%s\n", i, data2[i]);
//    }

    char json_data[1024];
    memset(json_data, 0, sizeof(json_data));

    strcpy(json_data, "{\n\t");
    strcat(json_data, "\"code\" : \"");
    strcat(json_data, code);
    strcat(json_data, "\",\n\t\"valeurs\" : [ ");

//    int i = 0;

//    printf("%zu", sizeof(*data));
//    printf("%zu", sizeof(data[0]));

    for (int i = 0; i < size; ++i) {
        regex_t regex;
        int is_number;
        is_number = regcomp(&regex, "^-?[0-9]+(\\.[0-9]+)?$", REG_EXTENDED);
        if (is_number != 0)
        {
            exit(EXIT_FAILURE);
        }
//        printf("data2[%d] :%s\n", i, data2[i]);
//        if(!is_number)
//            printf("Match\n");
//        else if (is_number == REG_NOMATCH)
//            printf("No match\n");
//        else
//        {
//            printf("regexec failed\n");
//            exit(EXIT_FAILURE);
//        }

        is_number = regexec(&regex, data2[i], 0, NULL, 0);
//        is_number = 1;
        regfree(&regex);
//        printf("is_number %s:%d\n", data2[i], is_number);
        if (i != size && is_number != 0)
            strcat(json_data, "\"");
        strcat(json_data, data2[i]);
        if (i != size - 1)
        {
            if (is_number != 0)
                strcat(json_data, "\"");
            strcat(json_data, ", ");
        }
        else if (is_number != 0)
            strcat(json_data, "\"");
    }

    strcat(json_data, " ]\n}\n");

    printf("%s", json_data);

    // Vérification de la validité du JSON
    if (!isJSONValid(json_data))
    {
        printf("Erreur : Le JSON n'est pas valide.\n");
        return 0;
    }

    int write_status = write(socketfd, json_data, strlen(json_data));
    if (write_status < 0)
    {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    return 0;
}