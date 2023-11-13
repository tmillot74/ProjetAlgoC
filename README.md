# ProjetAlgoC

Thibault MILLOT/ Thomas PONCET

## Commande message

Pour chaque commande, afin d'utiliser le format json, le premier argument doit être "1" et pour le format de base, n'importe quelle autre chaine de caractère.

Pour envoyer un message, il faut passer deux paramètres lors du lancement du client, le second argument doit être "message".

Exemple: Pour le JSON `./client 1 message`, sinon pour le format de base `./client truc message`.

Une fois lancé, le client écrit dans la console son message.

## Commande nom

Pour afficher le nom de l'utilisateur, il faut passer deux paramètres lors du lancement du client, le second argument doit être "nom"

Exemple: Pour le JSON `./client 1 nom`, sinon pour le format de base `./client truc nom`.

## Commande calcul

Pour effectuer un calcul, il faut passer 5 paramètres lors du lancement du client, le deuxième argument doit être "calcul", le troisième doit être l'opérateur et les deux derniers doivent être les nombres du calcul.

Exemple: `./client 1 calcul + 1 2` sinon pour le format de base `./client truc calcul '*' 4 2`.

## Commande couleurs

Pour envoyer des couleurs, il faut passer au minimum 4 paramètres lors du lancement du client, le deuxième argument doit être "couleurs", le troisième doit être le nombre de couleurs et les suivants doivent être les couleurs>

Exemple: `./client 1 couleurs 2 "#ff00ff" "#0000FF"` sinon pour le format de base `./client truc couleurs 2 "#ff00ff" "#0000FF"`.

## Commande balises

Pour envoyer des balises, il faut passer au minimum 4 paramètres lors du lancement du client, le deuxième argument doit être "balises", le troisième doit être le nombre de balises et les suivants doivent être les balises.

Exemple: `./client 1 balises 2 "#b" "#i"` sinon pour le format de base `./client truc balises 2 "#b" "#i"`.

## Analyse des couleurs d'une image

Pour analyser les couleurs d'une image, il faut passer 1 paramètre lors du lancement du client. Il s'agit du chemin vers l'image.

Exemple: `./client image.png`;