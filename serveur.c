/*
 * SPDX-FileCopyrightText: 2021 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <math.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serveur.h"
#include "json.h"
int socketfd;

int visualize_plot()
{
    const char *browser = "firefox";

    char command[256];
    snprintf(command, sizeof(command), "%s %s", browser, svg_file_path);

    int result = system(command);

    if (result == 0)
    {
        printf("SVG file opened in %s.\n", browser);
    }
    else
    {
        printf("Failed to open the SVG file.\n");
    }

    return 0;
}

double degreesToRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}

int plot(char *data)
{
    int i;
    char *saveptr = NULL;
    char *str = data;
    char *token = strtok_r(str, ",", &saveptr);
    int nbCouleurs = atoi(strtok(data, "couleurs: "));
//    printf("%d", nbCouleurs);
    const int num_colors = nbCouleurs;

    double angles[num_colors];
    memset(angles, 0, sizeof(angles));

    FILE *svg_file = fopen(svg_file_path, "w");
    if (svg_file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    fprintf(svg_file, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(svg_file, "<svg width=\"400\" height=\"400\" xmlns=\"http://www.w3.org/2000/svg\">\n");
    fprintf(svg_file, "  <rect width=\"100%%\" height=\"100%%\" fill=\"#ffffff\" />\n");

    double center_x = 200.0;
    double center_y = 200.0;
    double radius = 150.0;

    double start_angle = -90.0;

    str = NULL;
    i = 0;
    while (1)
    {
        token = strtok_r(str, ",", &saveptr);
        if (token == NULL)
        {
            break;
        }
        str = NULL;
        angles[i] = 360.0 / num_colors;

        double end_angle = start_angle + angles[i];

        double start_angle_rad = degreesToRadians(start_angle);
        double end_angle_rad = degreesToRadians(end_angle);

        double x1 = center_x + radius * cos(start_angle_rad);
        double y1 = center_y + radius * sin(start_angle_rad);
        double x2 = center_x + radius * cos(end_angle_rad);
        double y2 = center_y + radius * sin(end_angle_rad);

        fprintf(svg_file, "  <path d=\"M%.2f,%.2f A%.2f,%.2f 0 0,1 %.2f,%.2f L%.2f,%.2f Z\" fill=\"%s\" />\n",
                x1, y1, radius, radius, x2, y2, center_x, center_y, token);

        start_angle = end_angle;
        i++;
    }

    fprintf(svg_file, "</svg>\n");

    fclose(svg_file);

    visualize_plot();
    return 0;
}

/* renvoyer un message (*data) au client (client_socket_fd)
 */
int renvoie_message(int client_socket_fd, char *data, int json)
{
    if (json == 1)
    {
        char json_data[1024];
        strcpy(json_data, data);
        envoie_json(client_socket_fd, json_data);
    }
    else
    {
        int data_size = write(client_socket_fd, (void *)data, strlen(data));

        if (data_size < 0)
        {
            perror("erreur ecriture");
            return (EXIT_FAILURE);
        }
    }
    // Envoyer un message au client
    printf("Message envoyé: %s\n", data);

    return (EXIT_SUCCESS);
}

