
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
#include "operations.h"

float addition(int nbNum, float *tabNum)
{
    float result = 0;
    for (int i = 0; i < nbNum; i++) {
        result += tabNum[i];
    }
    return result;
}

float soustraction(int nbNum, float *tabNum)
{
    float result = tabNum[0];
    for (int i = 1; i < nbNum; i++) {
        result -= tabNum[i];
    }
    return result;
}

float multiplication(int nbNum, float *tabNum)
{
    float result = 1;
    for (int i = 0; i < nbNum; i++) {
        result *= tabNum[i];
    }
    return result;
}

float division(int nbNum, float *tabNum)
{
    float result = tabNum[0];
    for (int i = 1; i < nbNum; i++) {
        result /= tabNum[i];
    }
    return result;
}

float moyenne(int nbNum, float *tabNum)
{
    float result = 0;
    for (int i = 0; i < nbNum; i++) {
        result += tabNum[i];
    }
    return result / nbNum;
}

float variance(int nbNum, float *tabNum)
{
    float result = 0;
    float moy = moyenne(nbNum, tabNum);
    for (int i = 0; i < nbNum; i++) {
        result += pow(tabNum[i] - moy, 2);
    }
    return result / nbNum;
}

float ecart_type(int nbNum, float *tabNum)
{
    return sqrt(variance(nbNum, tabNum));
}

float minimum(int nbNum, float *tabNum)
{
    float result = tabNum[0];
    for (int i = 1; i < nbNum; i++) {
        if (tabNum[i] < result)
            result = tabNum[i];
    }
    return result;
}

float maximum(int nbNum, float *tabNum)
{
    float result = tabNum[0];
    for (int i = 1; i < nbNum; i++) {
        if (tabNum[i] > result)
            result = tabNum[i];
    }
    return result;
}