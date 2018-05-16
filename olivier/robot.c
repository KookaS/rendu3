/*!
 \file robot.c
 \brief Module gérant le type opaque robot
 \author Simon Gilgien
 \group Sylvain Pellegrini
		Simon Gilgien
 \modifications pour rendu2 public   R. Boulic
 \version 1.1
 \date 2 mai 2018
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "constantes.h"
#include "error.h"
#include "particule.h"
#include "robot.h"

#include <math.h>

#define EPAISSEUR_ROBOT		2
#define RAYON_CENTRE		0.1

typedef struct Robot ROBOT;
struct Robot
{
	C2D position;
	double angle;
	int particule_cible;
};

static COULEUR couleur_robot =  {0., 0., 0.};
static COULEUR couleur_centre = {1., 0., 0.};
static ROBOT* tab = NULL;
static int nb = 0;

void robot_set_nombre(int nb_robots)
{
	assert(nb_robots >=0);
	if(nb_robots >0)
		tab = realloc(tab, nb_robots*sizeof(ROBOT));
	else
	{
		if(tab)
			free(tab);
		tab = NULL;
	}
	nb = nb_robots;
}

//attribue valeurs robots
void robot_set_robot(int i, S2D pos, double angle)
{
	assert(0<i && i<=nb);
	tab[i-1].position.centre = pos;
	tab[i-1].position.rayon = R_ROBOT;
	tab[i-1].angle = angle;
}	

void robot_set_cible(int* i, int cible){
	
	tab[*i-1].particule_cible = cible;
}

void robot_ecrire_fichier(FILE* fichier)
{
	int i;
	
	fprintf(fichier, "# Fichier généré\n#\n%d\n", nb);
	
	if(nb)
	{
		for(i=0; i<nb; i++)
		{
			fprintf(fichier, "\t%g %g %g\n", tab[i].position.centre.x, 
											 tab[i].position.centre.y,
											 tab[i].angle);
		}
		fprintf(fichier, "FIN_LISTE\n");
	}	
}

int robot_nb_robots(void)
{
	return nb;
}

//renvoie position robot
C2D robot_position(int i)
{
	assert(0<i && i <= nb);
	return tab[i-1].position;
}

//renvoie alpha robot
double robot_orientation(int i)
{
	assert(0<i && i <= nb);
	return tab[i-1].angle;
}

void robot_dessiner(void)
{
	int i;
	S2D centre;
    for(i =1; i<=nb; i++)
	{
		centre = robot_position(i).centre;
		util_dessiner_cercle(robot_position(i), couleur_robot, false,
							 EPAISSEUR_ROBOT);
		util_dessiner_segment(centre,
							  util_deplacement(centre, robot_orientation(i), R_ROBOT),
							  couleur_robot, EPAISSEUR_ROBOT);
		C2D point_central = {centre, RAYON_CENTRE};
		util_dessiner_cercle(point_central, couleur_centre, true, EPAISSEUR_ROBOT);
	}
}

//renvoie true si collisions entre robots
bool robot_collision(int i)
{
	assert(0<i && i<=nb);
	int j;

	for (j=1; j < i; j++)
	{
		if (robot_collision_r_r(j, i))
		{
			error_collision(ROBOT_ROBOT, j, i);
			return true;
		}
	}
	return false;
}

//renvoie true si collisions entre robots et particules
bool robot_collision_particule(int i_part)
{
	int j;
	
	for (j = nb; j > 0; j--)
	{
		if(robot_collision_r_p(j, i_part))
		{
			error_collision(ROBOT_PARTICULE, j, i_part);
			return true;
		}
	}
	return false;
}

bool robot_collision_r_r(int i, int j)
{
	assert(0<i && i<=nb);
	assert(0<j && j<=nb);
	double dist;
	return util_collision_cercle(robot_position(i), robot_position(j), &dist);
}

bool robot_collision_r_p(int robot, int part)
{
	assert(0<robot && robot<=nb);
	double dist;
	return util_collision_cercle(robot_position(robot), particule_position(part),
								 &dist);
}

void robot_avance(int* i){
	
	C2D pos = robot_position(*i);
	double alpha = robot_orientation(*i);

	//varier le ratio de vitesse < DELTA_VTRAN
	pos.centre = util_deplacement(pos.centre, alpha, DELTA_VTRAN);
	robot_set_robot(*i, pos.centre, alpha);
	
}

void robot_recule(int* i){
	
	C2D pos = robot_position(*i);
	double alpha = robot_orientation(*i);
	
	pos.centre = util_deplacement(pos.centre, alpha, DELTA_VTRAN);
	robot_set_robot(*i, pos.centre, alpha);
	
}

void robot_rotation(int* i, double angle){

	double alpha = robot_orientation(*i) + angle;
	util_range_angle(&alpha);
	robot_set_robot(*i, robot_position(*i).centre, alpha);
	
}

C2D* robot_position_pointeur(int* i)
{
	assert(0<*i && *i <= nb);
	return &tab[*i-1].position;
}

double* robot_orientation_pointeur(int* i)
{
	assert(0<*i && *i <= nb);
	return &tab[*i-1].angle;
}

int* robot_cible_pointeur(int* i)
{
	assert(0<*i && *i <= nb);
	return &tab[*i-1].particule_cible;
}
/*
void robot_cible_attribution(int* i, int* val)
{
	assert(0<*i && *i <= nb);
	tab[*i-1].particule_cible = *val;
}*/