int recois_numeros_calcule(int client_socket_fd, char *data, int json)
{
    char* operation;
//    char operation;
//    float num1, num2;
//    sscanf(data, "%*s, %c, %f, %f", &operation, &num1, &num2);

    char* token;
    char* rest = data;

    token = strtok_r(rest, ", ", &rest);
    int i = 0;
    float num[1024];
    int nbNum = 0;
    while (token)
    {
//        printf("%s\n", token);
        if (i==1)
        {
            operation = token;
//            operation = token[0];
//            printf("Operation: %c\n", operation);
        }
//        else if (i==2)
//        {
//            num1 = atof(token);
////            printf("Num1: %f\n", num1);
//        }
//        else if (i==3)
//        {
//            num2 = atof(token);
////            printf("Num2: %f\n", num2);
//        }
        else if (i>1)
        {
//            printf("Num: %f\n", atof(token));
            num[nbNum] = atof(token);
            nbNum++;
        }
        token = strtok_r(rest, ", ", &rest);
        i++;
    }
//    for (i = 0; i < nbNum; i++)
//    {
//        printf("Num: %f\n", num[i]);
//    }

    // Effectuer le calcul
    float result = 0;
    if (strcmp(operation, "+") == 0)
    {
        for (i = 0; i < nbNum; i++)
        {
            result += num[i];
        }
    }
    else if (strcmp(operation, "-") == 0)
    {
        for (i = 0; i < nbNum; i++)
        {
            printf("%f\n", num[i]);
            if (i == 0)
                result = num[i];
            else
                result -= num[i];
            printf("%f\n", result);
        }
    }
    else if (strcmp(operation, "*") == 0)
    {
        for (i = 0; i < nbNum; i++)
        {
            if (i == 0)
                result = num[i];
            else
                result *= num[i];
        }
    }
    else if (strcmp(operation, "/") == 0)
    {
        for (i = 0; i < nbNum; i++)
        {
            if (i == 0)
                result = num[i];
            else
                result /= num[i];
        }
    }
    else if (strcmp(operation, "moyenne") == 0)
    {
        for (i = 0; i < nbNum; i++)
        {
            result += num[i];
        }
        result /= nbNum;
    }
    else if (strcmp(operation, "minimum") == 0)
    {
        result = num[0];
        for (i = 1; i < nbNum; i++)
        {
            if (num[i] < result)
            {
                result = num[i];
            }
        }
    }
    else if (strcmp(operation, "maximum") == 0)
    {
        result = num[0];
        for (i = 1; i < nbNum; i++)
        {
            if (num[i] > result)
            {
                result = num[i];
            }
        }
    }
    else if (strcmp(operation, "écart-type") == 0)
    {
        float moyenne = 0;
        for (i = 0; i < nbNum; i++)
        {
            moyenne += num[i];
        }
        moyenne /= nbNum;

        float somme = 0;
        for (i = 0; i < nbNum; i++)
        {
            somme += pow(num[i] - moyenne, 2);
        }
        result = sqrt(somme / nbNum);
    }
    else
    {
        printf("Opération non reconnue.\n");
        return (EXIT_FAILURE);
    }
//    switch (operation)
//    {
//        case '+':
//            result = num1 + num2;
//            break;
//        case '-':
//            result = num1 - num2;
//            break;
//        case '*':
//            result = num1 * num2;
//            break;
//        case '/':
//            result = num1 / num2;
//            break;
//        default:
//            printf("Opération non reconnue.\n");
//            return (EXIT_FAILURE);
//    }

    // Envoyer le résultat au client
    char result_str[1024];
    sprintf(result_str, "calcul: %f", result);
//    printf("Message envoyé: %s\n", result_str);
    renvoie_message(client_socket_fd, result_str, json);

    return (EXIT_SUCCESS);
}

int recois_couleurs(int client_socket_fd, char *data, int json)
{

//    char couleursData[1024];
//    char* tok = strtok(data, "couleurs : ");
//
//
//
//    while (tok != NULL)
//    {
//        printf("%s\n", tok);
//        tok = strtok(NULL, ", ");
//    }

    char* token;
    char* rest = data;

    token = strtok_r(rest, ", ", &rest);
    int i = 0;
    int nbCouleurs;
    char *couleurs[1024];
    while (token)
    {
//        printf("%s\n", token);
        if (i==1)
        {
            nbCouleurs = atoi(token);
//            printf("Nombre de couleurs: %d\n", nbCouleurs);
        }
        else if (i>1)
        {
            couleurs[i-2] = token;
//            printf("Couleur: %s\n", *couleurs);
        }
        token = strtok_r(rest, ", ", &rest);
        i++;
    }

//    printf("Nombre de couleurs: %d\n", nbCouleurs);
//    for (int i = 0; i < nbCouleurs; i++)
//    {
//        printf("Couleur: %s\n", couleurs[i]);
//    }

    // Enregistrer les couleurs dans le fichier
    FILE *fichier = fopen("couleurs.txt", "w");
    if (fichier == NULL)
    {
        perror("Error opening file");
        return 1;
    }

//    fprintf(fichier, "%d\n", nbCouleurs);
    for (i = 0; i < nbCouleurs; i++)
    {
        fprintf(fichier, "%s\n", couleurs[i]);
    }

    fclose(fichier);

    // Envoyer un message de confirmation au client
    char result_str[1024];
    sprintf(result_str, "couleurs: enregistré");
    renvoie_message(client_socket_fd, result_str, json);

    return (EXIT_SUCCESS);
}

