
/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/* 
 * Code pour traiter différents types d'opérations mathématiques
 */


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float addition(int nbNum, float *tabNum);
float soustraction(int nbNum, float *tabNum);
float multiplication(int nbNum, float *tabNum);
float division(int nbNum, float *tabNum);
float moyenne(int nbNum, float *tabNum);
float variance(int nbNum, float *tabNum);
float ecart_type(int nbNum, float *tabNum);
float maximum(int nbNum, float *tabNum);
float minimum(int nbNum, float *tabNum);