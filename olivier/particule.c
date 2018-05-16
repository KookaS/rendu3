/*!
 \file particule.c
 \brief Module contenant le type opaque particule
 \author Simon Gilgien
 \group Sylvain Pellegrini
		Simon Gilgien
 \modifications pour rendu2 public   R. Boulic
 \version 1.1
 \date 2 mai 2018
 */
 
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "error.h"
#include "particule.h"
#include "constantes.h"

#define EPAISSEUR_TRAIT_PARTICULE		1

typedef struct Particule PARTICULE;
struct Particule
{
	C2D position;
	double energie;
};

static COULEUR couleur_particule = {0.5, 0.5, 0.5};
static PARTICULE *tab = NULL;
static int nb = 0;
static double si = 0;

double sum_energie(){
  PARTICULE P;
  double total;
  for(size_t i = 1; i<nb; ++i){
    P = tab[i];
    total += P.energie;
  }
  return total;
}

void tamere(){
  PARTICULE P;
  for(int i = 1; i<=nb; ++i){
    P = tab[i];
    bool cond = (P.position.rayon*R_PARTICULE_FACTOR >= R_PARTICULE_MIN);
    bool condTaMere = (rand()/RAND_MAX <= DECOMPOSITION_RATE);
    if(cond && condTaMere){
      particule_decomposition(i);
    }
  }
}

void particule_decomposition(int index){
  PARTICULE * P = tab[index];
  double prevRayon = P->position->rayon;
  SD2 prevCentre = P->position->center;
  double newEnergie = (P->energie)*E_PARTICULE_FACTOR;

  S2D newCenter = {
    prevCentre.x + R_PARTICULE_FACTOR,
    prevCentre.y + R_PARTICULE_FACTOR
  };

  C2D newPos = {
    newCenter,
    prevRayon*R_PARTICULE_FACTOR
  };

  particule_set_particule(index, newPos, newEnergie);
  particule_set_nombre(nb+3);

  S2D signs = {-1, -1};

  for(uint8_t i = nb; i < nb+3; ++i){
    S2D Center = {
      prevCentre.x + R_PARTICULE_FACTOR*signs.x,
      prevCentre.y + R_PARTICULE_FACTOR*signs.y
    };
    C2D Pos = {
      Center,
      prevRayon*R_PARTICULE_FACTOR
    };
    particule_set_particule(i, Pos, newEnergie);
    signs.y = signs.x;
    signs.x *= -1;
    // - -     + -    - + 
  }

  nb += 3;
}

void particule_set_nombre(int nb_part)
{
	assert(nb_part >=0);
	if(nb_part >0)
		tab = realloc(tab, nb_part*sizeof(PARTICULE));
	else
	{
		if(tab)
			free(tab);
		tab = NULL;
	}
	nb = nb_part;
}

void particule_set_particule(int indice, C2D pos, double energie)
{
	assert(0<indice && indice <= nb);
	tab[indice-1].position = pos;
	tab[indice-1].energie = energie;
	
	//printf("particule %d ème de rayon %lf\n", indice, tab[indice-1].position.rayon);
}

void particule_ecrire_fichier(FILE *fichier)
{
	int i;
	
	fprintf(fichier, "\n%d\n", nb);
	if(nb)
	{
		for(i=0; i<nb; i++)
		{
			C2D pos = tab[i].position;
			fprintf(fichier, "\t%g %g %g %g\n", tab[i].energie,
												pos.rayon,
												pos.centre.x,
												pos.centre.y);
		}
		fprintf(fichier, "FIN_LISTE\n");
	}
}

int particule_nb_particules(void)
{
	return nb;
}

//renvoie particule position
C2D particule_position(int i)
{
	assert(0<i && i <=nb);
	return tab[i-1].position;
}

//renvoie particule energie
double particule_energie(int i)
{
	assert(0<i && i <=nb);
	return tab[i-1].energie;
}

void particule_dessiner(void)
{
	int i;
	for(i = 1; i <= nb; i++)
		util_dessiner_cercle(particule_position(i), couleur_particule, true,
							 EPAISSEUR_TRAIT_PARTICULE);
}

bool particule_collision(int i)
{
	int j;
	
	for (j = 1; j < i; j++)
	{
		if(particule_collision_p_p(j, i))
		{
			error_collision(PARTICULE_PARTICULE, j, i);
			return true;
		}
	}
	return false;
}

bool particule_collision_p_p(int i, int j)
{
	assert(0<i && i<=nb);
	assert(0<j && j<=nb);
	double dist;
	return util_collision_cercle(particule_position(i), particule_position(j), &dist);
}

bool particule_is_valid(C2D pos, double energie)
{
	return !util_point_dehors(pos.centre, DMAX) &&
		   pos.rayon <= R_PARTICULE_MAX			&&
		   pos.rayon >= R_PARTICULE_MIN 		&&
		   energie   <= E_PARTICULE_MAX			&&
		   energie   >= 0;
}

C2D* particule_position_pointeur(int* i)
{
	assert(0<*i && *i <=nb);
	return &tab[*i-1].position;
	
}

void particule_free(int* i){
	tab[*i-1].position.centre.x = 0;
	tab[*i-1].position.centre.y = 0;
	tab[*i-1].position.rayon = 0;
	tab[*i-1].energie = 0;
}
