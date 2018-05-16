/*!
 \file graphic.h
 \brief Module concentrant les dépendances OpenGL
 \author Simon Gilgien
 \group Sylvain Pellegrini
		Simon Gilgien
 \version 1.0
 \date 19 avril 2018
 */
 
#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdbool.h>

/**
 * \brief	Dessine un cercle dans la fenêtre de visualisation.
 * \param x			Coordonnée x du centre.
 * \param y			Coordonnée y du centre.
 * \param r			Rayon du cercle.
 * \param couleur	Couleur du cercle, sous forme d'un tableau de 3 entiers.
 * \param plein		Définit si le cercle est plein ou vide.
 * \param epaisseur	Définit l'épaisseur du trait.
 */
void graphic_cercle(double x, double y, double r, float* couleur, bool plein,
					double epaisseur);

/**
 * \brief	Dessinne un segment dans la fenêtre de visualisation.
 * \param x_a		Coordonnée x de l'origine du segment.
 * \param y_a		Coordonnée y de l'origine du segment.
 * \param x_b		Coordonnée x de l'arrivée du segment.
 * \param y_b		Coordonnée y de l'arrivée du segment.
 * \param couleur	Couleur du segment, sous forme d'un tableau de 3 entiers.
 * \param largeur	Largeur du trait.
 */
void graphic_segment(double x_a, double y_a, double x_b, double y_b, float *couleur,
					 double largeur);

/**
 * \brief	Ajuste l'aspect ratio de la fenêtre
 * \param largeur	La nouvelle largeur de la fenêtre
 * \param hauteur	La nouvelle hauteur de la fenêtre
 */
void graphic_reshape(int largeur, int hauteur);

/**
 * \brief	Prépare la fenêtre pour le dessin et s'assure que le rectangle spécifié
 *			est entièrement visible. gauche et haut DOIVENT être négatifs et droite
 *			et bas DOIVENT être positifs afin que la totalité du rectangle soit
 *			visible.
 * \param gauche	La limite gauche minimale.
 * \param droite	La limite droite minimale.
 * \param haut		La limite haute minimale.
 * \param bas		La limite basse minimale.
 */
void graphic_begin_draw(double gauche, double droite, double haut, double bas);

#endif
