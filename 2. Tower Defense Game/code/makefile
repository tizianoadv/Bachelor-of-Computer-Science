#
# CONFIGURATION GENERALE
#

EXEC = serveur client
OBJETS = socket.o fichier.o udp_gestion_requetes.o tcp_connexion.o ncurses.o fenetre.o interface.o sem.o liste.o udp_gestion_sem.o tcp_communication.o tcp_gestion_sem.o moteur.o interaction.o
NOM_PROJET = socketTCP

#
# SUFFIXES
#

.SUFFIXES: .c .o

#
# OBJETS
#

EXEC_O = $(EXEC:=.o)
OBJETS_O = $(OBJETS) $(EXEC_O)

#
# ARGUMENTS ET COMPILATEUR
#

CC = gcc
CCFLAGS = -Wall -O3 -Werror -ansi -pedantic
CCLIBS = -lncurses

#
# REGLES
#

all: $(OBJETS) $(EXEC_O)
	@echo "Creation des executables..."
	@for i in $(EXEC); do \
	$(CC) -o $$i $$i.o $(OBJETS) $(CCLIBS); \
	done
	@echo "Termine."

#
# REGLES PAR DEFAUT
#

.c.o: .h
	@cd $(dir $<) && ${CC} ${CCFLAGS} -c $(notdir $<) -o $(notdir $@)

#
# REGLES GENERALES
#

clean:
	@echo "Suppresion des objets, des fichiers temporaires..."
	@rm -f $(OBJETS) $(EXEC_O)
	@rm -f *~ *#
	@rm -f $(EXEC)
	@rm -f dependances
	@ipcrm -M 912
	@echo "Termine."

depend:
	@echo "Creation des dependances..."
	@sed -e "/^# DEPENDANCES/,$$ d" makefile > dependances
	@echo "# DEPENDANCES" >> dependances
	@for i in $(OBJETS_O); do \
	$(CC) -MM -MT $$i $(CCFLAGS) `echo $$i | sed "s/\(.*\)\\.o$$/\1.c/"` >> dependances; \
	done
	@cat dependances > makefile
	@rm dependances
	@echo "Termine."

#
# CREATION ARCHIVE
#

ARCHIVE_FILES = *

archive: clean
	@echo "Creation de l'archive $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz')..."
	@REP=`basename $$PWD`; cd .. && tar zcf $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz') $(addprefix $$REP/,$(ARCHIVE_FILES))
	@echo "Termine."

# DEPENDANCES
socket.o: socket.c socket.h fichier.h partie.h jeu.h udp_gestion_sem.h \
 liste.h sem.h
fichier.o: fichier.c fichier.h partie.h jeu.h
udp_gestion_requetes.o: udp_gestion_requetes.c udp_gestion_requetes.h \
 socket.h fichier.h partie.h jeu.h udp_gestion_sem.h liste.h sem.h \
 fenetre.h interface.h ncurses.h
tcp_connexion.o: tcp_connexion.c socket.h fichier.h partie.h jeu.h \
 udp_gestion_sem.h liste.h sem.h tcp_connexion.h tcp_communication.h \
 fenetre.h interface.h
ncurses.o: ncurses.c ncurses.h
fenetre.o: fenetre.c fenetre.h
interface.o: interface.c fenetre.h ncurses.h jeu.h interface.h
sem.o: sem.c sem.h partie.h jeu.h
liste.o: liste.c liste.h sem.h partie.h jeu.h
udp_gestion_sem.o: udp_gestion_sem.c udp_gestion_sem.h fichier.h partie.h \
 jeu.h liste.h sem.h
tcp_communication.o: tcp_communication.c tcp_communication.h partie.h \
 jeu.h socket.h fichier.h udp_gestion_sem.h liste.h sem.h fenetre.h \
 interface.h ncurses.h tcp_gestion_sem.h moteur.h
tcp_gestion_sem.o: tcp_gestion_sem.c socket.h fichier.h partie.h jeu.h \
 udp_gestion_sem.h liste.h sem.h tcp_gestion_sem.h fenetre.h interface.h
moteur.o: moteur.c moteur.h socket.h fichier.h partie.h jeu.h \
 udp_gestion_sem.h liste.h sem.h fenetre.h interface.h
interaction.o: interaction.c interaction.h jeu.h fenetre.h interface.h \
 socket.h fichier.h partie.h udp_gestion_sem.h liste.h sem.h
serveur.o: serveur.c udp_gestion_sem.h fichier.h partie.h jeu.h liste.h \
 sem.h tcp_connexion.h socket.h udp_gestion_requetes.h fenetre.h \
 interface.h
client.o: client.c ncurses.h fenetre.h jeu.h interface.h socket.h \
 fichier.h partie.h udp_gestion_sem.h liste.h sem.h \
 udp_gestion_requetes.h tcp_connexion.h tcp_communication.h interaction.h