int recois_balises(int client_socket_fd, char *data, int json)
{

//    char couleursData[1024];
//    char* tok = strtok(data, "couleurs : ");
//
//
//
//    while (tok != NULL)
//    {
//        printf("%s\n", tok);
//        tok = strtok(NULL, ", ");
//    }

    char* token;
    char* rest = data;

    token = strtok_r(rest, ", ", &rest);
    int i = 0;
    int nbCouleurs;
    char *couleurs[1024];
    while (token)
    {
//        printf("%s\n", token);
        if (i==1)
        {
            nbCouleurs = atoi(token);
//            printf("Nombre de couleurs: %d\n", nbCouleurs);
        }
        else if (i>1)
        {
            couleurs[i-2] = token;
//            printf("Couleur: %s\n", *couleurs);
        }
        token = strtok_r(rest, ", ", &rest);
        i++;
    }

//    printf("Nombre de couleurs: %d\n", nbCouleurs);
//    for (int i = 0; i < nbCouleurs; i++)
//    {
//        printf("Couleur: %s\n", couleurs[i]);
//    }

    // Enregistrer les couleurs dans le fichier
    FILE *fichier = fopen("balises.txt", "w");
    if (fichier == NULL)
    {
        perror("Error opening file");
        return 1;
    }

//    fprintf(fichier, "%d\n", nbCouleurs);
    for (i = 0; i < nbCouleurs; i++)
    {
        fprintf(fichier, "%s\n", couleurs[i]);
    }

    fclose(fichier);

    // réinitialise lensemble des données
    memset(data, 0, sizeof(*data));

    // Envoyer un message de confirmation au client
    char result_str[1024];
    sprintf(result_str, "balises: enregistré");
    renvoie_message(client_socket_fd, result_str, json);

    return (EXIT_SUCCESS);
}


/* accepter la nouvelle connection d'un client et lire les données
 * envoyées par le client. En suite, le serveur envoie un message
 * en retour
 */
int recois_envoie_message(int client_socket_fd, char data[1024])
{
    /*
     * extraire le code des données envoyées par le client.
     * Les données envoyées par le client peuvent commencer par le mot "message :" ou un autre mot.
     */
//    printf("Message recu: %s\n", data);
    char code[1024];
    int json = 0;

    sscanf(data, "%s", code);
    if (strcmp(code, "{") == 0)
    {
        json = 1;
        parse_json(data);
        sscanf(data, "%s", code);
    }

//    printf("Code: %s\n", code);
    printf("Message recu: %s\n", data);

    // Si le message commence par le mot: 'message:'
    if (strcmp(code, "message:") == 0)
    {
        renvoie_message(client_socket_fd, data, json);
    }
    // Si le message commence par le mot: 'nom:'
    else if (strcmp(code, "nom:") == 0)
    {
        renvoie_message(client_socket_fd, data, json);
    }
    // Si le message commence par le mot: 'calcul:'
    else if (strcmp(code, "calcul:") == 0)
    {
        recois_numeros_calcule(client_socket_fd, data, json);
    }
    // Si le message commence par le mot: 'couleurs:'
    else if (strcmp(code, "couleurs:") == 0)
    {
        char saveptr[1024];
        strcpy(saveptr, data);
        recois_couleurs(client_socket_fd, saveptr, json);
        plot(data);
    }
    // Si le message commence par le mot: 'balises:'
    else if (strcmp(code, "balises:") == 0)
    {
        recois_balises(client_socket_fd, data, json);
    }
//    else if ()
//    else
//    {
//        plot(data);
//    }

    return (EXIT_SUCCESS);
}

// Fonction de gestion du signal Ctrl+C
void gestionnaire_ctrl_c(int signal)
{
    printf("\nSignal Ctrl+C capturé. Sortie du programme.\n");
    // fermer le socket
    close(socketfd);
    exit(0); // Quitter proprement le programme.
}

int main()
{
    int bind_status;

    struct sockaddr_in server_addr;

    /*
     * Creation d'une socket
     */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        perror("Unable to open a socket");
        return -1;
    }

    int option = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    // détails du serveur (adresse et port)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Relier l'adresse à la socket
    bind_status = bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_status < 0)
    {
        perror("bind");
        return (EXIT_FAILURE);
    }

    // Enregistrez la fonction de gestion du signal Ctrl+C
    signal(SIGINT, gestionnaire_ctrl_c);

    // Écouter les messages envoyés par le client en boucle infinie
    while (1)
    {
        // Écouter les messages envoyés par le client
        listen(socketfd, 10);

        // Lire et répondre au client
        struct sockaddr_in client_addr;
        char data[1024];

        unsigned int client_addr_len = sizeof(client_addr);

        // nouvelle connection de client
        int client_socket_fd = accept(socketfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket_fd < 0)
        {
            perror("accept");
            return (EXIT_FAILURE);
        }

        // la réinitialisation de l'ensemble des données
        memset(data, 0, sizeof(data));

        // lecture de données envoyées par un client
        int data_size = read(client_socket_fd, (void *)data, sizeof(data));

        if (data_size < 0)
        {
            perror("erreur lecture");
            return (EXIT_FAILURE);
        }

        recois_envoie_message(client_socket_fd, data);
    }

    return 0;
}
