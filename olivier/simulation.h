/*!
 \file simulation.h
 \brief Module de gestion de la simulation à haut niveau et gestion des fichiers
 \author Sylvain Pellegrini
 \group Sylvain Pellegrini
		Simon Gilgien
 \version 1.0
 \date 19 avril 2018
 */
 
#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdbool.h>


/**
 * \brief	Lit un fichier et crée la situation correspondante. Appelle la fonction
 *			d'erreur correspondante.
 * \param nom_fichier	Le nom du fichier à lire.
 * \return	Si la lecture s'est bien passée (i.e. renvoie false en cas d'erreur et
 *			true sinon)
 */
bool simulation_lecture(char *nom_fichier);

/**
 * \brief	Écrit la situation courante dans un fichier lisible.
 * \param nom_fichier	Le nom du fichier à écrire.
 */
void simulation_ecriture(char *nom_fichier);

/**
 * \brief	Dessine l'état actuel de la simulation.
 */
void simulation_dessiner(void);
 
void simulation_automatisation(void);

void simulation_robot_cible(int r_nb, int p_nb);

void simulation_attribution(int nb_robots, int nb_particules);

#endif
