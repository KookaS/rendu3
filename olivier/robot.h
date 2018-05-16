/*!
 \file robot.h
 \brief Module gérant le type opaque robot
 \author Simon Gilgien
 \group Sylvain Pellegrini
		Simon Gilgien
 \modifications pour rendu2 public   R. Boulic
 \version 1.1
 \date 2 mai 2018
 */
 
#ifndef ROBOT_H
#define ROBOT_H

#include "utilitaire.h"

/**
 * \brief	Configure le nombre de robots. Les indices valides sont dans l'intervalle
 *			[1, nb_robots]. Si nb_robots = 0, les données sont effacées et aucun
 *			indice n'est valide. Les robots nouvellement créés doivent être configurés
 *			avant utilisation.
 * \param nb_robots	Le nombre de robots, doit être >=0.
 */
void robot_set_nombre(int nb_robots);

/**
 * \brief	Configure les paramètres d'un robot.
 * \param i		L'indice du robot à modifier, doit être un indice valide.
 * \param pos	La nouvelle position du robot.
 * \param angle	Le nouvel angle du robot.
 */
void robot_set_robot(int i, S2D pos, double angle);

void robot_set_cible(int* i, int cible);

/**
 * \brief			écriture de l'ensemble des robots dans un fichier.
 * \param fichier	pointeur vers le fichier ouvert en écriture

 */
void robot_ecrire_fichier(FILE* fichier);

/**
 * \brief	Retourne le nombre de robots existants, c'est-à-dire le plus grand indice
 *			valide.
 * \return	Le plus grand indice valide.
 */
int robot_nb_robots(void);

/**
 * \brief	Retourne la position d'un robot.
 * \param i	L'indice du robot, doit être un indice valide.
 * \return	La position du robot i.
 */
C2D robot_position(int i);

/**
 * \brief	Retourne l'orientation d'un robot.
 * \param i	L'indice du robot, doit être un indice valide.
 * \return l'orientation du robot, comprise dans ]-π, π].
 */
double robot_orientation(int i);

/**
 * \brief	Dessinne les robots en utilisant les fonctions de dessin de utilitaire.h
 */
void robot_dessiner(void);

/**
 * \brief	Contrôle si le robot i est en collision avec l'un des robots 
 *          d'indice inférieur à i.
 * \param i	L'indice du robot, doit être un indice valide.
 */
bool robot_collision(int i);

/**
 * \brief			Contrôle si un robot est en collision avec la particule i_part
 * \param i_part	L'indice de la particule testée, doit être un indice valide.
 */
bool robot_collision_particule(int i_part);

/**
 * \brief	Contrôle si les robots i et j sont en collision.
 * \param i	L'indice du premier robot, doit être un indice valide.
 * \param j L'indice du deuxième robot, doit être un indice valide.
 */
bool robot_collision_r_r(int i, int j);

/**
 * \brief	Contrôle si un robot est en collision avec une particule.
 * \param robot	L'indice du robot, doit être un indice de robot valide.
 * \param part	L'indice de la particule, doit être un indice de particule valide.
 */
bool robot_collision_r_p(int robot, int part);

void robot_avance(int* i);
void robot_recule(int* i);
void robot_rotation(int* i, double angle);

C2D* robot_position_pointeur(int* i);
double* robot_orientation_pointeur(int* i);
int* robot_cible_pointeur(int* i);
//void robot_cible_attribution(int* i, int* val);


#endif
