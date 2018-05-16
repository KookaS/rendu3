/*!
 \file simulation.c
 \brief Module gérant la simulation à haut niveau et les fichiers.
 \author Sylvain Pellegrini
 \group Sylvain Pellegrini
		Simon Gilgien
 \modifications pour rendu2 public   R. Boulic
 \version 1.1
 \date 2 mai 2018
 */
 
#include <stdio.h>
#include <string.h>
#include "robot.h"
#include "particule.h"
#include "utilitaire.h"
#include "error.h"
#include "constantes.h"
#include "simulation.h"

#include <math.h>

#define LARGEUR_CADRE	5
#define CIBLE_INIT		0

/**
 * \brief états de l'automate de lecture
 * SET_NB_ROBOT		lecture du nombre de robots
 * E_ROBOT			lecture des données des robots
 * SET_NB_PART		lecture du nombre de particules
 * E_PARTICULE		lecture des données des particules
 * FIN				lecture complétée
 */
enum etats_lecture {SET_NB_ROBOT, E_ROBOT,SET_NB_PART,E_PARTICULE,FIN};


//tout est check ici au préalale avant de d'attribuer les valeurs et la mémoire
static bool simulation_decodage_robot(int *etat,int nb_robots, char *tab,int *i,
									  int ligne);
									  
static bool simulation_decodage_particule(int *etat,int nb_particules,char *tab,int *i,
										  int ligne);
										  
static bool simulation_decodage_fin_liste(char *tab);

static bool simulation_fermeture_fichier_erreur(FILE *file);

static bool simulation_decodage_nombre_robots(char *tab,int *i, int *etat,
											  int *nb_robots,int ligne);
											  
static bool simulation_decodage_nombre_particules(char *tab,int *i,int *etat,
												  int *nb_particules,int ligne);

bool simulation_lecture(char *nom_fichier)
{
	FILE *file = NULL;
	int nb_robots, nb_particules,etat = SET_NB_ROBOT,ligne = 0,i;
	char tab[MAX_LINE];
	char c;
	
	if (!(file = fopen (nom_fichier, "r")))
	{
		error_file_missing(nom_fichier);
		return false;
	}
	while (fgets(tab,MAX_LINE,file))
	{
		++ligne;
		if (sscanf(tab," %c",&c)!=1 || c == '#')
			continue;
		switch(etat)
		{
			//ici tout est appelé;
		case SET_NB_ROBOT :
			if (!simulation_decodage_nombre_robots(tab,&i,&etat,&nb_robots,ligne))
				return simulation_fermeture_fichier_erreur(file);
			break;
		case E_ROBOT :
			if(!simulation_decodage_robot(&etat,nb_robots,tab,&i,ligne))
				return simulation_fermeture_fichier_erreur(file);
			break;	
		case SET_NB_PART :
			if (!simulation_decodage_nombre_particules(tab,&i,&etat,&nb_particules,
													   ligne))
				return simulation_fermeture_fichier_erreur(file);
			break;
		case E_PARTICULE :
			if(!simulation_decodage_particule(&etat,nb_particules,tab,&i,ligne))
				return simulation_fermeture_fichier_erreur(file);
			break;
		case FIN :
			error_useless_char(ligne);
			return simulation_fermeture_fichier_erreur(file);
		default :                                                              
			return simulation_fermeture_fichier_erreur(file);
		}			
	}
	if(etat != FIN)
	{
		error_end_of_file(ligne);
		return simulation_fermeture_fichier_erreur(file);
	}
	error_no_error_in_this_file();	
	fclose(file);									 
	return true;
}

void simulation_ecriture(char *nom_fichier)
{
	FILE *fichier;
	
	if((fichier = fopen(nom_fichier, "w")))
	{
		robot_ecrire_fichier(fichier);
		particule_ecrire_fichier(fichier);

		fclose(fichier);
	}
}

void simulation_dessiner(void)
{
	util_debut_dessin(-DMAX, DMAX, -DMAX, DMAX);

	COULEUR noir = {0., 0., 0.};
	S2D hg = {-DMAX, -DMAX};
	S2D hd = { DMAX, -DMAX};
	S2D bg = {-DMAX,  DMAX};
	S2D bd = { DMAX,  DMAX};
	util_dessiner_segment(hg, hd, noir, LARGEUR_CADRE);
	util_dessiner_segment(hd, bd, noir, LARGEUR_CADRE);
	util_dessiner_segment(bd, bg, noir, LARGEUR_CADRE);
	util_dessiner_segment(bg, hg, noir, LARGEUR_CADRE);

	robot_dessiner();

	particule_dessiner();
}

