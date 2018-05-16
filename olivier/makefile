# Definitions de macros

CC     = gcc
CFLAGS =
CPPFLAGS = -Wall
CFILES = error.c graphic.c particule.c robot.c simulation.c utilitaire.c main.cpp
OFILES = error.o  graphic.o  particule.o  robot.o  simulation.o  utilitaire.o  main.o  
LIBS = -lstdc++ -lglui -L/usr/X11R6/lib -lglut -lGLU -lGL -lXmu -lXext -lX11 -lXi -lm

# Definition de la premiere regle

projet.x: $(OFILES)
	$(CC) $(OFILES) $(LIBS) -o projet.x

# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' makefile | \
	  sed "/^CFILES =*/c\\CFILES = `echo *.c *.cpp`" | \
	  sed "/^OFILES =*/c\\OFILES = `echo *.c *.cpp | sed 's/\.cp*/.o /g'` " && \
	  $(CC) -MM $(CFILES) | \
	  egrep -v "/usr/include" \
	 )>makefile.new
	@mv makefile.new makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o projet.x *.c~ *.h~

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINEOA
error.o: error.c constantes.h tolerance.h error.h
graphic.o: graphic.c graphic.h
particule.o: particule.c error.h particule.h utilitaire.h tolerance.h \
 constantes.h
robot.o: robot.c constantes.h tolerance.h error.h particule.h \
 utilitaire.h robot.h
simulation.o: simulation.c robot.h utilitaire.h tolerance.h particule.h \
 error.h constantes.h simulation.h
utilitaire.o: utilitaire.c graphic.h utilitaire.h tolerance.h
main.o: main.cpp simulation.h graphic.h
