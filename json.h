
/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/*
 * Code permettant de stocker différents types de couleurs : format 24 bits (RGB) ou format 32 bits (32). 
 */

int parse_json(char *data);
//int envoie_json(int socketfd, char *code, char **data, int size);
int envoie_json(int socketfd, char *data);
int isJSONValid(const char *str);