static bool simulation_decodage_robot(int *etat,int nb_robots, char *tab,int *i,
									  int ligne)
{
	S2D pos;
	double angle;
	int offset = 0, n;
	if(simulation_decodage_fin_liste(tab))
	{
		if(*i < nb_robots)
		{
			error_fin_liste_robots(ligne);
			return false;
		}
		*etat = SET_NB_PART;
		return true;
	}
	if((*i) == nb_robots)
	{
		error_missing_fin_liste_robots(ligne);
		return false;
	}
	// %n Receives an integer of value equal to the number of characters read so far
    while(sscanf(&tab[offset]," %lf %lf %lf %n",&pos.x,&pos.y,&angle, &n)==3)
	{
		if(util_alpha_dehors(angle))
		{
			error_invalid_robot_angle(angle);
			return false;
		}
		util_range_angle(&angle);
		robot_set_robot(++(*i), pos, angle);
		robot_set_cible(i, CIBLE_INIT);
		
		if(robot_collision(*i))
			return(false);

		offset+=n;
		if((*i) == nb_robots && offset != strlen(tab))
		{
			error_useless_char(ligne);
			return false;
		}
	}
	if(offset != strlen(tab))
	{
		error_invalid_robot();
		return false;
	}
	return true;
}

static bool simulation_decodage_particule(int *etat,int nb_particules,char *tab,int *i,
										  int ligne)
{
	C2D pos;
	double energie;
	int offset = 0, n;
	if(simulation_decodage_fin_liste(tab))
	{
		if(*i < nb_particules)
		{
			error_fin_liste_particules(ligne);
			return false;
		}
		*etat = FIN;
		return true;
	}
	if((*i) == nb_particules)
	{
		error_missing_fin_liste_particules(ligne);
		return false;
	}
	// %n Receives an integer of value equal to the number of characters read so far
    while(sscanf(&tab[offset], " %lf %lf %lf  %lf %n ", &energie, &pos.rayon,
				 &pos.centre.x, &pos.centre.y, &n)==4)
	{
		if (!particule_is_valid(pos, energie))
		{
			error_invalid_particule_value(energie,pos.rayon,pos.centre.x,pos.centre.y);
			return false;
		}
		particule_set_particule(++(*i),pos,energie);

		if(particule_collision(*i))
			return false;
			
		if(robot_collision_particule(*i))
			return false;
			
		offset+=n;
		if ((*i) == nb_particules && offset != strlen(tab))
		{
			error_useless_char(ligne);
			return false;
		}
	}
	if(offset != strlen(tab))
	{
		error_invalid_particule(ligne);
		return false;
	}
	return true;
}

static bool simulation_decodage_fin_liste(char *tab)
{
	int n;
    sscanf(tab," FIN_LISTE %n",&n);
    if (n != strlen(tab))
		return false;
	else
		return true;	
}

static bool simulation_fermeture_fichier_erreur(FILE *file)
{
	if(!fclose(file)==0)
		printf("le fichier s'est mal fermé\n");
	robot_set_nombre(0);
	particule_set_nombre(0);
	return false;
}

static bool simulation_decodage_nombre_robots(char *tab,int *i, int *etat,
											  int *nb_robots,int ligne)
{
	int n;
	if ((sscanf(tab," %d %n", nb_robots,&n) !=1)||(*nb_robots < 0)) 
	{
		error_invalid_nb_robots();
		return false;
	}
	if (n!=strlen(tab))
	{
		error_useless_char(ligne);
		return false;
	}
    *etat = *nb_robots ? E_ROBOT : SET_NB_PART;
	robot_set_nombre(*nb_robots);
	*i = 0;
	return true;
}

static bool simulation_decodage_nombre_particules(char *tab,int *i,int *etat,
												  int *nb_particules,int ligne)
{
	int n;
	if ((sscanf(tab," %d %n", nb_particules,&n) !=1)||(*nb_particules < 0))
	{
		error_invalid_nb_particules();	
		return false;
	}
	if (n!=strlen(tab))
	{
		error_useless_char(ligne);
		return false;
	}
    *etat = *nb_particules ? E_PARTICULE : FIN;
	particule_set_nombre(*nb_particules);
	*i = 0;
	return true;
}
//------------------------------------------------------------------------------------
void simulation_automatisation(void){
	
	int nb_robots = robot_nb_robots();
	int nb_particules = particule_nb_particules();

	simulation_attribution(nb_robots, nb_particules);
	
	for(int i = 1; i <= nb_robots; i++){
		int cible = *robot_cible_pointeur(&i);
		if(cible){
			simulation_robot_cible(i, cible);			
		}		
	}

	
	//simulation_robot_cible(r_nb, p_nb);
	//printf("%d\n", nb_particules);
	//ATTENTION A APPELER simulation_attribution que au début et après une élimination
}

