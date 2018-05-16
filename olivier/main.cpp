/*!
 \file main.cpp
 \brief Module principal, gère l'interface graphique.
 \author Sylvain Pellegrini
 \group Sylvain Pellegrini
		Simon Gilgien
 \modifications pour rendu2 public   R. Boulic
 \version 1.1
 \date 2 mai 2018
 */
 
#include <GL/glui.h>
#include <GL/glut.h>
#include <cstring>
#include <cstdio>

extern "C"
{
	#include "simulation.h"
	#include "graphic.h"
}

#define CONTROL_AUTO	0
#define CONTROL_MANUEL	1
#define TAILLE_INITIALE 600

namespace
{
	int view_window;
	bool simulation_started = false;
	int record;
	unsigned count=0 ;
	char *filename_in  = NULL;
	char *filename_out = NULL;
	GLUI_Button *open  = NULL;
}

enum Widgets
{
	BUTTON_OPEN, BUTTON_SAVE, BUTTON_START_STOP, BUTTON_STEP, CHECKBOX_RECORD,
	CONTROL_MODE
};

/**
 * \brief	Initialise les fenêtres graphiques
 * \param argcp	Un pointeur sur argc à passer à glutInit()
 * \param argv	Un pointeur sur argv à passer à glutInit()
 */
void main_init_gui(int *argcp, char **argv);

/**
 * \brief	Callback réagissant à la mise à jour d'un widget.
 * \param widget	Le widget mis à jour, comme défini par Widgets.
 */
void main_widget_update(int widget);

/**
 * \brief	Callback dessinant l'état actuel de la simulation dans la fenêtre glut.
 */
void main_affichage();

/**
 * \brief	Callback de gestion des changement de taille de la fenêtre.
 * \param w	La nouvelle largeur de la fenêtre
 * \param h La nouvelle hauteur de la fenêtre
 */
void main_reshape(int w, int h);

/**
 * \brief	Fonction callback idle : effectue une seule mise à jour
 */
void main_update_one_step(void);

/**
 * \brief	Rassemble les fonctions GLUI
 * \param GL_window   id de la fenêtre GL associée
 */
 void main_create_glui_interface(int GL_window);

/**
 * \brief	Fonction main, parse la ligne de commande
 * \param argc	Nombre d'arguments.
 * \param argv	Arguments de la ligne de commande.
 */
 
int main (int argc, char* argv[])
{
	switch(argc)
	{
	case 3:
		if(strcmp(argv[1], "Error") == 0)
		{
			simulation_lecture(argv[2]);
			return EXIT_SUCCESS;
		}
		if(strcmp(argv[1], "Draw") == 0)
		{
			simulation_lecture(argv[2]);
            main_init_gui(&argc, argv);
			return EXIT_SUCCESS;
		}
		break;
	case 1:
        main_init_gui(&argc, argv);
		return EXIT_SUCCESS;
	}
	printf("Usage : %s [{Error|Draw} filename]\n", argv[0]);
	return EXIT_FAILURE;
}

void main_init_gui(int *argcp, char **argv)
{
    glutInit(argcp, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(TAILLE_INITIALE, TAILLE_INITIALE);
	view_window = glutCreateWindow("Simulation");
	
	glutDisplayFunc(main_affichage);
	glutReshapeFunc(main_reshape);

	main_create_glui_interface(view_window);
	
	glutMainLoop();
}

void main_create_glui_interface(int GL_window)
{
	filename_in  = (char*)malloc(sizeof(GLUI_String));
	filename_out = (char*)malloc(sizeof(GLUI_String));
	*filename_in  = 0;
	*filename_out = 0;

	GLUI_Master.set_glutIdleFunc(main_update_one_step);
	
	GLUI *glui = GLUI_Master.create_glui("Control");

	GLUI_Panel *opening = glui->add_panel("Opening");
	glui->add_edittext_to_panel(opening, "File name : ", GLUI_EDITTEXT_TEXT,
								filename_in);
	open = glui->add_button_to_panel(opening, "Open", BUTTON_OPEN, main_widget_update);

	GLUI_Panel *saving = glui->add_panel("Saving");
	glui->add_edittext_to_panel(saving, "File name : ", GLUI_EDITTEXT_TEXT,
								filename_out);
	glui->add_button_to_panel(saving, "Save", BUTTON_SAVE, main_widget_update);

	glui->add_column(true);
	GLUI_Panel *simulation = glui->add_panel("Simulation");
	glui->add_button_to_panel(simulation, "Start", BUTTON_START_STOP,
																main_widget_update);
	glui->add_button_to_panel(simulation, "Step" , BUTTON_STEP, main_widget_update);

	GLUI_Panel *recording = glui->add_panel("Recording");
	glui->add_checkbox_to_panel(recording, "Record", &record,CHECKBOX_RECORD,
																main_widget_update);
	glui->add_statictext_to_panel(recording, "Rate: 0.000");
	glui->add_statictext_to_panel(recording, "Turn: 0");

	glui->add_column(true);
	GLUI_Panel *control_mode = glui->add_panel("Control mode");
	GLUI_RadioGroup *control_group = glui->add_radiogroup_to_panel(control_mode, NULL,
																   CONTROL_MODE,
																   main_widget_update);
	glui->add_radiobutton_to_group(control_group, "Automatic");
	glui->add_radiobutton_to_group(control_group, "Manual");
	control_group->set_int_val(CONTROL_AUTO);

	GLUI_Panel *robot_control = glui->add_panel("Robot control");
	glui->add_statictext_to_panel(robot_control, "Translation: 0.000");
	glui->add_statictext_to_panel(robot_control, "Rotation:    0.000");

	glui->add_button("Exit", EXIT_SUCCESS, exit);

	glui->set_main_gfx_window(GL_window);	
}

void main_widget_update(int widget)
{
	switch(widget)
	{
	case BUTTON_OPEN:
		simulation_lecture(filename_in);
		if(glutGetWindow() != view_window)
			glutSetWindow(view_window);
		glutPostRedisplay();
		break;
		
	case BUTTON_SAVE:
		simulation_ecriture(filename_out);
		break;
		
	case BUTTON_START_STOP:
		if(!simulation_started)
		{
			simulation_started = true;
			printf("simulation started\n");
		}
		else
		{
			simulation_started = false;
			printf("simulation stopped\n");
		}
		break;
		
	case BUTTON_STEP:
		simulation_automatisation();
		
		printf("One step only\n");
		count++;
		printf("%u\n",count);		
		// ici UN SEUL appel à simulation_update();
		glutSetWindow(view_window);
		glutPostRedisplay();
		break;
		
	case CHECKBOX_RECORD:
		printf("checkbox record changed : reset turn counter\n");
		count = 0;
		break;
		
	case CONTROL_MODE:
		printf("Radiobutton activated: mode changed\n");
		break;
	}
}

void main_affichage()
{
	simulation_dessiner();
	glutSwapBuffers();
}

void main_reshape(int w, int h)
{
	graphic_reshape(w, h);
	glutPostRedisplay();
}

void main_update_one_step(void)
{
	if(simulation_started)
	{
		count++;
		printf("%u yaaa\n",count);
		simulation_automatisation();
		
		// ici UN SEUL appel à simulation_update();
		
		// ici mise à jour de GLUI avec valeur de count
		
		// Si record est actif alors enregistrer le taux
		
		glutSetWindow(view_window);
		glutPostRedisplay();
	}
}
