/*!
 \file utilitaire.c
 \brief Module de fonctions géométriques et de dessin bas niveau
 \author Simon Gilgien
 \group Sylvain Pellegrini
		Simon Gilgien
 \version 1.0
 \date 19 avril 2018
 */
 
#include <math.h>
#include <stdio.h>
#include "graphic.h"
#include "utilitaire.h"


// renvoie la distance entre les points a et b
double util_distance(S2D a, S2D b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

// renvoie l'angle que fait le bipoint ab et l'axe X du monde.
// L'angle doit être en radians et compris dans l'intervalle ]-pi, +pi]
double util_angle(S2D a, S2D b)
{
	double angle = atan2(b.y-a.y, b.x-a.x);
	util_range_angle(&angle);
	return angle;
}

// modifie si nécessaire l'angle pointé par p_angle
// pour qu'il soit compris dans l'intervalle ]-pi, +pi]
void util_range_angle(double * p_angle)
{
	while(*p_angle <= M_PI)
		*p_angle+= (2*M_PI);
	while(*p_angle > M_PI)
		*p_angle-= (2*M_PI);
}

// renvoie VRAI si le point est en dehors du domaine [-max, max]
bool util_point_dehors(S2D a, double max)
{
	return fabs(a.x) > max || fabs(a.y) > max;
}

// renvoie VRAI si l'angle alpha est en dehors de l'intervalle [-pi, pi]
bool util_alpha_dehors(double alpha)
{
	return fabs(alpha) > M_PI;
}

// renvoie VRAI si le point a est dans le cercle c
// plus précisément: si la distance de a au centre de c < rayon - EPSIL_ZERO
bool util_point_dans_cercle(S2D a, C2D c)
{
	return util_distance(a, c.centre) < c.rayon - EPSIL_ZERO;
}

// renvoie VRAI en cas de collision des cercles a et b selon l'Equ. 4
// le paramètre de sortie p_dist est la distance entre les centres de a et b
bool util_collision_cercle(C2D a, C2D b, double * p_dist)
{
	*p_dist = util_distance(a.centre, b.centre);
	return *p_dist < a.rayon + b.rayon - EPSIL_ZERO;
}

// renvoie la position obtenue après déplacement du point p d'une distance dist
// dans la direction définie par l'angle alpha
S2D util_deplacement(S2D p, double alpha, double dist)
{
	S2D resultat;
	resultat.x = p.x + cos(alpha)*dist;
	resultat.y = p.y + sin(alpha)*dist;
	return resultat;
}

// renvoie VRAI si la distance de a à b > EPSIL_ZERO et renvoie FAUX sinon.
// DE PLUS, dans le cas VRAI on utilise p_ecart_angle (qui doit être défini)
// pour récupérer l'écart angulaire entre le bipoint ab et un vecteur d'angle alpha.
// La valeur de l'écart angulaire doit être dans l'intervalle [-pi, pi].
bool util_ecart_angle(S2D a, double alpha, S2D b, double *p_ecart_angle)
{
	if(util_distance(a,b) > EPSIL_ZERO)
	{
		*p_ecart_angle = util_angle(a, b) - alpha;
		util_range_angle(p_ecart_angle);
		return true;
	}
	return false;
}

// renvoie VRAI si un vecteur d'angle alpha est aligné avec le vecteur ab. Pour
// déterminer cela on obtient l'écart angulaire avec la fonction util_ecart_angulaire
// et on revoie VRAI si cette fonction renvoie VRAI et si la valeur absolue de
// cet écart angulaire < EPSIL_ALIGNEMENT. Renvoie FAUX pour tous les autres cas.
bool util_alignement(S2D a, double alpha, S2D b)
{
	double ecart_angle;
	if(util_ecart_angle(a, alpha, b, &ecart_angle))
	{
		return fabs(ecart_angle) < EPSIL_ALIGNEMENT;
	}
	return false;
}

// renvoie VRAI si on peut calculer la nouvelle longueur du coté a lorsqu'on change
// la longueur du coté b, la longueur du coté c restant constante. Les longueurs des
// cotés a,b,c sont notées la, lb, lc. La nouvelle longueur du coté b est lb_new.
// le paramètre de sortie p_la_new doit être défini. Renvoie VRAI si:
// les 2 longueurs la et lc > EPSIL_ZERO, lb >= 0 et lb_new se trouve dans l'intervalle
// autorisé [lb, lc]. Le calcul de la_new est donné par l'Equ.5 qui résoud le cas
// particulier de la Fig 5c avec:
// la = delta_d, lb = D, lc = L, lb_new = r1+r2
// la_new = delta_d'
//
bool util_inner_triangle(double la, double lb, double lc, double lb_new,
						 double * p_la_new)
{
	if(la > EPSIL_ZERO &&
	   lb >= 0         &&
	   lc > EPSIL_ZERO &&
	   lb_new >= lb    &&
	   lb_new <= lc)
	{
		double cos_beta = (la*la + lc*lc - lb*lb) / (2*la*lc);
		double b = -2*lc*cos_beta;
		double c = lc*lc - lb_new*lb_new;
		*p_la_new = (-b - sqrt(b*b-4*c))/2;
		return true;
	}
	return false;
}

void util_dessiner_cercle(C2D cercle, COULEUR couleur, bool plein, double epaisseur)
{
	graphic_cercle(cercle.centre.x, cercle.centre.y, cercle.rayon, (float *) &couleur,
				   plein, epaisseur);
}

void util_dessiner_segment(S2D a, S2D b, COULEUR couleur, double largeur)
{
	graphic_segment(a.x, a.y, b.x, b.y, (float *)&couleur, largeur);
}

void util_debut_dessin(double gauche, double droite, double haut, double bas)
{
	graphic_begin_draw(gauche, droite, haut, bas);
}