void simulation_robot_cible(int r_nb, int p_nb){
	
	C2D* robot_nb = robot_position_pointeur(&r_nb);
	C2D* particule_nb = particule_position_pointeur(&p_nb);
	double* alpha = robot_orientation_pointeur(&r_nb); //angle robot
	double angle_diff; //angle robot-particule
	double dist; //dist rayon à rayon
	
	//déplace le robot et élimine la particule en question
	if(util_ecart_angle(robot_nb->centre, *alpha, particule_nb->centre, &angle_diff)
		&& !util_collision_cercle(*robot_nb, *particule_nb, &dist) 
		&& fabs(angle_diff) <= M_PI_4){
		robot_avance(&r_nb);
	}
	else if (fabs(angle_diff) <= M_PI_4){
		particule_free(&p_nb);
		*robot_cible_pointeur(&r_nb) = 0;
		//printf("%d\n", *robot_cible_pointeur(&r_nb));
	}
	
	//aligne le robot à la particule
	if(!util_alignement(robot_nb->centre, *alpha, particule_nb->centre))
	{
		if(fabs(angle_diff) > DELTA_VROT && angle_diff > 0){
				robot_rotation(&r_nb, DELTA_VROT);
		}
		else if(fabs(angle_diff) > DELTA_VROT && angle_diff < 0){
			robot_rotation(&r_nb, -DELTA_VROT);
		}
		else if(angle_diff > 0){
			robot_rotation(&r_nb, angle_diff);
		}
		else if(angle_diff < 0){
			robot_rotation(&r_nb, -angle_diff);
		}
	}		
}

void simulation_attribution(int nb_robots, int nb_particules){
	
	int i, j, tab_r[nb_robots], robot_free = 0;
	for(i = 1; i <= nb_robots; i++){
		if(*robot_cible_pointeur(&i) == 0){
			tab_r[robot_free] = i;
			++robot_free;
			//printf("robot_free : %d val de %d\n", robot_free, *robot_cible_pointeur(&i));
		}
	}
	printf("\n");

	double tab_p_rayon[robot_free], rayon;
	int tab_p_nb[robot_free];
	i=1;
	tab_p_rayon[i-1] = particule_position_pointeur(&i)->rayon;
	//printf("entrée :%lf \n", tab_p_rayon[i-1]);
	tab_p_nb[i-1] = i;
	
	for(i = 2; i <= nb_particules; i++){
		rayon = particule_position_pointeur(&i)->rayon;
		if(i <= robot_free){
			for(j = i-2; j >= 0; j--){
				if(rayon > tab_p_rayon[j]){
					double temp_d = tab_p_rayon[j];
					int temp_i = tab_p_nb[j];
					tab_p_rayon[j] = rayon;
					tab_p_rayon[j+1] = temp_d;
					tab_p_nb[j] = i;
					tab_p_nb[j+1] = temp_i;
				}
				else{
					tab_p_rayon[j+1] = rayon;
					tab_p_nb[j+1] = i;
					break;
				}
			}
		}
		else{
			//test toutes les autres particules
			for(j = robot_free-1; j >= 0 ; j--){
				if(j < robot_free-1 && rayon > tab_p_rayon[j]){
					double temp_d = tab_p_rayon[j];
					int temp_i = tab_p_nb[j];
					tab_p_rayon[j] = rayon;
					tab_p_rayon[j+1] = temp_d;
					tab_p_nb[j] = i;
					tab_p_nb[j+1] = temp_i;
					//printf("%lf\n", tab_p[i-1]);
				}
				else if(j == robot_free-1 && rayon > tab_p_rayon[j]){
					printf("passe\n");
					tab_p_rayon[j] = rayon;
					tab_p_nb[j] = i;
					break;
				}
			}
		}
		/*
		for(j = 0; j < i; j++){
			printf("%d	ID: %d	tab_p_rayon: %lf\n", j, tab_p_nb[j], tab_p_rayon[j]);
		}
		printf("\n");
		*/
	}
	
	for(j = 0; j < nb_particules; j++){
			//printf("%d	ID: %d	tab_p_rayon: %lf\n", j, tab_p_nb[j], tab_p_rayon[j]);
		}
		printf("\n");
	
	for(i = 0; i < robot_free && i < nb_particules; i++){
		*robot_cible_pointeur(&tab_r[i]) = tab_p_nb[i];
		/*
		printf("robot %d de tab_r:%d, pointe sur %d de tab_p_nb :%d de rayon %lf\n",
				i+1, tab_r[i], *robot_cible_pointeur(&tab_r[i]), tab_p_nb[i], tab_p_rayon[i]);
		*/		
		simulation_robot_cible(tab_r[i], tab_p_nb[i]);
	}
}
/**
 * prochaine opti:
 * ne plus réattribuer particules déjà attribuées
 * attribuer correctement
 * changer valeur de robot_cible qui ne marche pas
 * en fait change la valeur qui revient à 0 après...
 */


