/*
 *  temps.h
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

#ifndef TEMPS_H
#define TEMPS_H

#include <sys/time.h>
#include <time.h>

#define BASE1 1
#define INC1 2
#define N1 3
//#define BASE2 4   (Projet pour Hippocampe 0.5)
//#define N2 5
//#define BASE3 6

struct Cadence
{
  int base1;		// temps de base (en secondes)
  int inc;		// incrément en secondes
  int n1;		// nombre de coups (1er contrôle)
  //int base2;		// temps de base 2ème contrôle
  //int n2;
  //int base3;
};

struct Cadence reglerCadence(struct Cadence *c, int param, int val);
struct Cadence initCadence();
float allouer(struct Cadence c, float tempsCourant);
int alarme(struct timeval t0, float alloue);
void synchroniser(float time);

#endif
