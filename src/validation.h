
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

int isJSONValid(const char *chaine);
int isFunction(const char *chaine);
int isNumber(const char *chaine);
int isInt(const char *chaine);
int isOperator(const char *chaine);
int isHexa(const char *chaine);
int isBalise(const char *chaine);
