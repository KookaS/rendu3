/*!
 \file particule.h
 \brief Module gérant le type opaque particule
 \author Simon Gilgien
 \group Sylvain Pellegrini
		Simon Gilgien
 \modifications pour rendu2 public   R. Boulic
 \version 1.1
 \date 2 mai 2018
 */
 
#ifndef PARTICULE_H
#define PARTICULE_H

#include "utilitaire.h"

/**
 * \brief	Configure le nombre de particules. Les indices valides sont dans
 *			l'intervalle [1, nb]. Si nb = 0, les données sont effacées et aucun
 *			indice n'est valide. Les particules nouvellement créées doivent être
 *			configurées avant utilisation.
 * \param nb_part	Le nombre de particules, doit être >=0.
 */
void particule_set_nombre(int nb_part);

/**
 * \brief	Configure les paramètres d'une particule.
 * \param indice	L'indice de la particule à modifier, doit être un indice valide.
 * \param pos		La nouvelle position de la particule.
 * \param energie	La nouvelle énergie de la particule.
 * \param rayon		Le nouveau rayon de la particule.
 */
void particule_set_particule(int indice, C2D pos, double energie);

/**
 * \brief			écriture de l'ensemble des particules dans un fichier.
 * \param fichier	pointeur vers le fichier ouvert en écriture

 */
void particule_ecrire_fichier(FILE *fichier);

/**
 * \brief	Retourne le nombre de particules existantes, c'est-à-dire le plus grand
 *			indice valide.
 * \return	Le plus grand indice valide.
 */
int particule_nb_particules(void);

/**
 * \brief	Retourne la position d'une particule.
 * \param i	L'indice de la particule, doit être un indice valide.
 * \return	La position de la particule i.
 */
C2D particule_position(int i);

/**
 * \brief	Retourne l'énergie d'une particule.
 * \param i	L'indice de la particule, doit être un indice valide.
 * \return	L'énergie de la particule i.
 */
double particule_energie(int i);

/**
 * \brief	Contrôle si deux particules sont en collision
 * \param i	L'indice de la première particule, doit être un indice valide.
 * \param j L'indice de la deuxième particule, doit être un indice valide.
 */
bool particule_collision(int i);

/**
 * \brief	Contrôle si la particule d'indice i est en collision avec l'une des  
 *          particules d'indice inférieur à i.
 * \param i	L'indice de la particule, doit être un indice valide.
 */
bool particule_collision_p_p(int i, int j);

/**
 * \brief	Dessine l'état actuel des particules
 */
void particule_dessiner(void);

/**
 * \brief	Contrôle que les données fournies forment une particule valide.
 * \param pos		La position à tester.
 * \param energie	L'énergie à tester.
 */
bool particule_is_valid(C2D pos, double energie);

C2D* particule_position_pointeur(int* i);

void particule_free(int* i);

#endif
