/*
 * SPDX-FileCopyrightText: 2021 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "serveur.h"
#include "client.h"
#include "json.h"
#include "bmp.h"
#include "validation.h"

/*
 * Fonction d'envoi et de réception de messages
 * Il faut un argument : l'identifiant de la socket
 */

int envoie_recois_message(int socketfd, char *json)
{
    char data[1024];
    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    // Demandez à l'utilisateur d'entrer un message
    char message[1024];
    printf("Votre message (max 1000 caracteres): ");
    fgets(message, sizeof(message), stdin);
    message[strlen(message) - 1] = '\0';
    strcpy(data, "message: ");
    strcat(data, message);

    if (strcmp(json, "1") == 0)
    {
        envoie_json(socketfd, data);
    }

    int write_status = write(socketfd, data, strlen(data));
    if (write_status < 0)
    {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    // lire les données de la socket
    int read_status = read(socketfd, data, sizeof(data));
    if (read_status < 0)
    {
        perror("erreur lecture");
        return -1;
    }
    if (strcmp(json, "1") == 0)
    {
        parse_json(data);
    }

    printf("Message recu: %s\n", data);

    return 0;
}

int envoie_nom_de_client(int socketfd, char *json)
{
    char data[1024];
    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    // Récupére le hostname de la machine
    char nom[1024];
    gethostname(nom, sizeof(nom));
    printf("Votre nom: %s\n", nom);
    strcpy(data, "nom: ");
    strcat(data, nom);

    if (strcmp(json, "1") == 0)
    {
        envoie_json(socketfd, data);
    }

    int write_status = write(socketfd, data, strlen(data));
    if (write_status < 0) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    // lire les données de la socket
    int read_status = read(socketfd, data, sizeof(data));
    if (read_status < 0) {
        perror("erreur lecture");
        return -1;
    }

    if (strcmp(json, "1") == 0)
    {
        parse_json(data);
    }

    printf("Nom recu: %s\n", data);

    return 0;
}

int envoie_operateur_numeros(int socketfd, int argc, char **argv, char *json)
{
    char data[1024];
    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    char calcul[1024];

    // Vérifier qu'il y a au moins un opérateur
    if (argc < 4)
    {
        printf("Il faut un opérateur.\n");
        return 0;
    }

    // Vérifier que l'opérateur est valide
    if (!isOperator(argv[3]))
    {
        printf("Opération non reconnue.\n");
        return 0;
    }

    strcpy(calcul, argv[3]);
//    printf("%d\n", argc);


    // Vérifier qu'il y a au moins deux nombres
    if (argc-4 < 2)
    {
        printf("Il faut au moins deux nombres.\n");
        return 0;
    }

    // Vérifier que les nombres sont valides
    for (int i = 4; i < argc; i++)
    {
//        char tmp[1024];
        strcat(calcul, ", ");
        if (!isNumber(argv[i]))
        {
            printf("Nombre non valide : %s\n", argv[i]);
            return 0;
        }
        strcat(calcul, argv[i]);
    }
//    printf("%s", calcul);

//    sprintf(calcul, "%s, %s, %s", operateur, numA, numB);
    printf("Votre calcul: %s\n", calcul);
    strcpy(data, "calcul: ");
    strcat(data, calcul);



    if (strcmp(json, "1") == 0) {
        envoie_json(socketfd, data);
    }

    int write_status = write(socketfd, data, strlen(data));
    if (write_status < 0) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    // lire les données de la socket
    int read_status = read(socketfd, data, sizeof(data));
    if (read_status < 0) {
        perror("erreur lecture");
        return -1;
    }

    if (strcmp(json, "1") == 0) {
        parse_json(data);
    }

    printf("Calcul recu: %s\n", data);

    return 0;

}

int envoie_les_couleurs(int socketfd, int argc, char **argv, char *json)
{
    char data[1024];
    memset(data, 0, sizeof(data));
    char *nb_couleurs;

    if (argv[3] == NULL)
    {
        printf("Il faut un nombre de couleurs.\n");
        return 0;
    }
    else if (!isInt(argv[3]))
    {
        printf("Nombre de couleurs invalide : %s\n", argv[3]);
        return 0;
    }
    else
        nb_couleurs = argv[3];


    strcpy(data, "couleurs: ");
    strcat(data, nb_couleurs);

//    printf("%s", argv[4]);
//    strcat(data, data_color);

//    printf("%d\n", argc);
//    printf("%d %d\n", argc-4, atoi(nb_couleurs));

    if (argc-4 < atoi(nb_couleurs))
    {
        printf("Nombre de couleurs données inférieur à : %d\n", atoi(nb_couleurs));
        return 0;
    }

    for (int i = 0; i < atoi(nb_couleurs); i++)
    {
        strcat(data, ", ");
        if (!isHexa(argv[i+4]))
        {
            printf("Couleur non valide : %s\n", argv[i+4]);
            return 0;
        }
        strcat(data, argv[i+4]);
    }

    printf("%s\n", data);

    if (strcmp(json, "1") == 0)
    {
        envoie_json(socketfd, data);
    }

    int write_status = write(socketfd, data, strlen(data));
    if (write_status < 0) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    int read_status = read(socketfd, data, sizeof(data));
    if (read_status < 0) {
        perror("erreur lecture");
        return -1;
    }

    if (strcmp(argv[1], "1") == 0)
    {
        parse_json(data);
    }

    printf("Couleurs recu: %s\n", data);

    return 0;

}

int envoie_balises(int socketfd, int argc, char **argv, char* json)
{
    char data[1024];
    memset(data, 0, sizeof(data));
    char *nb_balises;

    if (argv[3] == NULL)
    {
        printf("Il faut un nombre de balises.\n");
        return 0;
    }
    else if (!isInt(argv[3]))
    {
        printf("Nombre de balises invalide : %s\n", argv[3]);
        return 0;
    }
    else
        nb_balises = argv[3];

    strcpy(data, "balises: ");
    strcat(data, nb_balises);


    if (argc-4 < atoi(nb_balises))
    {
        printf("Nombre de balises données inférieur à : %d\n", atoi(nb_balises));
        return 0;
    }

    for (int i = 0; i < atoi(nb_balises); i++)
    {
        strcat(data, ", ");
        if (!isBalise(argv[i+4]))
        {
            printf("Balise non valide : %s\n", argv[i+4]);
            return 0;
        }
        strcat(data, argv[i+4]);
    }

    printf("%s\n", data);

    if (strcmp(json, "1") == 0)
    {
        envoie_json(socketfd, data);
    }

    int write_status = write(socketfd, data, strlen(data));
    if (write_status < 0) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    int read_status = read(socketfd, data, sizeof(data));
    if (read_status < 0) {
        perror("erreur lecture");
        return -1;
    }

    if (strcmp(argv[1], "1") == 0)
    {
        parse_json(data);
    }

    printf("Balises recu: %s\n", data);

    return 0;
}


void analyse(char *pathname, char *data)
{
    // compte de couleurs
    couleur_compteur *cc = analyse_bmp_image(pathname);


    // Demandez à l'utilisateur d'entrer un nombre de couleurs
    int nbCouleurs;
    int count;
    strcpy(data, "couleurs: ");
    char temp_string[10] = "10,";
    printf("\nNombre de couleurs (max 30): ");
    scanf("%d", &nbCouleurs);
    if (nbCouleurs <= 30 && nbCouleurs > 0)
    {
        sprintf(temp_string, "%d,", nbCouleurs);
    }
    else
    {
        nbCouleurs = 10;
    }

    if (cc->size < 10)
    {
        sprintf(temp_string, "%d,", cc->size);
    }
    strcat(data, temp_string);

    // choisir 10 couleurs
    for (count = 1; count < nbCouleurs+1 && cc->size - count > 0; count++)
    {
        if (cc->compte_bit == BITS32)
        {
            sprintf(temp_string, "#%02x%02x%02x,", cc->cc.cc24[cc->size - count].c.rouge, cc->cc.cc32[cc->size - count].c.vert, cc->cc.cc32[cc->size - count].c.bleu);
        }
        if (cc->compte_bit == BITS24)
        {
            sprintf(temp_string, "#%02x%02x%02x,", cc->cc.cc32[cc->size - count].c.rouge, cc->cc.cc32[cc->size - count].c.vert, cc->cc.cc32[cc->size - count].c.bleu);
        }
        strcat(data, temp_string);
    }

    // enlever le dernier virgule
    data[strlen(data) - 1] = '\0';
}

int envoie_couleurs(int socketfd, char *pathname)
{
    char data[1024];
    memset(data, 0, sizeof(data));
    analyse(pathname, data);

    int write_status = write(socketfd, data, strlen(data));
    if (write_status < 0)
    {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int main(int argc, char **argv)
{
    int socketfd;

    struct sockaddr_in server_addr;

    if (argc < 2)
    {
        printf("usage: ./client chemin_bmp_image\n");
        return (EXIT_FAILURE);
    }

    /*
     * Creation d'une socket
     */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // détails du serveur (adresse et port)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // demande de connection au serveur
    int connect_status = connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connect_status < 0)
    {
        perror("connection serveur");
        exit(EXIT_FAILURE);
    }
    if (argc != 2)
    {
        if (!isFunction(argv[2]))
        {
            printf("Fontion non reconnue.\n");
            return (EXIT_FAILURE);
        }
        // envoyer et recevoir un message
        if (strcmp(argv[2], "message") == 0)
            envoie_recois_message(socketfd, argv[1]);
        else if (strcmp(argv[2], "nom") == 0)
            envoie_nom_de_client(socketfd, argv[1]);
        else if (strcmp(argv[2], "calcul") == 0)
            envoie_operateur_numeros(socketfd, argc, argv, argv[1]);
        else if (strcmp(argv[2], "couleurs") == 0)
            envoie_les_couleurs(socketfd, argc, argv, argv[1]);
        else if (strcmp(argv[2], "balises") == 0)
            envoie_balises(socketfd, argc, argv, argv[1]);
    }
    else
    {
        // envoyer et recevoir les couleurs prédominantes
        // d'une image au format BMP (argv[1])
        envoie_couleurs(socketfd, argv[1]);
    }

    close(socketfd);
}
