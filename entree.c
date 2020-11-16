/*
*  entree.c
*  Author(s) : Jérémy Pages <jeremy.pages@eleves.ec-nantes.fr>
*  Copyright (C) 2008 Jérémy Pages
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Library General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef __MINGW32__
#include <process.h>
#else
#include <pthread.h>
pthread_t analyse;
#endif

#include "entree.h"
#include "hippocampe.h"
#include "sortie.h"
#include "partie.h"
#include "recherche.h"
#include "regles.h"
#include "temps.h"

extern struct Partie enCours;
extern struct Cadence cad;
extern int prof_max, verb;
struct MVar mvProv;

/* Séparation des arguments de la commande */
/* Cette fonction permet d'extraire le premier argument de la commande */

char *extrait(char **commande)
{
  char *mot = NULL;
  char *inter;
  int i = 0;
  size_t taille_mot = 1;

  while (*(*commande + i) != ' ' && *(*commande + i) != '\n' && *(*commande + i) != '\0')
  {
    taille_mot = taille_mot + sizeof(char);
    mot = realloc(mot, taille_mot);
    *(mot + taille_mot - 2) = *(*commande + i);
    i++;
  }
  *(mot + taille_mot - 1) = '\0';

  i = 0;
  inter = strdup(*commande);
  while (*(inter + i + taille_mot) != '\0' && *(inter + i + taille_mot) != '\n')
  {
    *(inter + i) = *(inter + i + taille_mot);
    i++;
  }
  *(inter + i) = '\0';
  *commande = inter;

  return mot;
}

/* Traitement de la commande */

void traiter(char *commande, int *protocole)
{
  char *arg0 = NULL, *arg1;
  char *base;
  float tempsAlloue;
  int i;
  char s[8] = "";

  if (commande != NULL && strlen(commande) > 1)
  {
    journal("-> ", commande, "", 1);

    arg0 = extrait(&commande);

    if (!strcmp(arg0, "xboard"))
    {
      *protocole = PROTO_XBOARD;
    }
    else if (!strcmp(arg0, "protover"))
    {
      //envoi("feature analyze=0");
      //envoi("feature colors=0");
      //envoi("feature debug=0");
      envoi("feature myname=\"Hippocampe " VERSION "." REVISION "\"");
      envoi("feature ping=1");
      envoi("feature sigint=0");
      envoi("feature sigterm=0");
      envoi("feature variants=\"normal\"");
      envoi("feature done=1");
    }
    else if (!strcmp(arg0, "ping"))
    {
      sprintf(s, "pong %d", atoi(extrait(&commande)));
      envoi(s);
    }
    else if (!strcmp(arg0, "new"))
    {
      cad = initCadence();
      prof_max = 5;
      enCours = reglerPartie(cad, POS_INIT, NOIR);
    }
    else if (!strcmp(arg0, "quit"))
    {
      *protocole = PROTO_FIN;
      journal("** ", "Arrêt d'Hippocampe", "\n", 1);
    }
    else if (!strcmp(arg0, "force"))
    {
      enCours.couleur = AUCUNE;
    }
    else if (!strcmp(arg0, "go"))
    {
      enCours.couleur = enCours.posAct.trait;
      tempsAlloue = allouer(cad, enCours.temps);
      #ifdef __MINGW32__
      _beginthread((void*)&rechercher, 0, &enCours.posAct);
      #else
      pthread_create(&analyse, NULL, (void*)&rechercher, &enCours.posAct);
      pthread_detach(analyse);
      #endif
    }
    else if (!strcmp(arg0, "black"))
    {
      enCours.posAct.trait = NOIR;
      enCours.couleur = BLANC;
    }
    else if (!strcmp(arg0, "white"))
    {
      enCours.posAct.trait = BLANC;
      enCours.couleur = NOIR;
    }
    else if (!strcmp(arg0, "level"))
    {
      cad = reglerCadence(&cad, N1, atoi(extrait(&commande)));
      cad = reglerCadence(&cad, BASE1, 60 * atoi(extrait(&commande)));
      cad = reglerCadence(&cad, INC1, atoi(extrait(&commande)));
      //printf("Coups à jouer : %d\n", cad.n1);
    }
    else if (!strcmp(arg0, "st"))
    {
      cad = reglerCadence(&cad, N1, 1);
      cad = reglerCadence(&cad, BASE1, atoi(extrait(&commande)));
    }
    else if (!strcmp(arg0, "sd"))
    {
      arg1 = extrait(&commande);
      prof_max = atoi(arg1);
    }
    else if (!strcmp(arg0, "time"))
    {
      synchroniser((float)atof(extrait(&commande)) / 100);
    }
    else if (!strcmp(arg0, "result"))
    {
      arg1 = extrait(&commande);
      if (strcmp(arg1, "1-0"))
      {
        enCours.resultat = 'B';
      }
      if (strcmp(arg1, "1/2-1/2"))
      {
        enCours.resultat = 'X';
      }
      if (strcmp(arg1, "0-1"))
      {
        enCours.resultat = 'N';
      }
    }
    else if (!strcmp(arg0, "post"))
    {
      verb = 1;
    }
    else if (!strcmp(arg0, "nopost"))
    {
      verb = 0;
    }
    else if (!strcmp(arg0, "name"))
    {
      reglerAdv(extrait(&commande), HUMAIN);
    }
    else if (!strcmp(arg0, "computer"))
    {
      reglerAdv(NULL, ORDINATEUR);
    }
    else if (
      !strcmp(arg0, "accepted") ||
      !strcmp(arg0, "easy") ||
      !strcmp(arg0, "hint") ||
      !strcmp(arg0, "otim") ||
      !strcmp(arg0, "rejected")
    )
    {
    }
    /* Commandes personnalisées */
    else if (!strcmp(arg0, "board"))
    {
      afficherPosition(enCours.posAct, 1);
    }
    else if (!strcmp(arg0, "moves"))
    {
      chercherCoups(&enCours.posAct, 1);
    }
    else // Serait-ce un coup ?
    {
      struct Coup c;
      c = obtenirCoup(arg0);
      if (c.ca * c.cd * c.la * c.ld == 0 || c.spec == INDEFINI)
      {
        journal("** Commande inconnue: ", arg0, "\n", 1);
      }
      else
      {
        enCours.posAct = entrerCoup(enCours.posAct, c);
        if (enCours.couleur != AUCUNE)
        {
          #ifdef __MINGW32__
          _beginthread((void*)&rechercher, 0, &enCours.posAct);
          #else
          pthread_create(&analyse, NULL, (void*)&rechercher, &enCours.posAct);
          pthread_detach(analyse);
          #endif
        }
      }
    }
  }
}

/* Boucle principale du programme */

void attendre()
{
  char *com_entree = NULL;
  int protocole = PROTO_XBOARD;
  /*
  signal(SIGTERM, SIG_IGN);
  signal(SIGINT, SIG_IGN);
  */
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);

  while (protocole != PROTO_FIN)
  {
    com_entree = malloc(128 * sizeof(char));
    fgets(com_entree, 128, stdin);
    traiter(com_entree, &protocole);
    free(com_entree);
  }
}
