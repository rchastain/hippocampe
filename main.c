/*
 *  main.c
 *  Author(s) : Jérémy Pages <jeremy.pages@eleves.ec-nantes.fr>
 *  Copyright (C) 2008 Jérémy Pages
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hippocampe.h"
#include "entree.h"
#include "sortie.h"
#include "temps.h"

/* Variables globales */

FILE *fjournal;
struct Partie enCours;
struct MVar mvprov;
int prof_max, verb = 1;
struct Cadence cad;

/* Point d'entrée du programme */

int main(int argc, char** argv)
{
#ifdef __i386__
  char cjournal[] = "hippocampe32.log";
#else
  char cjournal[] = "hippocampe.log";
#endif
  fjournal = fopen(cjournal, "w+");
  journal("** ", "Démarrage d'Hippocampe", "\n", 1);
  srand(time(NULL));
  attendre();
  fclose(fjournal);
  return 0;
}
